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
 
#include "keybindings.hpp"

namespace plimgui_curses {

cKeyBindings::cKeyBindings(void) 
{
	m_keyBindings = new plimuni::cHashNodes(10003);

	/* PDCURSES */
	{
		m_keyTable.insert( std::make_pair( ALT_M, "Alt+m" ) );
		m_keyTable.insert( std::make_pair( ALT_N, "Alt+n" ) );
		m_keyTable.insert( std::make_pair( ALT_P, "Alt+p" ) );
		m_keyTable.insert( std::make_pair( ALT_Q, "Alt+q" ) );
	}
}

cKeyBindings::~cKeyBindings(void) 
{
	if (m_keyBindings) 
	{
		delete m_keyBindings;
	}
}

/* TODO: Change */
int cKeyBindings::expandKey(const int key) 
{
	char binding[1024];
	int c;
	
#if defined(__PDCURSES__)
	std::map< int, std::string>::iterator it;

	for ( it = m_keyTable.begin(); it != m_keyTable.end(); it++)
	{
		if ( (*it).first == key )
		{
			m_buffer.copy( (*it).second.c_str() );
			m_pendingKey = TRUE;
			return 1;
		}
	}

#else
	if ( key == KEY_FOLLOW_ALT ) 
	{
		/* Getch the next character */
		c = getch();
		
		sprintf( &binding[0], "Alt+%c", c );
		m_buffer.copy( &binding[0] );
		return 1;
	}
#endif

	return 0;
}

int cKeyBindings::isKeyBindingPending(void) 
{
	if (m_pendingKey) 
	{
		m_pendingKey = FALSE;
		return 1;
	}
	return 0;
}

int cKeyBindings::bindKey(const char* key, const char* command) 
{
	plimuni::cHashNode* node;
	plimuni::cString* newcmd;

	if ( (node = m_keyBindings->get( key, strlen( key ))) ) {
		if ( node->getPtr() ) {
			newcmd = (plimuni::cString*) node->getPtr();
			newcmd->copy( command );
		}
	} else {
		newcmd = new plimuni::cString( command );
		node = m_keyBindings->add( key, newcmd );
	}

	return 0;
}

plimuni::cString* cKeyBindings::getCommand(const char* key) 
{
	plimuni::cHashNode* node;

	if (!key)
		key = m_buffer.getBuffer();

	if ( (node = m_keyBindings->get( key, strlen( key ))) != NULL ) 
	{
		return (plimuni::cString*) node->getPtr();
	}
	else
		return NULL;
}

};