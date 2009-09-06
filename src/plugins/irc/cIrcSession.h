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

#ifndef __REACTOR_INTERNAL_PLUGIN_IRCSESSION_H__
#define __REACTOR_INTERNAL_PLUGIN_IRCSESSION_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include "../../cReactorSession.h"
#include "cIrc.h"
//#include "cIrcUser.h"

namespace NSInternalPluginIRC {

using namespace NSReactor;

class cIrcSession: public cReactorSession {
public:
	cIrcSession(cIrc* irc, const char* sessionName);
	virtual ~cIrcSession(void);
	
	int Connect(void);
	int Disconnect(void);

	int Fission(fd_set *rfds, fd_set *wfds, fd_set *efds);
	int Release(fd_set *rfds, fd_set *wfds, fd_set *efds);
private:
//	cIrcUser* m_myUser;
};

};

#endif
