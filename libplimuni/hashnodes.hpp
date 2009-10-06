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

#ifndef __PLIM_HASHNODES_H__
#define __PLIM_HASHNODES_H__

#include "stdafx.hpp"

#include "strings.hpp"
#include "treenodes.hpp"

namespace plimuni {

class cHashNode: 	public 	plimuni::cTreeNode 
{

public:
	cHashNode(plimuni::cTreeNodes* nodes, const char* identifier, int len, void* data)	:	plimuni::cTreeNode(nodes, NULL, NULL),
																														m_len(len),
																														m_data(data) 
	{
		m_identifier.copy(identifier);
	};
	
	virtual ~cHashNode(void) 
	{};
	
	const char* getIdentifier() 
	{ 
		return m_identifier.getBuffer(); 
	};
	
	const int getLen(void) 
	{ 
		return m_len; 
	};
	
	void* getPtr(void) 
	{ 
		return m_data; 
	};
	
private:
	plimuni::cString m_identifier;
	int m_len;
	void* m_data;
};

class cHashNodes
{

public:
	cHashNodes(int elements);
	virtual ~cHashNodes(void);
	
	int setNewElements(int elements) 
	{ 
		return resizeNodes(elements); 
	};
	
	virtual cHashNode* add(const char* identifier, const int len, void* data);
	virtual cHashNode* add(const char* identifier, void* data);
	cHashNode* get(const char* identifier, int len);
	int remove(cHashNode* node, int len);

protected:
	int resizeNodes(int elements);

private:
	int hGenerateIndex(const char* identifier, int len);

	plimuni::cTreeNodes** m_hashNodes;
	int m_elements;
};


};


#endif
