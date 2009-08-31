#include "textviewer.h"

namespace NSWindows {

cTextWindow::cTextWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 1, 1, 1, parent->GetWidth(), parent),
	m_lineTop(NULL),
	m_linesDrawed(0) {

	SetFocus( FALSE );

	m_lineBuffer = new cTreeNodes();
	CreateFormattingNodes();

	REGISTER_F_CALL( "int", OnCppKeyword );
	REGISTER_F_CALL( "void", OnCppKeyword );
	REGISTER_F_CALL( "typedef", OnCppKeyword );
	REGISTER_F_CALL( "union", OnCppKeyword );
	REGISTER_F_CALL( "char", OnCppKeyword );
	REGISTER_F_CALL( "const", OnCppKeyword );
	REGISTER_F_CALL( "virtual", OnCppKeyword );
	REGISTER_F_CALL( "mutable", OnCppKeyword );
	REGISTER_F_CALL( "operator", OnCppKeyword );
	REGISTER_F_CALL( "IC0ffeeCup", OnCppKeyword );
}

cTextWindow::~cTextWindow(void) {
	DestroyFormattingNodes();
	if (m_lineBuffer)
		delete m_lineBuffer;
}

cTextLine* cTextWindow::NewLine(const char* buffer, unsigned int uid) {
	cTextLine* line, *sub;
	cString bufferOut;

	if (!buffer)
		return NULL;

	bufferOut.Copy( "20:47 ,---- IC0ffeeCup ---- " );

	line = new cTextLine( m_lineBuffer, NULL, bufferOut.GetBuffer(), 0 );
	sub = new cTextLine( m_lineBuffer, line, buffer, 0 );

	if ( m_linesDrawed >= GetHeight() ) {
		ScrollDown( CalculatePrint( line ) + CalculatePrint( sub ) );
	}

	NeedPartialUpdate();
	Update(); /* Draw it instant */

	return line;
}

cTextLine* cTextWindow::NewLine(cTextLine* line, const char* buffer, unsigned int uid) {
	cTextLine* lineb;
	cString bufferOut;

	if (!buffer)
		return NULL;

	bufferOut.Copy( buffer );

	lineb = new cTextLine( m_lineBuffer, line, bufferOut.GetBuffer(), 0 );

	if ( m_linesDrawed >= GetHeight() ) {
		ScrollDown( CalculatePrint( lineb ) );
	}

	NeedPartialUpdate();
	Update(); /* Draw it instant */

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
	cCursesString bufferLine;

	if (!IsPartialNeccesary())
		return;
	
	if ( !m_lineTop) {
		m_lineTop = (cTextLine*) m_lineBuffer->GetFirstNode();
	}

	Erase();

	line = (cTextLine* ) m_lineTop;
	mtdy = GetHeight() + 1;
	atdy = 0;

	while ( line ) {

		if ( line->GetParentNode() ) {
			bufferLine.Copy("      | ");
			bufferLine.Cat( line );

			m_plimLexer.Refresh( bufferLine.GetBuffer() );

			atdy += PrintLexer( &m_plimLexer, 0, atdy);
		}
 		else {
			m_plimLexer.Refresh( line->GetBuffer() );
			atdy += PrintLexer( &m_plimLexer, 0, atdy);
		}

		if ( line->GetParentNode() ) {
			if ( line == line->GetParentNode()->GetLastNode() ) {
				bufferLine.Copy("      `----- conversation of user IC0ffeeCup ended at 21:30 ----");

				m_plimLexer.Refresh( bufferLine.GetBuffer() );

				atdy += PrintLexer( &m_plimLexer, 0, atdy);
			}
		}

		if (atdy > mtdy )
			break;

		m_linesDrawed = atdy;

		line = (cTextLine*) m_lineBuffer->GetNext(line);
	}

	cCursesWindow::PartialUpdate();
}

int cTextWindow::OnCppKeyword(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs) {
	attrs->usecolor = 1;
	attrs->fg = 4;
	attrs->bg = 0;
	attrs->flags = A_BOLD;
	return 0;
}

void cTextWindow::Pin(void) {
	int dif = GetHeight() - m_linesDrawed;

	if ( dif ) { 
		ScrollUp( dif + 1 );
	}
}

};
