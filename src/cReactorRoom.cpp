/***************************************************************************
 *   Copyright (C) 2009 by Lukasz Marcin Borzecki   *
 *   lukasz.borzecki@gmail.com   *
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

#include "cReactorRoom.h"
#include "cReactorUser.h"

namespace NSReactor {

cReactorRoom::cReactorRoom(cReactorSession* session)
:	cAbstractRoom(),
	cTreeNodes(),
	cTreeNode(session, NULL, NULL),
	m_session(session) {

}

cReactorRoom::~cReactorRoom(void) {
	
}

cAbstractUser* cReactorRoom::GetUser(const char* user) {
	cReactorUser* u = (cReactorUser*) cTreeNodes::GetFirstNode();

	while ( u ) {
		if (!strcmp(user, u->GetUserName()) )
			return u;

		u = (cReactorUser*) cTreeNodes::GetNext( u );
	}
}

cReactorSession* cReactorRoom::GetSession(void) {
	return m_session;
}

};
