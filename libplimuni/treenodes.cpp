/***************************************************************************
 *   Copyright (C) 2009 by Lukasz Marcin Borzecki   *
 *   lukasz.borzecki@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software  Foundation; either version 2 of the    *
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

cTreeNodes::cTreeNodes(void)	:	m_nodeFirst(NULL),
											m_nodeLast(NULL) 
{}

cTreeNodes::~cTreeNodes(void) 
{}

void cTreeNodes::setFirstNode(cTreeNode* node) 
{
	m_nodeFirst = node;
}

cTreeNode* cTreeNodes::getFirstNode(void) 
{
	return m_nodeFirst; 
}

void cTreeNodes::setLastNode(cTreeNode* node) 
{
	m_nodeLast = node;
}

cTreeNode* cTreeNodes::getLastNode(void) 
{
	return m_nodeLast;
}

cTreeNode* cTreeNodes::addNode(cTreeNode* node, cTreeNode* parent) 
{
	if (!parent) 
	{
		if (!getFirstNode()) 
		{
			setLastNode( node );
			setFirstNode( node );
		}
		else 
		{
			getLastNode()->setNextNode( node );
			node->setPrevNode( getLastNode() );
			setLastNode( node );
		}
	} 
	else 
	{
		/* Add the node to the node */
		if (!parent->getFirstNode()) 
		{
			parent->setLastNode( node );
			parent->setFirstNode( node );
		} 
		else 
		{
			parent->getLastNode()->setNextNode( node );
			node->setPrevNode( parent->getLastNode() );
			parent->setLastNode( node );
		}
	}

	return node;
}

cTreeNode* cTreeNodes::removeNode(cTreeNode* node, int del) 
{
	cTreeNode* rmnode;

	if (!node) 
		return NULL;

	if ( node->getParentNode() ) 
	{
		rmnode = node->getLastNode();

		while (rmnode) 
		{
			rmnode = removeNode(rmnode, del);
		}

		if ( node->getParentNode()->getFirstNode() == node )
		{
			node->getParentNode()->setFirstNode( node->getNextNode() );
		}
		
		if ( node->getParentNode()->getLastNode() == node ) 
		{
			node->getParentNode()->setLastNode( node->getPrevNode() );
		}
		
	}
	else 
	{
		if ( getFirstNode() == node ) 
		{
			setFirstNode( node->getNextNode() );
		}
		
		if ( getLastNode() == node )
		{
			setLastNode( node->getPrevNode() );
		}
		
	}

	rmnode = node->getPrevNode();

	if ( node->getNextNode() )
	{
		node->getNextNode()->setPrevNode( node->getPrevNode() );
	}
	
	if ( node->getPrevNode() )
	{
		node->getPrevNode()->setNextNode( node->getNextNode() );
	}
	
	if (del)
	{
		delete node;
	}
	
	return rmnode;
}

cTreeNode* cTreeNodes::getNext(cTreeNode* node) 
{
	cTreeNode* next;
	cTreeNode* temp;

	if (!node)
		return NULL;

	next = node->getNextNode();

	if (node->getFirstNode()) 
	{
		return node->getFirstNode();
	} 
	else 
	{
		if ( node->getParentNode() && !node->getNextNode()) 
		{
			temp = node->getParentNode();
			
			do 
			{
				if (temp->getNextNode())
					return temp->getNextNode();

				temp = temp->getParentNode();
			} 
			while (temp);
			
		}
	}

	return next;
}

cTreeNode* cTreeNodes::getPrev(cTreeNode* node) 
{
	cTreeNode* prev;

	if (!node) 
		return NULL;

	prev = node->getPrevNode();

	if (!prev && node->getParentNode()) 
	{
		return node->getParentNode();
	} 
	else 
	{
		if (prev && prev->getLastNode())
			return prev->getLastNode();
	}

	return prev;
}

};
