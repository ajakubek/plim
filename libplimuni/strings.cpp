#include "strings.hpp"

namespace plimuni {

cString::cString(void)
:	m_stringBuffer(NULL),
	m_length(0),
	m_allocated(0)
{
	
}

cString::cString(cString* string)
:	m_stringBuffer(NULL),
	m_length(0),
	m_allocated(0)
{
	copy(string);
}

cString::cString(const char* string)
:	m_stringBuffer(NULL),
	m_length(0),
	m_allocated(0)
{
	copy(string);
}

cString::~cString(void)
{

}

/* yes, we know that sizeof(char) is 1 byte, in case we will change it to ucs */
int cString::copy(const char* source)
{ 
	int sourceLength;
	
	if (!source)
		return 0;

	clean();

	sourceLength = ::strlen(source) + 1;

	if (sourceLength >= m_length)
	{
		m_stringBuffer = (char*) realloc(m_stringBuffer, sourceLength * sizeof(char));
		m_length = sourceLength - 1;
	}

	memcpy(m_stringBuffer, source, sourceLength * sizeof(char));

	return m_length;
}

int cString::copy(const char* source, int len) {
	/* Make a small copy */
	if (!source) return 0;

	char* x = (char*) malloc( (len + 1) * sizeof(char));	
	memcpy( x, source, (len + 1) * sizeof(char) );
	x[len] = '\0';

	if (x) {
		copy(x);
	}

	return m_length;
}

int cString::copy(const char* source, va_list args) {
	int len;
	char* buffer;

	if (!source)	return 0;

	len = strlen(source) * 100;
	buffer = (char*) malloc(len);

	vsprintf( buffer, source, args );

	len = copy( buffer );
	
	free( buffer );

	return len;
}

int cString::cat(const char* source)
{
	int newLength;
	int srcLength;

	if (!source)
		return 0;

	if (!m_stringBuffer) {
		return copy(source);
	}

	srcLength = strlen(source);
	newLength = m_length + srcLength;

	if (!srcLength)
		return 0;

	m_stringBuffer = (char*) realloc(m_stringBuffer, (newLength + 1) * sizeof(char));
	
	if (m_stringBuffer)
	{
		memcpy(m_stringBuffer + m_length, source, srcLength * sizeof(char));
		m_stringBuffer[m_length + srcLength] = '\0';
		m_length += srcLength;
	}
	
	return m_length;
}

int cString::remove(int index, int range, char* out)
{
	int newLength = m_length - index;
	
	if (!m_stringBuffer)
		return 0;

	if (m_stringBuffer)
	{
		memmove(m_stringBuffer + index, m_stringBuffer + index + range, (newLength + 1) * sizeof(char));
		m_length -= range;
	}

	return m_length;
}

int cString::insert(int index, const char source)
{
	int newLength = m_length + 1;
	
	m_stringBuffer = (char*) realloc(m_stringBuffer, (newLength + 1) * sizeof(char));
	
	if (m_stringBuffer)
	{
		memmove(m_stringBuffer + index + 1, m_stringBuffer + index, (m_length - index + 1) * sizeof(char));
		m_stringBuffer[index] = source;
		m_stringBuffer[newLength] = L'\0';
		m_length += 1;
	}

	return 0;
}

int cString::insert(int index, const char* source)
{
	int newLength = m_length;
	int srcLength;
	
	if (source)
	{
		srcLength = strlen(source);
		newLength += srcLength;
		
		m_stringBuffer = (char*) realloc(m_stringBuffer, (newLength + 1) * sizeof(char));
		
		if (m_stringBuffer)
		{
			memmove(m_stringBuffer + index + srcLength, m_stringBuffer + index, (m_length - index + 1) * sizeof(char));
			memcpy(m_stringBuffer + index, source, srcLength * sizeof(char));
			m_length += srcLength;
		}
	
	}
	
	return 0;
}

int cString::clean(void)
{
	free(m_stringBuffer);
	m_stringBuffer = NULL;
	m_length = 0;
	m_allocated = 0;
	return 0;
}

const char* cString::getBuffer(void)
{
	return (const char*) m_stringBuffer;
}

const char cString::getChar(int index)
{
	return (const char) m_stringBuffer[index];
}

/* Add utf-8 len routines, pending */
int cString::getLength(void)
{
	if (!m_stringBuffer)
		return 0;

	return m_length;
}

const char* cString::getBuffer(int index, int* range)
{ 
	if (index <= m_length)
	{
		if (range)	*range = m_length - index;
		return (const char*) m_stringBuffer + index;
	}
	else
		return NULL;
}


};
