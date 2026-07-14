/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: fileop.h,v 1.7 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_OS_FILEOP_H
#define _MIMETIC_OS_FILEOP_H
#include <string>

/**
  *@author 
  */
namespace mimetic
{

/// Defines some file utility functions
struct FileOp
{
    /* static funtions */
    static bool remove(const std::string&);
    static bool move(const std::string&, const std::string&);
    static bool exists(const std::string&);

    static unsigned int size(const std::string&);
    static unsigned int ctime(const std::string&); // creation time
    static unsigned int atime(const std::string&); // last time accessed(r/w)
    static unsigned int mtime(const std::string&); // last time written
};

}


#endif

