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

#ifndef __PLIM_WINDOWS_H__
#define __PLIM_WINDOWS_H__

#include <libplimuni/base.hpp>
#include <libplimuni/strings.hpp>
#include <libplimuni/treenodes.hpp>
#include <libplimuni/hashnodes.hpp>
#include <libplimuni/lexer.hpp>

#include "stdafx.hpp"
#include "application.hpp"

namespace plimgui_curses 
{

class cCursesWindow;

typedef enum _align { none = 0, left, top, bottom, right, client } align;

/* Structure on wich depends what the PrintLexer method will do */
typedef struct _PrintAttrs 
{
	int display: 1;
	int wrap: 1;
	int newline: 1;
	int reset: 1;
	int usecolor: 1;
	int noformcount: 1;
	int fg;
	int bg;
	int skipcount;
	int formcount;
	int flags;
} 
PlimAttrs;

/* Callback method */
typedef int (plimgui_curses::cCursesWindow::*OnPrintCallback) ( plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attr );
typedef char* (plimgui_curses::cCursesWindow::*OnTranslateCallback) ( plimuni::cPlimToken* token, void* data );

typedef struct _CallbackPtr 
{
	OnPrintCallback callback;
} 
CallbackPtr;

/*	ncurses base window class
	TODO: Add hide and show capabilities ie. newwin,subwin,delwin
	TODO: Suspend any drawings etc. when window is hidden (delwin)
	@author: IC0ffeeCup
*/ 
class cCursesWindow	:	public	plimuni::cTreeNode 
{

public:
	/* pos are relative to parent window */
	cCursesWindow(plimgui_curses::cApplication* app, int left, int top, int height, int width, cCursesWindow* parent);
	cCursesWindow(plimgui_curses::cApplication* app);
	virtual ~cCursesWindow();
	
	/*	Recreate window
	*/
	void recreateWindow(void);
	
	/*	Erase the window content
		@return none
	*/
	void erase(void);
	
	/*	Clear the window
		@return none
	*/
	void clear(void);
	
	/*	Mark the window for update
	*/
	void needUpdate(void);
	
	/*	Mark the window for repaint
	*/
	void needPartialUpdate(void);
	
	/*	Update the window sizes and repaint
	*/
	virtual void update(void);
	
	/* Force box update
	*/
	void forceUpdate(void);
	
	/*	Make a partial update without updating the sizes
	*/
	virtual void partialUpdate(void);
	
	/*	Print text in window
		@param cString
		@param start the line (ypos) from where we start
		@param valist
		@return number of lines used
	*/
	int print(plimuni::cString* string, int start); /* TODO: Add some overloaded Print and formating */
	int print(plimuni::cString* string, int x, int y); /* TODO: Add some overloaded Print and formating */
	
	/* Used for displaying text with predefined values from cPlimLexer
	*/
	int printLexer(plimuni::cPlimLexer* lexer, int x, int y);
	
	/* Handle special ncurses spec-chars
	*/
	int handleSpecChars(plimuni::cPlimToken* token, PlimAttrs* attrs);
	
	/*	Calculate text lines
		@param cString
		@return number of lines
	*/
	/* TODO: Add a routine that will take care about a lexer print size */
	int calculatePrint(plimuni::cString* string);
	
	/* Translate config variables
		@return cString
	*/
	plimuni::cString* translateConfigVariables(const char* buffer, OnTranslateCallback call, void* data);
	
	/*	Accessors */
	/*	Get the focus of the window
		@return window has focus either not
	*/
	int isFocused(void) 
	{ 
		return m_hasFocus; 
	};
	
	/*	Set the window focus, all input will be redirected to this window
		@param focus bool val
	*/
	void setFocus(int focus) 
	{ 
		if (m_hasFocus != focus) 
		{ 
			m_hasFocus = focus; /* TODO: some event */ 
		} 
	};
	
	/*	Get the visibility of the window
		@return visibility bool val
	*/
	int isVisible(void) 
	{ 
		return m_isVisible; 
	};
	
	/*	Set the visiblity of the window
		@param visibility bool val
	*/
	void setVisible(int visible) 
	{ 
		if (m_isVisible != visible) 
		{ 
			m_isVisible = visible; 
			recreateWindow(); 
			needUpdate(); 
			needPartialUpdate();  
		} 
	};
	
	void setColorPair(int pair) 
	{ 
		m_colorPair = pair; 
	};
	
	/*	Get the parent window
		@return cCursesWindow
	*/
	cCursesWindow* getParentWindow(void) 
	{ 
		return m_windowParent; 
	};
	
	/*	Get the parent window handle
		@return WINDOW structure
	*/
	::WINDOW* getParentWindowHandle(void) 
	{ 
		if (!m_windowParent) 
			return NULL; 
		return m_windowParent->getWindowHandle(); 
	};
	
	/*	Get the window handle
		@return WINDOW structure
	*/
	::WINDOW* getWindowHandle(void) 
	{ 
		return m_windowHandle; 
	};
	
	/*	Set the window handle, used for a fake root window mostly
		@param windowHandle, a curses WINDOW structure
	*/
	void setWindowHandle(::WINDOW* windowHandle) 
	{ 
		m_windowHandle = windowHandle; 
	};
	
	/*	Get the first window
		@return cCursesWindow
	*/
	cCursesWindow* getFirstWindow(void) 
	{ 
		return (cCursesWindow*) getFirstNode(); 
	}
	
	/*	Get the last window
		@return cCursesWindow
	*/
	cCursesWindow* getLastWindow(void) 
	{ 
		return (cCursesWindow*) getLastNode(); 
	}
	
