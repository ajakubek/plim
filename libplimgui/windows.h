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

#include <ncurses.h>

#include "application.h"
#include "base.h"
#include "strings.h"
#include "treenodes.h"

namespace NSWindows {

using namespace NSApplication;
using namespace NSString;

typedef enum _align { none = 0, left, top, bottom, right, client } align;

/*	ncurses base window class
	TODO: Add hide and show capabilities ie. newwin,subwin,delwin
	TODO: Suspend any drawings etc. when window is hidden (delwin)
	@author: IC0ffeeCup
*/
class cCursesWindow: public cTreeNode {
public:
	/* pos are relative to parent window */
	cCursesWindow(cApplication* app, int left, int top, int height, int width, cCursesWindow* parent);
	cCursesWindow(cApplication* app);
	virtual ~cCursesWindow();
	/*	Recreate window
	*/
	void RecreateWindow(void);
	/*	Erase the window content 
		@return none 
	*/
	void Erase(void);
	/*	Clear the window
		@return none
	*/
	void Clear(void);
	/*	Mark the window for update 
	*/
	void NeedUpdate(void);
	/*	Mark the window for repaint 
	*/
	void NeedPartialUpdate(void);
	/*	Update the window sizes and repaint 
	*/
	virtual void Update(void);
	/*	Make a partial update without updating the sizes 
	*/
	virtual void PartialUpdate(void);
	/*	Print text in window
		@param cString
		@param start the line (ypos) from where we start
		@param valist
		@return number of lines used
	*/
	int Print(cString* string, int start); /* TODO: Add some overloaded Print and formating */
	/*	Accessors */
	/*	Get the focus of the window
		@return window has focus either not 
	*/
	int IsFocused(void) { return m_hasFocus; };
	/*	Set the window focus, all input will be redirected to this window
		@param focus bool val
	*/
	void SetFocus(int focus) { if (m_hasFocus != focus) { m_hasFocus = focus; /* TODO: some event */ } };
	/*	Get the visibility of the window 
		@return visibility bool val
	*/
	int IsVisible(void) { return m_isVisible; };
	/*	Set the visiblity of the window
		@param visibility bool val
	*/
	void SetVisible(int visible) { if (m_isVisible != visible) { m_isVisible = visible; } };
	void SetColorPair(int pair) { m_colorPair = pair; };
	/*	Get the parent window
		@return cCursesWindow
	*/
	cCursesWindow* GetParentWindow(void) { return m_windowParent; };
	/*	Get the parent window handle
		@return WINDOW structure
	*/
	WINDOW* GetParentWindowHandle(void) { if (!m_windowParent) return NULL; return m_windowParent->GetWindowHandle(); };
	/*	Get the window handle 
		@return WINDOW structure
	*/
	WINDOW* GetWindowHandle(void) { return m_windowHandle; };
	/*	Set the window handle, used for a fake root window mostly 
		@param windowHandle, a curses WINDOW structure
	*/
	void SetWindowHandle(WINDOW* windowHandle) { m_windowHandle = windowHandle; };
	/*	Get the first window 
		@return cCursesWindow
	*/
	cCursesWindow* GetFirstWindow(void) { return (cCursesWindow*) GetFirstNode(); }
	/*	Get the last window
		@return cCursesWindow
	*/
	cCursesWindow* GetLastWindow(void) { return (cCursesWindow*) GetLastNode(); }
	/*	Set the window data 
		@param void* 
	*/
	void SetWindowData(void* data) { m_windowData = data; };
	/*	Get the window data
		@return void*
	*/
	void* GetWindowData(void) { return m_windowData; };
	/*	Set the window align
		@param align
	*/
	void SetWindowAlign(align a) { m_windowAlign = a; if (m_windowParent) { m_windowParent->NeedUpdate(); m_windowParent->Update(); } };
	/*	Get the window align
		@return align
	*/
	align GetWindowAlign(void) { return m_windowAlign; };
	/*	Get x raw of the window 
	*/
	int GetRawLeft(void) {  return m_xPos; }
	/*	Get x of the window 
	*/
	int GetLeft(void) { if (!m_windowParent) return 0; return m_xPos + m_windowParent->GetLeft() ; };
	/*	Get y raw of the window
	*/
	int GetRawTop(void) {  return m_yPos; }
	/*	Get y of the window 
	*/
	int GetTop(void) { if (!m_windowParent) return 0; return m_yPos + m_windowParent->GetTop(); };
	/*	Get width of the window 
	*/
	int GetWidth(void) { if (!m_windowParent) return getmaxx(m_windowHandle) ; return m_width; };
	/*	Get height of the window 
	*/
	int GetHeight(void) { if (!m_windowParent) return getmaxy(m_windowHandle); return m_height; };
	/*	Set the x of the window 
	*/
	void SetLeft(int left) { m_xPos = left; };
	/*	Set the y of the window 
	*/
	void SetTop(int top) { m_yPos = top;  };
	/* Set the width of the window 
	*/
	void SetWidth(int width) { m_width = width; };
	/* Set the height of the window 
	*/
	void SetHeight(int height) { m_height = height; };
	/* Set dims 
	*/
	void SetDimension(int left, int top, int width, int height) { 
		SetLeft(left); SetTop(top);
		SetWidth(width); SetHeight(height);
		/* The NeedUpdate should be called by the cableguy */
	}
	/*	If the window is a child 
	*/
	int IsChild(void) { return m_windowParent != NULL; };
	/*	If the window needs update */
	int IsUpdateNeccesary(void) { return m_needUpdate; };
	/*	If the window needs partial update */
	int IsPartialNeccesary(void) { return m_needPartialUpdate; };
	/*	Launch a keypress event 
		@param key, int
		@return int, if -1 than break Loop
	*/
	virtual int OnKeyPressed( const int key ); /* TODO: Need to change args ie. implement keybindings, add some buffer */
	virtual int OnResize(void);
	cCursesWindow* GetNextNode(void) { return (cCursesWindow*) cTreeNode::GetNextNode(); };
	cCursesWindow* GetPrevNode(void) { return (cCursesWindow*) cTreeNode::GetPrevNode(); };
protected:

private:
	/*! cApplication instance */
	cApplication* m_appInstance;
	/*! parent window */
	cCursesWindow* m_windowParent;
	/*! WINDOW structure derived by ncurses */
	WINDOW* m_windowHandle;
	/*! WINDOW coordinates */
	int m_xPos;
	int m_yPos;
	int m_width;
	int m_height;
	/*! Indicates if the window can handle focus */
	int m_canTabStop;
	/*! Indicates if the window is in focus state */
	int	m_hasFocus;
	/*! Idicates visibility of the window */
	int m_isVisible;
	/*! Force the window to update */
	int m_needUpdate;
	/*! Force the window to repaint */
	int m_needPartialUpdate;
	/*! First sub window */
	cCursesWindow* m_windowFirst;
	/*! Last sub window */
	cCursesWindow* m_windowLast;
	/*! custom data */
	void* m_windowData;
	/*! align of the window */
	align m_windowAlign;
	/*!	pointer to cBox */
	void* m_boxPtr;
	int m_colorPair;
};

};

#endif
