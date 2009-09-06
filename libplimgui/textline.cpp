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

#include "textviewer.h"

namespace NSWindows {

cTextLine::cTextLine( cTreeNodes* nodes, cTreeNode* node, const char* str )
:	cTreeNode( nodes, node, NULL ),
	cCursesString( str ),
	m_user( NULL ),
	m_lineType( ltCustomMessage ) {
	m_time = time(NULL);
}

cTextLine::~cTextLine(void) {

}

time_t* cTextLine::GetTime(void) {
	return &m_time;
}

void cTextLine::SetUser(cAbstractUser* user) {
	m_user = user;
}

cAbstractUser* cTextLine::GetUser(void) {
	return m_user;
}

void cTextLine::SetLineType(eLineType type) {
	m_lineType = type;
}

eLineType cTextLine::GetLineType(void) {
	return m_lineType;
}

};
