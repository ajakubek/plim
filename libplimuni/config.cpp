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

#include "config.hpp"

namespace plimuni
{

cPlimConfig::cPlimConfig(void) 
{
	m_configNodes = new plimuni::cTreeNodes();
}

cPlimConfig::~cPlimConfig(void) 
{
	if (m_configNodes)
	{
		delete m_configNodes;
	}
}

int cPlimConfig::loadConfig(const char* filename) 
{
	FILE* fd;
	char* filebuff;
	struct stat st;

	if (!filename)		return -1;

	fd = fopen( filename, "r" );

	if (fd) {
#if defined(_WIN32) || defined(_WIN64)
		return -1;
#else
		stat( filename, &st );
#endif
		filebuff = (char*) malloc( st.st_size );

		if (filebuff) 
		{
			if (!fread(filebuff, st.st_size, 1, fd )) 
			{
				free(filebuff);
				filebuff = NULL;
			}
			else 
			{
				analyse( filebuff );
				free( filebuff );
			}
		}

		fclose( fd );

/*		cPlimConfigNode* node;

		node = (cPlimConfigNode*) m_configNodes->GetFirstNode();

		while ( node ) {

			for ( int i = 0; i < node->GetLevelNode() * 2; i++)
				printf(" ");
			if ( node->GetType() == variable) 
				printf("Variable -> %s with content (%s)\n", node->GetBuffer(), node->GetVar());
			else
				printf("Section -> %s\n", node->GetBuffer());

			node = (cPlimConfigNode*) m_configNodes->GetNext(node);
		}

		node = GetConfigNodeByPath("plim.gui.window_textbuffer.");

		if (node) {
			printf("Node by path found (%s)\n", node->GetVar());
		} */

		return 0;
	}


	return -1;
}

int cPlimConfig::saveConfig(const char* filename) 
{
	if (!filename)
		return -1;

	return 0;
}

cPlimConfigNode* cPlimConfig::getConfigNodeByPath(const char* path) 
{
	cPlimConfigNode* node;
	plimuni::cPlimLexer lex;
	plimuni::cPlimToken* token;
	plimuni::cPlimToken* prev;
	
	lex.refresh( path );

	node = NULL;
	token = (plimuni::cPlimToken*) lex.getFirstNode();
	
	while ( token ) 
	{
		if ( token->getTokenCase() == PLIM_L_IDENTIFIER ) 
		{
			node = getConfigNode( node, token->getBuffer() );
			token = (plimuni::cPlimToken*) lex.getNext(token);
			
			if ( token && 
					token->getTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS &&
						token->getTokenExCase() == PLIM_L_SYMBOL_DOT) 
			{}
			else
				break;
		}

		token = (plimuni::cPlimToken*) lex.getNext(token);
	}

	return node;
}

const char* cPlimConfig::getConfigVarByPath(const char* path) {
	cPlimConfigNode* node = getConfigNodeByPath(path);
	
	if (node)	return node->getVar();
	
	return NULL;
}

cPlimConfigNode* cPlimConfig::getConfigNode(const char* id) {
	cPlimConfigNode* node;

	if (!id)
		return NULL;

	node = (cPlimConfigNode*) m_configNodes->getFirstNode();

	while ( node ) {

		if (!strcmp(node->getBuffer(), id)) {
			return node;
		}

		node = (cPlimConfigNode*) m_configNodes->getNext(node);//node->GetNextNode();
	}

	return NULL;
}

cPlimConfigNode* cPlimConfig::getConfigNode(cPlimConfigNode* node, const char* id) {
	cPlimConfigNode* n;

	if (!id)
		return NULL;

	if (!node) {
		return getConfigNode(id);
	}

	if (!node) return NULL;

	n = node->getFirstNode();

	while ( n ) {

		if (!strcmp(n->getBuffer(), id)) {
			return n;
		}

		n = n->getNextNode();
	}

	return NULL;
}

int cPlimConfig::analyse(const char* buffer) {
	cPlimToken* token;

	if (!buffer)	return -1;

	m_lexer = new cPlimLexer( buffer );

	token = m_lexer->getFirstNode();

	while ( token ) {
		if ( token->getTokenCase() == PLIM_L_IDENTIFIER ) {
			/* Start the processing of a section */
			CONFIG_DBG("Section opened", token->GetBuffer());

			token = analyseSection( token, NULL );
		}

		token = analyseComment( token );
	}


	if (m_lexer) {
		delete m_lexer;
	}
	return 1;
}

cPlimToken* cPlimConfig::analyseSection(cPlimToken* tokenx, cPlimConfigNode* node) {
	cPlimConfigNode* n, *parent;
	cPlimToken* token = tokenx;
	cPlimToken* t;
	int quit = 0;

	if (!tokenx) {
		return NULL;
	}

	parent = NULL;

	if ( node ) {
		parent = node->getParentNode();
	}

	if (!(n = getConfigNode( parent, token->getBuffer() )))
		n = new cPlimConfigNode( m_configNodes, (cTreeNode*) node, token, section );

	token = analyseComment(token); //(cPlimToken*) m_lexer->GetNext(token);

	while (token) {
		if (quit)	break;

		if ( token->getTokenCase() == PLIM_L_IDENTIFIER ) {
			/* Start the processing of a section */
			CONFIG_DBG("Section opened inside section", token->GetBuffer());

			t = analyseVariables( token, n );

			if ( !t )
				token = analyseSection( token, n );
			else
				token = t;

		}
		//else

		if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_BRACKETS ) {

			if ( token->getTokenExCase() == PLIM_L_SYMBOL_BRACKET_CURLY_OPEN ) {
				token = skipBlanks( token );

				if ( token ) {
					if ( token->getTokenCase() == PLIM_L_IDENTIFIER ) {

						t = analyseVariables( token, n );

						/* Check if there are some variables */
						if (t) {
							CONFIG_DBG("VARIABLE", t->GetBuffer());
							token = skipBlanks(analyseComment(t));

							if ( token ) {
								/* Process the rest of variables */
								while ( t ) {
									if (t) {
										t = analyseVariables( token, n );
										if ( t ) {
											token = skipBlanks(analyseComment( t ));
											continue;
										} else break;
									} else break;
								}
							}
							
							CONFIG_DBG("VARIABLE", token->GetBuffer());
						}



						if ( token ) {
							if (token->getTokenCase() == PLIM_L_IDENTIFIER) {
								CONFIG_DBG("Section opened inside section", token->GetBuffer());
								token = analyseSection( token, n );
							}
							else if (t)
								token =t;
						}
						//else
						//token = t;
					}
				}

			}


			if ( token->getTokenExCase() == PLIM_L_SYMBOL_BRACKET_CURLY_CLOSE ) {
				CONFIG_DBG("Closing bracket", "");
				quit = 1;
				
			}


		}
		//else

		if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS) {

			if ( token->getTokenExCase() == PLIM_L_SYMBOL_DOT) {
				/* Separate section here */

				CONFIG_DBG("Separation mark", token->GetBuffer());

				token = analyseComment(token); //(cPlimToken*) m_lexer->GetNext(token);
	
				if ( token ) {
					if ( token->getTokenCase() == PLIM_L_IDENTIFIER ) {
						/* Add a new section */
						CONFIG_DBG("Section added", token->GetBuffer());						
						return analyseSection( token, n );
					}

				}
				else {
					/* report error for a undefined section marker */
				}
			}

		}

