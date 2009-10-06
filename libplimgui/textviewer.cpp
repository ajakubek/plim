#include "textviewer.hpp"

namespace plimgui_curses
{

cTextWindow::cTextWindow(plimgui_curses::cApplication* app, plimgui_curses::cCursesWindow* parent)	:	cCursesWindow(app, 1, 1, 1, parent->getWidth(), parent),
																																		m_lineTop(NULL),
																																		m_linesDrawed(0),
																																		m_useSlibText(0),
																																		m_room(NULL) 
{

	setFocus( FALSE );

	m_lineBuffer = new plimuni::cTreeNodes();
	
	createFormattingNodes();

	REGISTER_F_CALL( "int", onCppKeyword );
	REGISTER_F_CALL( "void", onCppKeyword );
	REGISTER_F_CALL( "typedef", onCppKeyword );
	REGISTER_F_CALL( "union", onCppKeyword );
	REGISTER_F_CALL( "char", onCppKeyword );
	REGISTER_F_CALL( "const", onCppKeyword );
	REGISTER_F_CALL( "virtual", onCppKeyword );
	REGISTER_F_CALL( "mutable", onCppKeyword );
	REGISTER_F_CALL( "operator", onCppKeyword );
	REGISTER_F_CALL( "IC0ffeeCup", onCppKeyword );

}

cTextWindow::~cTextWindow( void ) 
{
	destroyFormattingNodes( );
	
	if ( m_lineBuffer )
	{
		delete m_lineBuffer;
	}
}

cTextLine* cTextWindow::addLine( const char* buffer, plimuni::cAbstractUser* user, ... ) 
{
	cTextLine* line;
	plimuni::cString b;
	int scrollby;
	va_list l;

	if (!buffer)	return NULL;

	va_start( l, user );
	b.copy( buffer, l );
	va_end ( l );

	if (m_useSlibText) 
	{
		
	}
	else 
	{
		line = new cTextLine( m_lineBuffer, NULL, b.getBuffer() );
		line->setUser( user );
		line->setLineType( ltUserMessage );

		scrollby = calculatePrint( &b );
	}

	if ( m_linesDrawed >= getHeight() ) 
	{
		scrollDown( scrollby );
	}

	needPartialUpdate();
	update(); /* Draw it instant */

	return line;
}

cTextLine* cTextWindow::addLine(const char* buffer, plimuni::cAbstractUser* user, va_list args) 
{
	cTextLine* line;
	plimuni::cString b;
	int scrollby;

	if (!buffer)	return NULL;

	b.copy( buffer, args );

	if (m_useSlibText) {
		
	}
	else
	{
		line = new cTextLine( m_lineBuffer, NULL, b.getBuffer() );
		line->setUser( user );
		line->setLineType( ltUserMessage );

		scrollby = calculatePrint( &b );
	}

	if ( m_linesDrawed >= getHeight() ) 
	{
		scrollDown( scrollby );
	}

	needPartialUpdate();
	update(); /* Draw it instant */

	return line;
}

cTextLine* cTextWindow::addDebugLine(const char* buffer, ...) 
{
	cTextLine* line;
	plimuni::cString b;
	int scrollby;
	va_list l;

	if (!buffer)	return NULL;

	va_start( l, buffer );
	b.copy( buffer, l );
	va_end ( l );

	line = new cTextLine( m_lineBuffer, NULL, b.getBuffer() );
	scrollby = calculatePrint( &b );

	if ( m_linesDrawed >= getHeight() ) 
	{
		scrollDown( scrollby );
	}

	needPartialUpdate();
	update(); /* Draw it instant */

	return line;
}

cTextLine* cTextWindow::addDebugLine(const char* buffer, va_list args) 
{
	cTextLine* line;
	plimuni::cString b;
	int scrollby;

	if (!buffer)	return NULL;

	b.copy( buffer, args );

	line = new cTextLine( m_lineBuffer, NULL, b.getBuffer() );
	scrollby = calculatePrint( &b );

	if ( m_linesDrawed >= getHeight() ) 
	{
		scrollDown( scrollby );
	}

	needPartialUpdate();
	update(); /* Draw it instant */

	return line;
}

void cTextWindow::scrollDown(int count) 
{
	cTextLine* line;

	if (!count)
		return;

	line = (cTextLine*) m_lineTop;

	while ( line ) 
	{

		if ( !count ) 
		{
			m_lineTop = line;
			break;
		}

		count--;

		line = (cTextLine*) m_lineBuffer->getNext( line );
	}

	needPartialUpdate();
}

void cTextWindow::scrollUp(int count) 
{
	cTextLine* line;

	if (!count)
		return;
	
	line = (cTextLine*) m_lineTop;

	while ( line ) 
	{

		if ( !count ) 
		{
			m_lineTop = line;
			break;
		}

		count--;

		line = (cTextLine*) m_lineBuffer->getPrev( line );
	}

	needPartialUpdate( );
}

void cTextWindow::pageUp(void) 
{
	scrollUp( getHeight() );
}

void cTextWindow::pageDown(void) 
{
	scrollDown( getHeight() );
}

int cTextWindow::onResize(void) 
{
	pin();
	return 0;
}

void cTextWindow::partialUpdate(void) 
{
	cTextLine* line;
	int mtdy, atdy;
	plimuni::cString bufferLine;

	if (!isPartialNeccesary())
		return;
	
	if ( !m_lineTop) 
	{
		m_lineTop = (cTextLine*) m_lineBuffer->getFirstNode();
	}

	erase();

	line = (cTextLine* ) m_lineTop;
	mtdy = getHeight() + 1;
	atdy = 0;

	while ( line ) 
	{
		/* TODO: Add translation of the config to buffer. */
		
		/* Skype like text. */
		if ( m_useSlibText && line->getLineType() == ltUserMessage ) 
		{
		
			if ( line->getParentNode() ) 
			{
				if ( m_slibText.getBuffer() )
				{
					bufferLine.copy(m_slibText.getBuffer());
				}
				
				bufferLine.cat( line );

				m_plimLexer.refresh( bufferLine.getBuffer() );

				atdy += printLexer( &m_plimLexer, 0, atdy);
			}
 			else 
			{ /* If have no parent then its the first line */
 				if ( m_firstText.getLength() ) 
				{
 					bufferLine.copy ( m_firstText.getBuffer() );
 					
	 				bufferLine.cat ( line->getBuffer() );
 
					m_plimLexer.refresh( bufferLine.getBuffer() );
					atdy += printLexer( &m_plimLexer, 0, atdy);
				}
			}

			if ( line->getParentNode() ) 
			{

				if ( line == line->getParentNode()->getLastNode() ) 
				{
					if ( m_lastText.getLength() ) 
					{
						bufferLine.copy(m_lastText.getBuffer());

						m_plimLexer.refresh( bufferLine.getBuffer() );

						atdy += printLexer( &m_plimLexer, 0, atdy);
					}
				}

			}
		}
		/* Normal styled text. */
		else 
		{
			bufferLine.clean();
			
			if ( line->getLineType() == ltUserMessage ) 
			{

				if (m_slibText.getLength() ) 
				{
					plimuni::cString* translated = translateConfigVariables( m_slibText.getBuffer(), TRANSLATE_PTR(translateCallback), line );
					
 					bufferLine.copy ( translated );

 					delete translated;
				}

 				bufferLine.cat ( line->getBuffer() );
			} 
			else 
			{
				plimuni::cString* translated = translateConfigVariables( line->getBuffer(), TRANSLATE_PTR(translateCallback), line );
				bufferLine.copy( translated );
				delete translated;
			}


			m_plimLexer.refresh( bufferLine.getBuffer() );
			atdy += printLexer( &m_plimLexer, 0, atdy);
		}

		if (atdy > mtdy )
			break;

		m_linesDrawed = atdy;

		line = (cTextLine*) m_lineBuffer->getNext(line);
	}

	cCursesWindow::partialUpdate();
}

/* Please note. Dont return a char* that is used somewhere cause it will be free`dup after the callback, thats why we allocated (char*) mal */
char* cTextWindow::translateCallback( plimuni::cPlimToken* token, void* data ) 
{
	cTextLine* line = (cTextLine*) data;
	char* mal = (char*) malloc(1024);
	
	memset(mal, 0, 1024);
	
	if (!token->compare("time")) 
	{
		struct tm *tm = localtime(line->getTime());
		
		strftime(mal, 1024, "%H:%M:%S", tm);

	}
	else
	if (!token->compare("user")) 
	{
		if (line->getUser()) 
		{
			sprintf(mal, "%s", line->getUser()->getUserName());
		}
	}

	return mal;
}

void cTextWindow::setSlibUse(int slib) 
{
	m_useSlibText = slib;
}

void cTextWindow::setFirstText(const char* text) 
{
	if (text)
		m_firstText.copy ( text ) ;
}

void cTextWindow::setSlibText(const char* text) 
{
	if (text)
		m_slibText.copy ( text );
}

void cTextWindow::setLastText(const char* text) 
{
	if (text)
		m_lastText.copy( text );
}

void cTextWindow::setRoom(plimuni::cAbstractRoom* room) 
{
	m_room = room;
}

plimuni::cAbstractRoom* cTextWindow::getRoom(void) 
{
	return m_room;
}

int cTextWindow::onCppKeyword(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs) 
{
	attrs->usecolor = 1;
	attrs->fg = 4;
	attrs->bg = 0;
	attrs->flags = A_BOLD;
	return 0;
}

void cTextWindow::pin(void) 
{
	int dif = getHeight() - m_linesDrawed;

	if ( dif ) { 
		scrollUp( dif + 1 );
	}
}

};
