#include "descriptor.h"

namespace NSApplication {

cDescriptor::cDescriptor(const int useRead, const int useWrite, const int useError, void* data) {
	if (useRead) {
		m_readDesc = (fd_set*) malloc(sizeof(fd_set));
	}
	if (useWrite) {
		m_writeDesc = (fd_set*) malloc(sizeof(fd_set));
	}
	if (useError) {
		m_errorDesc = (fd_set*) malloc(sizeof(fd_set));
	}
}

cDescriptor::~cDescriptor(void) {
	if (m_readDesc) {
		free(m_readDesc);
	}
	if (m_writeDesc) {
		free(m_writeDesc);
	}
	if (m_errorDesc) {
		free(m_errorDesc);
	}
}

void cDescriptor::SetupDescriptor(void) {
	if (m_readDesc) {
		FD_ZERO(m_readDesc);
		FD_SET(GetDescriptor(), m_readDesc);
	}
	if (m_writeDesc) {
		FD_ZERO(m_writeDesc);
		FD_SET(GetDescriptor(), m_writeDesc);
	}
	if (m_errorDesc) {
		FD_ZERO(m_errorDesc);
		FD_SET(GetDescriptor(), m_errorDesc);
	}
}

void cDescriptor::SelectDescriptor(void) {
	int selectRet;
	struct timeval val = { 0L, 0L };

	selectRet = select(GetDescriptor() + 1, m_readDesc, m_writeDesc, m_errorDesc, &val);

	if (selectRet == -1) {
		/* Indicate a error in timer about a select, let handle it by the cableguy */
		m_selectError = 1;
		return;
	}

}

void cDescriptor::IssetDescriptor(void) {
	/* Dummy */
}

};