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
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#include <libplimgui/strings.h>
#include <libplimgui/stringlist.h>

#include "../../cReactorSession.h"
#include "../../cReactorUser.h"

#include "cIrc.h"

namespace NSInternalPluginIRC {

using namespace NSReactor;
using namespace NSString;

class cIrcSession;

class cIrcProto {
public:
	cIrcProto(cIrcSession* session);
	virtual ~cIrcProto(void);
	void ParserInput(cString* string);
	int ParserIdentify(cPlimToken* token);
	void ParserServerMsg(cPlimToken* token);
	cPlimToken* ParserServerUserMsg(cPlimToken* token);
	cPlimToken* AfterParserServerUserMsg(cPlimToken* token, cString* nick, cString* user, cString* host, cString* type);
	void ServerCommand(cString* server, int command);
	void ServerUserMsg(cString* nick, cString* user, cString* host, cString* channel, cString* msg);
	void ServerJoinMsg(cString* nick, cString* user, cString* host, cString* channel);
	void ServerPartMsg(cString* nick, cString* user, cString* host, cString* channel);
private:
	cIrcSession* m_session;
	cPlimLexer lex;
};

class cIrcSession: public cReactorSession {
public:
	cIrcSession(cIrc* irc, const char* sessionName);
	virtual ~cIrcSession(void);

	void RegisterReactorInstance(cReactor* reactor);
	
	int Connect(void);
	int Disconnect(void);

	int Fission(fd_set *rfds, fd_set *wfds, fd_set *efds);
	int Release(fd_set *rfds, fd_set *wfds, fd_set *efds);

	/* Signals to UI
	*/
	void OnInputText(cString* buffer, cAbstractUser* user, cAbstractRoom* room);
	
	/* Accessors
	*/
	void SetUser(cReactorUser* user);
	cAbstractUser* GetUser(void);
	
	void SetIp(const char* host);
	const char* GetIp(void);
	
	void SetBindIp(const char* host);
	const char* GetBindIp(void);
	
	void SetPortNumber(int port);
	int GetPortNumber(void);

private:
	cReactorUser* m_myUser;
	cIrcProto* m_protoParser;
	cString m_ip;
	cString m_bindIp;
	int m_portNumber;
};

};

#endif
