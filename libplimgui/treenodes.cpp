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

cTreeNodes::cTreeNodes(void)
:	m_nodeFirst(NULL),
	m_nodeLast(NULL) {
	
}

cTreeNodes::~cTreeNodes(void) {
	/* TODO: Clean up */
}

void cTreeNodes::SetFirstNode(cTreeNode* node) {
	m_nodeFirst = node;
}

cTreeNode* cTreeNodes::GetFirstNode(void) {
	return m_nodeFirst; 
}

void cTreeNodes::SetLastNode(cTreeNode* node) {
	m_nodeLast = node;
}

cTreeNode* cTreeNodes::GetLastNode(void) {
	return m_nodeLast;
}

cTreeNode* cTreeNodes::RemoveNode(cTreeNode* node, int del) {
	cTreeNode* rmnode;

	if ( node->GetParentNode() ) {
		rmnode = node->GetLastNode();

		while (rmnode) {
			rmnode = RemoveNode(rmnode, 1);
		}

		if ( node->GetParentNode()->GetFirstNode() == node )
			node->GetParentNode()->SetFirstNode( node->GetNextNode() );

		if ( node->GetParentNode()->GetLastNode() == node ) 
			node->GetParentNode()->SetLastNode( node->GetPrevNode() );

		
	}
	else {
		if ( GetFirstNode() == node )
			SetFirstNode( node->GetNextNode() );

		if ( GetLastNode() == node )
			SetLastNode( node->GetPrevNode() );
	}

	rmnode = node->GetPrevNode();

	if ( node->GetNextNode() )
		node->GetNextNode()->SetPrevNode( node->GetPrevNode() );

	if ( node->GetPrevNode() )
		node->GetPrevNode()->SetNextNode( node->GetNextNode() );
	
	if (del)	delete node;

	return rmnode;
}

cTreeNode* cTreeNodes::GetNext(cTreeNode* node) {
	cTreeNode* next;
	cTreeNode* temp;

	if (!node)
		return NULL;

	next = node->GetNextNode();

	if (node->GetFirstNode()) {
		return node->GetFirstNode();
	} 
	else {
		if ( node->GetParentNode() && !node->GetNextNode()) {
			temp = node->GetParentNode();
			do {
				if (temp->GetNextNode())
					return temp->GetNextNode();

				temp = temp->GetParentNode();
			} while (temp);
		}
	}

	return next;
}

cTreeNode* cTreeNodes::GetPrev(cTreeNode* node) {
	cTreeNode* prev;

	if (!node) return NULL;

	prev = node->GetPrevNode();

	if (!prev && node->GetParentNode()) {
		return node->GetParentNode();
	} 
	else {
		if (prev && prev->GetLastNode())
			return prev->GetLastNode();
	}

	return prev;
}

};
