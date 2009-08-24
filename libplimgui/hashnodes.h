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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"
#include "treenodes.h"

using namespace NSString;

namespace NSTree {

class cHashNode: public cTreeNode {
public:
	cHashNode(cTreeNodes* nodes, const char* identifier, int len, void* data)
	:	cTreeNode(nodes, NULL, NULL),
		m_len(len),
		m_data(data) {
		m_identifier.Copy(identifier);
	};
	virtual ~cHashNode(void) {};
	const char* GetIdentifier() { return m_identifier.GetBuffer(); };
	const int GetLen() { return m_len; };
	void* GetPtr(void) { return m_data; };
private:
	cString m_identifier;
	int m_len;
	void* m_data;
};
 
class cHashNodes
{
public:
	cHashNodes(int cHashElements);
	virtual ~cHashNodes(void);
	int SetNewElements(int elements) { return ResizeNodes(elements); };
	virtual cHashNode* Add(const char* identifier, const int len, void* data);
	virtual cHashNode* Add(const char* identifier, void* data);
	cHashNode* Get(const char* identifier, int len);
	int Delete(cHashNode* node, int len);
protected:
	int ResizeNodes(int elements);
private:
	int HGenerateIndex(const char* identifier, int len);
	cTreeNodes** m_hashNodes;
	int m_elements;
};


};


#endif
