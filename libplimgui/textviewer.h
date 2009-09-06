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

#include <stdarg.h>
#include <ncurses.h>
#include "application.h"
#include "windows.h"
#include "strings.h"
#include "cursesstring.h"
#include "treenodes.h"
#include "lexer.h"
#include "abstract.h"

namespace NSWindows {

#define REGISTER_F_CALL(name, method) RegisterFormattingCallback(name, (OnPrintCallback) &NSWindows::cTextWindow::method)
#define TRANSLATE_PTR(method) ((OnTranslateCallback) &NSWindows::cTextWindow::method)

using namespace NSApplication;
using namespace NSString;
using namespace NSAbstract;

typedef enum _LineType { 
	ltUserMessage,
	ltCustomMessage
} eLineType;

class cTextLine: public cTreeNode, public cCursesString {
public:
	cTextLine(cTreeNodes* nodes, cTreeNode* node, const char* str);
	virtual ~cTextLine(void);
	/* Accessors */
	time_t* GetTime(void);
	void SetUser(cAbstractUser* user);
	cAbstractUser* GetUser(void);
	void SetLineType(eLineType type);
	eLineType GetLineType(void);
private:
	time_t m_time;
	cAbstractUser* m_user;
	eLineType m_lineType;
};

class cTextWindow: public cCursesWindow {
public: 
	cTextWindow(cApplication* app, cCursesWindow* parent);
	virtual ~cTextWindow(void);

	/* Add a new line, it handles self the slib text format */
	cTextLine* AddLine(const char* buffer, cAbstractUser* user, ...);
	cTextLine* AddLine(const char* buffer, cAbstractUser* user, va_list args);

	cTextLine* AddDebugLine(const char* buffer, ...);
	cTextLine* AddDebugLine(const char* buffer, va_list args);

	/* TODO: Remove this, deprecated. @IC0ffeeCup */
	cTextLine* NewLine(const char* buffer, unsigned int uid);
	cTextLine* NewLine(cTextLine* line, const char* buffer, unsigned int uid);
	/* End. */

	void ScrollDown(int count);
	void ScrollUp(int count);
	void PageUp(void);
	void PageDown(void);
	int OnResize(void);
	void PartialUpdate(void);
	/* Callback for translating config nodes 
	*/
	char* TranslateCallback( cPlimToken* token, void* data );
	/* Sets the textviewer to skype conversation format
	*/
	void SetSlibUse(int slib);
	/* Assign the format of displaying the text in 1 <-> last text node
	*/
	void SetFirstText(const char* text);
	void SetSlibText(const char* text);
	void SetLastText(const char* text);
	
	/* Set the room 
	*/
	void SetRoom(cAbstractRoom* room);
	cAbstractRoom* GetRoom(void);
	
	cTextLine* GetFirstLine(void) { return (cTextLine*) m_lineBuffer->GetFirstNode(); };
	cTextLine* GetLastLine(void) { return (cTextLine*) m_lineBuffer->GetLastNode(); };
protected:
	int OnCppKeyword(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs);
	void Pin(void);
private: 
	/*! Lines */
	cTreeNodes* m_lineBuffer;
	cTreeNode* m_lineTop;
	/*! Line count*/
	int m_linesDrawed;
	/*! plim lexer*/
	cPlimLexer m_plimLexer;
	/*! First text buffer */
	cString m_firstText;
	cString m_slibText;
	cString m_lastText;
	/*! Use slib text, skype like conversation */
	int m_useSlibText;
	/*! Abstract room */
	cAbstractRoom* m_room;
};

};
#endif
