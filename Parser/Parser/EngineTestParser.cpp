#ifdef _GOOGLE_TEST

#include "stdafx.h"
#include "gtest.h"
#include "ParserEndgine.h"

std::wstring g_strCurrentDir;
extern const TCHAR* g_strNameTempDir;

#define TEST_DELAY 500

class EngineTestParser :
	public ::testing::Test
{
protected:
	ParserEndgine m_parser;
	std::wstring m_strRootPath;
	void SetUp()
	{
		if (g_strCurrentDir.empty())
			g_strCurrentDir = m_parser.getCurrentPath();
		m_strRootPath = m_parser.getCurrentPath();
		if (m_strRootPath.empty())
			return;
		int nPos = m_strRootPath.find(_T("Parser"));
		if ( nPos == std::wstring::npos)
			return;
		m_strRootPath = m_strRootPath.substr(0, nPos);
	}

	bool isFileExist(const wchar_t* strName)
	{
		if (!strName)
		{
			SetLastError(ERROR_INVALID_DATA);
			return false;
		}
		return ::GetFileAttributes(strName) != DWORD(-1);
	}
};

TEST_F(EngineTestParser, BasicTest)
{
	std::wstring strPath(m_parser.getCurrentPath());
	ASSERT_FALSE(strPath.empty());
}

TEST_F(EngineTestParser,  findSubDirs_noParentDirectory)
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
	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_NE(0, nRes); 
}

TEST_F(EngineTestParser,  findFileByType_setInvalidParameters1)
{
	bool bRes = m_parser.findFileByType(NULL, _T("*.h"));
	ASSERT_FALSE(bRes);
}

TEST_F(EngineTestParser,  findFileByType_setInvalidParameters2)
{
	CCodeDirectories dir;
	bool bRes = m_parser.findFileByType(&dir, _T(""));
	ASSERT_FALSE(bRes);
}

TEST_F(EngineTestParser,  findFileByType_noFilesByType)
{
	CCodeDirectories dir;
	dir.m_strOriginalDir = m_strRootPath.c_str();
	bool bRes = m_parser.findFileByType(&dir, _T("*.h"));
	ASSERT_FALSE(bRes);
}

TEST_F(EngineTestParser,  findFileByType_setIncorrectCurrentDirectory)
{
	CCodeDirectories dir;
	dir.m_strOriginalDir = _T("failedDir");
	bool bRes = m_parser.findFileByType(&dir, _T("*.exe"));
	ASSERT_FALSE(bRes);
}

TEST_F(EngineTestParser,  findFileByType_checkFilesFound)
{
	CCodeDirectories dir;
	dir.m_strOriginalDir = m_strRootPath.c_str();
	dir.m_strOriginalDir += _T("Test\\Test");
	bool bRes = m_parser.findFileByType(&dir, _T("*.h"));
	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(bRes);
}

TEST_F(EngineTestParser,  findCodesFiles_noParentDirectory)
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

TEST_F(EngineTestParser,  findCodesFiles_noCodesFiles)
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
	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_FALSE(bRes); 
}

TEST_F(EngineTestParser,  findCodesFiles_haveCodesFiles)
{
	CPtrArray arr;
	CCodeDirectories* pDirs =  new CCodeDirectories;
	pDirs->m_strOriginalDir = m_strRootPath.c_str();
	pDirs->m_strOriginalDir += _T("Test\\Test\\");
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
	SetCurrentDirectory(g_strCurrentDir.c_str());
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
	CString strTempDir(m_strRootPath.c_str());
	strTempDir += _T("TempDir");
	bool bRes = m_parser.createTempDir(strTempDir);
	EXPECT_TRUE(bRes);
	SHFILEINFO shFileInfo;
	if (SHGetFileInfo(strTempDir, 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_TYPENAME) != 0)
	{
		bRes = true;
		RemoveDirectory(strTempDir);
	}
	else
	{
		bRes = false;
	}
	ASSERT_TRUE(bRes);
}

TEST_F(EngineTestParser,  encodeText_encode)
{
	std::string strEncode(m_parser.encodeText("12345678"));
	ASSERT_EQ(0, strEncode.compare("3456789:"));
}

TEST_F(EngineTestParser,  parseFile_noPath)
{
	CString strPath(_T(""));
	CString strFilename("ParserEndgine.cpp");
	bool bCreateTempDir = false;
	int nRes = m_parser.parseFile(strPath, strFilename, bCreateTempDir);
	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_EQ(-1, nRes);
}

TEST_F(EngineTestParser,  parseFile_noFileName)
{
	CString strPath(m_strRootPath.c_str());
	CString strFilename(_T(""));
	bool bCreateTempDir = false;
	int nRes = m_parser.parseFile(strPath, strFilename, bCreateTempDir);
	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_EQ(-1, nRes);
}

