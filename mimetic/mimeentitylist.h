/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: mimeentitylist.h,v 1.8 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_MIME_ENTITY_LIST_
#define _MIMETIC_MIME_ENTITY_LIST_
#include <list>
#include <string>
#include <memory>

namespace mimetic
{

class MimeEntity;

/// List of MimeEntity classes
typedef std::list<std::shared_ptr<MimeEntity>> MimeEntityList;


}

#endif
