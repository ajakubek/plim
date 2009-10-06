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

#ifndef __PLIM_APPLICATION_H__
#define __PLIM_APPLICATION_H__

#include <libplimuni/stdafx.hpp>
#include <libplimuni/base.hpp>
#include <libplimuni/treenodes.hpp>

#include "stdafx.hpp"
#include "keyboard.hpp"
#include "pallete.hpp"

namespace plimgui_curses {

/*	ncurses base application class, with cTreeNodes we can group our windows and inspect it in nice way
	@author: IC0ffeeCup
*/

class cApplication:	public 	plimuni::cTreeNodes,
							public	plimgui_curses::cKeyboard
{

public:
	/*	base construction for the main application
		@param argc argument count
		@param argv argument variables
	*/
	cApplication(int argc, char** argv);
	virtual ~cApplication(void);
	
	/* Close the app
	*/
	void close(void) 
	{ 
		m_endApp = 1; 
	};
	
	/*	Process messages in application
		@return application exit code
	*/
	int loop(void);
	
	/*	Get the first window 
		@return cCursesWindow
	*/
	plimuni::cTreeNode* getFirstWindow(void) 
	{ 
		return getFirstNode(); 
	};
	
	/*	Get the last window
		@return cCursesWindow
	*/
	plimuni::cTreeNode* getLastWindow(void) 
	{ 
		return getLastNode(); 
	};

	/*	get the terminal lines 
		@return number of lines in terminal window
	*/
	int getTermLines(void) 
	{ 
		return ::getmaxy(m_rootWindow); 
	};
	
	/*	get the term cols
		@return number of the columns in the terminal window
	*/
	int getTermCols(void) 
	{ 
		return ::getmaxx(m_rootWindow); 
	};
	
	/*	get the pair of colors
	*/
	int getColorPair(int fg, int bg) 
	{ 
		return m_colors->getPair(fg, bg); 
	};
	
	/*	get the root window handle
		@return ncurses WINDOW struct
	*/
	::WINDOW* getWindowHandle(void) 
	{ 
		return m_rootWindow; 
	};
	
	/* Terminal resize event */
	virtual int onTerminalSizeChanged(void);

	/* Signals */
	boost::signals2::signal <bool (const int)> onKeyPress;
	boost::signals2::signal<bool (cApplication*, plimuni::cString*)> onBindingPress;
	boost::signals2::signal<bool (int, int)> onResize;
	boost::signals2::signal<bool ()> onPostLoop;

protected:
	/*	Launch a keypress event 
		@param key, int
		@return int, if -1 than break Loop
	*/
	int onKeyClicked( const int key );
	
	/* Get a command and launch a event
	*/
	void onBindingClicked(void);
	
	/* base loop msg for processing all the msg */
	virtual int loopMsg(void);
	
	/* Runned just after the LoopMsg in Loop */
	virtual void postLoopMsg(void) 
	{};
	
private:
	void partialUpdateWindows(void);
	int onKeyEvent( const int key );
	void launchResizeEvents(void);

protected:
	void freeIterator(void);

private:
	plimgui_curses::cPallete* m_colors;
	::WINDOW* m_rootWindow;
	/*!terminal width*/
	int m_termWidth;
	/*!terminal height*/
	int m_termHeight;
	/*!timeout*/
	long m_timeout;
	/*!endloop*/
	int m_endApp;
	/*! Colors */
};

};

#endif
