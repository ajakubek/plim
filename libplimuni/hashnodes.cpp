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

#include "hashnodes.hpp"

namespace plimuni {



cHashNodes::cHashNodes(int elements)	:	m_elements(elements),
														m_hashNodes(NULL)
{
	if (m_elements > 0)
	{
		resizeNodes(m_elements);
	}
}

cHashNodes::~cHashNodes(void)
{
	resizeNodes( 0 );
}
 
cHashNode* cHashNodes::add(const char* identifier, const int len, void* data)
{
	cHashNode* node;
	int index;

	if (!identifier || !len)	return NULL;

	index = hGenerateIndex( identifier, len );

	if (index) {
		if (!m_hashNodes[index] )
			m_hashNodes[index] = new cTreeNodes();

		node = new cHashNode( m_hashNodes[index], identifier, len, data );
	}
	
	return node;
}

cHashNode* cHashNodes::add(const char* identifier, void* data)
{
	return add(identifier, strlen(identifier), data);
}

cHashNode* cHashNodes::get(const char* identifier, int len)
{
	int index;
	cHashNode* node;

	if (!identifier || !len)	return NULL;
	
	index = hGenerateIndex(identifier, len);

	if ( index < 0 )	return NULL;

	if (m_hashNodes)
	{
	
		if (m_hashNodes[index])
		{
			node = (cHashNode*) m_hashNodes[index]->getFirstNode();
			
			while (node)
			{
				if (!strncmp(node->getIdentifier(), identifier, len))
				{
					return node;
				}
				
				node = (cHashNode*) m_hashNodes[index]->getNext(node);
			}
		
		}
		
	}

	return NULL;
}

int cHashNodes::remove(cHashNode* node, const int len)
{
	int index;

	if (!node)
		return 0;

	index = hGenerateIndex( (const char*) node->getIdentifier(), len);

	if (m_hashNodes)
	{

		if (m_hashNodes[index])
		{
			m_hashNodes[index]->removeNode(node, 1);

			if ( !m_hashNodes[index]->getLastNode() )
				m_hashNodes[index] = NULL;

		}
	}

	return 1;
}

int cHashNodes::resizeNodes(int elements)
{
	bool fullClear = false;
	
	if (!elements && m_hashNodes)
	{
		/* Freeup all elements and chains 
		 */
		return FALSE;
	}
	else
	{
		if (elements >= m_elements)
		{
			elements = elements * 3 / 2;
			
			if ( !m_hashNodes )
			{
				fullClear = true;
			}
			
			m_hashNodes = (cTreeNodes**) realloc(m_hashNodes, sizeof(cTreeNodes*) * elements);
			
			if (m_hashNodes)
			{
				if (fullClear)
					memset( m_hashNodes, 0, sizeof(cTreeNodes*) * elements);
				else
					memset( m_hashNodes + m_elements, 0, sizeof(cTreeNodes*) * (elements - m_elements));
					
				m_elements = elements;
				
				return 1;
			}
			else
				return 0;
		}
	}
	
	return 0;
}

/* Private
 */

/* Could be changed */
int cHashNodes::hGenerateIndex(const char* identifier, int len)
{
	unsigned int pos = 0, i = 1;
	int p1, p2;
	int index;
	
	if (!identifier)	return 0;

	if ( len == 1 ) return (int) identifier[0];

	for (;i < len; i++)
	{
		p1 = (int) identifier[i];
		if (i == 1) p2 = (int) identifier[i];
		else			  p2 = (int) identifier[i -1];
		pos += (int) p1;
		pos += (int) ~((p1 % 3) ? p1 << i : p1 ^ ~i);
		pos *= (int) (p1 ^ i) + (len | (i + len - p2));
	}
	
	if (pos < 0)	pos = -pos;

	index =  pos % m_elements;
	
	if (index >= m_elements)
		index = m_elements;

	return index;
}

};
