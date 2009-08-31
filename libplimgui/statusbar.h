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

#ifndef __PLIM_STATUSBAR_H__
#define __PLIM_STATUSBAR_H__

#include <ncurses.h>
#include <string.h>

#include "application.h"
#include "windows.h"
#include "hashnodes.h"
#include "strings.h"
#include "lexer.h"

namespace NSWindows {

#define REGISTER_S_CALL(name, method) RegisterFormattingCallback(name, (OnPrintCallback) &NSWindows::cStatusWindow::method)

using namespace NSApplication;
using namespace NSTree;
using namespace NSString;

class cStatusItem: public cTreeNode {
public:
	cStatusItem(cTreeNodes* nodes, const char* name): cTreeNode(nodes, NULL, NULL) {};
	virtual ~cStatusItem(void) {}
	void SetCaption(const char* caption) { m_buffer.Copy(caption); };
	const char* GetCaption(void) { return m_buffer.GetBuffer(); };
	int GetCaptionLength(void) { return m_buffer.GetLength(); };
	const char* GetName(void) { return m_name.GetBuffer(); };
	cStatusItem* GetNextNode(void) { return (cStatusItem*) cTreeNode::GetNextNode(); };
	cStatusItem* GetPrevNode(void) { return (cStatusItem*) cTreeNode::GetPrevNode(); };
private:
	cString m_buffer;
	cString m_name;
};

/* TODO: Add panels etc. */
class cStatusWindow: public cCursesWindow {
public:
	cStatusWindow(cApplication* app, cCursesWindow* parent);
	virtual ~cStatusWindow(void);

	cStatusItem* Add(const char* identifier);
	cStatusItem* Get(const char* identifier);
	void Delete(const char* identifier);

	void PartialUpdate(void);
protected:
	int OnOpenCloseItem(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs);
private:
	cTreeNodes* m_panelNodes;
	cPlimLexer lexhex;
};
 
};

#endif
