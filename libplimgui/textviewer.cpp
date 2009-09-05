#include "textviewer.h"

namespace NSWindows {

cTextWindow::cTextWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 1, 1, 1, parent->GetWidth(), parent),
	m_lineTop(NULL),
	m_linesDrawed(0),
	m_useSlibText(0),
	m_room(NULL) {

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
	int scrollby;
	
	/* TODO: add search for uid of the user */
	if (!buffer)
		return NULL;

	if (m_useSlibText) {
	
		bufferOut.Copy( buffer );

		line = new cTextLine( m_lineBuffer, NULL, bufferOut.GetBuffer() );
		sub = new cTextLine( m_lineBuffer, line, buffer );
		
		scrollby = CalculatePrint( line ) + CalculatePrint( sub );
	}
	else
	{
		bufferOut.Copy( buffer );
		
		line = new cTextLine( m_lineBuffer, NULL, bufferOut.GetBuffer() );
		
		scrollby = CalculatePrint( line );
	}
	
	if ( m_linesDrawed >= GetHeight() ) {
		ScrollDown( scrollby );
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

	lineb = new cTextLine( m_lineBuffer, line, bufferOut.GetBuffer() );

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
		/* TODO: Add translation of the config to buffer. */
		
		/* Skype like text. */
		if (m_useSlibText) {
		
			if ( line->GetParentNode() ) {
				if ( m_slibText.GetBuffer() )
					bufferLine.Copy(m_slibText.GetBuffer());
				
				bufferLine.Cat( line );

				m_plimLexer.Refresh( bufferLine.GetBuffer() );

				atdy += PrintLexer( &m_plimLexer, 0, atdy);
			}
 			else { /* If have no parent then its the first line */
 				if ( m_firstText.GetLength() ) {
 					bufferLine.Copy ( m_firstText.GetBuffer() );
 					
	 				bufferLine.Cat ( line->GetBuffer() );
 
					m_plimLexer.Refresh( bufferLine.GetBuffer() );
					atdy += PrintLexer( &m_plimLexer, 0, atdy);
				}
			}

			if ( line->GetParentNode() ) {
				if ( line == line->GetParentNode()->GetLastNode() ) {
					if ( m_lastText.GetLength() ) {
						bufferLine.Copy(m_lastText.GetBuffer());

						m_plimLexer.Refresh( bufferLine.GetBuffer() );

						atdy += PrintLexer( &m_plimLexer, 0, atdy);
					}
				}
			}
		}
		/* Normal styled text. */
		else {
			bufferLine.Clean();
			
			if (m_slibText.GetLength() ) {
				cString* translated = TranslateConfigVariables( m_slibText.GetBuffer(), TRANSLATE_PTR(TranslateCallback), line );
				
 				bufferLine.Copy ( translated );

 				delete translated;
			}

 			bufferLine.Cat ( line->GetBuffer() );

			m_plimLexer.Refresh( bufferLine.GetBuffer() );
			atdy += PrintLexer( &m_plimLexer, 0, atdy);
		}

		if (atdy > mtdy )
			break;

		m_linesDrawed = atdy;

		line = (cTextLine*) m_lineBuffer->GetNext(line);
	}

	cCursesWindow::PartialUpdate();
}

char* cTextWindow::TranslateCallback( cPlimToken* token, void* data ) {
	cTextLine* line = (cTextLine*) data;
	char* mal = (char*) malloc(1024);
	
	memset(mal, 0, 1024);
	
	if (!token->Compare("time")) {
		struct tm *tm = localtime(line->GetTime());
		
		strftime(mal, 1024, "%H:%M:%S", tm);

	}
	
	return mal;
}

void cTextWindow::SetSlibUse(int slib) {
	m_useSlibText = slib;
}

void cTextWindow::SetFirstText(const char* text) {
	if (text)
		m_firstText.Copy ( text ) ;
}

void cTextWindow::SetSlibText(const char* text) {
	if (text)
		m_slibText.Copy ( text );
}

void cTextWindow::SetLastText(const char* text) {
	if (text)
		m_lastText.Copy( text );
}

void cTextWindow::SetRoom(cAbstractRoom* room) {
	m_room = room;
}

cAbstractRoom* cTextWindow::GetRoom(void) {
	return m_room;
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
