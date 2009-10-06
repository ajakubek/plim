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

#include "cReactor.hpp"
#include "cReactorPlugin.hpp"

namespace plimreactor 
{

cReactor::cReactor(void)	:	plimuni::cTreeNodes() 
{}

cReactor::~cReactor(void) 
{}

bool cReactor::load(const char* dynObjectName)
{
	return false;
};

bool cReactor::unload(const char* dynObjectName)
{
	return false;
};

bool cReactor::run(void)
{
	plimreactor::cReactorPlugin* plugin;

	/* Process plugins. */
	plugin = (plimreactor::cReactorPlugin*) getFirstNode();

	while ( plugin ) 
	{
		
		plugin = (plimreactor::cReactorPlugin*) getNext(plugin);
	}
	
	return true;
};

bool cReactor::status(void)
{
	return false;
};

int cReactor::nuclearChainReaction(void) 
{
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
		FD_SET(0, &rfds);
				
		/* Select */
		selr = select(1, &rfds, NULL, NULL, &val);

		if ( FD_ISSET(0, &rfds) ) {
			ret = -1;
		}

	}

	return ret;
}

};
