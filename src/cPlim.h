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

#include <libplimgui/application.h>
#include <libplimgui/windows.h>
#include <libplimgui/statusbar.h>
#include <libplimgui/textviewer.h>
#include <libplimgui/roster.h>
#include <libplimgui/input.h>
#include <libplimgui/treenodes.h>
#include <libplimgui/strings.h>
#include <sigc++/signal.h>

#include "cPlimReactor.h"

namespace NSPlim {

using namespace NSApplication;
using namespace NSWindows;
using namespace NSString;
using namespace NSTree;
using namespace NSReactor;

class cPlim: public cApplication, public cPlimReactor {
public:
	cPlim(int argc, char** argv);
	virtual ~cPlim( void );
	/* Public signals
	*/
protected:
	void BuildInterface( void );
	void PreBuildInterface( void );
	void BindSignals( void );
	void BuildBindings( void );
	void DestroyInterface( void );
	/* Signals method
	*/
	void SignalEnterInput(const char* buffer);
	void SignalBindingInput(cApplication* app, cString* command);
	/* Room windows
	*/
	cTextWindow* AddRoom(const char* id);
	cTreeNode* GetRoomNode(const char* id);
	cTextWindow* GetRoom(const char* id);
	cTextWindow* DeleteRoom(const char* id);
	
	void ShowRoom(const char* id);
private:
	cCursesWindow* m_root;
	cStatusWindow* m_statusBar;
	cStatusWindow* m_infoBar;
	cInputWindow* m_inputBar;
	cRosterWindow* m_rosterBar;
	cTextWindow* m_activeRoom;
	cTreeNode* m_activeNode;
	cTreeNodes* m_roomWindows;
};
	
};