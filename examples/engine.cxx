#include <algorithm>
#include <iostream>
#include <string>
#include <cctype>
#include <sys/types.h>
#include <regex.h>
#ifdef HAS_PCRE
#include <pcreposix.h>
#endif

#include "engine.h"

using namespace std;
using namespace mimetic;

engine::engine(const command_line& cl)
: m_cl(cl), m_pcre(false), 
 m_match_mode(match_type_none)
{
    // set options
    if(cl.is_set(p_match_shell) || cl.is_set(p_match_regex))
        m_match_mode = 
            cl.is_set(p_match_shell) ? 
            match_type_shell : 
            match_type_regex;

    m_match_mode |= 
        cl.is_set(p_case_insensitive) ? match_flag_case_insensitive : 0;

    m_match_mode |= 
        cl.is_set(p_perl_regex) ? match_flag_perl_mode: 0;

    if((m_match_mode & match_type_mask) == match_type_none)
        m_match_mode |= match_type_regex; // default
}

int engine::posix_regex_match(const string& text, const string& pattern, int match_mode)
{
    int r;
    regex_t rex;
    r = regcomp(&rex, pattern.c_str(), 
        ( match_mode & match_flag_case_insensitive ? REG_ICASE: 0));
    if(r != 0)
    {
        char buf[256];
        regerror(r, &rex, buf, 255);
        die(buf);
    }
    r = regexec(&rex, text.c_str(), 0, 0, 0);
    regfree(&rex);
    return r == 0;
}

int engine::perl_regex_match(const string& text, const string& pattern, int match_mode )
{
#ifdef HAS_PCRE
#else
    die("uuh?");
#endif
    return 0;
}

int engine::pattern_match(const string& text, const string& pattern, int match_mode)
{
    switch(match_mode & match_type_mask)
    {
    case match_type_none:
        die("match_type_none");
    case match_type_exact:
        return exact_match(text, pattern, match_mode);
    case match_type_regex:
        return regex_match(text, pattern, match_mode);
    case match_type_shell:
        return shell_match(text, pattern, match_mode);
    default:
        die("uuh?");
    }
    return 0;
}

int engine::shell_match(const string& text, const string& pattern, int match_mode)
{
    die("not impl");
    return 0;
}

int engine::regex_match(const string& text, const string& pattern, int match_mode)
{
    if(m_pcre)
        return engine::perl_regex_match(text, pattern, match_mode);
    else
        return engine::posix_regex_match(text, pattern, match_mode);
}

void engine::action_attach(MimeEntity& me, parts_hierarchy* ph, const string& fqn)
{
    bool isMultipart = me.header().contentType().isMultipart();
    bool isTopLevel = !ph->size();

    /*
        1) me is multipart:
            add the attach to me as the last part
        2) me is not multipart
            create a multipart/mixed with me and the attach childs
            and put it in the same level/position of me
        3) me is not multipart and is the top level entity
            same as 2) but move all me fields to the new top-level
    */
    Attachment* pA = new Attachment(fqn);
    if(!pA->isValid())
        die("attach error");
    if(isMultipart)
    {
        DBG( "isMultipart");
        me.body().parts().push_back(pA);
    } else {
        MimeEntity *mm;
        mm = new MultipartMixed;
        mm->body().parts().push_back(&me);
        mm->body().parts().push_back(pA);
        if(!isTopLevel)
        {
            DBG( "!isTopLevel");
            MimeEntity *parent = *ph->begin();
            replace(parent->body().parts().begin(), 
                parent->body().parts().end(), 
                &me, mm);
        } else {
            DBG( "isTopLevel");
            // add cp fields here
            Header::iterator bit, eit, pos;
            bit = me.header().begin(), me.header().end();
            string name; // field name
            pos = mm->header().begin(); // insert before others
            for(; bit != eit; ++bit)
            {
                name = bit->name();    
                transform(name.begin(), name.end(), 
                    name.begin(), ::tolower);
                if(name.find("content-") == 0 || name == "mime-version")
                    continue;
                mm->header().insert(pos, *bit);
            }
        }
    }
}

