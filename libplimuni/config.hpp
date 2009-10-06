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

#ifndef __PLIM_CONFIG_H__
#define __PLIM_CONFIG_H__


#include "stdafx.hpp"
#include "lexer.hpp"
#include "treenodes.hpp"
#include "strings.hpp"

/* CAUTION!!. This simple config lexer doesnt have error handling in the syntax yet. */

namespace plimuni 
{

#define USE_CONFIG_DEBUG
#undef USE_CONFIG_DEBUG
#ifdef USE_CONFIG_DEBUG
	#define CONFIG_DBG(x, y) printf("Found (%s) with content (%s)\n", x, y)
#else
	#define CONFIG_DBG(x, y)
#endif

typedef enum { section, variable } ConfigNodeType;

class cPlimConfigNode	:	public	plimuni::cTreeNode 
{

public:
	cPlimConfigNode(cTreeNodes* nodes, cTreeNode* node, cString* section, ConfigNodeType type)	:	plimuni::cTreeNode(nodes, node, NULL), 
																																	m_type(type) 
	{ 
		m_buffer = new cString(section); 
		m_var = new cString(); 
	};
	
	virtual ~cPlimConfigNode(void) 
	{ 
		if (m_buffer) 
		{
			delete m_buffer; 
		}
		
		if (m_var) 
		{
			delete m_var; 
		}
	};
	
	const char* getBuffer(void) 
	{ 
		return m_buffer->getBuffer(); 
	};
	
	void setBuffer(const char* buffer) 
	{ 
		m_buffer->copy( buffer ); 
	};
	
	const char* getVar(void) 
	{ 
		return m_var->getBuffer(); 
	};
	
	void setVar(const char* buffer) 
	{ 
		m_var->copy(buffer); 
	};
	
	int compareVar(const char* buffer) 
	{ 
		if (m_var) 
			return strcmp(m_var->getBuffer(), buffer); 
		else 
			return -1; 
	}
	
	ConfigNodeType getType(void) 
	{ 
		return m_type; 
	};
	
	cPlimConfigNode* getNextNode(void) 
	{ 
		return (cPlimConfigNode*) plimuni::cTreeNode::getNextNode(); 
	};
	
	cPlimConfigNode* getPrevNode(void) 
	{ 
		return (cPlimConfigNode*) plimuni::cTreeNode::getPrevNode(); 
	};
	
	cPlimConfigNode* getFirstNode(void) 
	{ 
		return (cPlimConfigNode*) plimuni::cTreeNode::getFirstNode(); 
	};
	
	cPlimConfigNode* getLastNode(void) 
	{ 
		return (cPlimConfigNode*) plimuni::cTreeNode::getLastNode(); 
	};
	
	cPlimConfigNode* getParentNode(void) 
	{ 
		return (cPlimConfigNode*) plimuni::cTreeNode::getParentNode(); 
	};
	
private:
	plimuni::cString* m_buffer;
	plimuni::cString* m_var;
	ConfigNodeType m_type;
};

class cPlimConfig 
{

public:
	cPlimConfig(void);
	virtual ~cPlimConfig(void);
	
	int loadConfig(const char* filename);
	int saveConfig(const char* filename);
	
	cPlimConfigNode* getConfigNodeByPath(const char* path);
	const char* getConfigVarByPath(const char* path);
	
	cPlimConfigNode* getConfigNode(const char* id);
	cPlimConfigNode* getConfigNode(cPlimConfigNode* node, const char* id);
	
protected:
	virtual int analyse(const char* buffer);
	plimuni::cPlimToken* analyseSection(plimuni::cPlimToken* tokenx, cPlimConfigNode* node);
	plimuni::cPlimToken* analyseVariables(plimuni::cPlimToken* token, cPlimConfigNode* node);
	plimuni::cPlimToken* analyseComment(plimuni::cPlimToken* tokenx);
	plimuni::cPlimToken* skipBlanks(plimuni::cPlimToken* token);
	
private:
	plimuni::cPlimLexer* m_lexer;
	plimuni::cTreeNodes* m_configNodes;

};

};

#endif
