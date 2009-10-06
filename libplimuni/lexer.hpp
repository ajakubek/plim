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

#ifndef __PLIM_LEXER_H__
#define __PLIM_LEXER_H__

#include "stdafx.hpp"

#include "treenodes.hpp"
#include "strings.hpp"

namespace plimuni {

#define PLIM_OK	1
#define PLIM_ERR	0

#define PLIM_L_UNRECOGNIZED							0x00000000
#define PLIM_L_IDENTIFIER								0x00000001
#define PLIM_L_SYMBOL									0x00000002
#define PLIM_L_BLANK										0x00000004
#define PLIM_L_DIGIT										0x00000008
#define PLIM_L_ESCAPE									0x00000010
#define PLIM_L_FEED										0x00000020

/* fFlags for digits */
#define PLIM_L_DIGIT_FLOAT								0x10000000
#define PLIM_L_DIGIT_INTEGER							0x20000000
#define PLIM_L_DIGIT_HEXADECIMAL						0x40000000
#define PLIM_L_DIGIT_OCTAL								0x80000000

#define PLIM_L_DIGIT_INTEGER_L_SUFFIX				0x00000020
#define PLIM_L_DIGIT_INTEGER_U_SUFFIX				0x00000040

#define PLIM_L_DIGIT_FLOAT_L_SUFFIX					0x00000080
#define PLIM_L_DIGIT_FLOAT_F_SUFFIX					0x00000100
#define PLIM_L_DIGIT_FLOAT_EXPONENT					0x00000200
#define PLIM_L_DIGIT_FLOAT_EXPONENT_PLUS			0x00000400
#define PLIM_L_DIGIT_FLOAT_EXPONENT_MINUS			0x00000800

#define PLIM_L_DIGIT_HEX_L_SUFFIX					0x00001000
#define PLIM_L_DIGIT_HEX_U_SUFFIX					0x00002000

#define PLIM_L_DIGIT_OCTAL_L_SUFFIX					0x00004000
#define PLIM_L_DIGIT_OCTAL_U_SUFFIX					0x00008000

/*	fIllegal flags */
#define PLIM_L_DIGIT_INTEGER_INVALID_SUFFIX		0x00000001
#define PLIM_L_DIGIT_INTEGER_INVALID_U_SUFFIX	0x00000002
#define PLIM_L_DIGIT_INTEGER_INVALID_L_SUFFIX	0x00000004
#define PLIM_L_DIGIT_INTEGER_INVALID_LU_SUFFIX	0x00000008

#define PLIM_L_DIGIT_FLOAT_INVALID_SUFFIX			0x00000010
#define PLIM_L_DIGIT_FLOAT_INVALID_L_SUFFIX		0x00000020
#define PLIM_L_DIGIT_FLOAT_INVALID_F_SUFFIX		0x00000040
#define PLIM_L_DIGIT_FLOAT_EXPONENT_NODIGITS		0x00000080
#define PLIM_L_DIGIT_FLOAT_TOO_MANY_DOTS			0x00000100

#define PLIM_L_DIGIT_HEX_INVALID_SUFFIX			0x00000200
#define PLIM_L_DIGIT_OCTAL_INVALID_SUFFIX			0x00000400
/* End of flags of digits */

/* fFlags for symbols */
#define PLIM_L_CASE_FLAGS_SYMBOLS					0x10000001

#define PLIM_L_SYMBOL_COLON							0x00100001
#define PLIM_L_SYMBOL_SEMICOLON						0x00100002
#define PLIM_L_SYMBOL_COMMA							0x00100004
#define PLIM_L_SYMBOL_DOT								0x00100008
#define PLIM_L_SYMBOL_BACKSLASH						0x00100010

/* fBrackets */
#define PLIM_L_CASE_FLAGS_BRACKETS					0x20000001

#define PLIM_L_SYMBOL_BRACKET_PARENTHESES_OPEN	0x30000001
#define PLIM_L_SYMBOL_BRACKET_PARENTHESES_CLOSE	0x30000002
#define PLIM_L_SYMBOL_BRACKET_SQUARE_OPEN			0x30000004
#define PLIM_L_SYMBOL_BRACKET_SQUARE_CLOSE		0x30000008
#define PLIM_L_SYMBOL_BRACKET_CURLY_OPEN			0x30000010
#define PLIM_L_SYMBOL_BRACKET_CURLY_CLOSE			0x30000020
#define PLIM_L_SYMBOL_BRACKET_ANGLE_OPEN			0x30000040
#define PLIM_L_SYMBOL_BRACKET_ANGLE_CLOSE			0x30000080
 
/* fMath syms */
#define PLIM_L_CASE_FLAGS_MATH						0x30000001

#define PLIM_L_SYMBOL_MATH_SIGN_PLUS				0x00000001
#define PLIM_L_SYMBOL_MATH_SIGN_MINUS				0x00000002
#define PLIM_L_SYMBOL_MATH_ASSIGN					0x00000004
#define PLIM_L_SYMBOL_MATH_SIGN_MULTIPLE			0x00000008
#define PLIM_L_SYMBOL_MATH_SIGN_DIVIDE				0x00000010
#define PLIM_L_SYMBOL_MATH_HATCH						0x00000020
#define PLIM_L_SYMBOL_MATH_PERCENT					0x00000040
#define PLIM_L_SYMBOL_MATH_LESS_THAN				PLIM_L_SYMBOL_BRACKET_ANGLE_OPEN
#define PLIM_L_SYMBOL_MATH_GREATER_THAN			PLIM_L_SYMBOL_BRACKET_ANGLE_CLOSE /* 40 */

/* fLiterals syms */
#define PLIM_L_CASE_FLAGS_LITERALS					0x40000001

#define PLIM_L_SYMBOL_LITERAL_QUOTE					0x00000001
#define PLIM_L_SYMBOL_LITERAL_DQUOTE				0x00000002

/* Ncurses syms */
#define PLIM_L_CASE_FLAGS_CURSES						0x10000008

#define PLIM_L_ATTR_BOLD								0x02
#define PLIM_L_ATTR_COLOR								0x03
#define PLIM_L_ATTR_RESET								0x0F
#define PLIM_L_ATTR_FIXED								0x11
#define PLIM_L_ATTR_REVERSE							0x12
#define PLIM_L_ATTR_REVERSE2							0x16
#define PLIM_L_ATTR_ITALIC								0x1D
#define PLIM_L_ATTR_UNDERLINE							0x1F
#define PLIM_L_ATTR_UNDERLINE2						0x15

/* End flags for symbols */

/* Just a temporary struct for PerformParse
*/
typedef struct _WeWantNewSouthParkEpisodesDamnYou {
	int a;
	int b;
	int c;
} TokenFlags;

class cPlimLexer;

/* TODO: Add some more advanced symbol parsing */
class cPlimToken	:	public plimuni::cTreeNode, 
							public plimuni::cString 
{

public:
	cPlimToken(cPlimLexer* nodes)	:	plimuni::cTreeNode((plimuni::cTreeNodes*) nodes, NULL, NULL), 
												plimuni::cString(),
												m_tokenCase(0), 
												m_tokenExCase(0), 
												m_illegalFlags(0) 
	{};
	
	virtual ~cPlimToken(void) 
	{};
	
	void setTokenCase(int flags) 
	{ 
		m_tokenCase = flags; 
	};
	
	int getTokenCase(void) 
	{ 
		return m_tokenCase; 
	};
	
	void setTokenExCase(int flags) 
	{ 
		m_tokenExCase = flags; 
	};
	
	int getTokenExCase(void) 
	{ 
		return m_tokenExCase; 
	};
	
	void setOffset(int offset) 
	{ 
		m_offset = offset; 
	};
	
	int getOffset(void) 
	{ 
		return m_offset; 
	};
	
	void setLen(int len) 
	{ 
		m_len = len; 
	};
	
	int getLen(void) 
	{ 
		return m_len; 
	};
	
	void setIllegalFlags(int flags) 
	{ 
		m_illegalFlags = flags; 
	};
	
	int getIllegalFlags(void) 
	{ 
		return m_illegalFlags; 
	};
	
	cPlimToken* getNextNode(void) 
	{ 
		return (cPlimToken*) plimuni::cTreeNode::getNextNode(); 
	};
	
	cPlimToken* getPrevNode(void) 
	{ 
		return (cPlimToken*) plimuni::cTreeNode::getPrevNode();
	};
	
private:
	int m_tokenCase;
	int m_tokenExCase;
	int m_illegalFlags;
	int m_offset;
	int m_len;
};

class cPlimLexer:	public	plimuni::cTreeNodes 
{

public:
	cPlimLexer(void);
	cPlimLexer(const char* string);
	virtual ~cPlimLexer(void);
	
