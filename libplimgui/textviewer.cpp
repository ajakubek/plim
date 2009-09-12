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

cTextWindow::~cTextWindow( void ) {
	DestroyFormattingNodes( );
	if ( m_lineBuffer )
		delete m_lineBuffer;
}

cTextLine* cTextWindow::AddLine( const char* buffer, cAbstractUser* user, ... ) {
	cTextLine* line;
	cString b;
	int scrollby;
	va_list l;

	if (!buffer)	return NULL;

	va_start( l, user );
	b.Copy( buffer, l );
	va_end ( l );

	if (m_useSlibText) {
		
	}
	else {
		line = new cTextLine( m_lineBuffer, NULL, b.GetBuffer() );
		line->SetUser( user );
		line->SetLineType( ltUserMessage );

		scrollby = CalculatePrint( &b );
	}

	if ( m_linesDrawed >= GetHeight() ) {
		ScrollDown( scrollby );
	}

	NeedPartialUpdate();
	Update(); /* Draw it instant */

	return line;
}

cTextLine* cTextWindow::AddLine(const char* buffer, cAbstractUser* user, va_list args) {
	cTextLine* line;
	cString b;
	int scrollby;

	if (!buffer)	return NULL;

	b.Copy( buffer, args );

	if (m_useSlibText) {
		
	}
	else {
		line = new cTextLine( m_lineBuffer, NULL, b.GetBuffer() );
		line->SetUser( user );
		line->SetLineType( ltUserMessage );

		scrollby = CalculatePrint( &b );
	}

	if ( m_linesDrawed >= GetHeight() ) {
		ScrollDown( scrollby );
	}

	NeedPartialUpdate();
	Update(); /* Draw it instant */

	return line;
}

cTextLine* cTextWindow::AddDebugLine(const char* buffer, ...) {
	cTextLine* line;
	cString b;
	int scrollby;
	va_list l;

	if (!buffer)	return NULL;

	va_start( l, buffer );
	b.Copy( buffer, l );
	va_end ( l );

	line = new cTextLine( m_lineBuffer, NULL, b.GetBuffer() );
	scrollby = CalculatePrint( &b );

	if ( m_linesDrawed >= GetHeight() ) {
		ScrollDown( scrollby );
	}

	NeedPartialUpdate();
	Update(); /* Draw it instant */

	return line;
}

cTextLine* cTextWindow::AddDebugLine(const char* buffer, va_list args) {
	cTextLine* line;
	cString b;
	int scrollby;

	if (!buffer)	return NULL;

	b.Copy( buffer, args );

	line = new cTextLine( m_lineBuffer, NULL, b.GetBuffer() );
	scrollby = CalculatePrint( &b );

	if ( m_linesDrawed >= GetHeight() ) {
		ScrollDown( scrollby );
	}

	NeedPartialUpdate();
	Update(); /* Draw it instant */

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
		if ( m_useSlibText && line->GetLineType() == ltUserMessage ) {
		
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
			
			if ( line->GetLineType() == ltUserMessage ) {

				if (m_slibText.GetLength() ) {
					cString* translated = TranslateConfigVariables( m_slibText.GetBuffer(), TRANSLATE_PTR(TranslateCallback), line );
					
 					bufferLine.Copy ( translated );

 					delete translated;
				}

 				bufferLine.Cat ( line->GetBuffer() );
			} 
			else {
				cString* translated = TranslateConfigVariables( line->GetBuffer(), TRANSLATE_PTR(TranslateCallback), line );
				bufferLine.Copy( translated );
				delete translated;
			}


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
/* Please note. Dont return a char* that is used somewhere cause it will be free`dup after the callback, thats why we allocated (char*) mal */
char* cTextWindow::TranslateCallback( cPlimToken* token, void* data ) {
	cTextLine* line = (cTextLine*) data;
	char* mal = (char*) malloc(1024);
	
	memset(mal, 0, 1024);
	
	if (!token->Compare("time")) {
		struct tm *tm = localtime(line->GetTime());
		
		strftime(mal, 1024, "%H:%M:%S", tm);

	}
	else
	if (!token->Compare("user")) {
		if (line->GetUser()) {
			sprintf(mal, "%s", line->GetUser()->GetUserName());
		}
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
