#include "statusbar.hpp"

namespace plimgui_curses 
{

cStatusWindow::cStatusWindow(plimgui_curses::cApplication* app, plimgui_curses::cCursesWindow* parent)	:	plimgui_curses::cCursesWindow(app, 0, 0, 1, parent->getWidth() + 10, parent) 
{
	m_panelNodes = new plimuni::cTreeNodes();

	createFormattingNodes();

	setFocus( FALSE );

	REGISTER_S_CALL( "[", onOpenCloseItem );
	REGISTER_S_CALL( "]", onOpenCloseItem );

}

cStatusWindow::~cStatusWindow(void) 
{
	if (m_panelNodes)
	{
		delete m_panelNodes;
	}
	
	destroyFormattingNodes();
}

cStatusItem* cStatusWindow::add(const char* identifier) 
{
	cStatusItem* item = new cStatusItem( m_panelNodes, identifier );
	return item;
}

cStatusItem* cStatusWindow::get(const char* identifier) 
{
	cStatusItem* item = (cStatusItem*) m_panelNodes->getFirstNode();

	while ( item ) {
		
		if (!strcmp(identifier, item->getName() )) {
			return item;
		}

		item = (cStatusItem*) m_panelNodes->getNext(item);
	}
	return NULL;
}

void cStatusWindow::remove( const char* identifier ) 
{
	m_panelNodes->removeNode( get(identifier) );
}


void cStatusWindow::partialUpdate(void) 
{
	cStatusItem* item = (cStatusItem*) m_panelNodes->getFirstNode();
	plimuni::cString buffer;
	int x;

	if (!isPartialNeccesary())
		return;
	
	::erase();
 
	::wbkgd(getWindowHandle(), COLOR_PAIR(getApplication()->getColorPair(0, COLOR_BLUE) ));

	x = 2;

	while ( item ) {

		buffer.copy("[");
		buffer.cat( item->getCaption() );
		buffer.cat("] ");

		lexhex.refresh( buffer.getBuffer() );

		printLexer(&lexhex, x, 0);

		x += buffer.getLength();

		item = (cStatusItem*) m_panelNodes->getNext(item);
	}
	
	plimgui_curses::cCursesWindow::partialUpdate();
}

int cStatusWindow::onOpenCloseItem(plimgui_curses::cCursesWindow* window, plimuni::cPlimToken* token, plimgui_curses::PlimAttrs* attrs) 
{
	attrs->fg = COLOR_GREEN;
	attrs->bg = COLOR_BLUE;
	attrs->usecolor = 1;
	return 0;
}

};