void engine::action(MimeEntity& me, parts_hierarchy* ph)
{
    MimeEntity* parent = (ph->size() ? *ph->begin() : &me);
    if(m_cl.is_set(p_add_header)) 
    {
        static const char* key = "add-header";
        command_line::iterator bit, eit;
        bit = m_cl.begin(key), eit = m_cl.end(key);
        for(; bit != eit; ++bit)
        {
            Field f(bit->second);
            parent->header().push_back(f);
        }
    } 
    if(m_cl.is_set(p_add_part_header)) {
        static const char* key = "add-part-header";
        command_line::iterator bit, eit;
        bit = m_cl.begin(key), eit = m_cl.end(key);
        for(; bit != eit; ++bit)
        {
            Field f(bit->second);
            me.header().push_back(f);
        }
    } 
    if(m_cl.is_set(p_attach)) 
    {
        static const char* key = "attach";
        command_line::iterator bit, eit;
        bit = m_cl.begin(key), eit = m_cl.end(key);
        for(; bit != eit; ++bit)
            action_attach(me, ph, bit->second);
    }

    if(m_cl.is_set(p_print_msg))
        cout << *parent;
    else if(m_cl.is_set(p_print_part))
        cout << me;
}


int engine::exact_match(const string& text, const string& pattern, int match_mode)
{
    if(match_mode & match_flag_case_insensitive)
    {
        istring is(text);
        return is == pattern;
    } else
        return text == pattern;
}


/*
 * expr: pat1 [=|~] pat2
 * pat1 is the pattern that represents the field name
 * pat2 is the pattern that represents the field value
 */
int engine::pattern_field_match(const MimeEntity& me, const string& expr, 
    int match_mode)
{
    int has_value = 0; // left part of the expr
    char prev = 0; // previous char
    string field_pat, value_pat;
    char op;
    for(size_t i = 0; i < expr.length(); ++i)
    {
        if( (expr[i] == '=' || expr[i] == '~') && prev != '\\')
        {
            has_value++; // right part
            op = expr[i];
            continue;
        }
        if(!has_value)
            field_pat.append(1, expr[i]);
        else
            value_pat.append(1, expr[i]);
        prev = expr[i];
    }
    field_pat = remove_external_blanks(field_pat);
    value_pat = remove_external_blanks(value_pat);
    // first try to find a field that match the field_pat pattern
    const Header& h = me.header();
    Header::const_iterator bit = h.begin(), eit = h.end();
    for( ; bit != eit; ++bit)
    {
        if(pattern_match(bit->name(), field_pat, match_mode))
        { // we've found a matching field, let's check the value
            if(!has_value)
                return 1;
            else 
                if(pattern_match(bit->value(), value_pat, match_mode))
                    return 1;
        }
    }
    return 0;
}

string engine::remove_external_blanks(const string& str) const
{
    // a dirty way to trim ext.blanks
    string s = str;
    for(int i = s.length() - 1; i >= 0; --i)
        if(s[i] == ' ')
            s.erase(i, 1);
        else
            break;
    while(s.length() && s[0] == ' ')
        s.erase(0, 1);
    return s;
}

int engine::fixed_field_match(const MimeEntity& me, const string& name, const string& value, int match_mode)
{
    if(!me.header().hasField(name))
        return 0;
    if(value.length() == 0) 
        return 1;  // it exists
    const string& field_value = me.header().field(name).value();
    return pattern_match(field_value, value, match_mode) ;
}

int engine::has_binary_attach(const MimeEntity& me, const command_line_switch& cls)
{
    const Header& h = me.header();
    const ContentType& ct = h.contentType();
    if(ct.type() == "text" || ct.type() == "multipart" || ct.type() == "message")
        return 0;
    const ContentTransferEncoding& cte = h.contentTransferEncoding();
    if(cte.mechanism() == "base64")
        return 1;
    return 1;
}

