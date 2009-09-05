/***************************************************************************
 *   Copyright (C) 2009 by Lukasz Marcin Borzecki   *
 *   lukasz.borzecki@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "cReactor.h"
#include "cPlim.h"

namespace NSReactor {

cReactor::cReactor(cApplication* plimapp)
:	cTreeNodes() {
	NSPlim::cPlim* plim = (NSPlim::cPlim*) plimapp;
	
	if (plimapp) {
		m_app = plimapp;
		m_config = plim->GetSharedConfig();
	}
	else {
		m_app = NULL;
		m_config = NULL;
	}
}

cReactor::~cReactor(void) {
	
}

int cReactor::NuclearChainReaction(void) {
	struct timeval val;
	int fmaxd, selr, ret;
	fd_set rfds, wfds, efds;

	fmaxd = 0;
	ret = 0;
	val.tv_sec = 1;
	val.tv_usec = 0;
	
	{ /* Descriptors stuff.
		/* Clear all descriptors */
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);
		
		/* Add the stdin */
		FD_SET(0, &rfds);
		/* Add plugins descriptors if they have such. */
		
		
		/* Select */
		selr = select(fmaxd + 1, &rfds, &wfds, &efds, &val);

		if ( FD_ISSET(0, &rfds) ) {
			ret = -1;
		}

		/* Process plugins. */
	}

	return ret;
}

cPlimConfig* cReactor::GetConfig(void) {
	return m_config;
}

};
