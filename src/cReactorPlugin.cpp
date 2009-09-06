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

#include "cReactorPlugin.h"

namespace NSReactor {

cReactorPlugin::cReactorPlugin(cReactor* reactor, const char* configPath)
:	cTreeNode(reactor, NULL, NULL),
	cTreeNodes(),
	m_reactor( NULL ), m_config( NULL )
 {
	if ( reactor ) {
		m_reactor = reactor;
		if ( m_reactor->GetConfig() )
			m_config = m_reactor->GetConfig()->GetConfigNodeByPath(configPath);
	}
}

cReactorPlugin::~cReactorPlugin(void) {
	
}

int cReactorPlugin::NuclearFission(fd_set *rfds, fd_set *wfds, fd_set *efds) {
	cReactorSession* session;

	session = GetFirstSession();

	while (session) {
		session->Fission(rfds, wfds, efds);
		session = GetNextSession( session );
	}

	return 0;
}

int cReactorPlugin::NuclearRelease(fd_set *rfds, fd_set *wfds, fd_set *efds) {
	cReactorSession* session;

	session = GetFirstSession();

	while (session) {
		session->Release(rfds, wfds, efds);
		session = GetNextSession( session );
	}

	return 0;
}

cPlimConfigNode* cReactorPlugin::GetConfig(const char* config) {
	if (m_reactor && m_config && m_reactor->GetConfig()) {
		return m_reactor->GetConfig()->GetConfigNode( m_config, config );
	}
	else
		return NULL;
}

void cReactorPlugin::SetPluginName(const char* name) {
	m_pluginName.Copy( name );
}

const char* cReactorPlugin::GetPluginName(void) {
	return m_pluginName.GetBuffer();
}

};
