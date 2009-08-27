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

#include "cPlim.h"

namespace NSPlim {

cPlim::cPlim(int argc, char** argv)
:	cApplication(argc, argv),
	cPlimReactor(),
	m_root(NULL), m_infoBar(NULL), m_inputBar(NULL), m_statusBar(NULL), m_rosterBar(NULL), m_roomWindows(NULL),
	m_activeRoom(NULL), m_activeNode(NULL) {

	BuildInterface();
	PreBuildInterface();
	BindSignals();
	BuildBindings();

	switch( Loop() ) {
		default:
			break;
	}

}

cPlim::~cPlim(void) {
	DestroyInterface();
}

void cPlim::BuildInterface(void) {
	if (!m_root) {
		m_root = new cCursesWindow( this, 0, 0, 0, 0, NULL);
	}

	if (!m_infoBar) {
		m_infoBar = new cStatusWindow( this, m_root );
		m_infoBar->SetWindowAlign(top);
	}

	if (!m_inputBar) {
		m_inputBar = new cInputWindow( this, m_root );
		m_inputBar->SetWindowAlign(bottom);
	}

	if (!m_statusBar) {
		m_statusBar = new cStatusWindow( this, m_root );
		m_statusBar->SetWindowAlign(bottom);
	}

	if (!m_rosterBar) {
		m_rosterBar = new cRosterWindow( this, m_root );
		m_rosterBar->SetWindowAlign(right);
	}

	if (!m_roomWindows) {
		m_roomWindows = new cTreeNodes();
	}
}

void cPlim::PreBuildInterface(void) {
	if (m_statusBar) {
		m_statusBar->Add("status_time")->SetCaption("16:57");
		m_statusBar->Add("status_nick")->SetCaption("IC0ffeeCup(+i)");
		m_statusBar->Add("status_server")->SetCaption("2:warszawa/#gentoo.pl(+nst)");
		m_statusBar->Add("status_active_window")->SetCaption("Act: 4");
	}
}

void cPlim::BindSignals( void ) {
	OnBindingPress.connect( sigc::mem_fun( this, &cPlim::SignalBindingInput ) );

	if (m_inputBar) {
		m_inputBar->OnEnter.connect( sigc::mem_fun( this, &cPlim::SignalEnterInput ) );
	}
}

void cPlim::BuildBindings( void ) {
	BindKey("Alt+m", "internal_window_new");
	BindKey("Alt+n", "internal_window_next");
	BindKey("Alt+p", "internal_window_prev");
	BindKey("Alt+q", "internal_app_quit");
}

void cPlim::DestroyInterface( void ) {
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

void cPlim::SignalEnterInput(const char* buffer) {
	if (!buffer)
		return;

	cTextLine* line;

	if (!strcmp(buffer, "/quit")) {
		Close();
	}

	if (m_activeRoom) {
		if (!m_activeRoom->GetLastLine()) {
			m_activeRoom->NewLine( buffer, 0 );
		}
		else
			m_activeRoom->NewLine( m_activeRoom->GetLastLine(), buffer, 0 );
	}

}

void cPlim::SignalBindingInput(cApplication* app, cString* command) {
	cString* str;
	cTextWindow* w;
	cTreeNode* node;

	if ( !app || !command )
		return;

	char room[1024];
	/* Test case */
	sprintf(&room[0], "proto:window%d", rand() );

	if (!command->Compare("internal_window_new")) {
		AddRoom(&room[0]);
		ShowRoom(&room[0]);
	}

	if (!command->Compare("internal_window_next")) {
		if (m_activeNode) {
			w = (cTextWindow*) m_activeRoom->GetNodeData();
			node = m_roomWindows->GetNext(m_activeNode);

			if (!node) {
				node = m_roomWindows->GetFirstNode();
			}

			if (node) {
				w = (cTextWindow*) node->GetNodeData();
				str = (cString*) w->GetNodeData();
				if (str)
					ShowRoom( str->GetBuffer() );
			}

		}
	}

	if (!command->Compare("internal_window_prev")) {
		if (m_activeNode) {
			w = (cTextWindow*) m_activeRoom->GetNodeData();
			node = m_roomWindows->GetPrev(m_activeNode);

			if (!node) {
				node = m_roomWindows->GetLastNode();
			}

			if (node) {
				w = (cTextWindow*) node->GetNodeData();
				str = (cString*) w->GetNodeData();
				if (str)
					ShowRoom( str->GetBuffer() );
			}

		}
	}
//	if (window && cmd) {
		//window->NewLine( cmd->GetBuffer(), 0 );
	//}

}

cTextWindow* cPlim::AddRoom(const char* id) {
	cTextWindow* w;
	cTreeNode* node;
	cString* str;

	if (!id)
		return NULL;

	str = new cString(id);

	w = new cTextWindow(this, m_root);

	if (w) {
		
		/* TODO: Some preconfig for dynamic window placement */
		w->SetWindowAlign(client);
		w->SetNodeData(str);
		w->SetVisible( FALSE );

		new cTreeNode( m_roomWindows, NULL, w);
	}

	return w;
}

cTreeNode* cPlim::GetRoomNode(const char* id) {
	cTreeNode* node;
	cString* str;
	cTextWindow* w;

	if (!id)
		return NULL;

	node = m_roomWindows->GetFirstNode();
	
	while (node) {
		w = (cTextWindow*) node->GetNodeData();

		if (w) {
			str = (cString*) w->GetNodeData();
			if (str) {
				if (!strcmp( id, str->GetBuffer() ) ) {
					return node;
				}
			}
		}

		node = m_roomWindows->GetNext(node);
	}

	return NULL;
}

cTextWindow* cPlim::GetRoom(const char* id) {
	cTreeNode* node;
	cTextWindow* w;

	if (!id)	return NULL;
	if ((node = GetRoomNode(id)) != NULL) {
		return (cTextWindow*) node->GetNodeData();
	}
	return NULL;
}

cTextWindow* cPlim::DeleteRoom(const char* id) {
	cString* str;
	cTreeNode* n;
	cTextWindow* w;

	if (!id)
		return NULL;

	n = GetRoomNode(id);

	if (n) {
		w = (cTextWindow*) n->GetNodeData();
		str = (cString*) w->GetNodeData();

		if (str)
			delete str;

		n = m_roomWindows->GetPrev(n);

		if (!n)
			n = (cTextWindow*) m_roomWindows->GetNext(n);

		if (m_activeNode == n) {
			m_activeNode = n;
			m_activeRoom = (cTextWindow*) m_activeNode->GetNodeData();
		}

		delete w;

		return m_activeRoom;
	}

	return NULL;
}

void cPlim::ShowRoom(const char* id) {
	cTreeNode* node = GetRoomNode(id);
	cString* str;
	cTextWindow* w, *ww;

	if (node) {
		w = (cTextWindow*) node->GetNodeData();
	} else
		w = NULL;

	if (w) {
		w->SetVisible( TRUE );
		m_activeRoom = w;
		m_activeNode = node;

		node = (cTreeNode*) m_roomWindows->GetFirstNode();
		
		while (node) {
			ww = (cTextWindow*) node->GetNodeData();

			if (ww != w) {
				if (ww) {
					ww->SetVisible( FALSE );
				}
			}

			node = (cTextWindow*) m_roomWindows->GetNext(node);
		}

	}

}

};