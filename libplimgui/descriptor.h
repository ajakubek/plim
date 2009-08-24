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

#ifndef __PLIM_TIMER_H__
#define __PLIM_TIMER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include "treenodes.h"

using namespace NSTree;

/* Blah didnt notice */
namespace NSApplication {

class cDescriptor: public cTreeNode {
public:
	cDescriptor(cTreeNodes* nodes, void* data);
	virtual ~cDescriptor(void);
	virtual void SetupDescriptor(void);
	/*	the cableguy should call from inside his derivered class the IsDescriptor{Read|Write|Error} for some transmit */
	virtual void IssetDescriptor(void);
	/*	abstract method that is used in the {Setup,Select,Isset}Descriptor methods */
	virtual int GetDescriptor(void) = 0;
	/*	Get the data from descriptor
		@return void ptr
	*/
	void* GetData(void) { return m_data; };
	cDescriptor* GetNextNode(void) { return (cDescriptor*) cTreeNode::GetNextNode(); };
	cDescriptor* GetPrevNode(void) { return (cDescriptor*) cTreeNode::GetPrevNode(); };
private:
	void* m_data;
};

};

#endif
