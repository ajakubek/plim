#include "box.hpp"

namespace plimgui_curses {

cBox::cBox(cCursesWindow *window)	:	m_windowInstance(window) 
{}

cBox::~cBox(void)
{
	if (m_windowInstance) 
	{}
} 

void cBox::update(void) 
{
	plimgui_curses::cCursesWindow* clientWindow = m_windowInstance;
	plimgui_curses::cCursesWindow* childWindow;

	if (!clientWindow)
		return;

	m_lastLeftPos = 0;
	m_lastTopPos = 0;
	m_lastRightPos = clientWindow->getWidth();
	m_lastBottomPos = clientWindow->getHeight();

	childWindow = (plimgui_curses::cCursesWindow*) clientWindow->getFirstWindow();

	while (childWindow) {
		/* The cableguy tells me that we need no anchors in ncurses */
		/* Tell us to update the window */

		//childWindow->NeedUpdate();

		/* IMHO, no test at this commit, so it can be buggy */
		switch (childWindow->getWindowAlign()) 
		{
			case left: {
				
				childWindow->setLeft( m_lastLeftPos  );
				childWindow->setTop( m_lastTopPos );
				childWindow->setHeight( m_lastBottomPos - m_lastTopPos );

				m_lastLeftPos += childWindow->getWidth()  ;


				break;
			}

			case right: 
			{

				childWindow->setLeft ( m_lastRightPos - childWindow->getWidth() - clientWindow->getLeft() );
				childWindow->setTop( m_lastTopPos  );
				childWindow->setHeight ( m_lastBottomPos - m_lastTopPos );

				m_lastRightPos -= childWindow->getWidth();

				break;
			}

			case top: 
			{
				childWindow->setLeft( 0 );
				childWindow->setTop( m_lastTopPos );
				childWindow->setWidth( clientWindow->getWidth() );

				m_lastTopPos += childWindow->getHeight() ;

				break;
			}

			case bottom: 
			{
				childWindow->setLeft( 0 );
				childWindow->setWidth( clientWindow->getWidth()  );
				childWindow->setTop( m_lastBottomPos - childWindow->getHeight()   );

				m_lastBottomPos -= childWindow->getHeight() ;

				break;
			}

			case client: 
			{
				childWindow->setLeft( m_lastLeftPos);
				childWindow->setTop ( m_lastTopPos );
				childWindow->setWidth( m_lastRightPos - m_lastLeftPos);
				childWindow->setHeight( m_lastBottomPos - m_lastTopPos);
				break;
			}

			case none:
			default: 
			{

				break;
			}
		}

		childWindow = childWindow->getNextNode();

	};
}

};
