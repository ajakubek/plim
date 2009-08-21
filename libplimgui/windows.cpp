#include "application.h"
#include "windows.h"
#include "box.h"

namespace NSWindows {

using namespace NSApplication;

cCursesWindow::cCursesWindow(cApplication* app, int left, int top, int height, int width, cCursesWindow* parent)
:	cTreeNode( app, parent, NULL ),
	m_appInstance(app),
	m_windowParent(parent),
	m_xPos(left), m_yPos(top),
	m_width(width), m_height(height),
	m_canTabStop(FALSE),
	m_hasFocus(FALSE),
	m_isVisible(TRUE),
	m_needUpdate(FALSE),
	m_needPartialUpdate(FALSE),
	m_windowData(NULL),
	m_windowAlign(none),
	m_windowFirst(NULL),
	m_windowLast(NULL) {

	if (m_appInstance) {
		
		if (!m_windowParent) {
			m_windowHandle = ::newwin(height, width, top, left);
		} else {
			if (!m_windowParent->GetWindowHandle()) {}

			m_windowHandle = ::subwin(m_windowParent->GetWindowHandle(), height, width, m_windowParent->GetTop() + top, m_windowParent->GetLeft() + left);
			leaveok(m_windowHandle, TRUE);

			m_windowParent->NeedUpdate();
		}

		if (m_windowHandle) {
			m_boxPtr = (void*) new cBox( this );

			if (m_windowParent)
				::touchwin(m_windowParent->GetWindowHandle());

			::wrefresh(m_windowHandle);
		}

		NeedUpdate();
	}
}

/* Constructor used for spawning a fake root window */
cCursesWindow::cCursesWindow(cApplication* app)
:	cTreeNode( app, NULL, NULL ),
	m_appInstance(app),
	m_windowParent(NULL),
	m_xPos(0), m_yPos(0),
	m_width(0), m_height(0),
	m_canTabStop(FALSE),
	m_hasFocus(FALSE),
	m_isVisible(TRUE),
	m_needUpdate(FALSE),
	m_needPartialUpdate(FALSE),
	m_windowFirst(NULL),
	m_windowLast(NULL) {

	if (m_appInstance) {
		SetWindowHandle( m_appInstance->GetWindowHandle() );

		NeedUpdate();
	}
}

cCursesWindow::~cCursesWindow(void) {
	if (m_boxPtr) {
		delete (cBox*) m_boxPtr;
		m_boxPtr = NULL;
	}

	if (m_windowHandle) {
		::delwin(m_windowHandle);
	}
}

void cCursesWindow::Erase(void) {
	::wclear(m_windowHandle);
}

void cCursesWindow::Clear(void) {
	::wclear(m_windowHandle);
}

void cCursesWindow::NeedUpdate(void) {
	m_needUpdate = TRUE;
}

void cCursesWindow::NeedPartialUpdate(void) {
	m_needPartialUpdate = TRUE;
}

void cCursesWindow::Update(void) {
	cBox* box = (cBox*) m_boxPtr;
	cCursesWindow* data;

	if (!m_needUpdate)
		return;

	if ( box ) {
		/* recalculating is done. */
		box->Update();
	}
	
	if ( m_windowParent ) { /* Update the sizes only if the window is a child */
		if (!m_windowHandle) {
			/*	if the derwin failed with the given coords than try to recreate the window 
			*/
			m_windowHandle = ::subwin(m_windowParent->GetWindowHandle(), m_height, m_width, GetTop(), GetLeft());
			leaveok(m_windowHandle, TRUE);
		}
	
		
		
		if (m_windowHandle) {
			/* Check for overlapping windows */
			

			/* Do first a resize, cause it shouldnt report a err even if it overlaps */
			if (::wresize( m_windowHandle, m_height, m_width) == ERR) {
				printw("%d, %d - %d\n", m_height, m_width, m_windowParent->GetWidth());
				//DEBUG("wresize error");
			}
			if (::mvwin( m_windowHandle, GetTop(), GetLeft()) == ERR) {
				//DEBUG("mvwin error");
			}
		}
	}

	/* Update child windows *
	item = m_windowFirst;

	while (item) {
		data = GetWindow(item);
		
		if (data)
			data->Update();

		item = item->next;
	} */

	data = GetFirstWindow();

	while ( data ) {
		data->Update();
		data = data->GetNextNode();
	}

	/* Set the window for repaint */
	NeedPartialUpdate();

	m_needUpdate = FALSE;
}

void cCursesWindow::PartialUpdate(void) {
	if (!m_needPartialUpdate)
		return;

	if (!m_windowParent) {
		Erase();
	}

	/* let it handle by the doupdate from curses */
	::wnoutrefresh(m_windowHandle);

	m_needPartialUpdate = FALSE;
}

int cCursesWindow::Print(cString* string, int start) {
	int ret = start;
	int x;
	int i;
	
	if (!string || !string->GetLength() || start < 0 || start > GetHeight() )
		return 0;

	x = 0;
	i = 0;

	for (; i < string->GetLength(); i++ ) {

		if ( x > GetWidth() ) {
			x = 0;
			ret += 1;
		}

		mvwinsch( m_windowHandle, ret, x, string->GetChar(i));
		x++;
		
	}

	return ret;
}

int cCursesWindow::OnKeyPressed( const int key ) {
	return 0;
}

int cCursesWindow::OnResize(void) {
	return 0;
}

};
