#include "input.h"

namespace NSWindows {

cInputWindow::cInputWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 1, 1, 1, parent->GetWidth(), parent),
	m_cursorPos(0) {
	SetFocus( TRUE );
	SetPrefixPrompt( "[#gentoo.pl]" );
}

cInputWindow::~cInputWindow(void) {

}

void cInputWindow::PartialUpdate(void) {
	if (!IsPartialNeccesary())
		return;

	Erase();

	PrintFormated( &m_prefixBuffer, 1, 0 );

	//::box(m_windowHandle, ACS_VLINE, ACS_HLINE);
	if (m_buffer.GetBuffer()) {
		//::wprintw(GetWindowHandle(), "%s", m_buffer->GetBuffer());
		Print( &m_buffer, m_prefixBuffer.GetLength() + 1, 0 );
	}

	::wmove( GetWindowHandle(), 0, m_prefixBuffer.GetLength() + m_cursorPos );

	cCursesWindow::PartialUpdate();
}

int cInputWindow::OnKeyPressed( const int key ) {
	char localBuff[2] = { key, 0 };
	int m_newHeight;
	char xxx[1024];
	cPlimToken* token;

	sprintf(&xxx[0], "(%c)(0x%x)(%i)", key, key, key );
	//OnEnter( &xxx[0]);
	switch ( key ) {
		case 127:
		case KEY_BACKSPACE: {
			if ( m_cursorPos > 0 ) {
				m_cursorPos--;
				m_buffer.Delete( m_cursorPos , 1, NULL );
			}
			break;
		}

		case 0x09: {
			break;
		}

		case 0x0A: {
			if ( m_buffer.GetLength() > 0 ) {
				OnEnter( m_buffer.GetBuffer() );

				lexer.Refresh( m_buffer.GetBuffer() );

				m_buffer.Clean();
				m_cursorPos = 0;
				/* TODO: Cursor navigation */
			}
			break;
		}
 
		default: {
			m_buffer.Cat( (char* ) &localBuff );
			m_cursorPos++;
			break;
		}

	}

	if (CalcSize()) {
		UpdateSize();
	}

	NeedPartialUpdate();

	return 1;
}

int cInputWindow::OnResize(void) {
	if (CalcSize()) {
		UpdateSize();

	}
	return 0;
}

int cInputWindow::CalcSize(void) {
	int m_newHeight = (int) ((m_prefixBuffer.GetLength() + m_buffer.GetLength()) / GetWidth()) + 1;

	if (GetHeight() != m_newHeight) {
		SetHeight( m_newHeight );
		return 1;
	}

	return 0;
}

void cInputWindow::UpdateSize(void) {
	if (IsChild()) {
		GetParentWindow()->ForceUpdate();
	}

	NeedPartialUpdate();
}

};
