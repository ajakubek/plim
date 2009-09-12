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

#ifndef __REACTOR_ROOM_H__
#define __REACTOR_ROOM_H__

#include <libplimgui/abstract.h>
#include <libplimgui/treenodes.h>

#include "cReactorSession.h"

namespace NSReactor {

using namespace NSAbstract;
using namespace NSTree;

class cReactorRoom: public cAbstractRoom, public cTreeNodes, public cTreeNode  {
public:
	cReactorRoom(cReactorSession* session);
	virtual ~cReactorRoom(void);
	cAbstractUser* GetUser(const char* user);
	cReactorSession* GetSession(void);
private:
	cReactorSession* m_session;
};

};

#endif
