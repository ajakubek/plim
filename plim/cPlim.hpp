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

#include <libplimuni/stdafx.hpp>
#include <libplimuni/abstract.hpp>
#include <libplimuni/treenodes.hpp>
#include <libplimuni/strings.hpp>

#include <libplimgui/stdafx.hpp>
#include <libplimgui/application.hpp>
#include <libplimgui/windows.hpp>
#include <libplimgui/statusbar.hpp>
#include <libplimgui/textviewer.hpp>
#include <libplimgui/roster.hpp>
#include <libplimgui/input.hpp>

#include "cReactor.hpp"
#include "cPlimConfig.hpp"

/* Test case */
#include "plugins/irc/cIrc.hpp"
#include "plugins/irc/cIrcSession.hpp"


extern NSConfig::cPlimConfig* SharedConfiguration;

namespace NSPlim {

#define PLIM_ROOM_DEBUG "PLIM_DEBUGWINDOW"

class cPlim	:	public	cApplication 
{

public:
	cPlim(int argc, char** argv);
	virtual ~cPlim( void );
	
	/* Loop 
	*/
	int loopMsg(void);
	
	/* Load a configuration file
		@return bool
	*/
	int loadConfiguration(const char* filename);
	
	/* Assign configuration to the UI 
	*/
	void assignConfiguration(cPlimConfig* config);
	
	/* Get the shared config file 
	*/
	cPlimConfig* getSharedConfig( void );
	
	/* Set the reactor 
	*/
	void setNuclearReactor(cReactor* reactor);
	cReactor* getNuclearReactor(void);
	
	/* Add some debug msg. 
	*/
	void addDbgMsg(const char* msg, ...);

		/* Public signals
	*/
	
protected:
	void buildInterface( void );
	void preBuildInterface( void );
	void bindSignals( void );
	void buildBindings( void );
	void destroyInterface( void );
	void parseInput(cString* cmd);

	/* Signals methods
	*/
	bool signalEnterInput(const char* buffer);
	bool signalBindingInput(cApplication* app, cString* command);
	
	/* Signals from the reactor 
	*/
	bool reactorSignalDebugMsg(NSAbstract::cAbstractSession* session, NSString::cString* message);
	bool reactorSignalCreateRoom(NSAbstract::cAbstractRoom* room);
	bool reactorSignalDestroyRoom(NSAbstract::cAbstractRoom* room);
	bool reactorSignalUserMessage(NSAbstract::cAbstractRoom* room, NSAbstract::cAbstractUser* user, NSString::cString* message);
	bool reactorSignalUserJoin(NSAbstract::cAbstractRoom* room, NSAbstract::cAbstractUser* user);
	bool reactorSignalUserPart(NSAbstract::cAbstractRoom* room, NSAbstract::cAbstractUser* user);

	/* Room windows
	*/
	cTextWindow* addRoom(const char* id);
	cTreeNode* getRoomNode(const char* id);
	cTextWindow* getRoom(const char* id);
	cTextWindow* deleteRoom(const char* id);
	
	void showRoom(const char* id);
private:
	cCursesWindow* m_root;
	cStatusWindow* m_statusBar;
	cStatusWindow* m_infoBar;
	cInputWindow* m_inputBar;
	cRosterWindow* m_rosterBar;
	cTextWindow* m_activeRoom;
	cTreeNode* m_activeNode;
	cTreeNodes* m_roomWindows;
	/*! Configuration */
	cPlimConfig* m_config;
	/*! home directory */
	cString* m_home;
	/*! Config file path */
	cString* m_homeConfig;
	/*! Nuclear Reactor */
	cReactor* m_nuclearReactor;
	/*! Test case */
	cIrc* m_irc;
	cIrcSession* m_ircSession;
};

};
