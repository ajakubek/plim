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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <libplimgui/lexer.h>
#include <libplimgui/treenodes.h>
#include <libplimgui/strings.h>

/* CAUTION!!. This simple config lexer doesnt have error handling in the syntax yet. */

namespace NSConfig {

using namespace NSTree;
using namespace NSString;

#define USE_CONFIG_DEBUG
#undef USE_CONFIG_DEBUG
#ifdef USE_CONFIG_DEBUG
	#define CONFIG_DBG(x, y) printf("Found (%s) with content (%s)\n", x, y)
#else
	#define CONFIG_DBG(x, y)
#endif

typedef enum { section, variable } ConfigNodeType;

class cPlimConfigNode: public cTreeNode {
public:
	cPlimConfigNode(cTreeNodes* nodes, cTreeNode* node, cString* section, ConfigNodeType type)
	:	cTreeNode(nodes, node, NULL), m_type(type) { m_buffer = new cString(section); m_var = new cString(); };
	virtual ~cPlimConfigNode(void) { if (m_buffer) delete m_buffer; if (m_var) delete m_var; };
	const char* GetBuffer(void) { return m_buffer->GetBuffer(); };
	void SetBuffer(const char* buffer) { m_buffer->Copy( buffer ); };
	const char* GetVar(void) { return m_var->GetBuffer(); };
	void SetVar(const char* buffer) { m_var->Copy(buffer); };
	ConfigNodeType GetType(void) { return m_type; };
	cPlimConfigNode* GetNextNode(void) { return (cPlimConfigNode*) cTreeNode::GetNextNode(); };
	cPlimConfigNode* GetPrevNode(void) { return (cPlimConfigNode*) cTreeNode::GetPrevNode(); };
	cPlimConfigNode* GetFirstNode(void) { return (cPlimConfigNode*) cTreeNode::GetFirstNode(); };
	cPlimConfigNode* GetLastNode(void) { return (cPlimConfigNode*) cTreeNode::GetLastNode(); };
	cPlimConfigNode* GetParentNode(void) { return (cPlimConfigNode*) cTreeNode::GetParentNode(); };
private:
	cString* m_buffer;
	cString* m_var;
	ConfigNodeType m_type;
};

class cPlimConfig {
public:
	cPlimConfig(void);
	virtual ~cPlimConfig(void);
	int LoadConfig(const char* filename);
	int SaveConfig(const char* filename);
	cPlimConfigNode* GetConfigNodeByPath(const char* path);
	cPlimConfigNode* GetConfigNode(const char* id);
	cPlimConfigNode* GetConfigNode(cPlimConfigNode* node, const char* id);
protected:
	virtual int Analyse(const char* buffer);
	cPlimToken* AnalyseSection(cPlimToken* tokenx, cPlimConfigNode* node);
	cPlimToken* AnalyseVariables(cPlimToken* token, cPlimConfigNode* node);
	cPlimToken* AnalyseComment(cPlimToken* tokenx);
	cPlimToken* SkipBlanks(cPlimToken* token);
private:
	cPlimLexer* m_lexer;
	cTreeNodes* m_configNodes;
};

};
