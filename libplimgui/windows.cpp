#include "application.hpp"
#include "windows.hpp"
#include "box.hpp"

namespace plimgui_curses
{

cCursesWindow::cCursesWindow(plimgui_curses::cApplication* app, 
										int left, int top, 
										int height, int width, 
										cCursesWindow* parent)	:	plimuni::cTreeNode( app, parent, NULL ),
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
																			m_colorPair(0),
																			m_sizeChanged(1),
																			m_formatNodes(NULL) 
{

	if (m_appInstance) 
	{

		if (!m_windowParent) 
		{
			m_windowHandle = ::newwin(height, width, top, left);
		} 
		else 
		{
			if (!m_windowParent->getWindowHandle()) 
			{}

			m_windowHandle = ::subwin(m_windowParent->getWindowHandle(), height, width, m_windowParent->getTop() + top, m_windowParent->getLeft() + left);
			
			::leaveok(m_windowHandle, TRUE);

			if (!m_windowHandle) 
			{}
			
			m_windowParent->needUpdate();
		}

		m_boxPtr = (void*) new cBox( this );

		if (m_windowParent)
			::touchwin(m_windowParent->getWindowHandle());

		::wrefresh(m_windowHandle);

		needUpdate();
	}

}

/* Constructor used for spawning a fake root window */
cCursesWindow::cCursesWindow(plimgui_curses::cApplication* app)	:	plimuni::cTreeNode( app, NULL, NULL ),
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
																							m_windowData(NULL),
																							m_windowAlign(none),
																							m_colorPair(0),
																							m_sizeChanged(0),
																							m_formatNodes(NULL)  
{

	if (m_appInstance) 
	{
		setWindowHandle( m_appInstance->getWindowHandle() );

		needUpdate();
	}
}

cCursesWindow::~cCursesWindow(void) 
{
	if (m_boxPtr) 
	{
		delete (cBox*) m_boxPtr;
		m_boxPtr = NULL;
	}

	if (m_formatNodes) 
	{
		/* Note, if you share some format nodes between windows you should in the
			window that do the share zero the pointer so it prevents deleting it */
		delete m_formatNodes;
	}
	
	if (m_windowHandle) 
	{
		::delwin(m_windowHandle);
	}
}

void cCursesWindow::recreateWindow(void) 
{
	cCursesWindow* child;
	cCursesWindow* lastChild;

	child = getFirstWindow();
	lastChild = getLastWindow();

	while ( child ) {

		if ( child->isSizeChanged() )
			child->recreateWindow();

		if (child == lastChild)
			break; /* RecreateWindow done. */

		child = (cCursesWindow*) m_appInstance->getNext( child );
	}

	if (m_windowHandle) {
		::delwin(m_windowHandle);
		m_windowHandle = NULL;
	}

	/* TODO: Update the childs for not showing */
	if (m_isVisible) {

		if (m_windowParent)
			m_windowHandle = ::subwin(m_windowParent->getWindowHandle(), m_height, m_width, getTop(), getLeft());
		else
			m_windowHandle = ::newwin(getHeight(), getWidth(), getTop(), getLeft());

		leaveok(m_windowHandle, TRUE);
	}

}

void cCursesWindow::erase(void) {
	::werase(m_windowHandle);
}

void cCursesWindow::clear(void) {
	::wclear(m_windowHandle);
}

void cCursesWindow::needUpdate(void) {
	m_needUpdate = TRUE;
}

void cCursesWindow::needPartialUpdate(void) {
	m_needPartialUpdate = TRUE;
}

void cCursesWindow::update(void) {
	cBox* box = (cBox*) m_boxPtr;
	cCursesWindow* data;

	if (!m_needUpdate)
		return;

	if ( box  && m_windowHandle) {
		/* recalculating is done. */
		box->update();
	}

	if ( m_windowParent ) { /* Update the sizes only if the window is a child */
		//if ( IsSizeChanged() ) {
			recreateWindow();
			m_sizeChanged = 0;
		//}

		if (m_windowHandle) {}
	}

 	if (!m_windowHandle) {
		m_needUpdate = FALSE;
		return;
	}

	/* Update child windows */
	data = getFirstWindow();

	while ( data ) {
		data->update();
		data = data->getNextNode();
	}

	/* Set the window for repaint */
	if (m_windowParent)
		needPartialUpdate();

	m_needUpdate = FALSE;
}

void cCursesWindow::forceUpdate(void) {
	cBox* box = (cBox*) m_boxPtr;
	cCursesWindow* data;

	if ( box ) {
		/* recalculating is done. */
		box->update();
	}

	/* Update child windows */
	data = getFirstWindow();

	while ( data ) {
		data->needUpdate();
		data = data->getNextNode();
	}

	needUpdate();
	update();
}

void cCursesWindow::partialUpdate(void) 
{
	if (!m_needPartialUpdate || !m_windowHandle )
	{
		return;
	}

	/* let it handle by the doupdate from curses */
	if (!m_windowParent)
		return;

	::wnoutrefresh(m_windowHandle);

	m_needPartialUpdate = FALSE;
}

int cCursesWindow::print(plimuni::cString* string, int start)
{
	int ret = start;
	int x;
	int i;

	if (!string || !string->getLength() || start < 0 || start > getHeight() )
		return 0;

	x = 0;
	i = 0;

	::wmove(getWindowHandle(), ret, x );

	for (; i < string->getLength(); i++ ) 
	{

		if ( x > getWidth() ) 
		{
			x = 0;
			ret += 1;
		}

		if ( ret > getHeight() - 1 )
			break;

		::wmove( m_windowHandle, ret, x );
		::waddch( m_windowHandle, string->getChar(i));

		x++;
	}

	return ret - start + 1;
}

int cCursesWindow::print(plimuni::cString* string, int x, int y) 
{
	int ret = y;
	int i;

	if (!string || !string->getLength() || y < 0 || y > getHeight() )
		return 0;

	i = 0;

	::wmove(getWindowHandle(), ret, x );

	for (; i < string->getLength(); i++ ) 
	{
		if ( x > getWidth() ) 
		{
			x = 0;
			ret += 1;
		}

		if ( ret > getHeight() - 1 )
			break;

		::wmove( m_windowHandle, ret, x - 1 );
		::waddch( m_windowHandle, string->getChar(i));

		x++;
	}

	return ret - y + 1;
}

int cCursesWindow::printLexer(plimuni::cPlimLexer* lexer, int x, int y) 
{
	int yy = y;
	int i;
	plimuni::cPlimToken* token;
	CallbackPtr* ptr;
	PlimAttrs attrs;

	if (!lexer) 
		return 0;

	bzero( &attrs, sizeof(PlimAttrs) );
	attrs.wrap = 1;
	attrs.flags = 0;
	attrs.display = 1;

	::wattrset( m_windowHandle, 0 );

	token = lexer->getFirstNode();

	while ( token ) 
	{

		if (handleSpecChars(token, &attrs)) 
		{
			
		}
		else 
		{
			ptr = getFormattingCallback( token->getBuffer() );

			if ( ptr ) 
			{
				if ( (this->*(ptr->callback)) ( this, token, &attrs ) == -1 ) 
				{
					bzero( &attrs, sizeof(PlimAttrs) );
					attrs.wrap = 1;
					attrs.display = 1;
				} 
				else 
				{
					attrs.formcount = 1;
				}
			}
		}

		if (attrs.reset) 
		{
			wattrset( m_windowHandle, 0 );
			attrs.flags = 0;
			attrs.usecolor = 0;
			attrs.reset = 0;
			attrs.formcount = 0;
			attrs.noformcount = 0;
		}

		if (attrs.usecolor) 
		{
			attrs.flags |= COLOR_PAIR(m_appInstance->getColorPair(attrs.fg, attrs.bg));
		}

		/* Assign some attrs and display the whole word */
		if (attrs.display) 
		{
			::wmove( m_windowHandle, yy, x );

			for (i = 0; i < token->getLength(); i++ ) 
			{
				if (attrs.skipcount) 
				{
					attrs.skipcount--;
					continue;
				}

				if ( attrs.wrap && x > getWidth() ) 
				{
					x = 0;
					yy += 1;
				}

				if ( yy > getHeight() - 1 )
					break;

				::wmove( m_windowHandle, yy, x );
				::waddch( m_windowHandle, token->getChar(i) | attrs.flags);

				if ( attrs.formcount > -1) 
				{
					attrs.formcount--;
					
					if (!attrs.formcount && !attrs.noformcount) 
					{
						 attrs.reset = 1;
					}
				}

				x++;
			}
		}
 
		token = token->getNextNode();
	}

	return yy - y + 1;
}

int cCursesWindow::handleSpecChars(plimuni::cPlimToken* token, PlimAttrs* attrs) 
{
	plimuni::cPlimToken* plim;
	int skipcount = 1;

	if (token->getTokenCase() != PLIM_L_CASE_FLAGS_CURSES)
		return 0;

	switch (token->getTokenExCase())
	{
	
		case PLIM_L_ATTR_COLOR: 
		{
			plim = token->getNextNode();

			if (plim) 
			{
				if ( plim->getTokenCase() == PLIM_L_DIGIT_INTEGER) 
				{
					attrs->fg = atoi(plim->getBuffer());
					skipcount++;
				} 
				else 
				{
					attrs->fg = 0;
				}

				plim = plim->getNextNode();

				if (plim) 
				{
					if (plim->getTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS && plim->getTokenExCase() == PLIM_L_SYMBOL_COMMA) 
					{
						skipcount++;
						plim = plim->getNextNode();
						if (plim) 
						{
							if (plim->getTokenCase() == PLIM_L_DIGIT_INTEGER) 
							{
								attrs->bg = atoi(plim->getBuffer());
								skipcount++;
							} 
							else 
							{
								attrs->bg = 0;
							}
						}
					} 
					else 
					{
						attrs->bg = 0;
					}
				} 
				else 
				{
					attrs->bg = 0;
				}
				
			}

			attrs->usecolor = 1;

			break;
		}

		case PLIM_L_ATTR_BOLD: 
		{
			attrs->flags |= A_BOLD;
			break;
		}

		case PLIM_L_ATTR_RESET: 
		{
			attrs->reset = 1; /* Reset whole thing */
		}

		case PLIM_L_ATTR_FIXED: 
		{
			/* dunno */
			break;
		}

		case PLIM_L_ATTR_REVERSE:
		case PLIM_L_ATTR_REVERSE2: 
		{
			attrs->flags |= A_REVERSE;
			break;
		}

		case PLIM_L_ATTR_ITALIC: 
		{
			/* Dunno, ncurses doesnt support italic stuff */
			break;
		}

		case PLIM_L_ATTR_UNDERLINE2:
		case PLIM_L_ATTR_UNDERLINE: 
		{
			attrs->flags |= A_UNDERLINE;
			break;
		}

		default:
			break;
	}

	attrs->skipcount = skipcount;

	return 0;
}

int cCursesWindow::calculatePrint(plimuni::cString* string) 
{
	if (string)
		return (int) ( string->getLength() / getWidth() ) + 1;
	else
		return 0;
}

plimuni::cString* cCursesWindow::translateConfigVariables(const char* buffer, OnTranslateCallback call, void* data) 
{
	plimuni::cPlimLexer lexhex(buffer);
	plimuni::cPlimToken* token;
	plimuni::cString* result;
	char sp[2] = { 0, 0 };
	
	if (!buffer)	return NULL;
	
	result = new plimuni::cString();
	
	token = (plimuni::cPlimToken*) lexhex.getFirstNode();
	
	while (token) {

		if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_MATH ) 
		{
			if ( token->getTokenExCase() == PLIM_L_SYMBOL_MATH_SIGN_PLUS ) 
			{
				/* Ok get the next token */
				token = (plimuni::cPlimToken*) lexhex.getNext(token);

				if (!token)	break;
				
				if ( token->getTokenCase() == PLIM_L_IDENTIFIER ) 
				{
					/* Process. */
					if (!token->compare( "b" )) 
					{
						/* Insert curses bold char */
						sp[0] = PLIM_L_ATTR_BOLD;
						result->cat( &sp[0] );
					}
					else
					if (!token->compare( "u")) 
					{
						sp[0] = PLIM_L_ATTR_UNDERLINE;
						/* Insert curses underline char */
						result->cat( &sp[0] );
					}
					else
					if (!token->compare( "c" )) 
					{
						sp[0] = PLIM_L_ATTR_COLOR;
						result->cat( &sp[0] );
						
						token = (plimuni::cPlimToken*) lexhex.getNext(token);
						/* Expect a space */
						if (token && token->getTokenCase() == PLIM_L_BLANK) 
						{
						
							token = (plimuni::cPlimToken*) lexhex.getNext(token);
							
							if (token) 
							{
								if ( token->getTokenCase() == PLIM_L_DIGIT_INTEGER) 
								{
									result->cat(token);
								}

								token = (plimuni::cPlimToken*) lexhex.getNext(token);
								
								if (token) 
								{
									if (token->getTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS && token->getTokenExCase() == PLIM_L_SYMBOL_COMMA) 
									{
										result->cat(",");
										
										token = (plimuni::cPlimToken*) lexhex.getNext(token);

										if (token) 
										{
											if (token->getTokenCase() == PLIM_L_DIGIT_INTEGER) 
											{
												result->cat(token);
											}
										}
									}
								}
							}
							
						}
					}
					else
					if (!token->compare( "r" )) 
					{
						sp[0] = PLIM_L_ATTR_RESET;
						result->cat( &sp[0] );
					}
					else
					{
						if (call) 
						{
						
							char* rb = (this->*(call)) ( token, data );
						
							if (rb) 
							{
								result->cat(rb);
								free(rb);
							}
						}
					}
					
					
				}
			} else /* Cause we dont know if we will handle more symbols. */
				result->cat(token);
		}
		else
			result->cat(token);
		
	
		token = (plimuni::cPlimToken*) lexhex.getNext(token);
	}

