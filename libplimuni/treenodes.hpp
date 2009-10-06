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

#ifndef __PLIM_TREENODES_H__
#define __PLIM_TREENODES_H__

#include "stdafx.hpp"

namespace plimuni {

class cTreeNodes;

class cTreeNode 
{

public:
	cTreeNode(cTreeNodes* nodes, cTreeNode* node, const void* ptr);
	virtual ~cTreeNode(void);
	
	void setNextNode(cTreeNode* node);
	virtual cTreeNode* getNextNode(void);
	
	void setPrevNode(cTreeNode* node);
	virtual cTreeNode* getPrevNode(void);
	
	void setParentNode(cTreeNode* node);
	virtual cTreeNode* getParentNode(void);
	
	void setFirstNode(cTreeNode* node);
	virtual cTreeNode* getFirstNode(void);
	
	void setLastNode(cTreeNode* node);
	virtual cTreeNode* getLastNode(void);
	
	int getLevelNode(void);
	
	void setNodeData(void* data);
	void* getNodeData(void);
	
	cTreeNodes* getOwner(void);
private:
	void* m_ptr;
	cTreeNodes* m_treeNodes;
	cTreeNode* m_nodeNext;
	cTreeNode* m_nodePrev;
	cTreeNode* m_nodeParent;
	cTreeNode* m_nodeFirst;
	cTreeNode* m_nodeLast;
};

class cTreeNodes 
{

public:
	cTreeNodes(void);
	virtual ~cTreeNodes(void);
	
	void setFirstNode(cTreeNode* node);
	virtual cTreeNode* getFirstNode(void);
	
	void setLastNode(cTreeNode* node);
	virtual cTreeNode* getLastNode(void);
	
	cTreeNode* addNode(cTreeNode* node, cTreeNode* parent);
	cTreeNode* removeNode(cTreeNode* node, int del = 0);
	
	virtual cTreeNode* getNext(cTreeNode* node);
	virtual cTreeNode* getPrev(cTreeNode* node);

private:
	cTreeNode* m_nodeFirst;
	cTreeNode* m_nodeLast;
};
 
};

#endif
