/***************************************************************************
 *   Copyright (C) 2009 by Lukasz Marcin Borzecki                          *
 *   lukasz.borzecki@gmail.com                                             *
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

#ifndef __PLIM_STRING_H__
#define __PLIM_STRING_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace NSString
{
/* TODO: Add utf-8 support */
/* TODO: Add intelli-reallocation of buffer */
/* TODO: Some operators */
class cString
{
public:
	cString(void);
	virtual ~cString(void);
	int Copy(const char* source);
	int Copy(const char* source, int len);
	int Copy(cString* string) { return Copy(string->GetBuffer()); };
	int Cat(const char* source);
	int Cat(cString* string) { return Cat(string->GetBuffer()); };
	int Delete(int index, int range, char* out);
	int Insert(int index, const char* source);
	int Insert(int index, char source);
	int Clean(void);
	const char* GetBuffer(void);
	const char GetChar(int index);
	int GetLength(void);
	const char* GetBuffer(int index, int* range);

protected:
	
private:
	int m_length;
	int m_allocated;
	char* m_stringBuffer;
};
 
};

#endif
