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

#include "roster.h"

namespace NSWindows {

cRosterWindow::cRosterWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow( app, 0, 0, 1, 20, parent),
	m_topItem(NULL),
	m_itemsDrawed(0) {

	SetFocus( FALSE );
	m_rosterItems = new cTreeNodes();

	CreateFormattingNodes();

	REGISTER_R_CALL( "@", OnStatusIcon );
	REGISTER_R_CALL( "color", OnColor );
	REGISTER_R_CALL( "i", OnStatusIcon );
	REGISTER_R_CALL( "reset", OnReset );
}

cRosterWindow::~cRosterWindow(void) {
	DestroyFormattingNodes();
	if (m_rosterItems) {
		delete m_rosterItems;
	}
}

cRosterItem* cRosterWindow::Add(void* data) {
	return new cRosterItem( m_rosterItems, NULL, data );
}

cRosterItem* cRosterWindow::Add(cRosterItem* item, cRosterItem* parent, void* data) {
	return new cRosterItem( m_rosterItems, parent, data );
}

cRosterItem* cRosterWindow::Delete(cRosterItem* item) {
	return (cRosterItem*) m_rosterItems->RemoveNode( item, 1 );
}

void cRosterWindow::PartialUpdate(void) {
	cRosterItem* item;
	int mtdy, atdy, ntdy;
	cString buffer;

	if ( !IsPartialNeccesary() )
		return;

	Erase();

	if (GetWindowAlign() == right) {
		::wmove( GetWindowHandle(), 0, 0 );
		::wvline( GetWindowHandle(), ACS_VLINE, GetHeight() );
	} else {
		::wmove( GetWindowHandle(), 0, GetWidth());
		::wvline( GetWindowHandle(), ACS_VLINE, GetHeight() );
		::wmove( GetWindowHandle(), 0, 0 );
	}

	item = m_topItem;

	if (!item)
		item = (cRosterItem*) m_rosterItems->GetFirstNode();

	atdy = 0;
	mtdy = GetHeight() + 1;

	while ( item ) {

		ntdy = GetNodeAttrs( item, &buffer );

		if ( item->IsVisible() ) {
			lexhex.Refresh( buffer.GetBuffer() );
			atdy += PrintLexer( &lexhex, 1, atdy );
		}

		if (atdy > mtdy)
			break;
		
		item = (cRosterItem*) m_rosterItems->GetNext(item);
	}

	m_itemsDrawed = atdy;

	cCursesWindow::PartialUpdate();
}

int cRosterWindow::GetNodeAttrs(cRosterItem* item, cString* string) {

	if ( rand() % 2 )
		string->Copy( "[@] color 4,0 IC0ffee reset color 2,0 Cup");
	else
		string->Copy( "[i] miGu reset color 3,0 gen2");

	return 1;
}

int cRosterWindow::OnStatusIcon(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs) {

	if ( !strcmp(token->GetBuffer(), "@" )) {
		attrs->fg = COLOR_BLUE;
		attrs->bg = 0;

	}

	if ( !strcmp(token->GetBuffer(), "i" )) {
		attrs->fg = COLOR_RED;
		attrs->bg = 0;
	}

	attrs->usecolor = 1;

	return 0;
}

int cRosterWindow::OnColor(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs) {
	int skipcount = token->GetLength() + 2; /* + handle two spaces */
	cPlimToken* plim = token->GetNextNode();
	
	if (plim) {
		plim = plim->GetNextNode();

		if (!plim)	return 0;

		if ( plim->GetTokenCase() == PLIM_L_DIGIT_INTEGER) {
			attrs->fg = atoi(plim->GetBuffer());
			skipcount++;
		} else {
			attrs->fg = 0;
		}

		plim = plim->GetNextNode();

		if (plim) {
			if (plim->GetTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS && plim->GetTokenExCase() == PLIM_L_SYMBOL_COMMA) {
				skipcount++;
				plim = plim->GetNextNode();
				if (plim) {
					if (plim->GetTokenCase() == PLIM_L_DIGIT_INTEGER) {
						attrs->bg = atoi(plim->GetBuffer());
						skipcount++;
					} else {
						attrs->bg = 0;
					}
				}
			} else {
				attrs->bg = 0;
			}
		} else {
			attrs->bg = 0;
		}

	}

	attrs->noformcount = 1;
	attrs->usecolor = 1;
	attrs->skipcount = skipcount;

	return 0;
}


int cRosterWindow::OnReset(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs) {
	attrs->reset = 1;
	attrs->skipcount = token->GetLength();
	return 0;
}

};
