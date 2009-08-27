/***************************************************************************
 *   Copyright (C) 2009 by Lukasz Marcin Borzecki                          *
 *   lukasz.borzecki@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __PLIM_KEYBINDINGS_H__
#define __PLIM_KEYBINDINGS_H__

#include "hashnodes.h"
#include "strings.h"

namespace NSApplication {

using namespace NSTree;
using namespace NSString;

#define KEY_FOLLOW_ALT 0x1b

/* TODO: Add some stack for keys */

class cKeyBindings {
public:
	cKeyBindings(void);
	~cKeyBindings(void);
	int ExpandKey(const int key);
	int IsKeyBindingPending(void);
	int BindKey(const char* key, const char* command);
	cString* GetCommand(const char* key);
private:
	cHashNodes* m_keyBindings;
	cString m_buffer;
	int m_pendingKey;
};

};

#endif
