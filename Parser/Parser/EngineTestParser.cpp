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

TEST_F(EngineTestParser,  findSubDirs_NoParentDirectory)
{
	CPtrArray arr;
	int nRes = m_parser.findSubDirs(arr);
	if (nRes != 0)
	{
		for (int i = 0; i < nRes; i++)
		{
			CCodeDirectories* pDirs = (CCodeDirectories*)arr.GetAt(i);
			delete pDirs;
		}
		arr.RemoveAll();
	}
	ASSERT_EQ(0, nRes); 
}

TEST_F(EngineTestParser,  findSubDirs_findAllSubDirectories)
{
	CPtrArray arr;
	CCodeDirectories* pDirs =  new CCodeDirectories;
	pDirs->m_strOriginalDir = m_parser.getCurrentPath();
	arr.Add(pDirs);
	int nRes = m_parser.findSubDirs(arr);
	if (nRes != 0)
	{
		for (int i = 0; i < nRes; i++)
		{
			pDirs = (CCodeDirectories*)arr.GetAt(i);
			delete pDirs;
		}
		arr.RemoveAll();
	}
	ASSERT_NE(0, nRes); 
}

TEST_F(EngineTestParser,  findFileByType_InvalidParameters1)
{
	bool bRes = m_parser.findFileByType(NULL, _T("*.h"));
	ASSERT_FALSE(bRes);
}

TEST_F(EngineTestParser,  findFileByType_InvalidParameters2)
{
	CCodeDirectories dir;
	bool bRes = m_parser.findFileByType(&dir, _T(""));
	ASSERT_FALSE(bRes);
}

TEST_F(EngineTestParser,  findFileByType_NoFilesByType)
{
	CCodeDirectories dir;
	dir.m_strOriginalDir = m_parser.getCurrentPath();
	bool bRes = m_parser.findFileByType(&dir, _T("*.h"));
	ASSERT_FALSE(bRes);
}

TEST_F(EngineTestParser,  findFileByType_IncorrectCurrentDirectory)
{
	CCodeDirectories dir;
	dir.m_strOriginalDir = _T("failedDir");
	bool bRes = m_parser.findFileByType(&dir, _T("*.exe"));
	ASSERT_FALSE(bRes);
}

TEST_F(EngineTestParser,  findFileByType_FilesFound)
{
	CCodeDirectories dir;
	//TCHAR strCurrentPath[MAX_PATH * sizeof(TCHAR)] = {0};
	//GetCurrentDirectory(MAX_PATH * sizeof(TCHAR), strCurrentPath);
	dir.m_strOriginalDir = m_parser.getCurrentPath();
	bool bRes = m_parser.findFileByType(&dir, _T("*.obj"));
	ASSERT_TRUE(bRes);
}

TEST_F(EngineTestParser,  findCodesFiles_NoParentDirectory)
{
	CPtrArray arr;
	int nRes = m_parser.findCodesFiles(arr);
	if (nRes != 0)
	{
		for (int i = 0; i < nRes; i++)
		{
			CCodeDirectories* pDirs = (CCodeDirectories*)arr.GetAt(i);
			delete pDirs;
		}
		arr.RemoveAll();
	}
	ASSERT_EQ(0, nRes); 
}


TEST_F(EngineTestParser,  findCodesFiles_NoCodesFiles)
{
	CPtrArray arr;
	CCodeDirectories* pDirs =  new CCodeDirectories;
	pDirs->m_strOriginalDir = m_parser.getCurrentPath();
	arr.Add(pDirs);
	int nRes = m_parser.findSubDirs(arr);
	EXPECT_NE(0, nRes);
	nRes = m_parser.findCodesFiles(arr);
	if (nRes != 0)
	{
		for (int i = 0; i < nRes; i++)
		{
			CCodeDirectories* pDirs = (CCodeDirectories*)arr.GetAt(i);
			delete pDirs;
		}
		arr.RemoveAll();
	}
	ASSERT_EQ(0, nRes); 
}


TEST_F(EngineTestParser,  restore_NoPathDirectory)
{
	std::wstring strPath(_T(""));
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.restore(); 
	ASSERT_EQ(1, nRes);
}

#endif //_GOOGLE_TEST
