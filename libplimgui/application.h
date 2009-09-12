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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <sigc++/signal.h>

#include "base.h"
#include "treenodes.h"
#include "descriptor.h"
#include "keyboard.h"
#include "pallete.h"

namespace NSApplication {

using namespace NSTree;

/*	ncurses base application class, with cTreeNodes we can group our windows and inspect it in nice way
	@author: IC0ffeeCup
*/
/* TODO: add signals */
class cApplication: public cTreeNodes, public cKeyboard, public sigc::trackable {
public:
	/*	base construction for the main application
		@param argc argument count
		@param argv argument variables
	*/
	cApplication(int argc, char** argv);
	virtual ~cApplication(void);
	/* Close the app
	*/
	void Close(void) { m_endApp = 1; };
	/*	Process messages in application
		@return application exit code
	*/
	int Loop(void);
	/*	Get the first window 
		@return cCursesWindow
	*/
	cTreeNode* GetFirstWindow(void) { return GetFirstNode(); };
	/*	Get the last window
		@return cCursesWindow
	*/
	cTreeNode* GetLastWindow(void) { return GetLastNode(); };

	cDescriptor* GetFirstDescriptor(void) { return (cDescriptor*) m_descriptors->GetFirstNode(); };
	cDescriptor* GetLastDescriptor(void) { return (cDescriptor*) m_descriptors->GetLastNode(); };

	/*	get the terminal lines 
		@return number of lines in terminal window
	*/
	int GetTermLines(void) { return getmaxy(m_rootWindow); };
	/*	get the term cols
		@return number of the columns in the terminal window
	*/
	int GetTermCols(void) { return getmaxx(m_rootWindow); };
	/*	get the pair of colors
	*/
	int GetColorPair(int fg, int bg) { return m_colors->GetPair(fg, bg); };
	/*	get the root window handle
		@return ncurses WINDOW struct
	*/ 
	WINDOW* GetWindowHandle(void) { return m_rootWindow; };
	/* Terminal resize event */
	virtual int OnTerminalSizeChanged(void);

	/* Signals */
	sigc::signal<void, const int> OnKeyPress;
	sigc::signal<void, cApplication*, cString*> OnBindingPress;
	sigc::signal<void, int, int> OnResize;
	sigc::signal<void> OnPostLoop;
protected:
	/*	Launch a keypress event 
		@param key, int
		@return int, if -1 than break Loop
	*/
	int OnKeyClicked( const int key );
	/* Get a command and launch a event
	*/
	void OnBindingClicked(void);
	/* base loop msg for processing all the msg */
	virtual int LoopMsg(void);
	/* Runned just after the LoopMsg in Loop */
	virtual void PostLoopMsg(void) { /* Dummy */ };
private:
	void PartialUpdateWindows(void);
	int OnKeyEvent( const int key );
	void LaunchResizeEvents(void);
	void FreeIterator(void);
	/*!root window ie. initscr() from ncurses*/
	WINDOW* m_rootWindow;
	/*!terminal width*/
	int m_termWidth;
	/*!terminal height*/
	int m_termHeight;
	/*!descriptors*/
	cTreeNodes* m_descriptors;
	/*!timeout*/
	long m_timeout;
	/*!endloop*/
	int m_endApp;
	/*! Colors */
	cPallete* m_colors;

};

};

#endif
