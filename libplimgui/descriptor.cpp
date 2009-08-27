#include "descriptor.h"

namespace NSApplication {

cDescriptor::cDescriptor(cTreeNodes* nodes, void* data)
:	cTreeNode(nodes, NULL, NULL),
	m_data(data) {
}

cDescriptor::~cDescriptor(void) {
}

void cDescriptor::SetupDescriptor(void) {
 
}

void cDescriptor::IssetDescriptor(void) {
	/* Dummy */
}

};