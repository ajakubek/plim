/***************************************************************************
 *   Copyright (C) 2009 by Lukasz Marcin Borzecki   *
 *   lukasz.borzecki@gmail.com   *
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

#include <libplimuni/stdafx.hpp>
#include <libplimuni/treenodes.hpp>
#include <libplimuni/lexer.hpp>

#include "stdafx.hpp"
#include "application.hpp"
#include "windows.hpp"

namespace plimgui_curses 
{

/* TODO: remove and add bindings from boost */
#define REGISTER_R_CALL(name, method) registerFormattingCallback(name, (OnPrintCallback) &plimgui_curses::cRosterWindow::method)

/* Short how it works and how to use
	You dont care nothing about the rosteritem, there is a property SetData, GetData
	that is used to extract your specified things like a structure or class, or hell
	god anyone know what. The rosterwindow handles the nodes itself.
	If you want display some text on a item, than override the GetNodeAttr method
	it handles both, text and height. And is called only when needed.
*/

class cRosterItem	:	public	plimuni::cTreeNode 
{

public:
	cRosterItem(plimuni::cTreeNodes* nodes, plimuni::cTreeNode* parent, void* data);
	~cRosterItem(void);
	
	void* getData(void);

	void setVisible(int visible) 
	{ 
		m_visible = visible; 
	};
	
	int isVisible(void) 
	{ 
		return m_visible; 
	};

	cRosterItem* getNextNode(void) 
	{ 
		return (cRosterItem*) plimuni::cTreeNode::getNextNode(); 
	};
	
	cRosterItem* getPrevNode(void) 
	{ 
		return (cRosterItem*) plimuni::cTreeNode::getPrevNode(); 
	};
private:
	int m_visible;
	void* m_data;
};

class cRosterWindow	:	public	plimgui_curses::cCursesWindow 
{

public:
	cRosterWindow(plimgui_curses::cApplication* app, plimgui_curses::cCursesWindow* parent);
	~cRosterWindow(void);
	
	/* Just create and return a cRosterItem
	*/
	cRosterItem* add(void* data);
	
	/* Add a item to roster, you can group it by assigning the parent
		@return newly added rosteritem
	*/
	cRosterItem* add(cRosterItem* item, cRosterItem* parent, void* data);
	
	/* Delete a roster item
		@return previous rosteritem if exists
	*/
	cRosterItem* remove(cRosterItem* item);

	/* Update or roster */
	void partialUpdate(void);

protected:
	virtual int getNodeAttrs(cRosterItem* item, plimuni::cString* string);

	int onStatusIcon(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs);
	int onColor(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs);
	int onReset(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs);

private:
	plimuni::cTreeNodes* m_rosterItems;
	plimuni::cPlimLexer lexhex;
	cRosterItem* m_topItem;
	
	int m_itemsDrawed;
};

};
