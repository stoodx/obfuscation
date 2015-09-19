#pragma once

class CSetting;

class CSetting2
{
public:
	CSetting2(CSetting* m_setting);
	~CSetting2(void);

	wchar_t* get(wchar_t* strKey);

private:
	CSetting* m_setting;
};