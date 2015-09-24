#pragma once
#include <map>
#include <string>
#include <codecvt>


#define MAX_STRING_NUMBER 9 

class CSetting
{
public:
	CSetting(void);
	~CSetting(void);


	wchar_t* get(wchar_t* strKey);
	wchar_t* getMap(wchar_t* strKey);
	void add(const std::wstring& strKey, const std::wstring& strValue)
	{
		m_map[strKey] = strValue;
	}

private:
	std::map<std::wstring, std::wstring> m_map;
	std::wstring m_str[MAX_STRING_NUMBER + 1];
	int m_nIndexStr;

	inline std::wstring s2ws(const std::string& str)
	{
		typedef std::codecvt_utf16<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	inline std::string ws2s(const std::wstring& wstr)
	{
		typedef std::codecvt_utf16<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

};