int engine::field_match(const MimeEntity& me, const command_line_switch& cls)
{
    const string& name = cls.first, value = cls.second;
    if(name == "field")
        return pattern_field_match(me, value, m_match_mode);
    else if (name == "ifield")
        return pattern_field_match(me, value, 
            m_match_mode | match_flag_case_insensitive);
    else
        return fixed_field_match(me, name, value,
            m_match_mode);
}

int engine::has_field(const MimeEntity& me, const command_line_switch& cls)
{
    return me.header().hasField(cls.second);
}

int engine::match_filename(const string& filename, const string& pattern)
{
    // convert shell pattern string to regex
    string re_pattern;
    char c;
    for(size_t i = 0; i < pattern.length(); ++i)
    {
        c = pattern[i];
        switch(c)
        {
        case '?':
            re_pattern.append(".");
            break;
        case '*':
            re_pattern.append(".*");
            break;
        case '[':
        case '.':
        case '=':
        case '<':
        case '>':
        case '+':
        case '_':
        case '\\':
        case '-':
        case ']':
            re_pattern.append(1, '\\');
            re_pattern.append(1, c);
            break;
        default:
            re_pattern.append(1, c);
        }
    }
    return regex_match(filename, re_pattern, 0);
}

int engine::attach_filename(const MimeEntity& me,const command_line_switch& cls)
{
    typedef list<string> filename_list;
    const Header& h = me.header();
    const ContentType& ct = h.contentType();
    const ContentDisposition& cd = h.contentDisposition();
    string pattern = cls.second;
    filename_list names;
    // content-type params
    names.push_back(ct.param("name"));
    names.push_back(ct.param("filename")); // should not exists
    // content-disposition params
    names.push_back(cd.param("name"));
    names.push_back(cd.param("filename")); // should not exists
    filename_list::const_iterator bit = names.begin(), eit = names.end();
    for( ; bit != eit; ++bit)
        if(match_filename(*bit, pattern))
            return 1;
    return 0;
}

MimeEntity* engine::match(MimeEntity& me, int level, parts_hierarchy* ph)
{
    int matched = 1, child_match = 0, free = 0;

    if(ph == 0)
    {
        ph = new parts_hierarchy;
        free++;
    }
    if(m_cl.is_set(p_recursive))
    {
        MimeEntityList& parts = me.body().parts();
        if( parts.size() )
        {
            ++level;
            MimeEntityList::iterator mbit, meit;
            mbit = parts.begin(), meit = parts.end();
            ph->insert(ph->begin(), &me);
            for( ; mbit != meit; ++mbit)
                child_match += (match(**mbit, level, ph ) ? 1 : 0);
            ph->erase(ph->begin());
        }
    }
    static char *std_fields[] = {
        "from", "sender", "to", "sujbect", "cc", "bcc",
        "user-agent", "date", "content-type", 
        "content-transfer-encoding", "content-disposition",
        "content-description", 
        0
    };
    command_line::const_iterator bit = m_cl.begin(), eit = m_cl.end();
    for(; bit != eit; ++bit)
    {
        const string& name = bit->first, value = bit->second;
        if(name == "attach-filename") {
            if(!attach_filename(me, *bit)) 
            {
                matched = 0;
                break;
            }
        } else if(name == "has-field") {
            if(!has_field(me, *bit)) 
            {
                matched = 0;
                break;
            }
        } else if(name == "has-binary-attach") {
            if(!has_binary_attach(me, *bit))
            {
                matched = 0;
                break;
            }
        } else if(name == "field") {
            if(!field_match(me, *bit))
            {
                matched = 0;
                break;
            }
        } else if(name == "ifield") {
            if(!field_match(me, *bit))
            {
                matched = 0;
                break;
            }
        } else {
            int break_loop = 0;
            char **std_name = std_fields;
            for( int i = 0 ; std_name[i] ; ++i) 
            {
                if(name == std_name[i])
                    if(!field_match(me, *bit))
                    {
                        matched = 0;
                        break_loop++;
                        break;
                    }
            }
            if(break_loop)
                break;
        }
    }
    if(matched)
        action(me, ph);
    if(free)
        delete ph;
    return ( matched || child_match ? &me : 0);
}
