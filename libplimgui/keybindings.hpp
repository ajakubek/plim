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

#include <libplimuni/stdafx.hpp>
#include <libplimuni/hashnodes.hpp>
#include <libplimuni/strings.hpp>

#include "stdafx.hpp"

namespace plimgui_curses {

#define KEY_FOLLOW_ALT 0x1b

class cKeyBindings 
{

public:
	cKeyBindings(void);
	~cKeyBindings(void);
	
	int expandKey(const int key);
	int isKeyBindingPending(void);
	int bindKey(const char* key, const char* command);
	
	plimuni::cString* getCommand(const char* key);

private:
#if defined(__PDCURSES__)
	std::map< int, std::string > m_keyTable;
#else

#endif
	plimuni::cHashNodes* m_keyBindings;
	plimuni::cString m_buffer;
	int m_pendingKey;
};

};

#endif
