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

#ifndef __PLIM_INPUT_H__
#define __PLIM_INPUT_H__

#include <libplimuni/stdafx.hpp>
#include <libplimuni/strings.hpp>
#include <libplimuni/lexer.hpp>

#include "stdafx.hpp"
#include "application.hpp"
#include "windows.hpp"

namespace plimgui_curses {

/* Handle the input from keyboard and autoscale all */
class cInputWindow:	public	plimgui_curses::cCursesWindow 
{

public:
	cInputWindow(plimgui_curses::cApplication* app, plimgui_curses::cCursesWindow* parent);
	virtual ~cInputWindow(void);

	/* Accessors */
	void setPrefixPrompt(const char* prompt) 
	{ 
		m_prefixBuffer.copy( prompt ); 
	};
	
	const char* getPrefixPrompt(void) 
	{ 
		return m_prefixBuffer.getBuffer(); 
	};

	/* Internal */
	void partialUpdate(void);
	
	int onKeyPressed( const int key );
	int onResize(void);
 
	/* Signals */
	boost::signals2::signal <bool (const char*)> onEnter;
	
protected:
	int calcSize(void);
	void updateSize(void);
	
private:
	plimuni::cPlimLexer lexer;
	plimuni::cString m_prefixBuffer;
	plimuni::cString m_buffer;

	int m_cursorPos;
};

};

#endif
