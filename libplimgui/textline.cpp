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

#include "textviewer.hpp"

namespace plimgui_curses {

cTextLine::cTextLine( plimuni::cTreeNodes* nodes, plimuni::cTreeNode* node, const char* str )	:	plimuni::cTreeNode( nodes, node, NULL ),
																																	plimuni::cString( str ),
																																	m_user( NULL ),
																																	m_lineType( ltCustomMessage ) 
{
	m_time = time(NULL);
}

cTextLine::~cTextLine(void) 
{}

time_t* cTextLine::getTime(void) 
{
	return &m_time;
}

void cTextLine::setUser(plimuni::cAbstractUser* user) 
{
	m_user = user;
}

plimuni::cAbstractUser* cTextLine::getUser(void) 
{
	return m_user;
}

void cTextLine::setLineType(eLineType type) 
{
	m_lineType = type;
}

eLineType cTextLine::getLineType(void) 
{
	return m_lineType;
}

};
