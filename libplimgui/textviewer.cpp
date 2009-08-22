#include "textviewer.h"

namespace NSWindows {

cTextWindow::cTextWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 1, 1, 1, parent->GetWidth(), parent),
	m_lineTop(NULL) {
	SetFocus( FALSE );
	m_lineBuffer = new cTreeNodes();
}

cTextWindow::~cTextWindow(void) {

	if (m_lineBuffer)
		delete m_lineBuffer;

}

cTextLine* cTextWindow::NewLine(const char* buffer, unsigned int uid) {
	cTextLine* line;
	cString bufferOut;

	if (!buffer)
		return NULL;

	bufferOut.Copy( "20:47 < IC0ffeeCup> " );
	bufferOut.Cat( buffer );

	line = new cTextLine( m_lineBuffer, NULL, bufferOut.GetBuffer(), 0 );

	NeedPartialUpdate();

	return line;
}

void cTextWindow::PartialUpdate(void) {
	cTextLine* line;
	int mtdy, atdy;

	if (!IsPartialNeccesary())
		return;
	
	if ( !m_lineTop) {
		m_lineTop = (cTextLine*) m_lineBuffer->GetFirstNode();
	}

	Erase();

	line = (cTextLine* ) m_lineTop;
	mtdy = GetHeight() - 1;
	atdy = 0;

	while ( line ) {
		atdy += Print( line, atdy );

		if (atdy > mtdy )
			break;

		line = (cTextLine*) m_lineBuffer->GetNext(line);
	}

	cCursesWindow::PartialUpdate();
}


};
