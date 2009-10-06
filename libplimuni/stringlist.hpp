/***************************************************************************
 *   Copyright (C) 2009 by Lukasz M. Borzecki                              *
 *   lukasz.borzecki@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __PLIM_STRINGLIST_H__
#define __PLIM_STRINGLIST_H__

#include "strings.hpp"

namespace plimuni {

class cStringListItem:	public	plimuni::cString 
{

public:
	cStringListItem(const char* buffer, const int len)	:	plimuni::cString(), 
																			m_next(NULL) 
	{
		copy( buffer, len );
	};
	
	virtual ~cStringListItem(void) 
	{};
	
	cStringListItem* m_next;
};

class cStringList 
{

public:
	cStringList( void );
	virtual ~cStringList( void );
	
	void queue( const char* buffer, const int len );
	cStringListItem* dequeue( void );

private:
	cStringListItem* m_frontQueue;
	cStringListItem* m_backQueue;
};

};

#endif
