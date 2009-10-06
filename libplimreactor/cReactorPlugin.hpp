/***************************************************************************
 *   Copyright (C) 2009 by Lukasz Marcin Borzecki                          *
 *   lukasz.borzecki@gmail.com                                             *
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

#ifndef __REACTOR_PLUGIN_H__
#define __REACTOR_PLUGIN_H__

#include <libplimuni/stdafx.hpp>
#include <libplimuni/treenodes.hpp>
#include <libplimuni/strings.hpp>
#include <libplimuni/abstract.hpp>

#include "stdafx.hpp"
#include "cReactor.hpp"

namespace plimreactor
{

class IReactorPlugin
{

public:
	virtual bool initialize(void) = 0;
	virtual bool finalize(void) = 0;
	virtual const char* name(void) = 0;
	virtual const int version(void) = 0;
	
};

/*
	Note. You must pass all the information to the reactor in a abstract classes defined in plimuni::cAbstract*.
*/

class cReactorPlugin	:	public	plimreactor::IReactorPlugin,
								public	plimuni::cTreeNode, 
								public	plimuni::cTreeNodes 
{

public:
	cReactorPlugin(plimreactor::cReactor* reactor);
	virtual ~cReactorPlugin(void);

	/* Bind signals to the reactor here. */
	virtual bool initialize(void)
	{
		return true;
	};
	
	/* Unbind and deintialize data here */
	virtual bool finalize(void)
	{
		return true;
	};
	
	const char* name(void)
	{
		return "Default template plugin";
	};
	
	const int version(void)
	{
		return 0x0101;
	};
	
protected:
	plimreactor::cReactor* nuclearReactor(void) 
	{ 
		return m_reactor;
	};
	
private:
	plimreactor::cReactor* m_reactor;

};

};

#endif
