#include "application.h"
#include "windows.h"

namespace NSApplication {

using namespace NSWindows;

cApplication::cApplication(int argc, char** argv)
:	cTreeNodes(), cKeyboard(),
	m_rootWindow(initscr()),
	m_termWidth(getmaxx(stdscr)), 
	m_termHeight(getmaxy(stdscr)),
	m_timeout(0),
	m_endApp(0),
	m_colors(NULL) {
	
	m_descriptors = new cTreeNodes();

	if (m_rootWindow) {
		/* No need for tcsetattr */
		noecho();
		cbreak();
		//curs_set(0);
		keypad(m_rootWindow, TRUE);
		leaveok(m_rootWindow, TRUE);
		nodelay(m_rootWindow, TRUE);
		timeout(0);
		raw();

		if (has_colors()) {
			start_color();
			use_default_colors();

			m_colors = new cPallete();
		}

		wrefresh(m_rootWindow);
		refresh();

	}
}
 
cApplication::~cApplication() {
	/* if you forget something to freeup */
	noraw();

	if (m_colors)	delete m_colors;

	delete m_descriptors;

	if ( m_rootWindow )
		endwin();
}

int cApplication::Loop(void) {
	int retCode;

	while (!(retCode = LoopMsg())) {
		if (m_endApp) {
			/* Say bye. */
			break;
		}

		OnPostLoop();
		PostLoopMsg();
	}

	return retCode;
}

int cApplication::OnKeyClicked( const int key ) {
	/* Handle all windows, and put the key to the focused one */
	if (key == 0x09) {
		return 0;
	}

	if ( OnTerminalSizeChanged() ) {
		return 1;
	}

	OnKeyPress( key );

	return OnKeyEvent( key );
}

int cApplication::LoopMsg(void) {
	struct timeval val;
	fd_set rfds, wfds, efds;
	int fmaxd, selectRet;
	cCursesWindow* data;
	cDescriptor* descriptor;

	if (!m_rootWindow)
		return -1;

	fmaxd = 0;
	
	val.tv_sec = 1;

	/* Default timeout, primitive. */	
	if ( m_timeout < 1000000 ) {
		val.tv_sec = 0;
		val.tv_usec = m_timeout;
		m_timeout += 1000;
	}
	else {
		val.tv_sec = 1;
		val.tv_usec = 0;
	}

	/* Check if there is somthing to input in the ui */
	if ( CheckKeyClicked() > 0 ) {
		m_timeout = 0;
		val.tv_sec = 0;
		/* Go for it */
	}

	/* Clear all descriptors */
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&efds);

	descriptor = GetFirstDescriptor();

	while (descriptor) {
		descriptor->SetupDescriptor();

		if ( fmaxd < descriptor->GetDescriptor() ) 
			fmaxd = descriptor->GetDescriptor();

		descriptor = descriptor->GetNextNode();
	}


	/* Select */
	selectRet = select(fmaxd + 1, &rfds, &wfds, &efds, &val);

	if ( selectRet == -1 ) {
		
	} else if ( selectRet ) {
		descriptor = GetFirstDescriptor();

		while (descriptor) {
			descriptor->IssetDescriptor();
			descriptor = descriptor->GetNextNode();
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

		OnResize(m_termWidth, m_termHeight);

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

int cApplication::OnKeyEvent( const int key ) {
	cCursesWindow* data = (cCursesWindow*) GetFirstWindow();

	do {
		if (data && data->IsFocused()) {
			return data->OnKeyPressed( key );
		}

		data = (cCursesWindow*) GetNext(data);

	} while (data);

	return 0;
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
