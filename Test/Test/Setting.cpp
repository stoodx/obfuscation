#include "stdafx.h"
#include "Setting.h"

#include <map>
#include <string>

CSetting::CSetting(void)
{
	m_nIndexStr = 0;
}


CSetting::~CSetting(void)
{
}

wchar_t* CSetting::get( wchar_t* strKey) 
{
	//char-2
	std::string strA =  ws2s(strKey);
	std::string  strDecA("");
	int nLen = strA.size();
	for(int i = 0; i < nLen; i++)
	{
		char ch = strA.at(i);
		if (ch >= '\"')
			ch -= 2;
		strDecA += ch;
	}
	m_str[m_nIndexStr] = s2ws(strDecA);
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