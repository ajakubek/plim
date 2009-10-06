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

#ifndef __STDAFX_REACTOR_H__
#define __STDAFX_REACTOR_H__

#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#if defined(_WIN32) || defined(_WIN64)
#	include <winsock2.h>
#else
#	include <unistd.h>
#	include <termios.h>
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#	include <sys/types.h>
#	include <sys/time.h>
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>
#	include <sys/stat.h>
#endif

#include <iostream>
#include <string>
#include <map>

#include <boost/thread.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/bimap.hpp>

#endif
