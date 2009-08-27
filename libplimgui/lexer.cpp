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

#include "lexer.h"

namespace NSString {

cPlimLexer::cPlimLexer(void)
:	cTreeNodes(), m_pos(0) {
	
}

cPlimLexer::cPlimLexer(const char* string)
:	cTreeNodes(),
	m_pos(0) {
	Refresh( string );
}

cPlimLexer::~cPlimLexer(void) {

}

void cPlimLexer::Refresh(const char* string) {
	cPlimToken* token;
	cPlimToken* temp;

	m_buffer.Copy(string);

	token = GetLastNode();

	while (token) {
		token = (cPlimToken*) RemoveNode(token, 1);
	}

	PerformParse();
}

void cPlimLexer::PerformParse(void) {
	char* src = (char*) m_buffer.GetBuffer();
	int len = m_buffer.GetLength();
	int lexerType;
	int i = 0;
	int pos;
	TokenFlags flags;
	cPlimToken* newToken;

	for (;i < len; )
	{
		lexerType = ScriptType(src[i]);	
		pos = i;

		switch (lexerType)
		{
			case PLIM_L_BLANK:
			{
				if (ScriptBlankParse(&src[i], len - i, &i, &flags) == PLIM_OK)
				{
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->SetOffset( pos );
						newToken->SetLen( i - pos );
						newToken->SetTokenCase(flags.a);
						newToken->SetTokenExCase(flags.b);
						newToken->SetIllegalFlags(flags.c);
						newToken->Copy( &src[pos], i - pos );
					}
				}

				break;
			}
			
			case PLIM_L_SYMBOL:
			{

				if (ScriptSymbolParse(&src[i], len - i, &i, &flags) == PLIM_OK)
				{
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->SetOffset( pos );
						newToken->SetLen( i - pos);
						newToken->SetTokenCase(flags.a);
						newToken->SetTokenExCase(flags.b);
						newToken->SetIllegalFlags(flags.c);
						newToken->Copy( &src[pos], i - pos );
					}
				}

				break;
			}
			
			case PLIM_L_DIGIT:
			{

				if (ScriptDigitParse(&src[i], len - i, &i, &flags) == PLIM_OK)
				{
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->SetOffset( pos );
						newToken->SetLen(  i - pos );
						newToken->SetTokenCase(flags.a);
						newToken->SetTokenExCase(flags.b);
						newToken->SetIllegalFlags(flags.c);
						newToken->Copy( &src[pos], i - pos );
					}
				}

				break;
			}
		
			case PLIM_L_IDENTIFIER:
			{

  				if (ScriptIdentifierParse(&src[i], len - i,  &i, &flags) == PLIM_OK)
				{
					newToken = new cPlimToken( this );
					if (newToken) {
						newToken->SetOffset( pos );
						newToken->SetLen( i - pos);
						newToken->SetTokenCase(flags.a);
						newToken->SetTokenExCase(flags.b);
						newToken->SetIllegalFlags(flags.c);
						newToken->Copy( &src[pos], i - pos );
					}

				}

				break;
			}
			
			default:
			{
				ScriptShiftPos(&i);
				break;
			}
		}
	
	}

}

int cPlimLexer::ScriptType(char c)
{
	if (IsDigit(c))
		return PLIM_L_DIGIT;
	if (IsEscape(c))
		return PLIM_L_ESCAPE;
	if (IsBlank(c))
		return PLIM_L_BLANK;
	if (IsSymbol(c))
		return PLIM_L_SYMBOL;
	if (IsIdentifier(c))
		return PLIM_L_IDENTIFIER;

	return PLIM_L_UNRECOGNIZED;
}

void cPlimLexer::ScriptShiftPos(int* newPos, int num) {
	if (newPos) {
		*newPos += num;
	}
}

int cPlimLexer::ScriptBlankParse(const char* src, int len, int* newPos, TokenFlags* flags)
{
	int i = 0;
	
	for (;i < len;i++)
	{
		if (!IsBlank(src[i])) {
			//ScriptShiftPos(newPos, -1);
			break;
		}

		ScriptShiftPos(newPos);
	}

	flags->a = PLIM_L_BLANK;
	flags->b = 0;
	flags->c = 0;

	return PLIM_OK;
}

