#include "textviewer.h"

namespace NSWindows {

cTextWindow::cTextWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 1, 1, 1, parent->GetWidth(), parent),
	m_lineTop(NULL),
	m_linesDrawed(0) {
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

	if ( m_linesDrawed >= GetHeight() ) {
		ScrollDown( CalculatePrint( line ) );
	}

	NeedPartialUpdate();

	return line;
}

void cTextWindow::ScrollDown(int count) {
	cTextLine* line;

	if (!count)
		return;

	line = (cTextLine*) m_lineTop;

	while ( line ) {

		if ( !count ) {
			m_lineTop = line;
			break;
		}

		count--;

		line = (cTextLine*) m_lineBuffer->GetNext( line );
	}

	NeedPartialUpdate();
}

void cTextWindow::ScrollUp(int count) {
	cTextLine* line;

	if (!count)
		return;
	
	line = (cTextLine*) m_lineTop;

	while ( line ) {

		if ( !count ) {
			m_lineTop = line;
			break;
		}

		count--;

		line = (cTextLine*) m_lineBuffer->GetPrev( line );
	}

	NeedPartialUpdate( );
}

void cTextWindow::PageUp(void) {
	ScrollUp( GetHeight() );
}

void cTextWindow::PageDown(void) {
	ScrollDown( GetHeight() );
}

int cTextWindow::OnResize(void) {
	Pin();
	return 0;
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
	mtdy = GetHeight();
	atdy = 0;

	while ( line ) {

		atdy += Print( line, atdy );

		if (atdy > mtdy )
			break;

		m_linesDrawed = atdy;

		line = (cTextLine*) m_lineBuffer->GetNext(line);
	}

	cCursesWindow::PartialUpdate();
}

void cTextWindow::Pin(void) {
	int dif = GetHeight() - m_linesDrawed;

	if ( dif ) {
		ScrollUp( dif );
	}
}

};
