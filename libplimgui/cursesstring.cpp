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

#include "cursesstring.h"

namespace NSString {

cCursesString::cCursesString(void)
:	cString() {

}

cCursesString::cCursesString(const char* str)
:	cString() {
	Copy(str);
}

cCursesString::~cCursesString(void) {

}

int cCursesString::GetFlags(int index, int* flags, int* colors) {
	char ch = GetChar(index);

	*colors = 0;

	if ( IsSpecial(index) ) {
		switch (ch)
		{
			case ATTR_BOLD: {
				*flags |= A_BOLD;
				break;
			}
			case ATTR_COLOR: {
				*colors = 1;
				break;
			}

			case ATTR_RESET: {
				*flags = ATTR_RESET;
				break;
			}

			case ATTR_FIXED: {
				/* dunno */
				break;
			}

			case ATTR_REVERSE:
			case ATTR_REVERSE2: {
				*flags |= A_REVERSE;
				break;
			}

			case ATTR_ITALIC: {
				/* Dunno, ncurses doesnt support italic stuff */
				break;
			}

			case ATTR_UNDERLINE2:
			case ATTR_UNDERLINE: {
				*flags |= A_UNDERLINE;
				break;
			}

			default:
				break;
		}

	}

	return 0;
}

};
