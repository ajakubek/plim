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

#include "stdafx.h"

namespace NSTree {

class cTreeNodes;

class cTreeNode {
public:
	cTreeNode(cTreeNodes* nodes, cTreeNode* node, const void* ptr);
	virtual ~cTreeNode(void);
	/* TODO: Add future ptr check */
	void SetNextNode(cTreeNode* node);
	virtual cTreeNode* GetNextNode(void);
	void SetPrevNode(cTreeNode* node);
	virtual cTreeNode* GetPrevNode(void);
	void SetParentNode(cTreeNode* node);
	virtual cTreeNode* GetParentNode(void);
	void SetFirstNode(cTreeNode* node);
	virtual cTreeNode* GetFirstNode(void);
	void SetLastNode(cTreeNode* node);
	virtual cTreeNode* GetLastNode(void);
	int GetLevelNode(void);
	void SetNodeData(void* data);
	void* GetNodeData(void);
	cTreeNodes* GetOwner(void);
private:
	void* m_ptr;
	cTreeNodes* m_treeNodes;
	cTreeNode* m_nodeNext;
	cTreeNode* m_nodePrev;
	cTreeNode* m_nodeParent;
	cTreeNode* m_nodeFirst;
	cTreeNode* m_nodeLast;
};

class cTreeNodes {
public:
	cTreeNodes(void);
	virtual ~cTreeNodes(void);
	void SetFirstNode(cTreeNode* node);
	virtual cTreeNode* GetFirstNode(void);
	void SetLastNode(cTreeNode* node);
	virtual cTreeNode* GetLastNode(void);
	cTreeNode* AddNode(cTreeNode* node, cTreeNode* parent);
	cTreeNode* RemoveNode(cTreeNode* node, int del = 0);
	virtual cTreeNode* GetNext(cTreeNode* node);
	virtual cTreeNode* GetPrev(cTreeNode* node);
protected:
	
private:
	cTreeNode* m_nodeFirst;
	cTreeNode* m_nodeLast;
};
 
};

#endif
