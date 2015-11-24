#include "stdafx.h"
#include "Setting.h"

#include <map>
#include <string>

#include "AES.h"

CSetting::CSetting(void)
{
	m_nIndexStr = 0;
}


CSetting::~CSetting(void)
{
}

#define DECRYP_ENABLE

wchar_t* CSetting::get( wchar_t* strKey) 
{
#ifdef DECRYP_ENABLE
	CAES::decryptString(strKey, m_str[m_nIndexStr]);
#else
	m_str[m_nIndexStr] = strKey;
#endif
	int nIndex = m_nIndexStr;
	if (m_nIndexStr == MAX_STRING_NUMBER)
		m_nIndexStr = 0;
	else
		m_nIndexStr++;
	return (wchar_t*)m_str[nIndex].c_str();
}

wchar_t* CSetting::getMap(wchar_t* strKey)
{
	std::wstring str = m_map.at(strKey).c_str();
	return get((wchar_t*)str.c_str());
}