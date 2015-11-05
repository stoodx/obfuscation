#include "stdafx.h"
#include "../Setting.h"
#include "Setting2.h"



CSetting2::CSetting2(CSetting* setting)
{
	init();

	m_setting = setting;
	m_setting->add(m_str4.GetBuffer(), ENCRYPT(L"xcnwg6"));
	m_setting->add(m_str5.GetBuffer(), ENCRYPT(L"xcnwg7"));
	m_setting->add(m_str6.GetBuffer(), ENCRYPT(L"xcnwg8"));
}


CSetting2::~CSetting2(void)
{
}


wchar_t* CSetting2::get(wchar_t* strKey)
{

	return m_setting->getMap(strKey);
}