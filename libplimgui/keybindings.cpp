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
 
#include "keybindings.h"

namespace NSApplication {

cKeyBindings::cKeyBindings(void) {
	m_keyBindings = new cHashNodes(10003);
}

cKeyBindings::~cKeyBindings(void) {
	if (m_keyBindings) {
		delete m_keyBindings;
	}
}

int cKeyBindings::ExpandKey(const int key) {
	char binding[1024];
	int c;

	if ( key == KEY_FOLLOW_ALT ) {
		/* Getch the next character */
		c = getch();
		
		sprintf( &binding[0], "Alt+%c", c );

		m_buffer.Copy( &binding[0] );
		m_pendingKey = TRUE;

		return 1;
	}
	/* Search by table */
	else {

	}

	return 0;
}

int cKeyBindings::IsKeyBindingPending(void) {
	if (m_pendingKey) {
		m_pendingKey = FALSE;
		return 1;
	}
	return 0;
}

int cKeyBindings::BindKey(const char* key, const char* command) {
	cHashNode* node;
	cString* newcmd;

	if ( (node = m_keyBindings->Get( key, strlen( key ))) ) {
		if ( node->GetPtr() ) {
			newcmd = (cString*) node->GetPtr();
			newcmd->Copy( command );
		}
	} else {
		newcmd = new cString( command );
		node = m_keyBindings->Add( key, newcmd );
	}

	return 0;
}

cString* cKeyBindings::GetCommand(const char* key) {
	cHashNode* node;

	if (!key)
		key = m_buffer.GetBuffer();

	if ( (node = m_keyBindings->Get( key, strlen( key ))) != NULL ) {
		return (cString*) node->GetPtr();
	}
	else
		return NULL;
}

};