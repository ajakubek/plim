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

#include "treenodes.h"

namespace NSTree {

cTreeNode::cTreeNode(cTreeNodes* nodes, cTreeNode* node, const void* ptr)
:	m_ptr((void*) ptr),
	m_treeNodes(nodes),
	m_nodeNext(NULL),
	m_nodePrev(NULL),
	m_nodeParent(node),
	m_nodeFirst(NULL),
	m_nodeLast(NULL) {

	/* Add the node to the tree */
	if (!node) {
		if (!nodes->GetFirstNode()) {
			nodes->SetLastNode( this );
			nodes->SetFirstNode( this );
		}
		else {
			nodes->GetLastNode()->SetNextNode( this );
			SetPrevNode( nodes->GetLastNode() );
			nodes->SetLastNode( this );
		}
	} 
	else {
		/* Add the node to the node */
		if (!node->GetFirstNode()) {
			node->SetLastNode( this );
			node->SetFirstNode( this );
		} else {
			node->GetLastNode()->SetNextNode( this );
			SetPrevNode( node->GetLastNode() );
			node->SetLastNode( this );
		}

	}

}

cTreeNode::~cTreeNode(void) {
	if ( m_treeNodes ) {
		m_treeNodes->RemoveNode( this );
	}
}

void cTreeNode::SetNextNode(cTreeNode* node) {
	m_nodeNext = node;
}

cTreeNode* cTreeNode::GetNextNode(void) {
	return m_nodeNext;
}

void cTreeNode::SetPrevNode(cTreeNode* node) {
	m_nodePrev = node;
}

cTreeNode* cTreeNode::GetPrevNode(void) {
	return m_nodePrev;
}

void cTreeNode::SetParentNode(cTreeNode* node) {
	m_nodeParent = node;
}

cTreeNode* cTreeNode::GetParentNode(void) {
	return m_nodeParent;
}

void cTreeNode::SetFirstNode(cTreeNode* node) {
	m_nodeFirst = node;
}

cTreeNode* cTreeNode::GetFirstNode(void) {
	return m_nodeFirst;
}

void cTreeNode::SetLastNode(cTreeNode* node) {
	m_nodeLast = node;
}

cTreeNode* cTreeNode::GetLastNode(void) {
	return m_nodeLast; 
}

int cTreeNode::GetLevelNode(void) {
	cTreeNode* node;
	int level = 1;

	node = GetParentNode();

	while (node) {
		level++;
		node = node->GetParentNode();
	}

	return level;
}

void cTreeNode::SetNodeData(void* data) {
	m_ptr = data;
}

void* cTreeNode::GetNodeData(void) {
	return m_ptr;
}

};
