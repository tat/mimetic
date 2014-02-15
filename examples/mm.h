#ifndef _MM_H_
#define _MM_H_
#include <list>
#include <vector>
#include <iostream>
#include <string>

#define MM_VERSION     "0.0.1"

#define DBG( s ) std::cerr << "[" << s << "]" << std::endl

#define usage() do { do_usage_if( 1 , __FILE__, __LINE__); } while(0);
#define usage_if(a) do { do_usage_if( a , __FILE__, __LINE__); } while(0);

#define die(msg) do { do_die_if( 1 , msg, __FILE__, __LINE__); } while(0);
#define die_if(a, msg) do { do_die_if( a , msg, __FILE__, __LINE__); } while(0);

void do_die_if(int, const std::string& , const std::string&, int );
void do_usage_if(int, const std::string&, int);

#endif
