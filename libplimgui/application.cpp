#include "application.h"
#include "windows.h"

namespace NSApplication {

using namespace NSWindows;

cApplication::cApplication(int argc, char** argv)
:	cTreeNodes(),
	m_rootWindow(initscr()),
	m_termWidth(getmaxx(stdscr)), 
	m_termHeight(getmaxy(stdscr)) {
	
	if (m_rootWindow) {
		/* No need for tcsetattr */
		noecho();
		cbreak();
		//curs_set(0);
		keypad(m_rootWindow, TRUE);
		leaveok(m_rootWindow, TRUE);
		nodelay(m_rootWindow, TRUE);

		if (has_colors()) {
			start_color();
		}

		wrefresh(m_rootWindow);
		refresh();

	}
}

cApplication::~cApplication() {
	/* if you forget something to freeup */
	
	if ( m_rootWindow )
		endwin();
}

int cApplication::Loop(void) {
	int retCode;
	
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

	while (!(retCode = LoopMsg())) {
		PostLoopMsg();
	}

	fcntl(0, F_SETFL, fcntl(0, F_GETFL) & ~O_NONBLOCK);

	return retCode;
}

int cApplication::OnKeyPressed( const int key ) {
	/* Handle all windows, and put the key to the focused one */
	if (key == 0x09) {
		return 0;
	}

	return 0;
}

/* TODO: nonblocking input, test case for now */
int cApplication::LoopMsg(void) {
	int keyPressed;
	int selectRet;
	int keyPressRet;
	struct timeval val;
	fd_set rfds;
	cCursesWindow* data;

	if (!m_rootWindow)
		return -1;

	val.tv_sec = 0;
	val.tv_usec = 1000;

	/* This key handling routine will be overrided with cKeyboardDescriptor */

	/* Clear all descriptors */
	FD_ZERO(&rfds);
	/* Set all descriptors */
	FD_SET(0, &rfds);

	/* Select */
	selectRet = select(1, &rfds, NULL, NULL, &val);

	if ( selectRet == -1 ) {
		/* Check for resize */

		keyPressed = wgetch( m_rootWindow );

#ifdef KEY_RESIZE
		if ( keyPressed == KEY_RESIZE ) {
			OnTerminalSizeChanged();
		} else
#endif
		if ( OnTerminalSizeChanged() ) {
		}

		return 0;
	} else if ( selectRet ) {
		if (FD_ISSET(0, &rfds)) {
			for (;;) {
				keyPressed = wgetch( m_rootWindow );

				if ( keyPressed == ERR ) {
					return 0; /* move on */
				}

				if (keyPressed < 0)	
					break;

				/* Handle key input */
				keyPressRet = OnKeyPressed( keyPressed );

				if ( keyPressRet < 0 ) {
					return -1; /* Bye */
				} else {
					LaunchKeyEvents(keyPressed);
				}
			}
			
		}
		
	}

	/* Do all paint and size methods on the end */
	data = (cCursesWindow*) GetFirstWindow( );
	
	if ( data ) {
		/* Launch update event */
		data->Update();
	}

	PartialUpdateWindows();

	/* if return 0 than we can process the loop */
	doupdate();
	return 0;
}

int cApplication::OnTerminalSizeChanged(void) {
	cCursesWindow* data;

	wrefresh(m_rootWindow);

	if ( m_termWidth != getmaxx(stdscr) || m_termHeight != getmaxy(stdscr) ) {
		m_termWidth = getmaxx(stdscr);
		m_termHeight = getmaxy(stdscr);

		/* Update just the first window wich should be the root window */
		data = (cCursesWindow*) GetFirstWindow( );
		
		if ( data ) {
			/* Update all child windows */
			data->NeedUpdate();
		}

		LaunchResizeEvents();

		return 1;
	}


	return 0;
}

void cApplication::PartialUpdateWindows(void) {
	cCursesWindow* data = (cCursesWindow*) GetFirstWindow();

	do {
		if (data)
			data->PartialUpdate();

		data = (cCursesWindow*) GetNext(data);

	} while (data);
}

void cApplication::LaunchKeyEvents(int key) {
	cCursesWindow* data = (cCursesWindow*) GetFirstWindow();

	do {
		if (data && data->IsFocused()) {
			data->OnKeyPressed( key );
			break;
		}

		data = (cCursesWindow*) GetNext(data);

	} while (data);

}

void cApplication::LaunchResizeEvents(void) {
	cCursesWindow* data = (cCursesWindow*) GetFirstWindow();

	do {
		if (data) {
			data->OnResize( );
		}

		data = (cCursesWindow*) GetNext(data);

	} while (data);

}


};
