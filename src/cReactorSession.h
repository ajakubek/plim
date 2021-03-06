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

#ifndef __REACTOR_SESSION_H__
#define __REACTOR_SESSION_H__

#include <libplimgui/abstract.h>
#include <libplimgui/treenodes.h>

#include "cReactor.h"

namespace NSReactor {

using namespace NSAbstract;
using namespace NSTree;

class cReactorSession: public cAbstractSession, public cTreeNodes, public cTreeNode {
public:
	cReactorSession(cTreeNodes* nodes, const char* sessionName);
	virtual ~cReactorSession(void);
	virtual void RegisterReactorInstance(cReactor* reactor);
	virtual int Connect(void) { return -1; };
	virtual int Disconnect(void) { return -1; };
	virtual int Fission(fd_set *rfds, fd_set *wfds, fd_set *efds) { return 0; }
	virtual int Release(fd_set *rfds, fd_set *wfds, fd_set *efds) { return 0; }

	virtual cAbstractUser* GetUser(void) { return NULL; };

	cAbstractRoom* GetRoom(const char* room);

	cReactor* GetNuclearReactor(void) { return m_reactor; };
private:
	cReactor* m_reactor;
};

};


#endif