TEST_F(EngineTestParser,  parseFile_cannotOpenFile)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	CString strPath(strTestDir);
	CString strFilename("ParserEndgine.cpp");
	bool bCreateTempDir = false;
	int nRes = m_parser.parseFile(strPath, strFilename, bCreateTempDir);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_EQ(-1, nRes);
}

TEST_F(EngineTestParser,  parseFile_haveNullFileLength)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//create a nul file
	CString strFilePath(strTestDir);
	strFilePath += _T("ParserEndgine.cpp");
	HANDLE hFile = CreateFile(strFilePath,  GENERIC_WRITE, 0,  
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile ==  INVALID_HANDLE_VALUE)
	{
		//del work dir
		strCommand = _T("");
		strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
		ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
		ASSERT_TRUE(false);
	}
	CloseHandle(hFile);

	CString strPath(strTestDir);
	CString strFilename("ParserEndgine.cpp");
	bool bCreateTempDir = false;
	int nRes = m_parser.parseFile(strPath, strFilename, bCreateTempDir);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_EQ(0, nRes);
}

TEST_F(EngineTestParser,  parseFile_define__NO__OBFUSCATION)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//copy file with NO__OBFUSCATION
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Parser\\Parser\\ParserEndgine.cpp");
	CString strDestFile(strTestDir);
	strDestFile += _T("ParserEndgine.cpp");
	CopyFile(strSourceFile, strDestFile, TRUE);

	CString strPath(strTestDir);
	CString strFilename("ParserEndgine.cpp");
	bool bCreateTempDir = false;
	int nRes = m_parser.parseFile(strPath, strFilename, bCreateTempDir);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_EQ(0, nRes);
}

TEST_F(EngineTestParser,  parseFile_parseWithCreateTempDir)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.cpp");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.cpp");
	CopyFile(strSourceFile, strDestFile, TRUE);

	CString strPath(strTestDir);
	CString strFilename(_T("Test.cpp"));
	bool bCreateTempDir = false;
	int nRes = m_parser.parseFile(strPath, strFilename, bCreateTempDir);

	//check
	CString strPathTempDirFile = strTestDir + g_strNameTempDir;
	strPathTempDirFile += _T("Test.cpp");
	bool bRes = isFileExist(strPathTempDirFile);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(nRes && bRes);
}

TEST_F(EngineTestParser,  parseFile_notFileForParsing)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.h");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.h");
	CopyFile(strSourceFile, strDestFile, TRUE);

	CString strPath(strTestDir);
	CString strFilename(_T("Test.h"));
	bool bCreateTempDir = false;
	int nRes = m_parser.parseFile(strPath, strFilename, bCreateTempDir);

	//check
	CString strPathTempDirFile = strTestDir + g_strNameTempDir;
	strPathTempDirFile += _T("Test.h");
	bool bRes = isFileExist(strPathTempDirFile);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(!nRes && !bRes);
}

TEST_F(EngineTestParser,  parseFile_createTempDirError)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.cpp");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.cpp");
	CopyFile(strSourceFile, strDestFile, TRUE);

	CString strPath(strTestDir);
	CString strFilename(_T("Test.cpp"));
	bool bCreateTempDir = true; //dir created
	int nRes = m_parser.parseFile(strPath, strFilename, bCreateTempDir);

	//check
	CString strPathTempDirFile = strTestDir + g_strNameTempDir;
	strPathTempDirFile += _T("Test.cpp");
	bool bRes = isFileExist(strPathTempDirFile);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(nRes == -1 && !bRes);

}

TEST_F(EngineTestParser,  parseFiles_noCCodeDirectories)
{
	CCodeDirectories* pDir = NULL;
	int nRes = m_parser.parseFiles(pDir);
	ASSERT_EQ(-1, nRes);
}

TEST_F(EngineTestParser,  parseFiles_emptyCCodeDirectories)
{
	CCodeDirectories dir;
	int nRes = m_parser.parseFiles(&dir);
	ASSERT_EQ(0, nRes);
}

TEST_F(EngineTestParser,  parseFiles_noFilesForObfuscation)
{
	CCodeDirectories dir;
	
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);
	
	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.h");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.h");
	CopyFile(strSourceFile, strDestFile, TRUE);

	//fill object
	dir.m_strOriginalDir = strTestDir;
	dir.m_listFiles.Add(_T("Test.h"));

	int nRes = m_parser.parseFiles(&dir);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_EQ(0, nRes);
}


TEST_F(EngineTestParser,  parseFiles_hasFilesForObfuscation)
{
	CCodeDirectories dir;
	
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);
	
	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.cpp");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.cpp");
	CopyFile(strSourceFile, strDestFile, TRUE);

	//fill object
	dir.m_strOriginalDir = strTestDir;
	dir.m_listFiles.Add(_T("Test.cpp"));

	int nRes = m_parser.parseFiles(&dir);

	//check
	CString strPathTempDirFile = strTestDir + g_strNameTempDir;
	strPathTempDirFile += _T("Test.cpp");
	bool bRes = isFileExist(strPathTempDirFile);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(nRes == 1 && bRes);
}

