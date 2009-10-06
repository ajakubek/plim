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

#ifndef __REACTOR_H__
#define __REACTOR_H__

#include <libplimuni/stdafx.hpp>
#include <libplimuni/treenodes.hpp>
#include <libplimuni/strings.hpp>
#include <libplimuni/abstract.hpp>

#include "stdafx.hpp"

namespace plimreactor 
{

class cReactor	:	public	plimuni::cTreeNodes 
{

public:
	cReactor(void);
	virtual ~cReactor(void);
	
	bool load(const char* dynObjectName);
	bool unload(const char* name);
	bool run(void);
	bool status(void);
	
	int nuclearChainReaction(void);

	/* Signals from the UI */
	boost::signals2::signal <bool (plimuni::cString*, plimuni::cAbstractUser*, plimuni::cAbstractRoom*)> onSentMessage;
	boost::signals2::signal <bool (plimuni::cAbstractUser*, plimuni::cAbstractRoom*)> onGuiRoomChange;
	boost::signals2::signal <bool (plimuni::cString*)> onCommand;

	/* Signals to the UI */
	boost::signals2::signal <bool (plimuni::cAbstractSession*, plimuni::cString*)> onDebugMsg;

	boost::signals2::signal <bool (plimuni::cAbstractRoom*, plimuni::cAbstractUser*, plimuni::cString*)> onUserMessage;
	boost::signals2::signal <bool (plimuni::cAbstractRoom*, plimuni::cAbstractUser*)> onUserStatusChange;

	boost::signals2::signal <bool (plimuni::cAbstractRoom*)> onCreateRoom;
	boost::signals2::signal <bool (plimuni::cAbstractRoom*)> onDestroyRoom;
	boost::signals2::signal <bool (plimuni::cAbstractRoom*, plimuni::cAbstractUser*)> onUserJoin;
	boost::signals2::signal <bool (plimuni::cAbstractRoom*, plimuni::cAbstractUser*)> onUserPart;

private:

};

};

#endif
