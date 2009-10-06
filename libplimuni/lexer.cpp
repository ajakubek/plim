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

#include "lexer.hpp"

namespace plimuni {

cPlimLexer::cPlimLexer(void)	:	plimuni::cTreeNodes(), 
											m_pos(0) 
{
	
}

cPlimLexer::cPlimLexer(const char* string)	:	plimuni::cTreeNodes(),
																m_pos(0) 
{
	refresh( string );
}

cPlimLexer::~cPlimLexer(void) 
{

}

void cPlimLexer::refresh(const char* string) 
{
	cPlimToken* token;
	cPlimToken* temp;

	m_buffer.copy(string);

	token = getLastNode();

	while (token) {
		token = (cPlimToken*) removeNode(token, 1);
	}

	performParse();
}

void cPlimLexer::performParse(void) 
{
	char* src = (char*) m_buffer.getBuffer();
	int len = m_buffer.getLength();
	int lexerType;
	int i = 0;
	int pos;
	TokenFlags flags;
	cPlimToken* newToken;

	for (;i < len; )
	{
		lexerType = scriptType(src[i]);	
		pos = i;

		switch (lexerType)
		{
			case PLIM_L_FEED: {
				if (scriptBlankParse(&src[i], len - i, &i, &flags) == PLIM_OK) {
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->setOffset( pos );
						newToken->setLen( i - pos );
						newToken->setTokenCase(PLIM_L_FEED);
						newToken->setTokenExCase(flags.b);
						newToken->setIllegalFlags(flags.c);
						newToken->copy( &src[pos], i - pos );
					}
				}
				break;
			}

			case PLIM_L_BLANK:
			{
				if (scriptBlankParse(&src[i], len - i, &i, &flags) == PLIM_OK)
				{
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->setOffset( pos );
						newToken->setLen( i - pos );
						newToken->setTokenCase(flags.a);
						newToken->setTokenExCase(flags.b);
						newToken->setIllegalFlags(flags.c);
						newToken->copy( &src[pos], i - pos );
					}
				}

				break;
			}
			
			case PLIM_L_SYMBOL:
			{

				if (scriptSymbolParse(&src[i], len - i, &i, &flags) == PLIM_OK)
				{
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->setOffset( pos );
						newToken->setLen( i - pos);
						newToken->setTokenCase(flags.a);
						newToken->setTokenExCase(flags.b);
						newToken->setIllegalFlags(flags.c);
						newToken->copy( &src[pos], i - pos );
					}
				}

				break;
			}
			
			case PLIM_L_DIGIT:
			{

				if (scriptDigitParse(&src[i], len - i, &i, &flags) == PLIM_OK)
				{
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->setOffset( pos );
						newToken->setLen(  i - pos );
						newToken->setTokenCase(flags.a);
						newToken->setTokenExCase(flags.b);
						newToken->setIllegalFlags(flags.c);
						newToken->copy( &src[pos], i - pos );
					}
				}

				break;
			}
		
			case PLIM_L_IDENTIFIER:
			{

  				if (scriptIdentifierParse(&src[i], len - i,  &i, &flags) == PLIM_OK)
				{
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->setOffset( pos );
						newToken->setLen( i - pos);
						newToken->setTokenCase(flags.a);
						newToken->setTokenExCase(flags.b);
						newToken->setIllegalFlags(flags.c);
						newToken->copy( &src[pos], i - pos );
					}

				}

				break;
			}
			
			default:
			{
				scriptShiftPos(&i);
				break;
			}
		}
	
	}

}

int cPlimLexer::scriptType(char c)
{
	if (isCRLF(c))
		return PLIM_L_FEED;
	if (isDigit(c))
		return PLIM_L_DIGIT;
	if (isEscape(c))
		return PLIM_L_ESCAPE;
	if (isBlank(c))
		return PLIM_L_BLANK;
	if (isSymbol(c))
		return PLIM_L_SYMBOL;
	if (isIdentifier(c))
		return PLIM_L_IDENTIFIER;

	return PLIM_L_UNRECOGNIZED;
}

