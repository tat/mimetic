#ifndef MM_CMDLINE_H_
#define MM_CMDLINE_H_
#include <string>
#include <vector>
#include <list>
#include <map>

enum {
    p_none,
    // match
    p_attachment_filename, p_has_binary_attach, p_has_field,
    p_field, p_ifield, p_std_field,
    // options
    p_match_shell, p_match_regex, 
    p_perl_regex, p_case_insensitive, p_encoding,
    p_invert_selection, p_recursive,
    // actions
    p_add_header, p_del_header, p_mod_header, 
    p_add_part_header, p_del_part_header, p_mod_part_header, 
    p_attach, p_detach, p_delete_part, p_delete_msg, p_print_part,
    p_print_msg, p_pipe_to,
    //others
    p_in, p_out, p_help, p_version,
    p_last_item
};

typedef std::pair<std::string, std::string> command_line_switch;
struct command_line
{
    typedef std::string string;
    typedef std::multimap<string,string>::iterator iterator;
    typedef std::multimap<string,string>::const_iterator const_iterator;
    command_line();
    bool parse_cmd_line(int, char**);
    bool is_set(const string&) const;
    bool is_set(int) const;
    string& operator[](const string&);
    iterator begin(const string& s = "");
    iterator end(const string& s = "");
    const_iterator begin(const string& s = "") const;
    const_iterator end(const string& s = "") const;
private:
    void die_if_not_valid() const;
    void add_switch(const string&, const string&);
    std::multimap<string,string> m_map;
    char m_is_opt_set[p_last_item];
};

#endif
