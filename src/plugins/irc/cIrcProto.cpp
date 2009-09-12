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
#include "cIrcRoom.h"
#include "cIrcUser.h"

namespace NSInternalPluginIRC {

cIrcProto::cIrcProto(cIrcSession* session)
:	m_session(session) {
	
}

cIrcProto::~cIrcProto(void) {

}

void cIrcProto::ParserInput(cString* string) {
	if ( !string || !string->GetLength() ) {
		return;
	}

	lex.Refresh( string->GetBuffer() );
	cPlimToken* token, *temp;

	token = (cPlimToken* ) lex.GetFirstNode();

	while ( token ) {
/*
16:18:44 -!- Session (irc:proto) recieved :crs!~crs@86.150.237.123 QUIT :Ping timeout                        │
16:18:49 -!- Session (irc:proto) recieved :lublin.irc.pl 421 PlimClient v :Unknown command
*/
		if ( token->GetTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS ) {
			
			if ( token->GetTokenExCase() == PLIM_L_SYMBOL_COLON ) {
				/* Ok start identify what type of msg we got. */
				temp = (cPlimToken*) lex.GetNext( token );

				switch ( ParserIdentify( temp ) ) {

					case 1: { /* User message here. */
						ParserServerUserMsg( temp );
						break;
					}

					case 0:  /* server message here. */
					default: {
						ParserServerMsg( temp );
						break;
					}
				}

				return;
			}


		}

		token = (cPlimToken* ) lex.GetNext( token );
	}
	
}

int cIrcProto::ParserIdentify(cPlimToken* token) {
	if (!token)	return -1;

	while ( token ) {
		
		if ( !token->Compare("!") || !token->Compare("@") )
		{
			return 1;
		}

		if ( token->GetTokenCase() == PLIM_L_BLANK )
			break;

		token = (cPlimToken*) lex.GetNext(token);
	}

	return 0;
}

void cIrcProto::ParserServerMsg(cPlimToken* token) {

}
// 16:15:59 -!- Session (irc:proto) recieved :PlimClient!~plim@coffeeredev.org JOIN :#gentoo.pl
cPlimToken* cIrcProto::ParserServerUserMsg(cPlimToken* token) {
	cString* user, *host, *nick, *type;

	user = host = nick = type = NULL;

	while ( token ) {
		nick = new cString();

		while ( token ) {
			if ( !token->Compare("!")) {
				token = (cPlimToken*) lex.GetNext( token );
				user = new cString();

				while ( token ) {
					if (!token->Compare("@")) {
						token = (cPlimToken*) lex.GetNext( token );
						host = new cString();

						while ( token ) {
							if ( token->GetTokenCase() == PLIM_L_BLANK ) {

								token = (cPlimToken*) lex.GetNext( token );

								type = new cString( token );

								token = (cPlimToken*) lex.GetNext( token );

								return AfterParserServerUserMsg( token, nick, user, host, type );
							}

							host->Cat( token );

							token = (cPlimToken*) lex.GetNext( token );
						}
					}

					user->Cat( token );

					token = (cPlimToken*) lex.GetNext( token );
				}
			}

			nick->Cat( token );

			token = (cPlimToken*) lex.GetNext( token );
		}

		token = (cPlimToken*) lex.GetNext(token);
	}

	return NULL;
}

cPlimToken* cIrcProto::AfterParserServerUserMsg(cPlimToken* token, cString* nick, cString* user, cString* host, cString* type) {
	cString* channel;
	cString* msg;

	if ( !token || !nick || !user || !host || !type ) 	return NULL;

	if (!type->Compare("PART")) {
// PlimClient!~plim@coffeeredev.org PART #worldchat :
		token = (cPlimToken*) lex.GetNext( token );

		channel = new cString();

		while ( token ) {

			if (token->GetTokenCase() == PLIM_L_BLANK)
				break;

			channel->Cat( token );

			token = (cPlimToken*) lex.GetNext( token );
		}

		ServerPartMsg( nick, user, host, channel );
	}
	else
	if (!type->Compare("JOIN")) {
		token = (cPlimToken*) lex.GetNext( token );

//		16:15:59 :PlimClient!~plim@coffeeredev.org JOIN :#gentoo.pl            │
		while ( token ) {
			if ( token->GetTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS ) {
				if ( token->GetTokenExCase() == PLIM_L_SYMBOL_COLON ) {

					token = (cPlimToken*) lex.GetNext( token );

					channel = new cString();

					while ( token ) {
						channel->Cat( token );
						token = (cPlimToken*) lex.GetNext( token );
					}

					ServerJoinMsg( nick, user, host, channel );
				}
			}
			token = (cPlimToken*) lex.GetNext( token );
		}
	}
	else
	if (!type->Compare("PRIVMSG")) {
//pl PRIVMSG #gentoo.pl :Pratchet
		channel = new cString();
		token = (cPlimToken*) lex.GetNext( token );

		while ( token ) {

			if ( token->GetTokenCase() == PLIM_L_BLANK )
				break;

			channel->Cat( token );

			token = (cPlimToken*) lex.GetNext( token );
		}

		while ( token ) {
			if ( token->GetTokenCase() == PLIM_L_CASE_FLAGS_SYMBOLS ) {
				if ( token->GetTokenExCase() == PLIM_L_SYMBOL_COLON ) {

					token = (cPlimToken*) lex.GetNext( token );

					msg = new cString();

					while ( token ) {
						msg->Cat( token );
						token = (cPlimToken*) lex.GetNext( token );
					}

					ServerUserMsg( nick, user, host, channel, msg );
				}
			}
			token = (cPlimToken*) lex.GetNext( token );
		}
	}

	return NULL;
}

void cIrcProto::ServerCommand(cString* server, int command) {

}

void cIrcProto::ServerUserMsg(cString* nick, cString* user, cString* host, cString* channel, cString* msg) {
	cReactor* reactor = m_session->GetNuclearReactor();
	cIrcRoom* croom;
	cIrcUser* cuser;

	if (reactor) {

		croom = (cIrcRoom*) m_session->GetRoom( channel->GetBuffer() );

		if ( croom ) {
			cuser = (cIrcUser*) croom->GetUser( nick->GetBuffer() );

			if (!cuser) {
				cuser = new cIrcUser( croom );
				cuser->SetUserName( nick->GetBuffer() );
			}

			reactor->OnUserMessage( croom, cuser, msg );
		}

	}

	delete nick;
	delete user;
	delete host;
	delete channel;
	delete msg;
}

void cIrcProto::ServerJoinMsg(cString* nick, cString* user, cString* host, cString* channel) {
	cReactor* reactor = m_session->GetNuclearReactor();
	cIrcRoom* croom;
	cIrcUser* cuser;

	if (reactor) {

		if ( !nick->Compare( m_session->GetUser()->GetUserName() ) ) {

			croom = new cIrcRoom( m_session );
			croom->SetRoomName( channel->GetBuffer() );

			reactor->OnCreateRoom( croom );
		}
		else {
			croom = (cIrcRoom*) m_session->GetRoom( channel->GetBuffer() );

			if ( croom ) {
				cuser = new cIrcUser( croom );
				cuser->SetUserName( nick->GetBuffer() );

				reactor->OnUserJoin( croom, cuser );
			}

		}

	}

	delete nick;
	delete user;
	delete host;
	delete channel;
}

void cIrcProto::ServerPartMsg(cString* nick, cString* user, cString* host, cString* channel) {
	cReactor* reactor = m_session->GetNuclearReactor();
	cIrcRoom* croom;
	cIrcUser* cuser;

	if (reactor) {

		if ( !nick->Compare( m_session->GetUser()->GetUserName() ) ) {
			croom = (cIrcRoom*) m_session->GetRoom( channel->GetBuffer() );

			reactor->OnDestroyRoom( croom );

			delete croom;
		}
		else {
			croom = (cIrcRoom*) m_session->GetRoom( channel->GetBuffer() );

			if ( croom ) {
				cuser = (cIrcUser /* plum */ * /* plum */) croom->GetUser( nick->GetBuffer() );

				if (cuser) {
					reactor->OnUserPart( croom, cuser );
				}

			}

		}
	}

	delete nick;
	delete user;
	delete host;
	delete channel;
}

};