	return result;
}

void cCursesWindow::createFormattingNodes(void) 
{
	if (!m_formatNodes) 
	{
		/* Should be allowed to read from config */
		m_formatNodes = new plimuni::cHashNodes(10003);
	}
}

void cCursesWindow::destroyFormattingNodes(void) 
{
	if (m_formatNodes) 
	{
		delete m_formatNodes;
		m_formatNodes = NULL;
	}
}

int cCursesWindow::registerFormattingCallback( const char* identifier, OnPrintCallback callback ) 
{
	CallbackPtr* ptr = (CallbackPtr*) malloc(sizeof(CallbackPtr));

	ptr->callback = callback;

	m_formatNodes->add( identifier, ptr );

	return 0;
}

int cCursesWindow::unregisterFormattingCallback( const char* identifier ) 
{
	plimuni::cHashNode* node;
	CallbackPtr* ptr;

	if (!identifier || !m_formatNodes)
		return 0;

	node = m_formatNodes->get( identifier, strlen( identifier ) );

	if ( node ) 
	{
		ptr = (CallbackPtr*) node->getNodeData();

		if (ptr) 
		{
			free(ptr);
		}

		m_formatNodes->remove( node, strlen( identifier ) );
	}

	return 0;
}

CallbackPtr* cCursesWindow::getFormattingCallback( const char* identifier ) 
{
	plimuni::cHashNode* node;

	if (!identifier || !m_formatNodes)
		return NULL;
	
	node = m_formatNodes->get( identifier, strlen( identifier ) );

	if ( node && node->getPtr() ) 
	{
		return (CallbackPtr*) node->getPtr();
	}

	return NULL;
}

int cCursesWindow::onKeyPressed( const int key ) 
{
	return 0;
}

int cCursesWindow::onResize(void) 
{
	//NeedUpdate();
	//NeedPartialUpdate();
	return 0;
}


};
