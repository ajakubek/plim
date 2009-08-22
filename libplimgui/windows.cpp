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
	m_windowLast(NULL),
	m_colorPair(0) {

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

	init_pair(2, COLOR_WHITE, COLOR_RED );

	wattron(GetWindowHandle(), COLOR_PAIR(2));

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
	m_windowLast(NULL),
	m_colorPair(0) {

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

void cCursesWindow::RecreateWindow(void) {
	cCursesWindow* child;
	cCursesWindow* lastChild;

	child = GetFirstWindow();
	lastChild = GetLastWindow();


	while ( child ) {

		child->RecreateWindow();

		if (child == lastChild)
			break; /* RecreateWindow done. */
		child = (cCursesWindow*) m_appInstance->GetNext( child );
	}

	if (m_windowHandle)
		::delwin(m_windowHandle);

	if (m_windowParent)
		m_windowHandle = ::subwin(m_windowParent->GetWindowHandle(), m_height, m_width, GetTop(), GetLeft());
	else
		m_windowHandle = ::newwin(GetHeight(), GetWidth(), GetTop(), GetLeft());

	leaveok(m_windowHandle, TRUE);
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
		RecreateWindow();

		if (m_windowHandle) {}
	}

	/* Update child windows */
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

	if (m_colorPair) {
		Erase();
		::wbkgd(GetWindowHandle(), COLOR_PAIR(m_colorPair));
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
