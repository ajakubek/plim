#include "application.hpp"
#include "windows.hpp"

namespace plimgui_curses {

cApplication::cApplication(int argc, char** argv)	:	plimuni::cTreeNodes(), 
																		plimgui_curses::cKeyboard(),
																		m_rootWindow( ::initscr() ),
																		m_termWidth(0), 
																		m_termHeight(0),
																		m_timeout(0),
																		m_endApp(0),
																		m_colors(NULL) 
{

	if (m_rootWindow) 
	{
		/* No need for tcsetattr */
		::noecho();
		::cbreak();
		//curs_set(0);
		
#if defined(__PDCURSES__)
		::PDC_return_key_modifiers( TRUE );
#endif

		::keypad(m_rootWindow, TRUE);
		::leaveok(m_rootWindow, TRUE);
		::nodelay(m_rootWindow, TRUE);
		::timeout(0);
		::raw();

		if ( ::has_colors() )
		{
			::start_color();
			::use_default_colors();

			m_colors = new plimgui_curses::cPallete();
		}

		::wrefresh(m_rootWindow);
		::refresh();

	}
}
 
cApplication::~cApplication() 
{
	/* if you forget something to freeup */
	::noraw();

	if (m_colors)
	{
		delete m_colors;
	}

	if ( m_rootWindow )
	{
		::endwin();
	}
}

int cApplication::loop(void) 
{
	int retCode;
	
#if defined(__linux) || defined(linux)
	::fcntl(0, F_SETFD, fcntl(0, F_GETFD) | O_NONBLOCK);
#endif

	while (!(retCode = loopMsg())) 
	{
		if (m_endApp) {
			/* Say bye. */
			break;
		}

		onPostLoop();
		postLoopMsg();
	}

	return retCode;
}

int cApplication::onKeyClicked( const int key ) 
{
	/* Handle all windows, and put the key to the focused one */
	if (key == 0x09) {
		return 0;
	}

#ifdef KEY_RESIZE
	if ( key == KEY_RESIZE ) {
		return 0;
	}
#else

#endif

	onKeyPress( key );

	return onKeyEvent( key );
}

void cApplication::onBindingClicked(void) 
{
	plimuni::cString* cmd;

	if (isKeyBindingPending()) {
		cmd = getCommand(NULL);

		onBindingPress( this, cmd );
	}
}

int cApplication::loopMsg(void) 
{
	plimgui_curses::cCursesWindow* data;

	if (!m_rootWindow)
		return -1;

	/* Do all paint and size methods on the end */
	data = (plimgui_curses::cCursesWindow*) getFirstWindow( );
	
	if ( data ) {
		/* Launch update event */
		data->update();
	}

	partialUpdateWindows();

	/* if return 0 than we can process the loop */
	::doupdate();
	return 0;
}

int cApplication::onTerminalSizeChanged(void) 
{
	plimgui_curses::cCursesWindow* data = (plimgui_curses::cCursesWindow*) getFirstWindow();
	int col, row;
#if defined(__linux) || defined(linux)
	struct winsize ws;
	if (!::ioctl(0, TIOCGWINSZ, &ws) )
	{
		col = ws.ws_col;
		row = ws.ws_row;
	}
#else
	col = ::getmaxx( stdscr );
	row = ::getmaxy( stdscr );
#endif
		if ( m_termWidth != col || m_termHeight != row ) {
			m_termWidth = col;
			m_termHeight = row;
#if defined(__PDCURSES__)
			::resize_term( m_termHeight, m_termWidth );
#else
			::resizeterm( m_termHeight, m_termWidth );
#endif

			::clear();
			::refresh();

			onResize(m_termWidth, m_termHeight);

			launchResizeEvents();
			
			plimgui_curses::cApplication::loopMsg();

			return 1;
		}

	return 0;
}

void cApplication::partialUpdateWindows(void) 
{
	plimgui_curses::cCursesWindow* data = (plimgui_curses::cCursesWindow*) getFirstWindow();

	do 
	{
		if (data)
			data->partialUpdate();

		data = (plimgui_curses::cCursesWindow*) getNext(data);

	} 
	while (data);

}

int cApplication::onKeyEvent( const int key ) 
{
	plimgui_curses::cCursesWindow* data = (plimgui_curses::cCursesWindow*) getFirstWindow();

	do 
	{
		if (data && data->isFocused()) 
		{
			return data->onKeyPressed( key );
		}

		data = (plimgui_curses::cCursesWindow*) getNext(data);

	} 
	while (data);

	return 0;
}

void cApplication::launchResizeEvents(void) 
{
	plimgui_curses::cCursesWindow* data = (plimgui_curses::cCursesWindow*) getFirstWindow();

	do 
	{
		if (data) 
		{
			data->needUpdate();
			data->onResize( );
		}

		data = (plimgui_curses::cCursesWindow*) getNext(data);

	} 
	while (data);

}

};
