#ifdef _GOOGLE_TEST

#include "stdafx.h"
#include "gtest.h"
#include "ParserEndgine.h"

class EngineTestParser :
	public ::testing::Test
{
protected:
	ParserEndgine m_parser;
	std::wstring m_strRootPath;
	void SetUp()
	{
		m_strRootPath = m_parser.getCurrentPath();
		if (m_strRootPath.empty())
			return;
		int nPos = m_strRootPath.find(_T("Parser"));
		if ( nPos == std::wstring::npos)
			return;
		m_strRootPath = m_strRootPath.substr(0, nPos);
	}
};

TEST_F(EngineTestParser, BasicTest)
{
	std::wstring strPath(m_parser.getCurrentPath());
	ASSERT_FALSE(strPath.empty());
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
	dir.m_strOriginalDir = m_strRootPath.c_str();
	dir.m_strOriginalDir += _T("Test\\Test");
	bool bRes = m_parser.findFileByType(&dir, _T("*.h"));
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
	pDirs->m_strOriginalDir = m_strRootPath.c_str();
	arr.Add(pDirs);
	int nRes = m_parser.findSubDirs(arr);
	EXPECT_NE(0, nRes);
	bool bRes = m_parser.findCodesFiles(arr);
	nRes = arr.GetSize();
	if (nRes != 0)
	{
		for (int i = 0; i < nRes; i++)
		{
			CCodeDirectories* pDirs = (CCodeDirectories*)arr.GetAt(i);
			delete pDirs;
		}
		arr.RemoveAll();
	}
	ASSERT_TRUE(bRes); 
}

TEST_F(EngineTestParser, getFileError_genericException)
{
	CFileException e; 
	int nEceptions[] = {
		CFileException::none,
		CFileException::genericException,  
		CFileException::fileNotFound,     
		CFileException::badPath,       
		CFileException::tooManyOpenFiles,           
		CFileException::accessDenied,              
		CFileException::invalidFile,                 
		CFileException::removeCurrentDir,                  
		CFileException::directoryFull,                    
		CFileException::badSeek,                       
		CFileException::hardIO,                          
		CFileException::sharingViolation,                             
		CFileException::lockViolation,                                
		CFileException::diskFull,                                   
		CFileException::endOfFile,
		-1,
		-2
	};
	int i = 0;
	int nRes = 0;
	while (nEceptions[i] != -2)
	{
		e.m_cause = nEceptions[i];
		nRes = m_parser.getFileError(&e);
		EXPECT_EQ(nRes, nEceptions[i]);
		i++;
	}
	ASSERT_EQ(nRes, nRes);
}

TEST_F(EngineTestParser,  createTempDir)
{
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