	virtual void refresh(const char* string);
	
	cPlimToken* getFirstNode(void) 
	{ 
		return (cPlimToken*) plimuni::cTreeNodes::getFirstNode(); 
	};
	
	cPlimToken* getLastNode(void) 
	{ 
		return (cPlimToken*) plimuni::cTreeNodes::getLastNode(); 
	};
	
protected:
	virtual void performParse(void);
	
	int scriptType(char c);
	void scriptShiftPos(int* newPos, int num = 1);
	int scriptBlankParse(const char* src, int len, int* newPos, TokenFlags* flags);
	int scriptIdentifierParse(const char* src, int len, int* newPos, TokenFlags* flags);
	int scriptDigitParse(const char *src, int len, int* newPos, TokenFlags* flags);
	int scriptSymbolParse(const char *src, int len, int* newPos, TokenFlags* flags);
	int scriptFloatSyntaxErrors(char c, int* flags);
	int scriptCompareSymbol(char c, char x);
	int scriptCheckFlags(int flags, int source);

private:
	int isSymbol(char c);
	int isODigit(char c);
	int isNonODigit(char c);
	int isXDigit(char c);
	int isDigit(char c);
	int isNonZeroDigit(char c);
	int isZeroDigit(char c);
	int isHexPrefix(char c);
	int isFSuffix(char c);
	int isLSuffix(char c);
	int isUSuffix(char c);
	int isIdentifier(char c);
	int isBlank(char c);
	int isCRLF(char c);
	int isEscape(char c);
	int isExponent(char c);
	int isSign(char c);
	int isDot(char c);
	plimuni::cString m_buffer;
	int m_pos;
};

};

#endif
