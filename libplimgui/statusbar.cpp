#include "statusbar.h"

namespace NSWindows {

cStatusWindow::cStatusWindow(cApplication* app, cCursesWindow* parent)
:	cCursesWindow(app, 0, 0, 1, parent->GetWidth() + 10, parent) {
	m_panelNodes = new cTreeNodes();

	CreateFormattingNodes();

	SetFocus( FALSE );

	REGISTER_S_CALL( "[", OnOpenCloseItem );
	REGISTER_S_CALL( "]", OnOpenCloseItem );

}

cStatusWindow::~cStatusWindow(void) {
	if (m_panelNodes)
	{
		delete m_panelNodes;
	}
	DestroyFormattingNodes();
}

cStatusItem* cStatusWindow::Add(const char* identifier) {
	cStatusItem* item = new cStatusItem( m_panelNodes, identifier );
	return item;
}

cStatusItem* cStatusWindow::Get(const char* identifier) {
	cStatusItem* item = (cStatusItem*) m_panelNodes->GetFirstNode();

	while ( item ) {
		
		if (!strcmp(identifier, item->GetName() )) {
			return item;
		}

		item = (cStatusItem*) m_panelNodes->GetNext(item);
	}
	return NULL;
}

void cStatusWindow::Delete( const char* identifier ) {
	m_panelNodes->RemoveNode( Get(identifier) );
}


void cStatusWindow::PartialUpdate(void) {
	cStatusItem* item = (cStatusItem*) m_panelNodes->GetFirstNode();
	cString buffer;
	int x;

	if (!IsPartialNeccesary())
		return;
	
	Erase();
 
	::wbkgd(GetWindowHandle(), COLOR_PAIR(GetApplication()->GetColorPair(0, COLOR_BLUE) ));

	x = 2;

	while ( item ) {

		buffer.Copy("[");
		buffer.Cat( item->GetCaption() );
		buffer.Cat("] ");

		lexhex.Refresh( buffer.GetBuffer() );

		PrintLexer(&lexhex, x, 0);

		x += buffer.GetLength();

		item = (cStatusItem*) m_panelNodes->GetNext(item);
	}
	
	cCursesWindow::PartialUpdate();
}

int cStatusWindow::OnOpenCloseItem(cCursesWindow* window, cPlimToken* token, PlimAttrs* attrs) {
	attrs->fg = COLOR_GREEN;
	attrs->bg = COLOR_BLUE;
	attrs->usecolor = 1;
	return 0;
}

};
