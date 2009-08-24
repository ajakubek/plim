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

#ifndef __PLIM_CURSESSTRING_H__
#define __PLIM_CURSESSTRING_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#include "strings.h"

namespace NSString
{

#define ATTR_BOLD			0x02
#define ATTR_COLOR		0x03
#define ATTR_RESET		0x0F
#define ATTR_FIXED		0x11
#define ATTR_REVERSE		0x12
#define ATTR_REVERSE2	0x16
#define ATTR_ITALIC		0x1D
#define ATTR_UNDERLINE	0x1F
#define ATTR_UNDERLINE2	0x15

class cCursesString: public cString
{
public:
	cCursesString(void);
	cCursesString(const char* str);
	virtual ~cCursesString(void);
	int GetFlags(int index, int* flags, int* colors);
	int IsSpecial(int index) { return GetChar(index) < 32; };
protected:

private:
};

};

#endif