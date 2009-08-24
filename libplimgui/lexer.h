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

#include <ncurses.h>
#include <ctype.h>
#include "treenodes.h"
#include "strings.h"

namespace NSString {

using namespace NSTree;

#define PLIM_OK	1
#define PLIM_ERR	0

#define PLIM_L_UNRECOGNIZED							0x00000000
#define PLIM_L_IDENTIFIER								0x00000001
#define PLIM_L_SYMBOL									0x00000002
#define PLIM_L_BLANK										0x00000004
#define PLIM_L_DIGIT										0x00000008
#define PLIM_L_ESCAPE									0x00000010

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
#define PLIM_L_CASE_FLAGS_SYMBOLS									0

#define PLIM_L_SYMBOL_COLON							0x00000001
#define PLIM_L_SYMBOL_SEMICOLON						0x00000002
#define PLIM_L_SYMBOL_COMMA							0x00000004
#define PLIM_L_SYMBOL_DOT								0x00000008
#define PLIM_L_SYMBOL_BACKSLASH						0x00000010

/* fBrackets */
#define PLIM_L_CASE_FLAGS_BRACKETS					0x00000001

#define PLIM_L_SYMBOL_BRACKET_PARENTHESES_OPEN	0x00000001
#define PLIM_L_SYMBOL_BRACKET_PARENTHESES_CLOSE	0x00000002
#define PLIM_L_SYMBOL_BRACKET_SQUARE_OPEN			0x00000004
#define PLIM_L_SYMBOL_BRACKET_SQUARE_CLOSE		0x00000008
#define PLIM_L_SYMBOL_BRACKET_CURLY_OPEN			0x00000010
#define PLIM_L_SYMBOL_BRACKET_CURLY_CLOSE			0x00000020
#define PLIM_L_SYMBOL_BRACKET_ANGLE_OPEN			0x00000040
#define PLIM_L_SYMBOL_BRACKET_ANGLE_CLOSE			0x00000080
 
/* fMath syms */
#define PLIM_L_CASE_FLAGS_MATH						0x00000002

#define PLIM_L_SYMBOL_MATH_SIGN_PLUS				0x00000001
#define PLIM_L_SYMBOL_MATH_SIGN_MINUS				0x00000002
#define PLIM_L_SYMBOL_MATH_ASSIGN					0x00000004
#define PLIM_L_SYMBOL_MATH_SIGN_MULTIPLE			0x00000008
#define PLIM_L_SYMBOL_MATH_SIGN_DIVIDE				0x00000010
#define PLIM_L_SYMBOL_MATH_LESS_THAN				PLIM_L_SYMBOL_BRACKET_ANGLE_OPEN
#define PLIM_L_SYMBOL_MATH_GREATER_THAN			PLIM_L_SYMBOL_BRACKET_ANGLE_CLOSE /* 40 */

/* fLiterals syms */
#define PLIM_L_CASE_FLAGS_LITERALS					0x00000004

#define PLIM_L_SYMBOL_LITERAL_QUOTE					0x00000001
#define PLIM_L_SYMBOL_LITERAL_DQUOTE				0x00000002

/* Ncurses syms */
#define PLIM_L_CASE_FLAGS_CURSES						0x00000008

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
class cPlimToken: public cTreeNode, public cString {
public:
	cPlimToken(cPlimLexer* nodes)
	:	cTreeNode((cTreeNodes*) nodes, NULL, NULL), cString(),
		m_tokenCase(0), m_tokenExCase(0), m_illegalFlags(0) {};
	virtual ~cPlimToken(void) {};
	void SetTokenCase(int flags) { m_tokenCase = flags; };
	int GetTokenCase(void) { return m_tokenCase; };
	void SetTokenExCase(int flags) { m_tokenExCase = flags; };
	int GetTokenExCase(void) { return m_tokenExCase; };
	void SetOffset(int offset) { m_offset = offset; };
	int GetOffset(void) { return m_offset; };
	void SetLen(int len) { m_len = len; };
	int GetLen(void) { return m_len; };
	void SetIllegalFlags(int flags) { m_illegalFlags = flags; };
	int GetIllegalFlags(void) { return m_illegalFlags; };
	cPlimToken* GetNextNode(void) { return (cPlimToken*) cTreeNode::GetNextNode(); };
	cPlimToken* GetPrevNode(void) { return (cPlimToken*) cTreeNode::GetPrevNode(); };
private:
	int m_tokenCase;
	int m_tokenExCase;
	int m_illegalFlags;
	int m_offset;
	int m_len;
};

class cPlimLexer: public cTreeNodes {
public:
	cPlimLexer(void);
	cPlimLexer(const char* string);
	virtual ~cPlimLexer(void);
	virtual void Refresh(const char* string);
	cPlimToken* GetFirstNode(void) { return (cPlimToken*) cTreeNodes::GetFirstNode(); };
	cPlimToken* GetLastNode(void) { return (cPlimToken*) cTreeNodes::GetLastNode(); };
protected:
	virtual void PerformParse(void);
	int ScriptType(char c);
	void ScriptShiftPos(int* newPos, int num = 1);
	int ScriptBlankParse(const char* src, int len, int* newPos, TokenFlags* flags);
	int ScriptIdentifierParse(const char* src, int len, int* newPos, TokenFlags* flags);
	int ScriptDigitParse(const char *src, int len, int* newPos, TokenFlags* flags);
	int ScriptSymbolParse(const char *src, int len, int* newPos, TokenFlags* flags);
	int ScriptFloatSyntaxErrors(char c, int* flags);
	int ScriptCompareSymbol(char c, char x);
	int ScriptCheckFlags(int flags, int source);
private:
	int IsSymbol(char c);
	int IsODigit(char c);
	int IsNonODigit(char c);
	int IsXDigit(char c);
	int IsDigit(char c);
	int IsNonZeroDigit(char c);
	int IsZeroDigit(char c);
	int IsHexPrefix(char c);
	int IsFSuffix(char c);
	int IsLSuffix(char c);
	int IsUSuffix(char c);
	int IsIdentifier(char c);
	int IsBlank(char c);
	int IsCRLF(char c);
	int IsEscape(char c);
	int IsExponent(char c);
	int IsSign(char c);
	int IsDot(char c);
	cString m_buffer;
	int m_pos;
};

};

#endif
