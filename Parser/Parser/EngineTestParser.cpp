#ifdef _GOOGLE_TEST

#include "stdafx.h"
#include "gtest.h"
#include "ParserEndgine.h"

class EngineTestParser :
	public ::testing::Test
{
protected:
	ParserEndgine m_parser;
};

TEST_F(EngineTestParser, BasicTest)
{
	std::wstring strPath(m_parser.getCurrentPath());
	ASSERT_FALSE(strPath.empty());
}

TEST_F(EngineTestParser,  obfuscate_NoPathDirectory)
{
	std::wstring strPath(_T(""));
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.obfuscate(); 
	ASSERT_EQ(1, nRes);
}

TEST_F(EngineTestParser,  restore_NoPathDirectory)
{
	std::wstring strPath(_T(""));
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.restore(); 
	ASSERT_EQ(1, nRes);
}


#endif //_GOOGLE_TEST
