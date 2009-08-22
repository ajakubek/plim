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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <curses.h>
#include <signal.h>

#include <libplimgui/application.h>
#include <libplimgui/windows.h>
#include <libplimgui/input.h>
#include <libplimgui/statusbar.h>
#include <libplimgui/textviewer.h>

using namespace NSApplication;
using namespace NSWindows;

cApplication* app;
cTextWindow* window;
cCursesWindow* root;
cInputWindow* inputWindow;
cStatusWindow* statusWindowTop;
cStatusWindow* statusWindowBottom;

void OnEnterInput(const char* buffer) {
	if (window) {
		window->NewLine( buffer, 0 );
	}
}

/* Temporary code! 
	For creating a new app, inheriting cApplication will be needed to handle app events */
int main(int argc, char** argv) {

	app = new cApplication(argc, argv);

	root = new cCursesWindow(app, 0, 0, 0, 0, NULL);

	statusWindowTop = new cStatusWindow(app, root);
	statusWindowTop->SetWindowAlign(top);

	inputWindow = new cInputWindow(app, root);
	inputWindow->SetWindowAlign(bottom);

	statusWindowBottom = new cStatusWindow(app, root);
	statusWindowBottom->SetWindowAlign(bottom);
 
	window = new cTextWindow(app, root);
	window->SetWindowAlign(client);
	window->SetColorPair(2);
 
	inputWindow->OnEnter.connect( &OnEnterInput );

	switch( app->Loop() ) {
		default:
			break;
	}

	delete root;
	delete app;

	return 0;
}

