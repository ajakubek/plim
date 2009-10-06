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

#include "treenodes.hpp"

namespace plimuni {

cTreeNode::cTreeNode(cTreeNodes* nodes, cTreeNode* node, const void* ptr)	:	m_ptr((void*) ptr),
																										m_treeNodes(nodes),
																										m_nodeNext(NULL),
																										m_nodePrev(NULL),
																										m_nodeParent(node),
																										m_nodeFirst(NULL),
																										m_nodeLast(NULL) 
{

	if (nodes) 
	{
		/* Add the node to the tree */
		if (!node) 
		{
			if (!nodes->getFirstNode()) 
			{
				nodes->setLastNode( this );
				nodes->setFirstNode( this );
			}
			else 
			{
				nodes->getLastNode()->setNextNode( this );
				setPrevNode( nodes->getLastNode() );
				nodes->setLastNode( this );
			}
		} 
		else 
		{
			/* Add the node to the node */
			if (!node->getFirstNode()) 
			{
				node->setLastNode( this );
				node->setFirstNode( this );
			} 
			else 
			{
				node->getLastNode()->setNextNode( this );
				setPrevNode( node->getLastNode() );
				node->setLastNode( this );
			}

		}
	
	}
}

cTreeNode::~cTreeNode(void) 
{
	if ( m_treeNodes ) {
		m_treeNodes->removeNode( this );
	}
}

void cTreeNode::setNextNode(cTreeNode* node) 
{
	m_nodeNext = node;
}

cTreeNode* cTreeNode::getNextNode(void) 
{
	return m_nodeNext;
}

void cTreeNode::setPrevNode(cTreeNode* node) 
{
	m_nodePrev = node;
}

cTreeNode* cTreeNode::getPrevNode(void) {
	return m_nodePrev;
}

void cTreeNode::setParentNode(cTreeNode* node) 
{
	m_nodeParent = node;
}

cTreeNode* cTreeNode::getParentNode(void) 
{
	return m_nodeParent;
}

void cTreeNode::setFirstNode(cTreeNode* node) 
{
	m_nodeFirst = node;
}

cTreeNode* cTreeNode::getFirstNode(void) 
{
	return m_nodeFirst;
}

void cTreeNode::setLastNode(cTreeNode* node) 
{
	m_nodeLast = node;
}

cTreeNode* cTreeNode::getLastNode(void) 
{
	return m_nodeLast; 
}

int cTreeNode::getLevelNode(void) 
{
	cTreeNode* node;
	int level = 1;

	node = getParentNode();

	while (node) {
		level++;
		node = node->getParentNode();
	}

	return level;
}

void cTreeNode::setNodeData(void* data) 
{
	m_ptr = data;
}

void* cTreeNode::getNodeData(void) 
{
	return m_ptr;
}

cTreeNodes* cTreeNode::getOwner(void) 
{
	return m_treeNodes;
}

};
