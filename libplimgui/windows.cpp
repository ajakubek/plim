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
	m_colorPair(0),
	m_sizeChanged(1),
	m_formatNodes(NULL) {

	if (m_appInstance) {

		if (!m_windowParent) {
			m_windowHandle = ::newwin(height, width, top, left);
		} else {
			if (!m_windowParent->GetWindowHandle()) {}

			m_windowHandle = ::subwin(m_windowParent->GetWindowHandle(), height, width, m_windowParent->GetTop() + top, m_windowParent->GetLeft() + left);
			leaveok(m_windowHandle, TRUE);
			if (!m_windowHandle) {
			}
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
	m_windowData(NULL),
	m_windowAlign(none),
	m_colorPair(0),
	m_sizeChanged(0),
	m_formatNodes(NULL)  {

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

	if (m_formatNodes) {
		/* Note, if you share some format nodes between windows you should in the
			window that do the share zero the pointer so it prevents deleting it */
		delete m_formatNodes;
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

		if ( child->IsSizeChanged() )
			child->RecreateWindow();

		if (child == lastChild)
			break; /* RecreateWindow done. */

		child = (cCursesWindow*) m_appInstance->GetNext( child );
	}

	if (m_windowHandle) {
		::delwin(m_windowHandle);
		m_windowHandle = NULL;
	}

	/* TODO: Update the childs for not showing */
	if (m_isVisible) {

		if (m_windowParent)
			m_windowHandle = ::subwin(m_windowParent->GetWindowHandle(), m_height, m_width, GetTop(), GetLeft());
		else
			m_windowHandle = ::newwin(GetHeight(), GetWidth(), GetTop(), GetLeft());

		leaveok(m_windowHandle, TRUE);
	}

}

void cCursesWindow::Erase(void) {
	::werase(m_windowHandle);
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

	if ( box  && m_windowHandle) {
		/* recalculating is done. */
		box->Update();
	}

	if ( m_windowParent ) { /* Update the sizes only if the window is a child */
		//if ( IsSizeChanged() ) {
			RecreateWindow();
			m_sizeChanged = 0;
		//}

		if (m_windowHandle) {}
	}

 	if (!m_windowHandle) {
		m_needUpdate = FALSE;
		return;
	}

	/* Update child windows */
	data = GetFirstWindow();

	while ( data ) {
		data->Update();
		data = data->GetNextNode();
	}

	/* Set the window for repaint */
	if (m_windowParent)
		NeedPartialUpdate();

	m_needUpdate = FALSE;
}

void cCursesWindow::ForceUpdate(void) {
	cBox* box = (cBox*) m_boxPtr;
	cCursesWindow* data;

	if ( box ) {
		/* recalculating is done. */
		box->Update();
	}

	/* Update child windows */
	data = GetFirstWindow();

	while ( data ) {
		data->NeedUpdate();
		data = data->GetNextNode();
	}

	NeedUpdate();
	Update();
}

void cCursesWindow::PartialUpdate(void) {
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

int cCursesWindow::Print(cString* string, int start) {
	int ret = start;
	int x;
	int i;

	if (!string || !string->GetLength() || start < 0 || start > GetHeight() )
		return 0;

	x = 0;
	i = 0;

	wmove(GetWindowHandle(), ret, x );

	for (; i < string->GetLength(); i++ ) {
		

		if ( x > GetWidth() ) {
			x = 0;
			ret += 1;
		}

		if ( ret > GetHeight() - 1 )
			break;

		wmove( m_windowHandle, ret, x );
		waddch( m_windowHandle, string->GetChar(i));

		x++;
	}

	return ret - start + 1;
}

int cCursesWindow::Print(cString* string, int x, int y) {
	int ret = y;
	int i;

	if (!string || !string->GetLength() || y < 0 || y > GetHeight() )
		return 0;

	i = 0;

	wmove(GetWindowHandle(), ret, x );

	for (; i < string->GetLength(); i++ ) {
		if ( x > GetWidth() ) {
			x = 0;
			ret += 1;
		}

		if ( ret > GetHeight() - 1 )
			break;

		wmove( m_windowHandle, ret, x - 1 );
		waddch( m_windowHandle, string->GetChar(i));

		x++;
	}

	return ret - y + 1;
}

/* Lazy, after the lexer is here this will be probally removed! */
int cCursesWindow::PrintFormated(cCursesString* string, int x, int y) {
	int yy = y;
	int i;
	int flags;
	int useColors;
	int val;

	if (!string || !string->GetLength() || y < 0 || y > GetHeight() )
		return 0;

	i = 0;
	flags = 0;
	useColors = 0;

	wmove( GetWindowHandle(), yy, x );
	wattrset( GetWindowHandle(), 0 );

	for (; i < string->GetLength(); i++ ) {
		if ( x > GetWidth() ) {
			x = 0;
			yy += 1;
		}

		if ( yy > GetHeight() - 1 )
			break;

		string->GetFlags(i, &flags, &useColors);

		if ( useColors ) {
			/* ignore the end */
			char x[2] = { 0, 0 };
			/* Just an test case for getting the pair, should be moved somewhere and changed */
			int firstCol;
			int secondCol;

			x[0] = string->GetChar(i + 1);			
			firstCol = atoi((char*) &x[0]);

			x[0] = string->GetChar(i + 2);
			secondCol = atoi((char*) &x[0]);
			
			wattron(m_windowHandle, COLOR_PAIR(m_appInstance->GetColorPair(firstCol, secondCol)));
			
		}

		if ( flags == ATTR_RESET ) {
			wattrset( m_windowHandle, 0 );
			flags = 0;
		}

		if (!string->IsSpecial(i)) {
			wmove( m_windowHandle, yy, x - 1);
			waddch( m_windowHandle, string->GetChar(i) | flags);
			x++;
		}

	}

	return yy - y + 1;
}

int cCursesWindow::PrintLexer(cPlimLexer* lexer, int x, int y) {
	int yy = y;
	int i;
	cPlimToken* token;
	CallbackPtr* ptr;
	PlimAttrs attrs;

	if (!lexer) return 0;

	bzero( &attrs, sizeof(PlimAttrs) );
	attrs.wrap = 1;
	attrs.flags = 0;
	attrs.display = 1;

	wattrset( m_windowHandle, 0 );

	token = lexer->GetFirstNode();

	while ( token ) {

		if (HandleSpecChars(token, &attrs)) {
			
		}
		else {
			ptr = GetFormattingCallback( token->GetBuffer() );

			if ( ptr ) {
				if ( (this->*(ptr->callback)) ( this, token, &attrs ) == -1 ) {
					bzero( &attrs, sizeof(PlimAttrs) );
					attrs.wrap = 1;
					attrs.display = 1;
				} else {
					attrs.formcount = 1;
				}
			}
		}

		if (attrs.reset) {
			wattrset( m_windowHandle, 0 );
			attrs.flags = 0;
			attrs.usecolor = 0;
			attrs.reset = 0;
			attrs.formcount = 0;
			attrs.noformcount = 0;
		}

		if (attrs.usecolor) {
			attrs.flags |= COLOR_PAIR(m_appInstance->GetColorPair(attrs.fg, attrs.bg));
		}

		/* Assign some attrs and display the whole word */
		if (attrs.display) {
			wmove( m_windowHandle, yy, x );

			for (i = 0; i < token->GetLength(); i++ ) {
				if (attrs.skipcount) {
					attrs.skipcount--;
					continue;
				}

				if ( attrs.wrap && x > GetWidth() ) {
					x = 0;
					yy += 1;
				}

				if ( yy > GetHeight() - 1 )
					break;

				wmove( m_windowHandle, yy, x );
				waddch( m_windowHandle, token->GetChar(i) | attrs.flags);

				if ( attrs.formcount > -1) {
					attrs.formcount--;
					if (!attrs.formcount && !attrs.noformcount) {
						 attrs.reset = 1;
					}
				}

				x++;
			}
		}
 
		token = token->GetNextNode();
	}

	return yy - y + 1;
}

int cCursesWindow::HandleSpecChars(cPlimToken* token, PlimAttrs* attrs) {
	cPlimToken* plim;
	int skipcount = 1;

	if (token->GetTokenCase() != PLIM_L_CASE_FLAGS_CURSES)
		return 0;

	switch (token->GetTokenExCase())
	{
		case ATTR_COLOR: {
			plim = token->GetNextNode();

			if (plim) {
				if ( plim->GetTokenCase() == PLIM_L_DIGIT_INTEGER) {
					attrs->fg = atoi(plim->GetBuffer());
					skipcount++;
				} else {
					attrs->fg = 0;
				}

				plim = plim->GetNextNode();

				if (plim) {
					if (plim->GetTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS && plim->GetTokenExCase() == PLIM_L_SYMBOL_COMMA) {
						skipcount++;
						plim = plim->GetNextNode();
						if (plim) {
							if (plim->GetTokenCase() == PLIM_L_DIGIT_INTEGER) {
								attrs->bg = atoi(plim->GetBuffer());
								skipcount++;
							} else {
								attrs->bg = 0;
							}
						}
					} else {
						attrs->bg = 0;
					}
				} else {
					attrs->bg = 0;
				}
				
			}

			attrs->usecolor = 1;

			break;
		}

		case ATTR_BOLD: {
			attrs->flags |= A_BOLD;
			break;
		}

		case ATTR_RESET: {
			attrs->reset = 1; /* Reset whole thing */
		}

		case ATTR_FIXED: {
			/* dunno */
			break;
		}

		case ATTR_REVERSE:
		case ATTR_REVERSE2: {
			attrs->flags |= A_REVERSE;
			break;
		}

		case ATTR_ITALIC: {
			/* Dunno, ncurses doesnt support italic stuff */
			break;
		}

		case ATTR_UNDERLINE2:
		case ATTR_UNDERLINE: {
			attrs->flags |= A_UNDERLINE;
			break;
		}

		default:
			break;
	}

	attrs->skipcount = skipcount;

	return 0;
}

int cCursesWindow::CalculatePrint(cString* string) {
	if (string)
		return (int) ( string->GetLength() / GetWidth() ) + 1;
	else
		return 0;
}

void cCursesWindow::CreateFormattingNodes(void) {
	if (!m_formatNodes) {
		/* Should be allowed to read from config */
		m_formatNodes = new cHashNodes(10003);
	}
}

void cCursesWindow::DestroyFormattingNodes(void) {
	if (m_formatNodes) {
		delete m_formatNodes;
		m_formatNodes = NULL;
	}
}

int cCursesWindow::RegisterFormattingCallback( const char* identifier, OnPrintCallback callback ) {
	CallbackPtr* ptr = (CallbackPtr*) malloc(sizeof(CallbackPtr));

	ptr->callback = callback;

	m_formatNodes->Add( identifier, ptr );

	return 0;
}

int cCursesWindow::UnregisterFormattingCallback( const char* identifier ) {
	cHashNode* node;
	CallbackPtr* ptr;

	if (!identifier)
		return 0;

	node = m_formatNodes->Get( identifier, strlen( identifier ) );

	if ( node ) {
		ptr = (CallbackPtr*) node->GetNodeData();

		if (ptr) {
			free(ptr);
		}

		m_formatNodes->Delete( node, strlen( identifier ) );
	}

	return 0;
}

CallbackPtr* cCursesWindow::GetFormattingCallback( const char* identifier ) {
	cHashNode* node;

	if (!identifier)
		return NULL;

	node = m_formatNodes->Get( identifier, strlen( identifier ) );

	if ( node && node->GetPtr() ) {
		return (CallbackPtr*) node->GetPtr();
	}

	return NULL;
}

int cCursesWindow::OnKeyPressed( const int key ) {
	return 0;
}

int cCursesWindow::OnResize(void) {
	return 0;
}


};
