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

#include "cPlim.hpp"

NSConfig::cPlimConfig* SharedConfiguration = NULL;

namespace NSPlim {

cPlim::cPlim(int argc, char** argv)
:	cApplication(argc, argv),
	m_root(NULL), m_infoBar(NULL), m_inputBar(NULL), m_statusBar(NULL), m_rosterBar(NULL), m_roomWindows(NULL),
	m_activeRoom(NULL), m_activeNode(NULL), m_config(NULL) {
	
	m_home = new cString( getenv("HOME") );
	m_home->cat( "/.plim/" );

	m_homeConfig = new cString( m_home );
	m_homeConfig->cat( "config" );
	
	setNuclearReactor( new cReactor( this ) );
	
	buildInterface();
	preBuildInterface();
	bindSignals();
	buildBindings();

	if (loadConfiguration( m_homeConfig->getBuffer() )) {
		addDbgMsg( "+time +b-!-+r Configuration loaded from %s", m_homeConfig->getBuffer());
	}
	else {
		addDbgMsg( "+time +b-!-+r Failed to load configuration from %s", m_homeConfig->getBuffer());
	}


	if ( getNuclearReactor() ) {
		addDbgMsg( "+time +b-!-+r Nuclear Reactor was loaded successful" );

		m_irc = new cIrc( getNuclearReactor(), "reactor.irc" );
		
		if ( m_irc ) {
			addDbgMsg( "+time +b-!-+r Internal irc plugin was loaded successful" );
			
			m_ircSession = new cIrcSession( m_irc, "irc:proto" );
			
			if (m_ircSession) {
				addDbgMsg( "+time +b-!-+r Internal irc session was loaded successful" );
			}
			
			m_irc->registerSessionReactors();
		}
	}

	
	cApplication::loopMsg();

	switch( loop() ) {
		default:
			break;
	}

}

cPlim::~cPlim(void) {
	destroyInterface();

	if (m_home)			delete m_home;
	if (m_homeConfig)	delete m_homeConfig;
	if (m_config)		m_config = NULL;
}

int cPlim::loopMsg(void) {
	int reactor;
	
	if ( onTerminalSizeChanged() );
	
	if (m_nuclearReactor) {
		reactor = m_nuclearReactor->nuclearChainReaction();
	}

	if (reactor == -1 && checkKeyClicked() > 0 ) {}
	
	return cApplication::loopMsg();
}

int cPlim::loadConfiguration(const char* filename) {
	if (!filename)	return 0;

	if (!SharedConfiguration) {
		SharedConfiguration = new cPlimConfig();
		m_config = SharedConfiguration;
	}
	
	if (!m_config->loadConfig( filename )) {
		/* Assign conf to gui */
		assignConfiguration( m_config );
		return 1;
	}

	return 0;
}

void cPlim::assignConfiguration(cPlimConfig* config) {
	cPlimConfigNode* node;
	
	if (!config)	return;
	
	/* Assign static configurations. */
	
	node = config->getConfigNodeByPath( "plim.textbuffer.lines.use_msg_levels" );
	
	if (node) {
		
	}
	else {
		
	}
		
}

cPlimConfig* cPlim::getSharedConfig( void ) {
	return m_config;
}

void cPlim::setNuclearReactor(cReactor* reactor) {
	/* TODO: Add some unload and load props */
	m_nuclearReactor = reactor;
}

cReactor* cPlim::getNuclearReactor(void) {
	return m_nuclearReactor;
}

void cPlim::addDbgMsg(const char* msg, ...) {
	cTextWindow* dbg;
	va_list l;
	va_start( l, msg);

	dbg = getRoom( PLIM_ROOM_DEBUG );

	if (dbg) {
		dbg->addDebugLine( msg, l );
	}

	va_end( l );
}

void cPlim::buildInterface(void) {
	if (!m_root) {
		m_root = new cCursesWindow( this, 0, 0, 0, 0, NULL);
	}

	if (!m_infoBar) {
		m_infoBar = new cStatusWindow( this, m_root );
		m_infoBar->setWindowAlign(top);
	}

	if (!m_inputBar) {
		m_inputBar = new cInputWindow( this, m_root );
		m_inputBar->setWindowAlign(bottom);
	}

	if (!m_statusBar) {
		m_statusBar = new cStatusWindow( this, m_root );
		m_statusBar->setWindowAlign(bottom);
	}

	if (!m_rosterBar) {
		m_rosterBar = new cRosterWindow( this, m_root );
		m_rosterBar->setWindowAlign(right);
	}

	if (!m_roomWindows) {
		m_roomWindows = new cTreeNodes();
	}
}

void cPlim::preBuildInterface(void) {
	if (m_statusBar) {
		m_statusBar->add("status_time")->setCaption("16:57");
		m_statusBar->add("status_nick")->setCaption("IC0ffeeCup(+i)");
		m_statusBar->add("status_server")->setCaption("2:warszawa/#gentoo.pl(+nst)");
		m_statusBar->add("status_active_window")->setCaption("Act: 4");
	}

	/* Add debug window */
	if (m_roomWindows) {
		addRoom( PLIM_ROOM_DEBUG );
		showRoom( PLIM_ROOM_DEBUG );
	}
}

void cPlim::parseInput(cString* cmd) {
	if ( !cmd || !cmd->getLength() )	return;

	
}

void cPlim::bindSignals( void ) {
	onBindingPress.connect( boost::bind( &NSPlim::cPlim::signalBindingInput, this, _1, _2 ) );

	if (m_inputBar) {
		m_inputBar->onEnter.connect( boost::bind( &NSPlim::cPlim::signalEnterInput, this, _1 ) );
	}
	
	if ( m_nuclearReactor ) {

		m_nuclearReactor->onDebugMsg.connect ( boost::bind( &NSPlim::cPlim::reactorSignalDebugMsg, this, _1, _2 ) );
		m_nuclearReactor->onCreateRoom.connect ( boost::bind( &NSPlim::cPlim::reactorSignalCreateRoom, this, _1 ) );
		m_nuclearReactor->onDestroyRoom.connect ( boost::bind( &NSPlim::cPlim::reactorSignalDestroyRoom, this, _1) );
		m_nuclearReactor->onUserJoin.connect( boost::bind( &NSPlim::cPlim::reactorSignalUserJoin, this, _1, _2 ) );
		m_nuclearReactor->onUserPart.connect( boost::bind( &NSPlim::cPlim::reactorSignalUserPart, this, _1, _2 ) );
		m_nuclearReactor->onUserMessage.connect( boost::bind( &NSPlim::cPlim::reactorSignalUserMessage, this, _1, _2, _3 ) );

	}
}

void cPlim::buildBindings( void ) {
	bindKey("Alt+m", "internal_window_new");
	bindKey("Alt+n", "internal_window_next");
	bindKey("Alt+p", "internal_window_prev");
	bindKey("Alt+q", "internal_app_quit");
}

void cPlim::destroyInterface( void ) {
	if (m_rosterBar) {
		delete m_rosterBar;
	}
	if (m_statusBar) {
		delete m_statusBar;
	}
	if (m_inputBar) {
		delete m_inputBar;
	}
	if (m_infoBar) {
		delete m_infoBar;
	}
	if (m_root) {
		delete m_root;
	}
	if (m_roomWindows) {
		delete m_roomWindows;
	}
}

bool cPlim::signalEnterInput(const char* buffer) {
	cString s;
	
	if (!buffer)
		return true;

	cTextLine* line;
	
	s.copy( buffer );
	
	if (!strcmp(buffer, "/quit")) {
		close();
		return true;
	}

	if (!s.compare("join gentoo") || !s.compare("join worldchat")) {
		m_nuclearReactor->onSentMessage( &s, NULL, NULL);
		return true;
	}

	if (!s.compare( "irc:connect" )) {
		if (m_ircSession) {
			
			m_ircSession->setBindIp( "192.168.1.45" );
			m_ircSession->setIp( "212.182.63.110" );
			m_ircSession->setPortNumber( 6667 );
			
			addDbgMsg( "+time +b-!-+r Trying to connect to %s", m_ircSession->getIp() );
			
			int r;
			if ((r = m_ircSession->connect()) > -1) {
				addDbgMsg( "+time +b-!-+r Internal irc session connected successfuly" );
			} 
			else {
				addDbgMsg( "+time +b-!-+r Internal irc session returned %d", r );
			}
				
		}

		return true;
	}
	
	/* DEBUG PURPOSE */
	if (m_activeRoom && m_activeRoom != getRoom( PLIM_ROOM_DEBUG) ) {

		if (m_nuclearReactor)	{
			if ( m_activeRoom ) {
				cReactorRoom* room = (cReactorRoom*) m_activeRoom->getRoom();
				cReactorSession* session = (cReactorSession*) room->getSession();
				cReactorUser* user = (cReactorUser*) session->getUser();


				m_nuclearReactor->onSentMessage( &s, user, room);
				m_activeRoom->addLine( buffer, user );
			}
		}


	}
	return true;
}

bool cPlim::signalBindingInput(cApplication* app, cString* command) {
	cString* str;
	cTextWindow* w;
	cTreeNode* node;

	if ( !app || !command )
		return true;

	char room[1024];
	/* Test case */
	sprintf(&room[0], "proto:window%d", rand() );

	if (!command->compare("internal_app_quit")) {
		close();
		return true;
	}
	
	if (!command->compare("internal_window_new")) {
		addRoom(&room[0]);
		//ShowRoom(&room[0]);
	}

	if (!command->compare("internal_window_next")) {
		if (m_activeNode) {
			w = (cTextWindow*) m_activeRoom->getNodeData();
			node = m_roomWindows->getNext(m_activeNode);

			if (!node) {
				node = m_roomWindows->getFirstNode();
			}

			if (node) {
				w = (cTextWindow*) node->getNodeData();
				str = (cString*) w->getNodeData();
				if (str)
					showRoom( str->getBuffer() );
			}

		}
	}

	if (!command->compare("internal_window_prev")) {
		if (m_activeNode) {
			w = (cTextWindow*) m_activeRoom->getNodeData();
			node = m_roomWindows->getPrev(m_activeNode);

			if (!node) {
				node = m_roomWindows->getLastNode();
			}

			if (node) {
				w = (cTextWindow*) node->getNodeData();
				str = (cString*) w->getNodeData();
				if (str)
					showRoom( str->getBuffer() );
			}

		}
	}
	
//	if (window && cmd) {
		//window->NewLine( cmd->GetBuffer(), 0 );
	//}
	return true;
}

/* Reactor signals 
*/
bool cPlim::reactorSignalDebugMsg(cAbstractSession* session, cString* message) {
	addDbgMsg( "+time +b-!-+r Session (%s) recieved %s", session->getSessionName(), message->getBuffer());
	return true;
}

bool cPlim::reactorSignalCreateRoom(cAbstractRoom* room) {
	if (room) {
		addRoom( room->getRoomName() )->setRoom( room );
	}
	return true;
}

bool cPlim::reactorSignalDestroyRoom(cAbstractRoom* room) {
	if (room) 
	{
		deleteRoom( room->getRoomName() );
	}
	
	return true;
}

bool cPlim::reactorSignalUserMessage(cAbstractRoom* room, cAbstractUser* user, cString* message) {
	cTextWindow* win;

	if ( room && user && message ) {
		win = getRoom( room->getRoomName() );

		if (win) {
			win->addLine( message->getBuffer(), user );
		}
	}
	return true;
}

bool cPlim::reactorSignalUserJoin(cAbstractRoom* room, cAbstractUser* user) {
	cTextWindow* win;

	if ( room && user ) {
		win = getRoom( room->getRoomName() );

		if (win) {
			cString c( "User ");
			c.cat(user->getUserName());
			c.cat(" joined to channel");
			win->addDebugLine( c.getBuffer() );
		}
	}
	return true;
}

bool cPlim::reactorSignalUserPart(cAbstractRoom* room, cAbstractUser* user) {
	cTextWindow* win;

	if ( room && user ) {
		win = getRoom( room->getRoomName() );

		if (win) {
			cString c( "User ");
			c.cat(user->getUserName());
			c.cat(" parted from channel");
			win->addDebugLine( c.getBuffer() );
		}
	}
	return true;
}

cTextWindow* cPlim::addRoom(const char* id) {
	cTextWindow* w;
	cTreeNode* node;
	cString* str;
	cPlimConfigNode* nodeconf;
	
	if (!id)
		return NULL;

	str = new cString(id);

	w = new cTextWindow(this, m_root);

	if (w) {
		if (m_config) {
			nodeconf = m_config->getConfigNodeByPath( "plim.textbuffer.lines.use_msg_levels" );

			if (nodeconf && !nodeconf->compareVar("true"))
				w->setSlibUse( 1 );

			nodeconf = m_config->getConfigNodeByPath( "plim.textbuffer.lines.first" );

			if (nodeconf)
				w->setFirstText( nodeconf->getVar() );
				
			nodeconf = m_config->getConfigNodeByPath( "plim.textbuffer.lines.slib" );

			if (nodeconf)
				w->setSlibText( nodeconf->getVar() );

			nodeconf = m_config->getConfigNodeByPath( "plim.textbuffer.lines.last" );

			if (nodeconf)
				w->setLastText( nodeconf->getVar() );

		}

		/* TODO: Some preconfig for dynamic window placement */
		w->setWindowAlign(client);
		w->setNodeData( str );
		w->setVisible( FALSE );

		new cTreeNode( m_roomWindows, NULL, w);
	}

	return w;
}

cTreeNode* cPlim::getRoomNode(const char* id) {
	cTreeNode* node;
	cString* str;
	cTextWindow* w;

	if (!id)
		return NULL;

	node = m_roomWindows->getFirstNode();
	
	while (node) {
		w = (cTextWindow*) node->getNodeData();

		if (w) {
			str = (cString*) w->getNodeData();
			if (str) {
				if (!strcmp( id, str->getBuffer() ) ) {
					return node;
				}
			}
		}

		node = m_roomWindows->getNext(node);
	}

	return NULL;
}

cTextWindow* cPlim::getRoom(const char* id) {
	cTreeNode* node;
	cTextWindow* w;

	if (!id)	return NULL;
	if ((node = getRoomNode(id)) != NULL) {
		return (cTextWindow*) node->getNodeData();
	}
	return NULL;
}

cTextWindow* cPlim::deleteRoom(const char* id) {
	cString* str;
	cTreeNode* n;
	cTextWindow* w;

	if (!id)
		return NULL;

	n = getRoomNode(id);

	if (n) {
		w = (cTextWindow*) n->getNodeData();
		str = (cString*) w->getNodeData();

		if (str)
			delete str;

		n = m_roomWindows->getPrev(n);

		if (!n)
			n = (cTextWindow*) m_roomWindows->getNext(n);

		if (m_activeNode == n) {
			m_activeNode = n;
			m_activeRoom = (cTextWindow*) m_activeNode->getNodeData();
		}

		delete w;

		return m_activeRoom;
	}

	return NULL;
}

void cPlim::showRoom(const char* id) {
	cTreeNode* node = getRoomNode(id);
	cString* str;
	cTextWindow* w, *ww;

	if (node) {
		w = (cTextWindow*) node->getNodeData();
	} else
		w = NULL;

	if (w) {
		w->setVisible( TRUE );
		m_activeRoom = w;
		m_activeNode = node;

		node = (cTreeNode*) m_roomWindows->getFirstNode();
		
		while (node) {
			ww = (cTextWindow*) node->getNodeData();

			if (ww != w) {
				if (ww) {
					ww->setVisible( FALSE );
				}
			}

			node = (cTextWindow*) m_roomWindows->getNext(node);
		}

	}

}

};
