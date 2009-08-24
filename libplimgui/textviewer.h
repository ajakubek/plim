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

#include <ncurses.h>
#include "application.h"
#include "windows.h"
#include "strings.h"
#include "cursesstring.h"
#include "treenodes.h"
#include "lexer.h"

namespace NSWindows {

#define REGISTER_F_CALL(name, method) RegisterFormattingCallback(name, (OnPrintCallback) &NSWindows::cTextWindow::method)
using namespace NSApplication;
using namespace NSString;

class cTextLine: public cTreeNode, public cCursesString {
public:
	cTextLine(cTreeNodes* nodes, cTreeNode* node, const char* str, unsigned int uid);
	virtual ~cTextLine(void);
};


class cTextWindow: public cCursesWindow {
public: 
	cTextWindow(cApplication* app, cCursesWindow* parent);
	virtual ~cTextWindow(void);
	cTextLine* NewLine(const char* buffer, unsigned int uid);
	cTextLine* NewLine(cTextLine* line, const char* buffer, unsigned int uid);
	void ScrollDown(int count);
	void ScrollUp(int count);
	void PageUp(void);
	void PageDown(void);
	int OnResize(void);
	void PartialUpdate(void);
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
};

};
#endif
