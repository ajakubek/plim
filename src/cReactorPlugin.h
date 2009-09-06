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

#ifndef __REACTOR_PLUGIN_H__
#define __REACTOR_PLUGIN_H__

#include <libplimgui/treenodes.h>
#include <libplimgui/strings.h>
#include <libplimgui/abstract.h>

#include "cReactor.h"
#include "cReactorSession.h"

/* TODO: 
	Add sessions to plugins, wich will manage the connection to specified protocol, protocol parsing if any will be done in the plugin.
	Add users and rooms to session, to easily interact between the reactor and GUI.
	@IC0ffeeCup
*/

namespace NSReactor {

using namespace NSAbstract;
using namespace NSString;
using namespace NSTree;

class cReactorPlugin: public cTreeNode, public cTreeNodes {
public:
	cReactorPlugin(cReactor* reactor, const char* configPath);
	virtual ~cReactorPlugin(void);

	/* Absorb some sessions
	*/
	virtual int NuclearFission(fd_set *rfds, fd_set *wfds, fd_set *efds);
	virtual int NuclearRelease(fd_set *rfds, fd_set *wfds, fd_set *efds);

	/* Gets the node inside m_config
	*/
	cPlimConfigNode* GetConfig(const char* config);

	cReactorSession* GetFirstSession(void) { return (cReactorSession*) cTreeNodes::GetFirstNode(); };
	cReactorSession* GetLastSession(void) { return (cReactorSession*) cTreeNodes::GetLastNode(); };
	cReactorSession* GetNextSession(cReactorSession* session) { return (cReactorSession*) cTreeNodes::GetNext(session); };
	cReactorSession* GetPrevSession(cReactorSession* session) { return (cReactorSession*) cTreeNodes::GetPrev(session); };

	/* Accessors 
	*/
	void SetPluginName(const char* name);
	const char* GetPluginName(void);

	cReactorPlugin* GetNextNode(void) { return (cReactorPlugin*) cTreeNode::GetNextNode(); };
	cReactorPlugin* GetPrevNode(void) { return (cReactorPlugin*) cTreeNode::GetPrevNode(); };

protected:

private:
	cString m_pluginName;
	cReactor* m_reactor;
	cPlimConfigNode* m_config;
	cTreeNodes* m_sessionNodes;
};

};

#endif
