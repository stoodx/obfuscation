#include "stdafx.h"
#include "Setting.h"

#include <map>
#include <string>

CSetting::CSetting(void)
{
}


CSetting::~CSetting(void)
{
}


wchar_t* CSetting::get(wchar_t* strKey)
{
	return (wchar_t*)m_map.at(strKey).c_str();
}