TEST_F(EngineTestParser,  parseFiles_errorNotExistDir)
{
	CCodeDirectories dir;
	dir.m_strOriginalDir = _T("blabla");
	dir.m_listFiles.Add(_T("Test.cpp"));
	int nRes = m_parser.parseFiles(&dir);
	ASSERT_EQ(-1, nRes);
}


TEST_F(EngineTestParser,  obfuscate_noPathDirectory)
{
	std::wstring strPath(_T(""));
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.obfuscate(); 
	ASSERT_EQ(1, nRes);
}

TEST_F(EngineTestParser,  obfuscate_noFilesForObfuscation)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.h");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.h");
	CopyFile(strSourceFile, strDestFile, TRUE);

	CString strFilename(_T("Test.h"));
	std::wstring strPath(strTestDir.GetBuffer());
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.obfuscate();

	//check
	CString strPathTempDirFile = strTestDir + g_strNameTempDir;
	strPathTempDirFile += _T("Test.h");
	bool bRes = isFileExist(strPathTempDirFile);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(!nRes && !bRes);
}

TEST_F(EngineTestParser,  obfuscate_haveFilesForObfuscation)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.cpp");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.cpp");
	CopyFile(strSourceFile, strDestFile, TRUE);

	CString strFilename(_T("Test.cpp"));
	std::wstring strPath(strTestDir.GetBuffer());
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.obfuscate();

	//check
	CString strPathTempDirFile = strTestDir + g_strNameTempDir;
	strPathTempDirFile += _T("Test.cpp");
	bool bRes1 = isFileExist(strPathTempDirFile);
	CString strPathArchFile = strTestDir;
	strPathArchFile += _T("arch_obfuscate.obf");
	bool bRes2 = isFileExist(strPathArchFile);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(!nRes && bRes1 && bRes2);
}

TEST_F(EngineTestParser,  restore_noPathDirectory)
{
	std::wstring strPath(_T(""));
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.restore(); 
	ASSERT_EQ(1, nRes);
}

TEST_F(EngineTestParser,  restore_invalidPathDirectory)
{
	std::wstring strPath(_T("blabla"));
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.restore(); 
	ASSERT_EQ(0, nRes);
}

TEST_F(EngineTestParser,  restore_noArchFile)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.cpp");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.cpp");
	CopyFile(strSourceFile, strDestFile, TRUE);

	CString strFilename(_T("Test.cpp"));
	std::wstring strPath(strTestDir.GetBuffer());
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.restore();

	CString strPathArchFile = strTestDir;
	strPathArchFile += _T("arch_obfuscate.obf");
	bool bRes = isFileExist(strPathArchFile);

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(!nRes &&  !bRes);
}

TEST_F(EngineTestParser,  restore_fullOperation)
{
	//create work dir
	CString strCommand; 
	CString strTestDir(m_strRootPath.c_str());
	strTestDir += _T("TestDir\\");
	strCommand.Format(_T("/c mkdir %s"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	//copy file
	CString strSourceFile(m_strRootPath.c_str());
	strSourceFile += _T("Test\\Test\\Test.cpp");
	CString strDestFile(strTestDir);
	strDestFile += _T("Test.cpp");
	CopyFile(strSourceFile, strDestFile, TRUE);

	//obfuscation
	CString strFilename(_T("Test.cpp"));
	std::wstring strPath(strTestDir.GetBuffer());
	m_parser.setCurrentPath(strPath);
	int nRes = m_parser.obfuscate();

	//check
	CString strPathTempDirFile = strTestDir + g_strNameTempDir;
	strPathTempDirFile += _T("Test.cpp");
	bool bRes1 = isFileExist(strPathTempDirFile);
	CString strPathArchFile = strTestDir;
	strPathArchFile += _T("arch_obfuscate.obf");
	bool bRes2 = isFileExist(strPathArchFile);

	bool bRes3 = false;
	if (!nRes && bRes1 && bRes2)
	{
		SetCurrentDirectory(g_strCurrentDir.c_str());
		nRes = m_parser.restore();
		bRes2 = isFileExist(strPathArchFile);
		bRes3 = !bRes2 && !nRes;
	}

	//del work dir
	strCommand = _T("");
	strCommand.Format(_T("/c rmdir %s /S /Q"), strTestDir);
	ShellExecute(NULL, NULL,  _T("cmd.exe"),  strCommand, NULL, SW_HIDE);
	Sleep(TEST_DELAY);

	SetCurrentDirectory(g_strCurrentDir.c_str());
	ASSERT_TRUE(bRes3);
}


#endif //_GOOGLE_TEST