void cPlimLexer::scriptShiftPos(int* newPos, int num) 
{
	if (newPos) {
		*newPos += num;
	}
}

int cPlimLexer::scriptBlankParse(const char* src, int len, int* newPos, TokenFlags* flags)
{
	int i = 0;
	
	for (;i < len;i++)
	{
		if (!isBlank(src[i])) {
			//ScriptShiftPos(newPos, -1);
			break;
		}

		scriptShiftPos(newPos);
	}

	flags->a = PLIM_L_BLANK;
	flags->b = 0;
	flags->c = 0;

	return PLIM_OK;
}

int cPlimLexer::scriptIdentifierParse(const char* src, int len, int* newPos, TokenFlags* flags) 
{
	int i = 0;
	char x;

	for (;i < len;i++)
	{
		x = src[i];

		if (isSymbol(src[i])) {

			break;
		}

		if (!isIdentifier(src[i]) && (i > 0 && !isDigit(src[i]))) break;

		scriptShiftPos(newPos);

	}

	flags->a = PLIM_L_IDENTIFIER;
	flags->b = 0;
	flags->c = 0;

	return PLIM_OK;
}

int cPlimLexer::scriptDigitParse(const char *src, int len, int* newPos, TokenFlags* flags) 
{
	int result = 0;
	int illegalResult = 0;
	int i = 0;
	int counter = 0;
	int is_float = 0;
	int pos = 0;
	
	for (;i < len; i++)
	{
		pos = i;

		if (isNonZeroDigit(src[i]))
		{
			result = PLIM_L_DIGIT_INTEGER;

			do
			{
				if (i >= len || 
						isBlank(src[i]) ||
						(isSymbol(src[i]) && !isDot(src[i]) /*&& !IsSign(src[i])*/) ) {
					scriptShiftPos(newPos, -1);
					break;
				}

				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				scriptFloatSyntaxErrors(src[i], &result);
				
				if (!isDigit(src[i]))
				{
				/*
					if (IsSign(src[i]))
					{
						illegalresult |= PLIM_L_DIGIT_FLOAT_EXPONENT_NODIGITS;
					}
					else */
					/* Check for float or suffixes */
					if (isDot(src[i]))
					{
					
						if (result & PLIM_L_DIGIT_FLOAT)
						{
							illegalResult |= PLIM_L_DIGIT_FLOAT_TOO_MANY_DOTS;
						}
						else
						{
							result = PLIM_L_DIGIT_FLOAT;
						}

						continue;
					}
					else
					/* Check for exponent */
					if (isExponent(src[i]))
					{
						/* Mark the exponent as invalid */
						if (result & PLIM_L_DIGIT_FLOAT_EXPONENT)	illegalResult |= PLIM_L_DIGIT_FLOAT_INVALID_SUFFIX;

						result |= PLIM_L_DIGIT_FLOAT | PLIM_L_DIGIT_FLOAT_EXPONENT;
						
						scriptShiftPos(newPos);
						scriptShiftPos(&i);
						scriptFloatSyntaxErrors(src[i], &result);

						if (i >= len) break;
						
						/* Check for sign, if there is no, then assume its a + */
						if (isSign(src[i]))
						{
							if (scriptCompareSymbol('-', src[i])) result |= PLIM_L_DIGIT_FLOAT_EXPONENT_MINUS;
							else	result |= PLIM_L_DIGIT_FLOAT_EXPONENT_PLUS;

							scriptShiftPos(newPos);
							scriptShiftPos(&i);
							scriptFloatSyntaxErrors(src[i], &result);

							if (i >= len) break;

							if (!isDigit(src[i]))
								illegalResult |= PLIM_L_DIGIT_FLOAT_EXPONENT_NODIGITS;

						}
						else
						{
							if (!isDigit(src[i]))
							{
								illegalResult |= PLIM_L_DIGIT_FLOAT_EXPONENT_NODIGITS;
							}
							else
								result |= PLIM_L_DIGIT_FLOAT_EXPONENT_PLUS;
						}
						
					}
					else
					if (isLSuffix(src[i]) || isFSuffix(src[i]) || isUSuffix(src[i]))
					{
						/* Check for suffixes on a float */
						if (result & PLIM_L_DIGIT_FLOAT)
						{
							if (isLSuffix(src[i]))
							{
								result |= PLIM_L_DIGIT_FLOAT_L_SUFFIX;
							}
							else
							if (isFSuffix(src[i]))
							{
								result |= PLIM_L_DIGIT_FLOAT_F_SUFFIX;
							}
							else
							if (!isSymbol(src[i]) || !isBlank(src[i]))
							{
								illegalResult |= PLIM_L_DIGIT_FLOAT_INVALID_SUFFIX;
							}
							
							scriptShiftPos(newPos);
							scriptShiftPos(&i);
							
							if (i >= len)	break;
							
							if (isSymbol(src[i]) || isBlank(src[i]))
							{
								scriptShiftPos(newPos, -1);
								break;
							}
							else
							{
								illegalResult |= PLIM_L_DIGIT_FLOAT_INVALID_SUFFIX;
							}
							
						}
						/* Check for suffixes on a decimal */
						else
						{
							if (scriptCheckFlags(PLIM_L_DIGIT_INTEGER | PLIM_L_DIGIT_INTEGER_L_SUFFIX | PLIM_L_DIGIT_INTEGER_U_SUFFIX, result))
							{
								if (isLSuffix(src[i]))
								{
									if (result & PLIM_L_DIGIT_INTEGER_L_SUFFIX)
									{
										illegalResult |= PLIM_L_DIGIT_INTEGER_INVALID_SUFFIX;
									}
									else
									{
										result |= PLIM_L_DIGIT_INTEGER_L_SUFFIX;
									}
								}
								else
								if (isUSuffix(src[i]))
								{
									if (result & PLIM_L_DIGIT_INTEGER_U_SUFFIX)
									{
										illegalResult |= PLIM_L_DIGIT_INTEGER_INVALID_SUFFIX;
									}
									else
									{
										result |= PLIM_L_DIGIT_INTEGER_U_SUFFIX;
									}
								}
								else
								{
									illegalResult |= PLIM_L_DIGIT_INTEGER_INVALID_SUFFIX;
								}

							}
						}					
					}
					else
					{
						if (i >= len ||
							isBlank(src[i]) ||
							(isSymbol(src[i]) && !isDot(src[i]) /*&& !IsSign(src[i])*/) ) {

							break;
						}
						
						if (scriptCheckFlags(PLIM_L_DIGIT_INTEGER | PLIM_L_DIGIT_INTEGER_L_SUFFIX | PLIM_L_DIGIT_INTEGER_U_SUFFIX, result) && !isDigit(src[i]))
						{
							illegalResult |= PLIM_L_DIGIT_INTEGER_INVALID_SUFFIX;
							continue;
						}
					}

				} /* If digit happen  */
				else
				if (isDigit(src[i]))
				{
					/* Check if we have a suffix already on our digit */
					if (scriptCheckFlags(PLIM_L_DIGIT_INTEGER_L_SUFFIX | PLIM_L_DIGIT_INTEGER_U_SUFFIX, result))
					{
						illegalResult |= PLIM_L_DIGIT_INTEGER_INVALID_SUFFIX;
					}
					
				}
				
			} while (i < len);
		
		}
		else if (isZeroDigit(src[i]))
		/* Process octal numbers or hexadecimal */
		{
			result = PLIM_L_DIGIT_INTEGER;
			
			scriptShiftPos(newPos);
			scriptShiftPos(&i);
			
			if (isHexPrefix(src[i]))
			{
				result = PLIM_L_DIGIT_HEXADECIMAL;

				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				do
				{
					if (isSymbol(src[i]) || isBlank(src[i]) || i >= len) {

						break;
					}

					if (!isXDigit(src[i]))
					{
						if (isLSuffix(src[i]))
						{
							if (result & PLIM_L_DIGIT_HEX_L_SUFFIX)
							{
								illegalResult |= PLIM_L_DIGIT_HEX_INVALID_SUFFIX;
							}
							else
							{
								result |= PLIM_L_DIGIT_HEX_L_SUFFIX;
							}
						}
						else
						if (isUSuffix(src[i]))
						{
							if (result & PLIM_L_DIGIT_HEX_U_SUFFIX)
							{
								illegalResult |= PLIM_L_DIGIT_HEX_INVALID_SUFFIX;
							}
							else
							{
								result |= PLIM_L_DIGIT_HEX_U_SUFFIX;
							}
						}
						else
						{
							illegalResult |= PLIM_L_DIGIT_HEX_INVALID_SUFFIX;
						}
					}
					else
					{
						if (scriptCheckFlags(PLIM_L_DIGIT_HEX_U_SUFFIX | PLIM_L_DIGIT_HEX_L_SUFFIX, result))
						{
							illegalResult |= PLIM_L_DIGIT_HEX_INVALID_SUFFIX;
						}
					}
					
					scriptShiftPos(newPos);
					scriptShiftPos(&i);

				}
					while (1);

			}
			else
			if (isDigit(src[i]))
			{
				result = PLIM_L_DIGIT_OCTAL;

				do
				{
					if (isSymbol(src[i]) || isBlank(src[i]) || i >= len) {

						break;
					}

					if (!isODigit(src[i]))
					{
						if (isLSuffix(src[i]))
						{
							if (result & PLIM_L_DIGIT_OCTAL_L_SUFFIX)
							{
								illegalResult |= PLIM_L_DIGIT_OCTAL_INVALID_SUFFIX;
							}
							else
							{
								result |= PLIM_L_DIGIT_OCTAL_L_SUFFIX;
							}
						}
						else
						if (isUSuffix(src[i]))
						{
							if (result & PLIM_L_DIGIT_OCTAL_U_SUFFIX)
							{
								illegalResult |= PLIM_L_DIGIT_OCTAL_INVALID_SUFFIX;
							}
							else
							{
								result |= PLIM_L_DIGIT_OCTAL_U_SUFFIX;
							}
						}
						else
						{
							illegalResult |= PLIM_L_DIGIT_OCTAL_INVALID_SUFFIX;
						}
					}
					else
					{
						if (scriptCheckFlags(PLIM_L_DIGIT_OCTAL_U_SUFFIX | PLIM_L_DIGIT_OCTAL_L_SUFFIX, result))
						{
							illegalResult |= PLIM_L_DIGIT_OCTAL_INVALID_SUFFIX;
						}
					}
					
					scriptShiftPos(newPos);
					scriptShiftPos(&i);

				}
					while (1);

			}
			
		}

		flags->a = result;
		flags->b = 0;
		flags->c = illegalResult;

		break;
	}

	return PLIM_OK;
}

