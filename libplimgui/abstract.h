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

#ifndef __PLIM_ABSTRACT_H__
#define __PLIM_ABSTRACT_H__

/* Used in reactor plugins. */
/* Plugin => Sessions -> {Session[proto:connection] -> {room=>{users}} | {room=>{user} } single user conversation */

#include "strings.h"

namespace NSAbstract {

using namespace NSString;

class cAbstractProtocol {
public:
	cAbstractProtocol(void): m_socket(-1) {};
	virtual ~cAbstractProtocol(void) {} ;
	/* Connect, disco
	*/
	virtual int Connect(void) = 0;
	virtual int Disconnect(void) = 0;
	/* Protocol recv, send
	*/
	virtual int Fission(fd_set *rfds, fd_set *wfds, fd_set *efds) = 0;
	virtual int Release(fd_set *rfds, fd_set *wfds, fd_set *efds) = 0;
	/* Accessors
	*/
	void SetSocket(int s) { m_socket = s; };
	int GetSocket(void) { return m_socket; };
private:
	int m_socket;
};

class cAbstractSession: public cAbstractProtocol {
public:
	cAbstractSession(void): cAbstractProtocol() {};
	virtual ~cAbstractSession(void) {};
	/* Accessors
	*/
	void SetSessionName(const char* name) { m_sessionName.Copy( name ); };
	const char* GetSessionName(void) { return m_sessionName.GetBuffer(); };
private:
	cString m_sessionName;
};

class cAbstractUser {
public:
	cAbstractUser(void) {};
	virtual ~cAbstractUser(void) {};
	/* Accessors
	*/
	void SetUserName(const char* name) { m_userName.Copy( name ); };
	const char* GetUserName(void) { return m_userName.GetBuffer(); };
private:
	cString m_userName;
};

class cAbstractRoom {
public:
	cAbstractRoom(void) {};
	virtual ~cAbstractRoom(void) {};
	/* Accessors
	*/
	void SetRoomName(const char* name) { m_roomName.Copy( name ); };
	const char* GetRoomName(void) { return m_roomName.GetBuffer(); };
private:
	cString m_roomName;
};

};


#endif
