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

#include "stdafx.hpp"
#include "strings.hpp"

namespace plimuni 
{

class cAbstractSession
{

public:
	cAbstractSession(void)
	{};
	
	virtual ~cAbstractSession(void) 
	{};
	
	/* Accessors
	*/
	void name(const char* name)
	{ 
		m_sessionName.copy( name ); 
	};
	
	const char* name(void) 
	{ 
		return m_sessionName.getBuffer(); 
	};
	
private:
	plimuni::cString m_sessionName;
};

class cAbstractUser 
{

public:
	cAbstractUser(void) 
	{};
	
	virtual ~cAbstractUser(void) 
	{};
	
	void name(const char* name) 
	{ 
		m_userName.copy( name ); 
	};
	
	const char* name(void) 
	{ 
		return m_userName.getBuffer(); 
	};

private:
	plimuni::cString m_userName;
};

class cAbstractRoom 
{

public:
	cAbstractRoom(void) 
	{};
	
	virtual ~cAbstractRoom(void) 
	{};

	void name(const char* name) 
	{ 
		m_roomName.copy( name ); 
	};
	
	const char* name(void) 
	{ 
		return m_roomName.getBuffer(); 
	};
	
private:
	plimuni::cString m_roomName;
	
};

};


#endif