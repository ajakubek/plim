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

#include <libplimgui/treenodes.h>
#include <libplimgui/strings.h>
#include <libplimgui/abstract.h>
#include <libplimgui/application.h>

#include <sigc++/signal.h>

#include "cPlimConfig.h"

namespace NSReactor {

using namespace NSTree;
using namespace NSString;
using namespace NSAbstract;

using namespace NSApplication;
using namespace NSConfig;

class cReactor: public cTreeNodes {
public:
	cReactor(cApplication* plimapp);
	virtual ~cReactor(void);
	
	/* Add this to you app loop 
		@return int,
					-1, we can read something from the input.
					> -1, process.
	*/
	int NuclearChainReaction(void);

	/* Accessors */
	cPlimConfig* GetConfig(void);

	/* Signals from the UI */
	sigc::signal <void, cString*, cAbstractUser*, cAbstractRoom*> OnGuiInputText;
	sigc::signal <void, cAbstractUser*, cAbstractRoom*> OnGuiRoomChange;
	
	/* Signals to the UI */
	sigc::signal <void, cString*, cAbstractUser*, cAbstractRoom*> OnMsgRecv;
	sigc::signal <void, cString*, cAbstractUser*, cAbstractRoom*> OnMsgSend;
	sigc::signal <void, cAbstractUser*, cAbstractRoom*> OnUserStatusChange;
	sigc::signal <void, cAbstractRoom*> OnOpenRoom;

private:
	cApplication* m_app;
	cPlimConfig* m_config;
};

};

#endif
