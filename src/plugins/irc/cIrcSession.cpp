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

#include "cIrcSession.h"
#include "cIrcUser.h"

namespace NSInternalPluginIRC {

cIrcSession::cIrcSession(cIrc* irc, const char* sessionName)
:	cReactorSession( irc, sessionName ) {
	m_protoParser = new cIrcProto( this );
}

cIrcSession::~cIrcSession(void) {
	if (m_protoParser)	delete m_protoParser;
}

void cIrcSession::RegisterReactorInstance(cReactor* reactor) {
	cReactorSession::RegisterReactorInstance(reactor);
	
	if (reactor) {
		cString s("Signals connected!");
		reactor->OnSentMessage.connect( sigc::mem_fun( this, &cIrcSession::OnInputText ) );
		//reactor->OnDebugMsg( this, &s );
	}
}

int cIrcSession::Connect(void) {
	int on = 1;
	int s;
	struct sockaddr_in sin;

	SetSocket( socket( AF_INET, SOCK_STREAM, 0 ) );

	if ( GetSocket() == -1 )	return -1;

	memset(&sin, 0, sizeof(struct sockaddr_in));

	sin.sin_addr.s_addr = inet_addr( GetIp() );
	sin.sin_family = AF_INET;
	sin.sin_port = htons( GetPortNumber() );

	s = connect( GetSocket(), (struct sockaddr*) &sin, sizeof( sin ) );
	
	if (s == -1) SetSocket( -1 );

	ioctl( GetSocket(), FIONBIO, &on );

	m_myUser = new cIrcUser( NULL );
	m_myUser->SetUserName( "PlimClient" );
	
	send( GetSocket(), "NICK PlimClient\n\r", strlen("NICK PlimClient\n\r"), 0);
	send( GetSocket(), "USER plim irc plugin huhh :Yes i`m plim!\n\r", strlen("USER plim irc plugin huhh :Yes i`m plim!\n\r"), 0);

	return s;
}

int cIrcSession::Disconnect(void) {	
	int ret = close( GetSocket() );
	
	SetSocket( -1 );
	
	return ret;
}

int cIrcSession::Fission(fd_set *rfds, fd_set *wfds, fd_set *efds) {
	if ( GetSocket() == -1 )
		return -1;

	if (rfds) {
		FD_SET( GetSocket(), rfds );
	}

	if (wfds) {
		FD_SET( GetSocket(), wfds );
	}
	
	return 0;
}

int cIrcSession::Release(fd_set *rfds, fd_set *wfds, fd_set *efds) {
	cReactor* reactor;
	cString *string;
	char* buffer;
	ssize_t ret, l, p;
	
	if ( GetSocket() == -1 )	return -1;

	reactor = GetNuclearReactor();

	if (FD_ISSET( GetSocket(), rfds ) ) {
		/* We can read */
		ret = -1;
		l = 4096;
		p = 0;
		buffer = NULL;

		do {
			buffer = (char*) realloc( buffer, l );
	
			ret = recv( GetSocket(), &buffer[p], l - p - 1, 0 );
			
			if (ret == -1)
				break;
			else
			if ( ret > 0 ) {
				p += ret;
				l *= 2;
			}
			else
			if ( ret == 0 ) {
				break;
			}

		}	while ( 1 );

		if (buffer) {
			buffer[p - 1] = '\0';

			if (reactor) {
				cStringList slist;

				l = 0;

				for (int i = 0; i < p; i++) {
					if ( buffer[i] == '\n' || buffer[i] == '\r') {
						slist.Queue( &buffer[l], i - l );
						i++; if ( buffer[i] == '\n' || buffer[i] == '\r') i++;
						l = i;
					}
				}

				while (cStringListItem* i = slist.Dequeue()) {
					//reactor->OnDebugMsg( this, i );
					m_protoParser->ParserInput( i );
					delete i;
				}

			}

			if (buffer)
				free( buffer );
		}
	}

	if (FD_ISSET( GetSocket(), wfds ) ) {
		/* We can write */

	}

	return 0;
}

void cIrcSession::OnInputText(cString* buffer, cAbstractUser* user, cAbstractRoom* room) {
	if ( GetSocket() == -1 )	return;

	if (!buffer->Compare("join gentoo")) {
		send( GetSocket(), "JOIN #gentoo.pl\n\r", strlen("JOIN #gentoo.pl\n\r"), 0);
		return;
	}

	if (!buffer->Compare("join worldchat")) {
		send( GetSocket(), "JOIN #worldchat\n\r", strlen("JOIN #worldchat\n\r"), 0);

	}

	if ( user && room && buffer ) {
		buffer->Cat( "\r\n" );

		cString proto( "PRIVMSG ");
		proto.Cat( room->GetRoomName() );
		proto.Cat( " :");
		proto.Cat( buffer );

		send( GetSocket(), proto.GetBuffer(), proto.GetLength(), 0 );
	}

}

void cIrcSession::SetUser(cReactorUser* user) {
	m_myUser = user;
}

cAbstractUser* cIrcSession::GetUser(void) {
	return m_myUser;
}

void cIrcSession::SetIp(const char* ip) {
	m_ip.Copy( ip );
}

const char* cIrcSession::GetIp(void) {
	return m_ip.GetBuffer();
}

void cIrcSession::SetBindIp(const char* ip) {
	m_bindIp.Copy ( ip );
}

const char* cIrcSession::GetBindIp(void) {
	return m_bindIp.GetBuffer();
}

void cIrcSession::SetPortNumber(int port) {
	m_portNumber = port;
}

int cIrcSession::GetPortNumber(void) {
	return m_portNumber;
}

};
