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

using namespace NSApplication;
using namespace NSWindows;

/* Temporary code! 
	For creating a new app, inheriting cApplication will be needed to handle app events */
int main(int argc, char** argv) {
	cApplication* app;
	cCursesWindow* window;
	cCursesWindow* root;
	cInputWindow* inputWindow;
	cStatusWindow* statusWindowTop;
	cStatusWindow* statusWindowBottom;

	app = new cApplication(argc, argv);
	root = new cCursesWindow(app, 0, 0, 0, 0, NULL);

	inputWindow = new cInputWindow(app, root);
	inputWindow->SetWindowAlign(bottom);

	statusWindowBottom = new cStatusWindow(app, root);
	statusWindowBottom->SetWindowAlign(bottom);

	statusWindowTop = new cStatusWindow(app, root);
	statusWindowTop->SetWindowAlign(top);

	window = new cCursesWindow(app, 1, 1, 2, 2, root);
	window->SetWindowAlign(client);

	switch( app->Loop() ) {
		default:
			break;
	}

	delete root;
	delete app;

	return 0;
}