int cPlimLexer::scriptSymbolParse(const char *src, int len, int* newPos, TokenFlags* flags)
{
	int result = 0, illegalResult = 0;
	int i = 0;
	int pos = 0;
	int case_flags = PLIM_L_SYMBOL;
	
	for (;i < len;)
	{
		if (!isSymbol(src[i]))	{

			break;
		}
		
		pos = i;

		/* Check for ncurses spec chars first */
		if (src[i] < 32) {
			case_flags = PLIM_L_CASE_FLAGS_CURSES;
			result = (int) src[i];
			scriptShiftPos(newPos);
		}
		else switch (src[i])
		{
			/* Main symbols */
			case '\\':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_BACKSLASH;
				scriptShiftPos(newPos);
				scriptShiftPos(&i);

				break;
			}
 
			case ':':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_COLON;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;
			}
			
			case ';':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_SEMICOLON;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
								
				break;
			}

			case ',':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_COMMA;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;
			}

			case '.':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_DOT;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;
			}
			/* End of main symbols */
			
			/* Bracket symbols */
			case '(':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_PARENTHESES_OPEN;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;			
			}
			
			case ')':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_PARENTHESES_CLOSE;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;			
			}
			
			case '[':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_SQUARE_OPEN;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;			
			}

			case ']':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_SQUARE_CLOSE;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;			
			}
			
			case '{':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_CURLY_OPEN;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;			
			}

			case '}':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_CURLY_CLOSE;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);

				break;			
			}
						
			case '<':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS | PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_BRACKET_ANGLE_OPEN;

				scriptShiftPos(newPos);
				scriptShiftPos(&i);

				break;			
			}
			
			case '>':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS | PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_BRACKET_ANGLE_CLOSE;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;			
			}		
			/* End of bracket symbols */
			
			/* Math symbols */
			case '#':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_HATCH;

				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				break;
			}

			case '+':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_SIGN_PLUS;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;						
			}

			case '-':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_SIGN_MINUS;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;						
			}
			
			case '=':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_ASSIGN;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;						
			}

			case '*':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_SIGN_MULTIPLE;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;						
			}

			case '/':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_SIGN_DIVIDE;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;						
			}

			case '%':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_PERCENT;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;									
			}
			/* End of math symbols */
			
			/* Literal symbols */
			
			case '"':
			{
				case_flags = PLIM_L_CASE_FLAGS_LITERALS;
				result = PLIM_L_SYMBOL_LITERAL_DQUOTE;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;						
			}

			case '\'':
			{
				case_flags = PLIM_L_CASE_FLAGS_LITERALS;
				result = PLIM_L_SYMBOL_LITERAL_QUOTE;
				
				scriptShiftPos(newPos);
				scriptShiftPos(&i);
				
				break;
			}
			/* End of literal symbols */
			
			default:
			{
				scriptShiftPos(newPos);
				scriptShiftPos(&i);

				break;
			}
		}

		flags->a = case_flags;
		flags->b = result;
		flags->c = illegalResult;
		
		break;
	}

	return PLIM_OK;
}