		token = analyseComment(token);//(cPlimToken*) m_lexer->GetNext(token);
	}

	return token;

}

cPlimToken* cPlimConfig::analyseVariables(cPlimToken* token, cPlimConfigNode* node) {
	cPlimToken* save = token;
	cPlimConfigNode* var;
	cString buff;

	if (!token)	return NULL;

	token = skipBlanks( analyseComment(token) );

	while ( token ) {
		
		if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_MATH ) {

			if ( token->getTokenExCase() == PLIM_L_SYMBOL_MATH_ASSIGN ) {

				/* Ok we got a assign symbol, now we should search for a variable */
				while (token) {
					if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_LITERALS ) {
						if ( token->getTokenExCase() == PLIM_L_SYMBOL_LITERAL_DQUOTE ) {
							/* Ok, we are inside our variable. */
							token = (cPlimToken*) m_lexer->getNext(token);

							while ( token ) {
								/* Check for escaping chars */
								//CONFIG_DBG("Check escape started", token->GetBuffer());
								if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_MATH ) {
									CONFIG_DBG("Escape started", token->GetBuffer());
									if ( token->getTokenExCase() == PLIM_L_SYMBOL_MATH_SIGN_DIVIDE ) {
										/* Ok we got a escape char here, if the following token will be a dqoute than we make a escape seq. */
										token = (cPlimToken*) m_lexer->getNext(token);
										CONFIG_DBG("Escape started", token->GetBuffer());
										if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_LITERALS && token->getTokenExCase() == PLIM_L_SYMBOL_LITERAL_DQUOTE ) {
											/* Escape done here. */
											CONFIG_DBG("Escape ended", token->GetBuffer());
											buff.cat( token->getBuffer() );
											token = (cPlimToken*) m_lexer->getNext(token);

										} else {
											/* Nothing, just add the backslash to the var buffer. */
											CONFIG_DBG("Escape ended", token->GetBuffer());
										}

									}
								}

								if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_LITERALS )
									if ( token->getTokenExCase() == PLIM_L_SYMBOL_LITERAL_DQUOTE ) {
										/* Say goodbye and create a variable. */

										var = new cPlimConfigNode( m_configNodes, node, save, variable );
										var->setVar( buff.getBuffer() );
										
										CONFIG_DBG("Found variable", buff.GetBuffer());

										return (cPlimToken*) m_lexer->getNext(token);
									}

								buff.cat( token->getBuffer() );

								token = (cPlimToken*) m_lexer->getNext(token);
							}
							
						}

					}

					token = skipBlanks( analyseComment( token ) );
				}

				return token;
			} else
				break;
		} else 
			break;


		token = analyseComment(token);
	}

	

	return NULL;
}

cPlimToken* cPlimConfig::analyseComment(cPlimToken* tokenx) {
	cPlimToken* token = tokenx;

	if (!token) return NULL;
	
	if ( token->getTokenCase() == PLIM_L_CASE_FLAGS_MATH ) {

		if ( token->getTokenExCase() == PLIM_L_SYMBOL_MATH_HATCH ) {

			CONFIG_DBG( "Comment found", token->GetBuffer() );

			while ( token ) {
				if ( token->getTokenCase( ) == PLIM_L_FEED ) {
					CONFIG_DBG("Terminate comment", "" );
					return token;// (cPlimToken*) m_lexer->GetNext( token );
				}

				token = (cPlimToken*) m_lexer->getNext( token );
			}

		}

	}

	return (cPlimToken*) m_lexer->getNext(token);
}

cPlimToken* cPlimConfig::skipBlanks(cPlimToken* token) {
	cPlimToken* t;

	if (!token) return NULL;

	t = token;//(cPlimToken*) m_lexer->GetNext(token);

	while ( t ) {
		if (t->getTokenCase() != PLIM_L_BLANK) //&& t->GetTokenCase() != PLIM_L_FEED)
			return t;

		t = (cPlimToken*) m_lexer->getNext(t);
	}
	return t;
}

};