int cPlimLexer::ScriptIdentifierParse(const char* src, int len, int* newPos, TokenFlags* flags) {
	int i = 0;
	char x;

	for (;i < len;i++)
	{
		x = src[i];

		if (IsSymbol(src[i])) {

			break;
		}

		if (!IsIdentifier(src[i]) && (i > 0 && !IsDigit(src[i]))) break;

		ScriptShiftPos(newPos);

	}

	flags->a = PLIM_L_IDENTIFIER;
	flags->b = 0;
	flags->c = 0;

	return PLIM_OK;
}

int cPlimLexer::ScriptDigitParse(const char *src, int len, int* newPos, TokenFlags* flags) {
	int result = 0;
	int illegalResult = 0;
	int i = 0;
	int counter = 0;
	int is_float = 0;
	int pos = 0;
	
	for (;i < len; i++)
	{
		pos = i;

		if (IsNonZeroDigit(src[i]))
		{
			result = PLIM_L_DIGIT_INTEGER;

			do
			{
				if (i >= len || 
						IsBlank(src[i]) ||
						(IsSymbol(src[i]) && !IsDot(src[i]) /*&& !IsSign(src[i])*/) ) {
					ScriptShiftPos(newPos, -1);
					break;
				}

				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				ScriptFloatSyntaxErrors(src[i], &result);
				
				if (!IsDigit(src[i]))
				{
				/*
					if (IsSign(src[i]))
					{
						illegalresult |= PLIM_L_DIGIT_FLOAT_EXPONENT_NODIGITS;
					}
					else */
					/* Check for float or suffixes */
					if (IsDot(src[i]))
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
					if (IsExponent(src[i]))
					{
						/* Mark the exponent as invalid */
						if (result & PLIM_L_DIGIT_FLOAT_EXPONENT)	illegalResult |= PLIM_L_DIGIT_FLOAT_INVALID_SUFFIX;

						result |= PLIM_L_DIGIT_FLOAT | PLIM_L_DIGIT_FLOAT_EXPONENT;
						
						ScriptShiftPos(newPos);
						ScriptShiftPos(&i);
						ScriptFloatSyntaxErrors(src[i], &result);

						if (i >= len) break;
						
						/* Check for sign, if there is no, then assume its a + */
						if (IsSign(src[i]))
						{
							if (ScriptCompareSymbol(L'-', src[i])) result |= PLIM_L_DIGIT_FLOAT_EXPONENT_MINUS;
							else	result |= PLIM_L_DIGIT_FLOAT_EXPONENT_PLUS;

							ScriptShiftPos(newPos);
							ScriptShiftPos(&i);
							ScriptFloatSyntaxErrors(src[i], &result);

							if (i >= len) break;

							if (!IsDigit(src[i]))
								illegalResult |= PLIM_L_DIGIT_FLOAT_EXPONENT_NODIGITS;

						}
						else
						{
							if (!IsDigit(src[i]))
							{
								illegalResult |= PLIM_L_DIGIT_FLOAT_EXPONENT_NODIGITS;
							}
							else
								result |= PLIM_L_DIGIT_FLOAT_EXPONENT_PLUS;
						}
						
					}
					else
					if (IsLSuffix(src[i]) || IsFSuffix(src[i]) || IsUSuffix(src[i]))
					{
						/* Check for suffixes on a float */
						if (result & PLIM_L_DIGIT_FLOAT)
						{
							if (IsLSuffix(src[i]))
							{
								result |= PLIM_L_DIGIT_FLOAT_L_SUFFIX;
							}
							else
							if (IsFSuffix(src[i]))
							{
								result |= PLIM_L_DIGIT_FLOAT_F_SUFFIX;
							}
							else
							if (!IsSymbol(src[i]) || !IsBlank(src[i]))
							{
								illegalResult |= PLIM_L_DIGIT_FLOAT_INVALID_SUFFIX;
							}
							
							ScriptShiftPos(newPos);
							ScriptShiftPos(&i);
							
							if (i >= len)	break;
							
							if (IsSymbol(src[i]) || IsBlank(src[i]))
							{
								ScriptShiftPos(newPos, -1);
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
							if (ScriptCheckFlags(PLIM_L_DIGIT_INTEGER | PLIM_L_DIGIT_INTEGER_L_SUFFIX | PLIM_L_DIGIT_INTEGER_U_SUFFIX, result))
							{
								if (IsLSuffix(src[i]))
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
								if (IsUSuffix(src[i]))
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
							IsBlank(src[i]) ||
							(IsSymbol(src[i]) && !IsDot(src[i]) /*&& !IsSign(src[i])*/) ) {

							break;
						}
						
						if (ScriptCheckFlags(PLIM_L_DIGIT_INTEGER | PLIM_L_DIGIT_INTEGER_L_SUFFIX | PLIM_L_DIGIT_INTEGER_U_SUFFIX, result) && !IsDigit(src[i]))
						{
							illegalResult |= PLIM_L_DIGIT_INTEGER_INVALID_SUFFIX;
							continue;
						}
					}

				} /* If digit happen  */
				else
				if (IsDigit(src[i]))
				{
					/* Check if we have a suffix already on our digit */
					if (ScriptCheckFlags(PLIM_L_DIGIT_INTEGER_L_SUFFIX | PLIM_L_DIGIT_INTEGER_U_SUFFIX, result))
					{
						illegalResult |= PLIM_L_DIGIT_INTEGER_INVALID_SUFFIX;
					}
					
				}
				
			} while (i < len);
		
		}
		else if (IsZeroDigit(src[i]))
		/* Process octal numbers or hexadecimal */
		{
			result = PLIM_L_DIGIT_INTEGER;
			
			ScriptShiftPos(newPos);
			ScriptShiftPos(&i);
			
			if (IsHexPrefix(src[i]))
			{
				result = PLIM_L_DIGIT_HEXADECIMAL;

				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				do
				{
					if (IsSymbol(src[i]) || IsBlank(src[i]) || i >= len) {

						break;
					}

					if (!IsXDigit(src[i]))
					{
						if (IsLSuffix(src[i]))
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
						if (IsUSuffix(src[i]))
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
						if (ScriptCheckFlags(PLIM_L_DIGIT_HEX_U_SUFFIX | PLIM_L_DIGIT_HEX_L_SUFFIX, result))
						{
							illegalResult |= PLIM_L_DIGIT_HEX_INVALID_SUFFIX;
						}
					}
					
					ScriptShiftPos(newPos);
					ScriptShiftPos(&i);

				}
					while (1);

			}
			else
			if (IsDigit(src[i]))
			{
				result = PLIM_L_DIGIT_OCTAL;

				do
				{
					if (IsSymbol(src[i]) || IsBlank(src[i]) || i >= len) {

						break;
					}

					if (!IsODigit(src[i]))
					{
						if (IsLSuffix(src[i]))
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
						if (IsUSuffix(src[i]))
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
						if (ScriptCheckFlags(PLIM_L_DIGIT_OCTAL_U_SUFFIX | PLIM_L_DIGIT_OCTAL_L_SUFFIX, result))
						{
							illegalResult |= PLIM_L_DIGIT_OCTAL_INVALID_SUFFIX;
						}
					}
					
					ScriptShiftPos(newPos);
					ScriptShiftPos(&i);

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

int cPlimLexer::ScriptSymbolParse(const char *src, int len, int* newPos, TokenFlags* flags)
{
	int result, illegalResult;
	int i = 0;
	int pos = 0;
	int case_flags = PLIM_L_SYMBOL;
	
	for (;i < len;)
	{
		if (!IsSymbol(src[i]))	{

			break;
		}
		
		pos = i;

		/* Check for ncurses spec chars first */
		if (src[i] < 32) {
			case_flags = PLIM_L_CASE_FLAGS_CURSES;
			result = (int) src[i];
			ScriptShiftPos(newPos);
		}
		else switch (src[i])
		{
			/* Main symbols */
			case L'\\':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_BACKSLASH;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);

				break;
			}
 
			case L':':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_COLON;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;
			}
			
			case L';':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_SEMICOLON;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
								
				break;
			}

			case L',':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_COMMA;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;
			}

			case L'.':
			{
				case_flags = PLIM_L_CASE_FLAGS_SYMBOLS;
				result = PLIM_L_SYMBOL_DOT;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;
			}
			/* End of main symbols */
			
			/* Bracket symbols */
			case L'(':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_PARENTHESES_OPEN;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;			
			}
			
			case L')':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_PARENTHESES_CLOSE;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;			
			}
			
			case L'[':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_SQUARE_OPEN;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;			
			}

			case L']':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_SQUARE_CLOSE;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;			
			}
			
			case L'{':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_CURLY_OPEN;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;			
			}

			case L'}':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS;
				result = PLIM_L_SYMBOL_BRACKET_CURLY_CLOSE;

				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);

				break;			
			}
						
			case L'<':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS | PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_BRACKET_ANGLE_OPEN;

				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);

				break;			
			}
			
			case L'>':
			{
				case_flags = PLIM_L_CASE_FLAGS_BRACKETS | PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_BRACKET_ANGLE_CLOSE;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;			
			}		
			/* End of bracket symbols */
			
			/* Math symbols */
			case L'+':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_SIGN_PLUS;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;						
			}

			case L'-':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_SIGN_MINUS;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;						
			}
			
			case L'=':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_ASSIGN;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;						
			}

			case L'*':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_SIGN_MULTIPLE;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;						
			}

			case L'/':
			{
				case_flags = PLIM_L_CASE_FLAGS_MATH;
				result = PLIM_L_SYMBOL_MATH_SIGN_DIVIDE;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;						
			}

			/* End of math symbols */
			
			/* Literal symbols */
			
			case L'"':
			{
				case_flags = PLIM_L_CASE_FLAGS_LITERALS;
				result = PLIM_L_SYMBOL_LITERAL_DQUOTE;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;						
			}

			case L'\'':
			{
				case_flags = PLIM_L_CASE_FLAGS_LITERALS;
				result = PLIM_L_SYMBOL_LITERAL_QUOTE;
				
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);
				
				break;
			}
			/* End of literal symbols */
			
			default:
			{
				ScriptShiftPos(newPos);
				ScriptShiftPos(&i);

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

int cPlimLexer::ScriptFloatSyntaxErrors(char c, int* flags) {
	if (*flags & PLIM_L_DIGIT_FLOAT)
	{
		if ( IsDot(c) )
		{
			*flags |= PLIM_L_DIGIT_FLOAT_TOO_MANY_DOTS;
			return 1;
		}
	}

	return 0;
}

int cPlimLexer::ScriptCompareSymbol(char c, char x) {
	return (c == x);
}

int cPlimLexer::ScriptCheckFlags(int flags, int source) {
	return (source & flags);
}

/* Private */
int cPlimLexer::IsSymbol(char c)
{
	/* Include control chars */
	return (ispunct(c) && !(c == '_')) || (c < 32);
}

int cPlimLexer::IsODigit(char c)
{
	return (c >= '0' && c < '8');
}

int cPlimLexer::IsNonODigit(char c)
{
	return (c > '7' && c <= '9');
}

int cPlimLexer::IsXDigit(char c)
{
	return isxdigit(c);
}

int cPlimLexer::IsDigit(char c)
{
	return isdigit(c);
}

int cPlimLexer::IsNonZeroDigit(char c)
{
	return IsDigit(c) && !IsZeroDigit(c);
}

int cPlimLexer::IsZeroDigit(char c)
{
	return (c == '0');
}

int cPlimLexer::IsHexPrefix(char c)
{
	return (c == 'x' || c == 'X');
}

int cPlimLexer::IsFSuffix(char c)
{
	return (c == 'F' || c == 'f');
}

int cPlimLexer::IsLSuffix(char c)
{
	return (c == 'L' || c == 'l');
}

int cPlimLexer::IsUSuffix(char c)
{
	return (c == 'U' || c == 'u');
}

int cPlimLexer::IsIdentifier(char c)
{
	return (isalpha(c) || (c == '_'));
}

int cPlimLexer::IsBlank(char c)
{
	return (c == 0x20 || c == '\t' || IsCRLF(c));
}

int cPlimLexer::IsCRLF(char c)
{
	return (c == L'\r' || c == L'\n');
}

int cPlimLexer::IsEscape(char c)
{
	return (c == L'\\');
}

int cPlimLexer::IsExponent(char c)
{
	return (c == L'e' || c == L'E');
}

int cPlimLexer::IsSign(char c)
{
	return (c == L'-' || c == L'+');
}

int cPlimLexer::IsDot(char c)
{
	return (c == L'.');
}

};
