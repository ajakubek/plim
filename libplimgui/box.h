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

#ifndef __PLIM_BOX_H__
#define __PLIM_BOX_H__

#include <ncurses.h>
#include "windows.h"

namespace NSWindows {

/* TODO: More to add. */
class cBox {
public:
	cBox(cCursesWindow* window);
	~cBox(void);
	/* Update the sizes of window and it childs */
	void Update(void);
private:
	cCursesWindow* m_windowInstance;
	int m_lastLeftPos;
	int m_lastTopPos;
	int m_lastRightPos;
	int m_lastBottomPos;
};

};

#endif
