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

#include "pallete.h"

namespace NSApplication {

cPallete::cPallete(void)
:	cTreeNodes() {
	/* Initialize the default colors, need some tuneup */
	for (int i = 1; i < 64; i++ )
		new cColor( this, i, (int) (i / 8), i % 7);
}

cPallete::~cPallete(void) {
}

int cPallete::GetPair(int fg, int bg) {
	cColor* color;
	int ret;

	color = GetFirstNode();

	while (color) {
		
		if ((ret = color->IsPair(fg, bg)) > -1)
		{

			return ret;
		}
		color = color->GetNextNode();
	} 

	return -1;
}

};
