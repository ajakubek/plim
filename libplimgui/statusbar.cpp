#include "statusbar.h"

namespace NSWindows {

cStatusWindow::cStatusWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 0, 0, 1, parent->GetWidth() + 10, parent) {
	
	init_pair(1, COLOR_WHITE, COLOR_BLUE );

	wattron(GetWindowHandle(), COLOR_PAIR(1));

	SetFocus( FALSE );
}

cStatusWindow::~cStatusWindow(void) {
}

void cStatusWindow::PartialUpdate(void) {
	if (!IsPartialNeccesary())
		return;
	
	Erase();
 
	::wbkgd(GetWindowHandle(), COLOR_PAIR(1));
	//::box(m_windowHandle, ACS_VLINE, ACS_HLINE);
	::wprintw(GetWindowHandle(), " UTF-8 | Handbook: http://tiny.pl/2cbz | Dokumentacja: http://tiny.pl/2cb3 | [00:37:50] <mkay> WOL� 10-letniego Niemca <-- pedo!");
	
	
	cCursesWindow::PartialUpdate();
}

};
