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

#ifndef __PLIM_TEXTVIEWER_H__
#define __PLIM_TEXTVIEWER_H__

#include <libplimuni/stdafx.hpp>
#include <libplimuni/strings.hpp>
#include <libplimuni/treenodes.hpp>
#include <libplimuni/lexer.hpp>
#include <libplimuni/abstract.hpp>

#include "stdafx.hpp"
#include "application.hpp"
#include "windows.hpp"

namespace plimgui_curses {

#define REGISTER_F_CALL(name, method) registerFormattingCallback(name, (OnPrintCallback) &plimgui_curses::cTextWindow::method)
#define TRANSLATE_PTR(method) ((OnTranslateCallback) &plimgui_curses::cTextWindow::method)

typedef enum _LineType 
{ 
	ltUserMessage,
	ltCustomMessage
} 
eLineType;

class cTextLine	:	public	plimuni::cTreeNode, 
							public 	plimuni::cString 
{

public:
	cTextLine(plimuni::cTreeNodes* nodes, plimuni::cTreeNode* node, const char* str);
	virtual ~cTextLine(void);
	
	/* Accessors */
	time_t* getTime(void);
	
	void setUser(plimuni::cAbstractUser* user);
	plimuni::cAbstractUser* getUser(void);
	
	void setLineType(eLineType type);
	eLineType getLineType(void);
	
private:
	plimuni::cAbstractUser* m_user;
	time_t m_time;
	eLineType m_lineType;
};

class cTextWindow	:	public	plimgui_curses::cCursesWindow 
{

public: 
	cTextWindow(plimgui_curses::cApplication* app, plimgui_curses::cCursesWindow* parent);
	virtual ~cTextWindow(void);
	

	/* Add a new line, it handles self the slib text format */
	cTextLine* addLine(const char* buffer, plimuni::cAbstractUser* user, ...);
	cTextLine* addLine(const char* buffer, plimuni::cAbstractUser* user, va_list args);

	cTextLine* addDebugLine(const char* buffer, ...);
	cTextLine* addDebugLine(const char* buffer, va_list args);

	void scrollDown(int count);
	void scrollUp(int count);
	
	void pageUp(void);
	void pageDown(void);
	
	int onResize(void);
	
	void partialUpdate(void);
	/* Callback for translating config nodes 
	*/
	char* translateCallback( plimuni::cPlimToken* token, void* data );
	
	/* Sets the textviewer to skype conversation format
	*/
	void setSlibUse(int slib);
	
	/* Assign the format of displaying the text in 1 <-> last text node
	*/
	void setFirstText(const char* text);
	void setSlibText(const char* text);
	void setLastText(const char* text);
	
	/* Set the room 
	*/
	void setRoom(plimuni::cAbstractRoom* room);
	plimuni::cAbstractRoom* getRoom(void);
	
	
	cTextLine* getFirstLine(void) { return (cTextLine*) m_lineBuffer->getFirstNode(); };
	cTextLine* getLastLine(void) { return (cTextLine*) m_lineBuffer->getLastNode(); };
	
protected:
	int onCppKeyword(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs);
	
	void pin(void);
	
private: 
	plimuni::cAbstractRoom* m_room;	/*! Abstract room */
	plimuni::cPlimLexer m_plimLexer;	/*! plim lexer*/
	plimuni::cTreeNodes* m_lineBuffer;	/*! Lines */
	plimuni::cTreeNode* m_lineTop;
	plimuni::cString m_firstText;	/*! First text buffer */
	plimuni::cString m_slibText;	/*! Use slib text, skype like conversation */
	plimuni::cString m_lastText;
	/*! Line count*/
	int m_linesDrawed;
	int m_useSlibText;
};

};
#endif
