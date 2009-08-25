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

#include <string.h>

#include "application.h"
#include "windows.h"
#include "treenodes.h"
#include "lexer.h"

namespace NSWindows {

using namespace NSString;

#define REGISTER_R_CALL(name, method) RegisterFormattingCallback(name, (OnPrintCallback) &NSWindows::cRosterWindow::method)

/* Short how it works and how to use
	You dont care nothing about the rosteritem, there is a property SetData, GetData
	that is used to extract your specified things like a structure or class, or hell
	god anyone know what. The rosterwindow handles the nodes itself.
	If you want display some text on a item, than override the GetNodeAttr method
	it handles both, text and height. And is called only when needed.
*/
	
class cRosterItem: public cTreeNode {
public:
	cRosterItem(cTreeNodes* nodes, cTreeNode* parent, void* data);
	~cRosterItem(void);
	void* GetData(void);

	void SetVisible(int visible) { m_visible = visible; };
	int IsVisible(void) { return m_visible; };

	cRosterItem* GetNextNode(void) { return (cRosterItem*) cTreeNode::GetNextNode(); };
	cRosterItem* GetPrevNode(void) { return (cRosterItem*) cTreeNode::GetPrevNode(); };
private:
	int m_visible;
	void* m_data;
};

class cRosterWindow: public cCursesWindow {
public:
	cRosterWindow(cApplication* app, cCursesWindow* parent);
	~cRosterWindow(void);
	/* Just create and return a cRosterItem
	*/
	cRosterItem* Add(void* data);
	/* Add a item to roster, you can group it by assigning the parent
		@return newly added rosteritem
	*/
	cRosterItem* Add(cRosterItem* item, cRosterItem* parent, void* data);
	/* Delete a roster item
		@return previous rosteritem if exists
	*/
	cRosterItem* Delete(cRosterItem* item);

	/* Update or roster */
	void PartialUpdate(void);

protected:
	virtual int GetNodeAttrs(cRosterItem* item, cString* string);

	int OnStatusIcon(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs);
	int OnColor(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs);
	int OnReset(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs);
private:
	cTreeNodes* m_rosterItems;
	cPlimLexer lexhex;
	cRosterItem* m_topItem;
	int m_itemsDrawed;
};

};
