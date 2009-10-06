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

#include "stdafx.hpp"

namespace plimuni 
{

/* TODO: Add utf-8 support */
/* TODO: Add intelli-reallocation of buffer */
/* TODO: Some operators */
class cString
{
public:
	cString(void);
	cString(cString* string);
	cString(const char* string);
	virtual ~cString(void);
	
	int copy(const char* source);
	int copy(const char* source, int len);
	
	int copy(cString* string) 
	{ 
		return copy(string->getBuffer()); 
	};
	
	int copy(const char* source, va_list args);
	
	int cat(const char* source);
	int cat(cString* string) 
	{ 
		return cat( string->getBuffer() ); 
	};
	
	int remove(int index, int range, char* out);
	int insert(int index, const char* source);
	int insert(int index, char source);
	int clean(void);
	
	int compare(const char* buffer) 
	{ 
		return strcmp(m_stringBuffer, buffer); 
	};
	
	int compare(cString* buffer) 
	{ 
		return strcmp(m_stringBuffer, buffer->getBuffer()); 
	};
	
	const char* getBuffer(void);
	const char* getBuffer(int index, int* range);
	const char getChar(int index);
	
	int getLength(void);

protected:
	
private:
	int m_length;
	int m_allocated;
	char* m_stringBuffer;
};


};
#endif
