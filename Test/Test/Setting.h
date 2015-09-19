#pragma once
#include <map>
#include <string>


class CSetting
{
public:
	CSetting(void);
	~CSetting(void);


	wchar_t* get(wchar_t* strKey);
	void add(const std::wstring& strKey, const std::wstring& strValue)
	{
		m_map[strKey] = strValue;
	}
	
private:
	std::map<std::wstring, std::wstring> m_map;
};