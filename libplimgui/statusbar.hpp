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

#include <libplimuni/stdafx.hpp>
#include <libplimuni/hashnodes.hpp>
#include <libplimuni/strings.hpp>
#include <libplimuni/lexer.hpp>

#include "stdafx.hpp"
#include "application.hpp"
#include "windows.hpp"

namespace plimgui_curses {

#define REGISTER_S_CALL(name, method) registerFormattingCallback(name, (OnPrintCallback) &plimgui_curses::cStatusWindow::method)


class cStatusItem	:	public	plimuni::cTreeNode 
{

public:
	cStatusItem(plimuni::cTreeNodes* nodes, const char* name)	:	plimuni::cTreeNode(nodes, NULL, NULL) 
	{};
	
	virtual ~cStatusItem(void) 
	{};
	
	void setCaption(const char* caption) 
	{ 
		m_buffer.copy(caption); 
	};
	
	const char* getCaption(void) 
	{ 
		return m_buffer.getBuffer(); 
	};
	
	int getCaptionLength(void)
	{ 
		return m_buffer.getLength(); 
	};
	
	const char* getName(void) 
	{ 
		return m_name.getBuffer(); 
	};
	
	cStatusItem* getNextNode(void) 
	{ 
		return (cStatusItem*) plimuni::cTreeNode::getNextNode(); 
	};
	
	cStatusItem* getPrevNode(void) 
	{ 
		return (cStatusItem*) plimuni::cTreeNode::getPrevNode(); 
	};
	
private:
	plimuni::cString m_buffer;
	plimuni::cString m_name;
	
};

/* TODO: Add panels etc. */
class cStatusWindow	:	public	plimgui_curses::cCursesWindow 
{

public:
	cStatusWindow(plimgui_curses::cApplication* app, plimgui_curses::cCursesWindow* parent);
	virtual ~cStatusWindow(void);

	cStatusItem* add(const char* identifier);
	cStatusItem* get(const char* identifier);
	void remove(const char* identifier);

	void partialUpdate(void);
	
protected:
	int onOpenCloseItem(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs);
	
private:
	plimuni::cTreeNodes* m_panelNodes;
	plimuni::cPlimLexer lexhex;
};
 
};

#endif