int cPlimLexer::scriptFloatSyntaxErrors(char c, int* flags) 
{
	if (*flags & PLIM_L_DIGIT_FLOAT)
	{
		if ( isDot(c) )
		{
			*flags |= PLIM_L_DIGIT_FLOAT_TOO_MANY_DOTS;
			return 1;
		}
	}

	return 0;
}

int cPlimLexer::scriptCompareSymbol(char c, char x) 
{
	return (c == x);
}

int cPlimLexer::scriptCheckFlags(int flags, int source) 
{
	return (source & flags);
}

/* Private */
int cPlimLexer::isSymbol(char c)
{
	/* Include control chars */
	return (ispunct(c) && !(c == '_')) || (c < 32) || isEscape(c);
}

int cPlimLexer::isODigit(char c)
{
	return (c >= '0' && c < '8');
}

int cPlimLexer::isNonODigit(char c)
{
	return (c > '7' && c <= '9');
}

int cPlimLexer::isXDigit(char c)
{
	return isxdigit(c);
}

int cPlimLexer::isDigit(char c)
{
	return isdigit(c);
}

int cPlimLexer::isNonZeroDigit(char c)
{
	return isDigit(c) && !isZeroDigit(c);
}

int cPlimLexer::isZeroDigit(char c)
{
	return (c == '0');
}

int cPlimLexer::isHexPrefix(char c)
{
	return (c == 'x' || c == 'X');
}

int cPlimLexer::isFSuffix(char c)
{
	return (c == 'F' || c == 'f');
}

int cPlimLexer::isLSuffix(char c)
{
	return (c == 'L' || c == 'l');
}

int cPlimLexer::isUSuffix(char c)
{
	return (c == 'U' || c == 'u');
}

int cPlimLexer::isIdentifier(char c)
{
	return (isalpha(c) || (c == '_'));
}

int cPlimLexer::isBlank(char c)
{
	return (c == ' ' || c == '\t' || isCRLF(c));
}

int cPlimLexer::isCRLF(char c)
{
	return (c == '\r' || c == '\n');
}

int cPlimLexer::isEscape(char c)
{
	return (c == '\\');
}

int cPlimLexer::isExponent(char c)
{
	return (c == 'e' || c == 'E');
}

int cPlimLexer::isSign(char c)
{
	return (c == '-' || c == '+');
}

int cPlimLexer::isDot(char c)
{
	return (c == '.');
}

};
