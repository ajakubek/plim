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

#include "roster.hpp"

namespace plimgui_curses {

cRosterWindow::cRosterWindow(cApplication* app, cCursesWindow* parent)	:	plimgui_curses::cCursesWindow( app, 0, 0, 1, 20, parent),
																									m_topItem(NULL),
																									m_itemsDrawed(0) 
{

	setFocus( FALSE );
	
	m_rosterItems = new plimuni::cTreeNodes();

	createFormattingNodes();

	REGISTER_R_CALL( "@", onStatusIcon );
	REGISTER_R_CALL( "color", onColor );
	REGISTER_R_CALL( "i", onStatusIcon );
	REGISTER_R_CALL( "reset", onReset );
}

cRosterWindow::~cRosterWindow(void) 
{
	destroyFormattingNodes();
	
	if (m_rosterItems) 
	{
		delete m_rosterItems;
	}
}

cRosterItem* cRosterWindow::add(void* data) 
{
	return new cRosterItem( m_rosterItems, NULL, data );
}

cRosterItem* cRosterWindow::add(cRosterItem* item, cRosterItem* parent, void* data) 
{
	return new cRosterItem( m_rosterItems, parent, data );
}

cRosterItem* cRosterWindow::remove(cRosterItem* item) 
{
	return (cRosterItem*) m_rosterItems->removeNode( item, 1 );
}

void cRosterWindow::partialUpdate(void) 
{
	cRosterItem* item;
	int mtdy, atdy, ntdy;
	plimuni::cString buffer;

	if ( !isPartialNeccesary() )
		return;

	::erase();

	if (getWindowAlign() == right) {
		::wmove( getWindowHandle(), 0, 0 );
		::wvline( getWindowHandle(), ACS_VLINE, getHeight() );
	} else {
		::wmove( getWindowHandle(), 0, getWidth());
		::wvline( getWindowHandle(), ACS_VLINE, getHeight() );
		::wmove( getWindowHandle(), 0, 0 );
	}

	item = m_topItem;

	if (!item)
		item = (cRosterItem*) m_rosterItems->getFirstNode();

	atdy = 0;
	mtdy = getHeight() + 1;

	while ( item ) {

		ntdy = getNodeAttrs( item, &buffer );

		if ( item->isVisible() ) {
			lexhex.refresh( buffer.getBuffer() );
			atdy += printLexer( &lexhex, 1, atdy );
		}

		if (atdy > mtdy)
			break;
		
		item = (cRosterItem*) m_rosterItems->getNext(item);
	}

	m_itemsDrawed = atdy;

	plimgui_curses::cCursesWindow::partialUpdate();
}

int cRosterWindow::getNodeAttrs(cRosterItem* item, plimuni::cString* string) {

	if ( rand() % 2 )
		string->copy( "[@] color 4,0 IC0ffee reset color 2,0 Cup");
	else
		string->copy( "[i] miGu reset color 3,0 gen2");

	return 1;
}

int cRosterWindow::onStatusIcon(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs) 
{

	if ( !strcmp(token->getBuffer(), "@" )) 
	{
		attrs->fg = COLOR_BLUE;
		attrs->bg = 0;

	}

	if ( !strcmp(token->getBuffer(), "i" )) 
	{
		attrs->fg = COLOR_RED;
		attrs->bg = 0;
	}

	attrs->usecolor = 1;

	return 0;
}

int cRosterWindow::onColor(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs) 
{
	int skipcount = token->getLength() + 2; /* + handle two spaces */
	plimuni::cPlimToken* plim = token->getNextNode();
	
	if (plim) {
		plim = plim->getNextNode();

		if (!plim)	return 0;

		if ( plim->getTokenCase() == PLIM_L_DIGIT_INTEGER) 
		{
			attrs->fg = atoi(plim->getBuffer());
			skipcount++;
		} 
		else 
		{
			attrs->fg = 0;
		}

		plim = plim->getNextNode();

		if (plim) 
		{
			if (plim->getTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS && plim->getTokenExCase() == PLIM_L_SYMBOL_COMMA) 
			{
				skipcount++;
				plim = plim->getNextNode();
				if (plim) 
				{
					if (plim->getTokenCase() == PLIM_L_DIGIT_INTEGER) 
					{
						attrs->bg = atoi(plim->getBuffer());
						skipcount++;
					} 
					else 
					{
						attrs->bg = 0;
					}
				}
			} 
			else 
			{
				attrs->bg = 0;
			}
		} 
		else 
		{
			attrs->bg = 0;
		}

	}

	attrs->noformcount = 1;
	attrs->usecolor = 1;
	attrs->skipcount = skipcount;

	return 0;
}


int cRosterWindow::onReset(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs) 
{
	attrs->reset = 1;
	attrs->skipcount = token->getLength();
	
	return 0;
}

};
