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

#ifndef __PLIM_PALLETE_H__
#define __PLIM_PALLETE_H__

#include <libplimuni/stdafx.hpp>
#include <libplimuni/treenodes.hpp>

#include "stdafx.hpp"

/* Simple init_pair interface for curses */
namespace plimgui_curses {

/* Could use here the 256 color if compiled in ncurses */

class cColor:	public	plimuni::cTreeNode 
{

public:
	cColor(plimuni::cTreeNodes* nodes, int pair, int fg, int bg)	:	plimuni::cTreeNode(nodes, NULL, NULL),
																							m_fg(fg), 
																							m_bg(bg), 
																							m_pair(pair) 
	{
		if (!fg) fg = -1;
		if (!bg) bg = -1;
		::init_pair(pair, fg, bg);
	};
	
	virtual ~cColor(void)
	{};
	
	/* Returns the pair of cols, else -1 */
	int isPair(int fg, int bg) 
	{
		if (fg == -1) fg = 0;
		if (bg == -1) bg = 0;
		if ( m_fg == fg && m_bg == bg )
			return m_pair;
		return -1;
	};
	
	cColor* getNextNode(void) 
	{ 
		return (cColor*) plimuni::cTreeNode::getNextNode(); 
	};
	
	cColor* getPrevNode(void) 
	{ 
		return (cColor*) plimuni::cTreeNode::getPrevNode(); 
	};

private:
	int m_pair;
	int m_fg;
	int m_bg;
};

class cPallete	:	public	plimuni::cTreeNodes 
{

public: 
	cPallete(void);
	virtual ~cPallete(void);
	
	cColor* getFirstNode(void) 
	{
		return (cColor*) plimuni::cTreeNodes::getFirstNode(); 
	};
	
	cColor* getLastNode(void) 
	{
		return (cColor*) plimuni::cTreeNodes::getLastNode(); 
	};
	
	/* returns -1 on false */
	int getPair(int fg, int bg);
};

};

#endif
