#ifndef _MM_ENGINE_H_
#define _MM_ENGINE_H_
#include <mimetic/mimetic.h>
#include <stack>
#include <memory>
#include "mm.h"
#include "cmdline.h"

struct engine
{
    typedef mimetic::MimeEntity MimeEntity;
    typedef std::string string;
    typedef std::list<std::shared_ptr<MimeEntity>> parts_hierarchy;
    engine(const command_line&);
    std::shared_ptr<MimeEntity> match(const std::shared_ptr<MimeEntity>&, int level = 0, parts_hierarchy* ph = 0);
private:
    enum { 
        match_type_none,
        match_type_exact, 
        match_type_regex,
        match_type_shell,
        // flags
        match_flag_case_insensitive     = 1 << 4,
        match_flag_perl_mode        = 1 << 5,
        // masks
        match_type_mask = 0x0F,
        match_flag_mask = 0xF0
    };
    void action_attach(const std::shared_ptr<MimeEntity>&, parts_hierarchy*, const string& fqn);

    void action(const std::shared_ptr<MimeEntity>&, parts_hierarchy*);
    int perl_regex_match(const string&, const string&, int);
    int match_filename(const string&, const string&);
    int posix_regex_match(const string&, const string&, int);

    int regex_match(const string&, const string&, int);
    int exact_match(const string&, const string&, int);
    int shell_match(const string&, const string&, int);
    int pattern_match(const string&, const string&, int);

    int pattern_field_match(const std::shared_ptr<MimeEntity>& me, const string&, int);
    int fixed_field_match(const std::shared_ptr<MimeEntity>&, const string&, const string&, int);

    int has_binary_attach(const std::shared_ptr<MimeEntity>&, const command_line_switch&);
    int has_field(const std::shared_ptr<MimeEntity>&, const command_line_switch&);
    int attach_filename(const std::shared_ptr<MimeEntity>&, const command_line_switch&);
    int field_match(const std::shared_ptr<MimeEntity>&, const command_line_switch&);

    string remove_external_blanks(const string&) const;
private:
    command_line m_cl;
    bool m_ignore_childs, m_pcre;
    int m_match_mode;
};

#endif
