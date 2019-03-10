#include "Initsock.h"

Initsock * Initsock::m_pInitsock = nullptr;


Initsock * Initsock::GetInitsock()
{
	if (!m_pInitsock)
		m_pInitsock = new Initsock;
	return m_pInitsock;
}

void Initsock::Release()
{
	if (m_pInitsock)
		delete m_pInitsock;
	m_pInitsock = nullptr;
}
