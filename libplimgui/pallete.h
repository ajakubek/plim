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

#include <ncurses.h>
#include "treenodes.h"

using namespace NSTree;

/* Simple init_pair interface for curses */
namespace NSApplication {

/* Could use here the 256 color ncurses if compiled in ncurses */

class cColor: public cTreeNode {
public:
	cColor(cTreeNodes* nodes, int pair, int fg, int bg)
	:	cTreeNode(nodes, NULL, NULL),
		m_fg(fg), m_bg(bg), m_pair(pair) {
		if (!fg) fg = -1;
		if (!bg) bg = -1;
		init_pair(pair, fg, bg);
	};
	virtual ~cColor(void){};
	/* Returns the pair of cols, else -1 */
	int IsPair(int fg, int bg) {
		if (fg == -1) fg = 0;
		if (bg == -1) bg = 0;
		if ( m_fg == fg && m_bg == bg )
			return m_pair;
		return -1;
	};
	cColor* GetNextNode(void) { return (cColor*) cTreeNode::GetNextNode(); };
	cColor* GetPrevNode(void) { return (cColor*) cTreeNode::GetPrevNode(); };
private:
	int m_pair;
	int m_fg;
	int m_bg;
};

class cPallete: public cTreeNodes {
public: 
	cPallete(void);
	virtual ~cPallete(void);
	cColor* GetFirstNode(void) { return (cColor*) cTreeNodes::GetFirstNode(); };
	cColor* GetLastNode(void) { return (cColor*) cTreeNodes::GetLastNode(); };
	/* returns -1 on false */
	int GetPair(int fg, int bg);
};

};

#endif
