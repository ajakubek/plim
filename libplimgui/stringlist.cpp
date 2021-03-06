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

#include "stringlist.h"

namespace NSString {

cStringList::cStringList( void )
:	m_frontQueue( NULL ),
	m_backQueue( NULL ) {

}

cStringList::~cStringList( void ) {
	while (cStringListItem *i = Dequeue())
		delete i;
}

void cStringList::Queue( const char* buffer, const int len ) {
	cStringListItem* i;

	if (!buffer || !len) {
		return;
	}

	i = new cStringListItem( buffer, len );

	if (!m_frontQueue)
		m_backQueue = m_frontQueue = i;
	else {
		m_backQueue->m_next = i;
		m_backQueue = i;
	}
}

cStringListItem* cStringList::Dequeue( void ) {
	cStringListItem* i;

	if ( m_frontQueue ) {
		i = m_frontQueue;
		m_frontQueue = m_frontQueue->m_next;
		return i;
	}

	return NULL;
}

};
