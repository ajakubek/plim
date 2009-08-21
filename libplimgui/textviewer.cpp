#include "textviewer.h"

namespace NSWindows {

cTextWindow::cTextWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 1, 1, 1, parent->GetWidth(), parent) {
	SetFocus( FALSE );

}

cTextWindow::~cTextWindow(void) {
	
}

void cTextWindow::PartialUpdate(void) {
	if (!IsPartialNeccesary())
		return;
	
	Erase();

	cCursesWindow::PartialUpdate();
}


};
