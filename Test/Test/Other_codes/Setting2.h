#pragma once

#define ENCRYPT(x) x

class CSetting;

class CSetting2
{
public:
	CSetting2(CSetting* m_setting);
	~CSetting2(void);

	wchar_t* get(wchar_t* strKey);

	void init()
	{
		m_str4 = ENCRYPT(L"mg{6");
		m_str5 = ENCRYPT(L"mg{7");
		m_str6 = ENCRYPT(L"mg{8");
	}

private:
	CSetting* m_setting;

	CString m_str4;
	CString m_str5;
	CString m_str6;
};