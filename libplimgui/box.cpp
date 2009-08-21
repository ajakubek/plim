#include "box.h"

namespace NSWindows {

cBox::cBox(cCursesWindow *window)
:	m_windowInstance(window) {
	
}

cBox::~cBox(void) {
	if (m_windowInstance) {
		/* dummy */
	}
}

void cBox::Update(void) {
	cCursesWindow* clientWindow = m_windowInstance;
	cCursesWindow* childWindow;

	if (!clientWindow)
		return;

	m_lastLeftPos = 0;
	m_lastTopPos = 0;
	m_lastRightPos = clientWindow->GetWidth() - (clientWindow->IsChild() ? clientWindow->GetRawLeft() : 0);
	m_lastBottomPos = clientWindow->GetHeight() - (clientWindow->IsChild() ? clientWindow->GetRawTop() : 0);

	childWindow = (cCursesWindow*) clientWindow->GetFirstWindow();

	while (childWindow) {
		/* The cableguy tells me that we need no anchors in ncurses */
		/* Tell us to update the window */
		childWindow->NeedUpdate();

		if (childWindow) {

			/* IMHO, no test at this commit, so it can be buggy */
			switch (childWindow->GetWindowAlign()) {
				case left: {
					childWindow->SetLeft( m_lastLeftPos );
					childWindow->SetTop( m_lastTopPos );
					childWindow->SetHeight( m_lastBottomPos - clientWindow->GetRawTop() );
					
					m_lastLeftPos += childWindow->GetWidth()  ;

					break;
				}

				case right: {

					childWindow->SetLeft ( clientWindow->GetWidth() - childWindow->GetWidth() );
					childWindow->SetTop( m_lastTopPos  );
					childWindow->SetHeight ( m_lastBottomPos - clientWindow->GetRawTop() );

					m_lastRightPos -= childWindow->GetWidth() ;

					break;
				}

				case top: {
					childWindow->SetLeft( 0 );
					childWindow->SetTop( m_lastTopPos + clientWindow->GetRawTop() );
					childWindow->SetWidth( clientWindow->GetWidth() );

					m_lastTopPos += childWindow->GetHeight();

					break;
				}

				case bottom: {
					childWindow->SetLeft( 0 );
					childWindow->SetWidth( clientWindow->GetWidth()  );
					childWindow->SetTop( m_lastBottomPos - childWindow->GetHeight()   );

					m_lastBottomPos -= childWindow->GetHeight();

					break;
				}

				case client: {
					childWindow->SetLeft( m_lastLeftPos );
					childWindow->SetTop ( m_lastTopPos );
					childWindow->SetWidth( m_lastRightPos - clientWindow->GetRawLeft() );
					childWindow->SetHeight( m_lastBottomPos - clientWindow->GetRawTop() );
					break;
				}

				case none:
				default: {

					break;
				}
			}

		}

		childWindow = childWindow->GetNextNode();

	};
}

};
