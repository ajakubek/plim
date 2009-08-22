#include "input.h"

namespace NSWindows {

cInputWindow::cInputWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 1, 1, 1, parent->GetWidth(), parent),
	m_cursorPos(0) {
	m_buffer = new cString();
	SetFocus( TRUE );

}

cInputWindow::~cInputWindow(void) {
	if (m_buffer) {
		delete m_buffer;
	}
}

void cInputWindow::PartialUpdate(void) {
	if (!IsPartialNeccesary())
		return;
	
	Erase();
	
	//::box(m_windowHandle, ACS_VLINE, ACS_HLINE);
	if (m_buffer->GetBuffer()) {
		//::wprintw(GetWindowHandle(), "%s", m_buffer->GetBuffer());
		Print( m_buffer, 0 );
	}

	cCursesWindow::PartialUpdate();
}

int cInputWindow::OnKeyPressed( const int key ) {
	char localBuff[2] = { key, 0 };
	int m_newHeight;

	/* TODO: Add key bindings */

	switch ( key ) {
		case 127:
		case KEY_BACKSPACE: {
			if ( m_cursorPos > 0 ) {
				m_cursorPos--;
				m_buffer->Delete( m_cursorPos , 1, NULL );
			}
			break;
		}

		case 0x09:
		case 0x0A: {
			break;
		}

		default: {
			m_buffer->Cat( (char* ) &localBuff );
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
	int m_newHeight = (int) (m_buffer->GetLength() / GetWidth()) + 1;

	if (GetHeight() != m_newHeight) {
		SetHeight( m_newHeight );
		return 1;
	}

	return 0;
}

void cInputWindow::UpdateSize(void) {
	if (IsChild())
		GetParentWindow()->NeedUpdate();
	else
		NeedUpdate();
}

};
