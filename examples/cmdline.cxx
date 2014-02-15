#include <getopt.h>
#include <iostream>
#include "mm.h"
#include "cmdline.h"

using namespace std;

command_line::command_line()
{
    for(int i = 0; i < p_last_item; ++i)
        m_is_opt_set[i] = 0;
}

command_line::iterator command_line::begin(const string& key)
{
    if(key.length())
        return m_map.lower_bound(key);
    else
        return m_map.begin();
}

command_line::iterator command_line::end(const string& key)
{
    if(key.length())
        return m_map.upper_bound(key);
    else
        return m_map.end();
}

command_line::const_iterator command_line::begin(const string& key) const
{
    if(key.length())
        return m_map.lower_bound(key);
    else
        return m_map.begin();
}

command_line::const_iterator command_line::end(const string& key) const
{
    if(key.length())
        return m_map.upper_bound(key);
    else
        return m_map.end();
}



string& command_line::operator[](const string& key)
{
    iterator it = m_map.find(key);
    if(it == m_map.end())
        it = m_map.insert(command_line_switch(key,""));
    return it->second;
}

bool command_line::is_set(int i) const
{
    return m_is_opt_set[i];
}

bool command_line::is_set(const string& switch_name) const
{
    return m_map.find(switch_name) != m_map.end();
}

void command_line::die_if_not_valid() const
{
    die_if(is_set(p_match_shell) && is_set(p_match_regex),
        "just one of --match-shell and --match-regex can be specified");
    die_if(is_set(p_match_shell) && is_set(p_perl_regex),
        "--match-shell and --perl-regex are not compatible");
    die_if(is_set(p_print_msg) && is_set(p_print_part),
        "just one of --print-message and --print-part can be specified");
    die_if(is_set(p_delete_msg) && is_set(p_delete_part),
        "just one of --delete-msg and --delete-part can be specified");
}

void command_line::add_switch(const string& name, const string& arg)
{
    m_map.insert(command_line_switch(name,arg));
}

bool command_line::parse_cmd_line(int argc, char **argv)
{
    #define CMDLINE_ARG( name, desc, arg ) { name "\000" desc, arg, 0, 0 }
    #define CMDLINE_ARG_VAL( name, desc, arg, val ) { name "\000" desc, arg, 0, val }
    enum { NO_ARG, REQ_ARG, OPT_ARG };
    int opt_idx = 0;
    static struct option long_options[] = {
    // "name", has-arg, flag, val
        CMDLINE_ARG_VAL("attachment-filename",
            "filename of the attachment", 
        REQ_ARG, p_attachment_filename),
        CMDLINE_ARG_VAL("has-binary-attach", 
            "match if the message has attachments", 
        NO_ARG,p_has_binary_attach), 
        CMDLINE_ARG_VAL("has-field","",OPT_ARG,p_has_field), 
        CMDLINE_ARG_VAL("field", "",OPT_ARG,p_field), 
        CMDLINE_ARG_VAL("ifield","",OPT_ARG,p_ifield), 
        CMDLINE_ARG_VAL("from", "",OPT_ARG,    p_std_field), 
        CMDLINE_ARG_VAL("sender", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("to", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("subject", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("cc", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("bcc", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("user-agent", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("date", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("content-type", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("content-transfer-encoding","",OPT_ARG,p_std_field),
        CMDLINE_ARG_VAL("content-disposition", "", OPT_ARG,p_std_field), 
        CMDLINE_ARG_VAL("content-description", "", OPT_ARG,p_std_field), 

        CMDLINE_ARG_VAL("add-header", "", REQ_ARG, p_add_header), 
        CMDLINE_ARG_VAL("del-header", "", REQ_ARG, p_del_header), 
        CMDLINE_ARG_VAL("mod-header", "", REQ_ARG, p_mod_header), 
        CMDLINE_ARG_VAL("add-part-header", "", REQ_ARG, p_add_part_header), 
        CMDLINE_ARG_VAL("del-part-header", "", REQ_ARG, p_del_part_header), 
        CMDLINE_ARG_VAL("mod-part-header", "", REQ_ARG, p_mod_part_header), 
        CMDLINE_ARG_VAL("attach", "", REQ_ARG, p_attach), 
        CMDLINE_ARG_VAL("detach", "", NO_ARG, p_detach), 
        CMDLINE_ARG_VAL("delete-part", "", NO_ARG, p_delete_part), 
        CMDLINE_ARG_VAL("delete-message", "", NO_ARG, p_delete_msg), 
        CMDLINE_ARG_VAL("print-part", "", NO_ARG,p_print_part), 
        CMDLINE_ARG_VAL("print-message", "", NO_ARG,p_print_msg), 
        CMDLINE_ARG_VAL("pipe-to", "", REQ_ARG, p_pipe_to), 

        CMDLINE_ARG_VAL("recursive", "", NO_ARG, p_recursive), 
        CMDLINE_ARG_VAL("invert-selection", "", NO_ARG, p_invert_selection), 
        CMDLINE_ARG_VAL("match-shell", "", NO_ARG, p_match_shell), 
        CMDLINE_ARG_VAL("match-regex", "", NO_ARG, p_match_regex), 
        CMDLINE_ARG_VAL("perl-regex", "", NO_ARG, p_perl_regex), 
        CMDLINE_ARG_VAL("case-insensitive", "", NO_ARG, p_case_insensitive), 
        CMDLINE_ARG_VAL("encoding", "", REQ_ARG, p_encoding), 
        CMDLINE_ARG_VAL("in", "", REQ_ARG, p_in), 
        CMDLINE_ARG_VAL("out", "", REQ_ARG, p_out), 
        CMDLINE_ARG_VAL("help", "", NO_ARG, p_help), 
        CMDLINE_ARG_VAL("version", "", NO_ARG, p_version), 
        {0, 0, 0, 0}
    };


    int ret;
    while((ret = getopt_long(argc,argv, "hv", long_options, &opt_idx))!= -1)
    {
        switch(ret)
        {
        case ':':
            die("missing parameter");
        case '?':
            die("unknown option");
        case 'v':
            cerr << MM_VERSION << endl;
            exit(1);
            break;
        case 'h':
            //usage_if(1);
            for(int i = 0; long_options[i].name; ++i)
            {
                const char *name= long_options[i].name;
                cerr << "--" << name;
                switch(long_options[i].has_arg)
                {
                case NO_ARG:
                    break;
                case REQ_ARG:
                    cerr << " ARG";
                    break;
                case OPT_ARG:
                    cerr << " [ARG]";
                    break;
                default:
                    die("unknown has_arg");
                }
                // past the end of name
                const char *desc = 1 + name + strlen(name);
                cerr << "\t" << desc << endl;
            }
            exit(0);
            break;
        default:
            m_is_opt_set[ret]++;
            for(int i = 0; long_options[i].name; ++i)
            {
                struct option *opt = &long_options[i];
                if(ret == opt->val)
                {
                    if(opt->has_arg == NO_ARG || !optarg)
                        add_switch(opt->name,""); 
                    else 
                        add_switch(opt->name, optarg); 
                }
            }
        }
    }
    /*
    switch(argc - optind)
    {
    case 0:
        // read from stdin write to stdout
        break;
    case 1:
        //stdin provided in argv[optind]
        pCl->args.push_back(argv[optind]);
        break;
    case 2:
        //stdin & stdout provided in argv[optind] && argv[++optind]
        pCl->args.push_back(argv[optind]);
        pCl->args.push_back(argv[++optind]);
        break;
    default:
        usage();
    }
    cerr << endl;
    */
    return true;
}
