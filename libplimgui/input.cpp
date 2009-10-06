#include "input.hpp"

namespace plimgui_curses 
{

cInputWindow::cInputWindow(cApplication* app, cCursesWindow* parent)	:	plimgui_curses::cCursesWindow(app, 1, 1, 1, parent->getWidth(), parent),
																								m_cursorPos(0) 
{
#if defined(_WIN32) || defined(_WIN64)
	plimgui_curses::cCursesWindow::setFocus( TRUE );
#else
	setFocus( TRUE );
#endif

	setPrefixPrompt( "[#gentoo.pl]" );
}

cInputWindow::~cInputWindow(void) 
{}

void cInputWindow::partialUpdate(void) 
{
	if (!isPartialNeccesary())
		return;

	::erase();

	print( &m_prefixBuffer, 1, 0 );

	if (m_buffer.getBuffer()) 
	{
		print( &m_buffer, m_prefixBuffer.getLength() + 1, 0 );
	}

	::wmove( getWindowHandle(), 0, m_prefixBuffer.getLength() + m_cursorPos );

	plimgui_curses::cCursesWindow::partialUpdate();
}

/* TODO: Change */
int cInputWindow::onKeyPressed( const int key ) 
{
	char localBuff[2] = { key, 0 };
	int m_newHeight;
	char xxx[1024];
	plimuni::cPlimToken* token;

	sprintf(&xxx[0], "(%c)(0x%x)(%i)", key, key, key );
	//OnEnter( &xxx[0]);
	
	switch ( key ) {
		case 127:
		case KEY_BACKSPACE: 
		{
			if ( m_cursorPos > 0 ) 
			{
				m_cursorPos--;
				m_buffer.remove( m_cursorPos , 1, NULL );
			}
			
			break;
		}

		case 0x09: 
		{
			break;
		}
		case 13:
		case 0x0A: 
		{
			if ( m_buffer.getLength() > 0 ) 
			{
				onEnter( m_buffer.getBuffer() );

				lexer.refresh( m_buffer.getBuffer() );

				m_buffer.clean();
				m_cursorPos = 0;
				/* TODO: Cursor navigation */
			}
			break;
		}
 
		default: 
		{
			m_buffer.cat( (char* ) &localBuff );
			m_cursorPos++;
			break;
		}

	}

	if (calcSize()) 
	{
		updateSize();
	}

	needPartialUpdate();

	return 1;
}

int cInputWindow::onResize(void) {
	if (calcSize()) 
	{
		updateSize();

	}
	return 0;
}

int cInputWindow::calcSize(void) 
{
	int m_newHeight = (int) ((m_prefixBuffer.getLength() + m_buffer.getLength()) / getWidth()) + 1;

	if (getHeight() != m_newHeight) 
	{
		setHeight( m_newHeight );
		return 1;
	}

	return 0;
}

void cInputWindow::updateSize(void) {
	if (isChild()) 
	{
		getParentWindow()->forceUpdate();
	}

	needPartialUpdate();
}

};
