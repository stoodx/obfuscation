#include "stdafx.h"
#include "../Setting.h"
#include "Setting2.h"

#define ENCRYPT(x) x


CSetting2::CSetting2(CSetting* setting)
{
	m_setting = setting;
	m_setting->add(ENCRYPT(L"key4"), ENCRYPT(L"value4"));
	m_setting->add(ENCRYPT(L"key5"), ENCRYPT(L"value5"));
	m_setting->add(ENCRYPT(L"key6"), ENCRYPT(L"value6"));
}


CSetting2::~CSetting2(void)
{
}


wchar_t* CSetting2::get(wchar_t* strKey)
{

	return m_setting->get(strKey);
}