	/*	Set the window data
		@param void*
	*/
	void setWindowData(void* data) 
	{ 
		m_windowData = data; 
	};
	
	/*	Get the window data
		@return void*
	*/
	void* getWindowData(void) 
	{ 
		return m_windowData; 
	};
	
	/*	Set the window align
		@param align
	*/
	void setWindowAlign(align a) 
	{ 
		m_windowAlign = a; 
		if (m_windowParent) 
		{ 
			m_windowParent->needUpdate(); 
			m_windowParent->update(); 
		} 
	};
	
	/*	Get the window align
		@return align
	*/
	align getWindowAlign(void) 
	{ 
		return m_windowAlign; 
	};
	
	/*	Get x raw of the window
	*/
	int getRawLeft(void) 
	{  
		return m_xPos; 
	};
	
	/*	Get x of the window
	*/
	int getLeft(void) 
	{ 
		if (!m_windowParent) 
			return 0; 
		return m_xPos + m_windowParent->getLeft(); 
	};
	
	/*	Get y raw of the window
	*/
	int getRawTop(void) 
	{  
		return m_yPos; 
	}
	
	/*	Get y of the window
	*/
	int getTop(void) 
	{ 
		if (!m_windowParent) 
			return 0; 
		return m_yPos + m_windowParent->getTop(); 
	};
	
	/*	Get width of the window
	*/
	int getWidth(void) 
	{ 
		if (!m_windowParent) 
			return ::getmaxx(m_windowHandle); 
		return m_width; 
	};
	
	/*	Get height of the window
	*/
	int getHeight(void) 
	{ 
		if (!m_windowParent) 
			return ::getmaxy(m_windowHandle);
		return m_height; 
	};
	
	/*	Set the x of the window
	*/
	void setLeft(int left) 
	{ 
		m_sizeChanged = m_xPos != left; 
		m_xPos = left; 
	};
	
	/*	Set the y of the window
	*/
	void setTop(int top) 
	{ 
		m_sizeChanged = m_yPos != top; 
		m_yPos = top;  
	};
	
	/* Set the width of the window
	*/
	void setWidth(int width) 
	{ 
		m_sizeChanged = m_width != width; 
		m_width = width; 
	};
	
	/* Set the height of the window
	*/
	void setHeight(int height) 
	{ 
		m_sizeChanged = m_height != height; 
		m_height = height; 
	};
	
	/* Set dims
	*/
	void setDimension(int left, int top, int width, int height) 
	{
		setLeft(left); setTop(top);
		setWidth(width); setHeight(height);
		/* The NeedUpdate should be called by the cableguy */
	}
	
	/* If the size changed */
	int isSizeChanged(void) 
	{ 
		return m_sizeChanged; 
	}
	
	/*	If the window is a child
	*/
	int isChild(void) 
	{ 
		return m_windowParent != NULL; 
	};
	
	/*	If the window needs update */
	int isUpdateNeccesary(void) 
	{ 
		return m_needUpdate; 
	};
	
	/*	If the window needs partial update */
	int isPartialNeccesary(void) 
	{ 
		return m_needPartialUpdate && m_windowHandle; 
	};
	
	/*	Launch a keypress event
		@param key, int
		@return int, if -1 than break Loop
	*/
	cCursesWindow* getNextNode(void) 
	{ 
		return (cCursesWindow*) plimuni::cTreeNode::getNextNode(); 
	};
	
	cCursesWindow* getPrevNode(void) 
	{ 
		return (cCursesWindow*) plimuni::cTreeNode::getPrevNode(); 
	};
	
	/* Get the application instance
	*/
	cApplication* getApplication(void) 
	{ 
		return m_appInstance; 
	};

	/* Create and destroy hashnodes */
	void createFormattingNodes(void);
	void destroyFormattingNodes(void);

	/* Sets the formatting nodes for PrintLexer */
	void setFormattingNodes(plimuni::cHashNodes* nodes) 
	{ 
		m_formatNodes = nodes; 
	};
	
	plimuni::cHashNodes* getFormattingNodes(void) 
	{ 
		return m_formatNodes; 
	};

 	/* Register formatting routine for PrintLexer */
	int registerFormattingCallback( const char* identifier, OnPrintCallback callback );
	int unregisterFormattingCallback( const char* identifier );
	
	CallbackPtr* getFormattingCallback( const char* identifier );

	virtual int onKeyPressed( const int key ); /* TODO: Need to change args ie. implement keybindings, add some buffer */
	virtual int onResize(void);
protected:

private:
	plimuni::cHashNodes* m_formatNodes;	/*! Hash nodes for PrintLexer callbacks */
	cApplication* m_appInstance;/*! cApplication instance */
	cCursesWindow* m_windowParent;	/*! parent window */
	::WINDOW* m_windowHandle;		/*! WINDOW structure derived by ncurses */
	int m_xPos; /*! WINDOW coordinates */
	int m_yPos;
	int m_width;
	int m_height;
	int m_canTabStop;/*! Indicates if the window can handle focus */
	int	m_hasFocus;/*! Indicates if the window is in focus state */
	int m_isVisible;	/*! Idicates visibility of the window */
	int m_needUpdate;	/*! Force the window to update */
	int m_needPartialUpdate;	/*! Force the window to repaint */
	int m_sizeChanged;
	cCursesWindow* m_windowFirst;	/*! First sub window */
	cCursesWindow* m_windowLast;	/*! Last sub window */
	void* m_windowData;	/*! custom data */
	align m_windowAlign;	/*! align of the window */
	void* m_boxPtr;	/*!	pointer to cBox */
	int m_colorPair;
};


};

#endif
