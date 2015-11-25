#include "stdafx.h"
#include "Arch.h"
#include "AES.h"
#include "ParserEngine.h"

CWinApp theApp;

#define __NO__OBFUSCATION

const char* g_strSearchMacrosA = "ENCRYPT(x)";
const char* g_strSearchStringA = "ENCRYPT(";
const TCHAR* g_strNameTempDir = _T("TEMP_OBFUSCATION\\");
extern const TCHAR* g_strArchFilename;

using namespace std;

ParserEngine::ParserEngine(void)
	: m_strCurrentPath(_T(""))
{
	TCHAR strCurrentPath[MAX_PATH * sizeof(TCHAR)] = {0};
	GetCurrentDirectory(MAX_PATH * sizeof(TCHAR), strCurrentPath);
	m_strCurrentPath =  strCurrentPath;
	m_strCurrentPath += _T("\\");
}


ParserEngine::~ParserEngine(void)
{
}



int ParserEngine::obfuscate()
{
	//return:
	//0 - normal
	//1 - error

	if (!m_strCurrentPath.GetLength())
		return 1;

	CPtrArray listDirs;
	int nSizeDir = 0;
	int nResult = 0; //no error 

	//save the rood dir to the list
	CCodeDirectories* pDirs =  new CCodeDirectories;
	ASSERT(pDirs);
	pDirs->m_strOriginalDir = m_strCurrentPath;
	listDirs.Add(pDirs);
	//Find all subdirs in root
	findSubDirs(listDirs);
	//find codes files
	if (findCodesFiles(listDirs))
	{
		nSizeDir = listDirs.GetSize();
		for (int i = 0; i < nSizeDir; i++)
		{
			pDirs = (CCodeDirectories*)listDirs.GetAt(i);
			int nRet = parseFiles(pDirs);
			if (nRet == -1) //error
			{
				nResult = 1;
				break;
			}
			if (nRet == 0) // no files for obfuscation
			{
				delete pDirs;
				listDirs.RemoveAt(i);
				nSizeDir = listDirs.GetSize();
				if (nSizeDir == 0)
				{
					break;
				}
				i--;
			}
		}
	}

	//clean the dir list 
	nSizeDir = listDirs.GetSize();
	if (nSizeDir)
	{
		if (!nResult)
		{//archive  result
			CArch arch;
			nResult = arch.writeArch(m_strCurrentPath, &listDirs);
		}
		int nFilesNumber = 0;
		for (int i = 0; i < nSizeDir; i++)
		{
			pDirs = (CCodeDirectories*)listDirs.GetAt(i);
			nFilesNumber += pDirs->m_listFiles.GetSize();
			delete pDirs;
		}
		listDirs.RemoveAll();
		if (!nResult) //no error
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Obfuscate operation - completed!\nObfuscated files number: %i\n"), nFilesNumber);
#endif
		}
	}
	return nResult;
}

int ParserEngine::restore()
{
	//return:
	//0 - normal
	//1 - error

	if (!m_strCurrentPath.GetLength())
		return 1;

	int nResult = 0;
	CPtrArray listDirs;
	CArch arch;
	CString strErr;

	//restore the list dirs
#ifndef _GOOGLE_TEST
	_tprintf(_T("Restore the archive of obfuscation\n"));
#endif
	if ((nResult = arch.readArch(m_strCurrentPath, &listDirs)) != 1) 
	{//no error
		if (listDirs.GetSize() == 0)
			return 0; // no obfucation files
		//delete the arch file
		CString strArchPath = m_strCurrentPath + g_strArchFilename;
		if (DeleteFile(strArchPath))
		{//no err
#ifndef _GOOGLE_TEST
			_tprintf(_T("Arch file %s was deleted\n"), g_strArchFilename);
#endif
		}
		else
		{//err
			void* cstr;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
				(LPWSTR) &cstr,
				0,
				NULL
			);
			strErr.Format(_T("Obfuscate error: Cannot delete the arch file:\n%s\nerror: %s\nmake it by hand\n"), 
				strArchPath, (TCHAR*)cstr);
			LocalFree(cstr);
#ifndef _GOOGLE_TEST
			_tprintf(strErr);
#endif
		}
		//restore the original files
#ifndef _GOOGLE_TEST
		_tprintf(_T("Restore the original files\n"));
#endif
		while (listDirs.GetSize() != 0)
		{
			CCodeDirectories* pDirs = (CCodeDirectories*)listDirs.GetAt(0);
			CString strTemDir = pDirs->m_strOriginalDir + g_strNameTempDir;
#ifndef _GOOGLE_TEST
			_tprintf(_T("Original direrctory: %s\n"), pDirs->m_strOriginalDir);
#endif
			while(pDirs->m_listFiles.GetSize() != 0)
			{
				CString strFilename = pDirs->m_listFiles.GetAt(0);
				CString strFullTempDirFilename = strTemDir + strFilename;
				CString strFullOriginalFilename = pDirs->m_strOriginalDir + strFilename;
				if (CopyFile(strFullTempDirFilename, strFullOriginalFilename, FALSE))
				{//no err
#ifndef _GOOGLE_TEST
					_tprintf(_T("%s -> restored.\n"), strFilename); 
#endif
				}
				else
				{//err
					void* cstr;
					FormatMessage(
						FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL,
						GetLastError(),
						MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
						(LPWSTR) &cstr,
						0,
						NULL
					);
					strErr.Format(_T("Obfuscate error: Cannot copy the file:\nto %s\nerror: %s\nmake it by hand\n"), 
						strFullTempDirFilename,  strFullOriginalFilename, (TCHAR*)cstr);
					LocalFree(cstr);
#ifndef _GOOGLE_TEST
					_tprintf(strErr);
#endif
					nResult = 1;
				}
				pDirs->m_listFiles.RemoveAt(0);
				if (nResult == 0)
				{//delete the temp file
					DeleteFile(strFullTempDirFilename);
				}
			}
			delete pDirs;
			listDirs.RemoveAt(0);
			if (nResult == 0)
			{//delete temp dir
				RemoveDirectory(strTemDir);
			}
		}
#ifndef _GOOGLE_TEST
		if (nResult == 0)
		{
			_tprintf(_T("Resore operation - completed!\n"));
		}
		else
		{
			_tprintf(_T("Resore operation is not completed!\nFinish it by hand\n"));
		}
#endif
	}

	return nResult;
}

int ParserEngine::findSubDirs(CPtrArray& listDirs)
{
	//return size of dirs in root
	if (listDirs.GetSize() == 0)
		return 0;

	CString strCurrentDir;
	CStringArray directoryArray;
	CFileFind finder;

	strCurrentDir = ((CCodeDirectories*)listDirs.GetAt(0))->m_strOriginalDir;
	SetCurrentDirectory(strCurrentDir);

	BOOL bWorking = finder.FindFile(_T("*.*"));
	do
	{
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if(finder.IsDirectory() != 0) 
			{//we found a dir
				CString strDirName = finder.GetFileName();
				if (strDirName != _T("..") && strDirName != _T("."))
				{
					CString strDir = strCurrentDir;
					strDir += strDirName + _T('\\');
					directoryArray.Add(strDir);
					CCodeDirectories* pDirs = new CCodeDirectories;
					ASSERT(pDirs);
					pDirs->m_strOriginalDir = strDir;
					listDirs.Add(pDirs);
				}
			}
		}//while (bWorking)
		if (directoryArray.GetSize() == 0)
			break; //a normal end
		strCurrentDir = directoryArray.GetAt(0);
		directoryArray.RemoveAt(0);
		SetCurrentDirectory(strCurrentDir);
	}while(1);
	
	return listDirs.GetSize();
}


bool ParserEngine::findCodesFiles(CPtrArray& listDirs)
{
	//return false - error
	int nSize = listDirs.GetSize();
	if (nSize == 0)
		return false;

	for (int i = 0; i < nSize; i++)
	{
		CCodeDirectories* pDirs = (CCodeDirectories*) listDirs.GetAt(i);
		bool result[4] = {false};
		result[0] = findFileByType(pDirs,  _T("*.h"));
		result[1] = findFileByType(pDirs,  _T("*.hpp"));
		result[2] = findFileByType(pDirs,  _T("*.c"));
		result[3] = findFileByType(pDirs,  _T("*.cpp"));

		if (!result[1] && !result[0]  && !result[2]  && !result[3])
		{
			delete pDirs;
			listDirs.RemoveAt(i);
			nSize = listDirs.GetSize();
			if (nSize == 0)
			{
#ifndef _GOOGLE_TEST
				_tprintf(_T("Obfuscate: No codes files.\n"));
#endif
				return false;
			}
			i--;
		}
	}
	nSize = listDirs.GetSize();
	if (nSize == 0)
	{
#ifndef _GOOGLE_TEST
		_tprintf(_T("Obfuscate: No codes files.\n"));
#endif
		return false;
	}
	else
		return true;
}

bool ParserEngine::findFileByType(CCodeDirectories* pDirs, CString strTypeFile)
{
	//return true - we found files
	if(!pDirs || strTypeFile.GetLength() == 0)
		return false;
	CString strCurrentDir;
	CStringArray directoryArray;
	CFileFind finder;

	strCurrentDir = pDirs->m_strOriginalDir;
	if(!SetCurrentDirectory(strCurrentDir))
		return false;

	BOOL bWorking = finder.FindFile(strTypeFile);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDirectory() == 0) 
		{//only file
			CString strFilename = finder.GetFileName();
			pDirs->m_listFiles.Add(strFilename);
		}
	}

	if (pDirs->m_listFiles.GetSize() == 0)
		return false;
	else
		return true;
}


int ParserEngine::parseFiles(CCodeDirectories* pDirs)
{
	//return:
	//-1 - error
	//0 - no files
	//>0 - number of files, the files were parsed

	if (!pDirs)
		return -1;

	int nSize = pDirs->m_listFiles.GetSize();
	if (!nSize)
	{
#ifndef _GOOGLE_TEST
		_tprintf(_T("No files for obfuscation in %s\n"), pDirs->m_strOriginalDir);
#endif
		return 0;
	}
#ifndef _GOOGLE_TEST
	_tprintf(_T("Parsing files in %s:\n"), pDirs->m_strOriginalDir);
#endif
	for (int i = 0; i < nSize; i++)
	{
		int nRet = parseFile(pDirs->m_strOriginalDir, pDirs->m_listFiles.GetAt(i), pDirs->m_bTempDirCreated);
		if (nRet < 0) //error
		{
			return -1;
		}
		if (nRet == 0) //file is not for obfuscation
		{
			pDirs->m_listFiles.RemoveAt(i);
			nSize = pDirs->m_listFiles.GetSize();
			if (!nSize)
				break;
			i--;
		}
	}
	nSize = pDirs->m_listFiles.GetSize();
	if (!nSize)
	{
#ifndef _GOOGLE_TEST
		_tprintf(_T("No files for obfuscation in %s\n"), pDirs->m_strOriginalDir);
#endif
		return 0;
	}	
	return nSize;
}

int ParserEngine::parseFile(const CString& strPath, const CString& strFilename, bool& bTempDirCreated)
{
	//return:
	//-1 - error
	//0 - file is not for obfuscation
	//>0 - file length, file was obfuscated.

	CFileException e;
	CFile fileOriginal;
	CString strFullPathName;
	CString strPathTempDir;
	int nFileLength = 0;
	char* pBufFileOriginalA = NULL;
	CStringA strFileOriginalA(""), strFileObfuscatedA(""); 
	int index = 0;
	int nLenEncrypt = 0;
	bool bObfuscation = false;
	CString strErr;

	if (strPath.GetLength() == 0 || strFilename.GetLength() == 0)
		return -1;

	strPathTempDir = strPath + g_strNameTempDir;
	strFullPathName = strPath + strFilename;
	if (!fileOriginal.Open(strFullPathName, CFile::modeRead, &e))
	{
#ifndef _GOOGLE_TEST
		_tprintf(_T("Cannot open the file: %s\nError=%i\n"), strFilename, getFileError(&e));
#endif
		return -1;
	}
	nFileLength = (int)fileOriginal.GetLength();
	if (nFileLength == 0)
	{
		fileOriginal.Close();
#ifndef _GOOGLE_TEST 
		_tprintf(_T("The file: %s has the null length.\n"), strFilename);
#endif
		return 0;
	}
	pBufFileOriginalA = new char[nFileLength + 1];
	ASSERT(pBufFileOriginalA);
	memset(pBufFileOriginalA, 0, nFileLength + 1);
	fileOriginal.SeekToBegin(); 
	fileOriginal.Read(pBufFileOriginalA, nFileLength);
	fileOriginal.Close();
	strFileOriginalA = pBufFileOriginalA;
	delete [] pBufFileOriginalA;
	nFileLength = 0;

	//check on _NO_OBFUSCATION
	index = strFileOriginalA.Find("#define __NO__OBFUSCATION");
	if (index != -1)
	{//we must skip that file
#ifndef _GOOGLE_TEST
		_tprintf(_T("%s -> passed\n"), strFilename);
#endif
		return 0;
	}

	//check on the macros
	index = strFileOriginalA.Find(g_strSearchMacrosA);
	if (index != -1)
	{//we found the macros
		int nLenMacros = strlen(g_strSearchMacrosA);
		strFileObfuscatedA += strFileOriginalA.Left(index + nLenMacros);
		strFileOriginalA = strFileOriginalA.Right(strFileOriginalA.GetLength() - nLenMacros - index);
	}

	//check on obfuscation on "encrypt("
	nLenEncrypt = strlen(g_strSearchStringA);
	while((index = strFileOriginalA.Find(g_strSearchStringA)) != -1) 
	{
		//find "encrypt("
		strFileObfuscatedA += strFileOriginalA.Left(index + nLenEncrypt);
		strFileOriginalA = strFileOriginalA.Right(strFileOriginalA.GetLength() - index - nLenEncrypt);
		//find the string for obfuscation
		char c;
		while (true) //the first "
		{
			c = strFileOriginalA.GetAt(0);
			strFileObfuscatedA += c;
			strFileOriginalA = strFileOriginalA.Right(strFileOriginalA.GetLength() - 1);
			if (c == '\"')
				break;
		}
		CStringA strForObfuscationA("");
		while (true) //the second "
		{
			c = strFileOriginalA.GetAt(0);
			strFileOriginalA = strFileOriginalA.Right(strFileOriginalA.GetLength() - 1);
			if (c == '\"')
				break;
			strForObfuscationA += c;
		}
		strFileObfuscatedA += encodeText(strForObfuscationA) + '\"';
		bObfuscation = true;
	}
	//add the last part of file
	if (bObfuscation)
	{
		strFileObfuscatedA += strFileOriginalA;
		//create temp dir
		if (!bTempDirCreated)
		{
			if (!createTempDir(strPathTempDir))
				return -1; //did not create the temp dir
			bTempDirCreated = true;
#ifndef _GOOGLE_TEST
			_tprintf(_T("Temp directory:\n%s\n"), strPathTempDir);
#endif
		}
		//copy an original file to the temp dir
		CString strPathTempDirFile = strPathTempDir + strFilename;
		if (!CopyFile(strFullPathName, strPathTempDirFile, TRUE))
		{
			void* cstr;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
				(LPWSTR) &cstr,
				0,
				NULL
			);
			strErr.Format(_T("Obfuscate error: Cannot copy a file:\n%s\nto\n%s\nerror: %s\n"), 
				strFullPathName, strPathTempDirFile, (TCHAR*)cstr);
			LocalFree(cstr);
#ifndef _GOOGLE_TEST
			_tprintf(strErr);
#endif
			return -1;
		}
#ifndef _GOOGLE_TEST
		_tprintf(_T("%s was copied to Temp directory\n"), strFilename);
#endif
		//modify an original file
		if (!fileOriginal.Open(strFullPathName, CFile::modeReadWrite | CFile::shareDenyNone, &e))
		{
#ifndef _GOOGLE_TEST
			_tprintf(_T("Cannot open the file  %s for modification.\nError=%i\n"), strFilename, getFileError(&e));
#endif
			return -1;
		}
		fileOriginal.SeekToBegin(); 
		nFileLength = strFileObfuscatedA.GetLength();
		fileOriginal.Write(strFileObfuscatedA.GetBuffer(), nFileLength);
		fileOriginal.Flush();
		fileOriginal.Close();
	}
#ifndef _GOOGLE_TEST
	if ( bObfuscation)
	{
		_tprintf(_T("%s -> obfuscated\n"), strFilename);
	}
	else
	{
		_tprintf(_T("%s -> passed\n"), strFilename);
	}
#endif
	return nFileLength;
}

CStringA ParserEngine::encodeText(CStringA strText)
{
	////plus 2 to each char
	//int nLen = strText.GetLength();
	//CStringA str("");
	//for (int i = 0; i < nLen; i++)
	//{
	//	char ch = strText.GetAt(i);
	//	if (ch >= '\"')
	//		ch += 2;
	//	str += ch;
	//}
	//return str;

	string strIn(strText.GetBuffer());
	string strOut;
	CAES::encryptString(strIn, strOut);
	return CStringA(strOut.c_str());
}

int ParserEngine::getFileError(CFileException *e)
{
		switch(e->m_cause)
		{
		case CFileException::none:
		{
			return CFileException::none;
		}
		case CFileException::genericException : 
		{
			return CFileException::genericException;
		}
		case CFileException::fileNotFound:     
		{
			return CFileException::fileNotFound;
		}
		case CFileException::badPath:       
		{
			return CFileException::badPath;
		}
		case CFileException::tooManyOpenFiles:           
		{
			return CFileException::tooManyOpenFiles;
		}
		case CFileException::accessDenied:              
		{
			return CFileException::accessDenied;
		}
		case CFileException::invalidFile:                 
		{
			return CFileException::invalidFile;
		}
		case CFileException::removeCurrentDir:                  
		{
			return CFileException::removeCurrentDir;
		}
		case CFileException::directoryFull:                    
		{
			return CFileException::directoryFull;
		}
		case CFileException::badSeek:                       
		{
			return CFileException::badSeek;
		}
		case CFileException::hardIO:                          
		{
			return CFileException::hardIO;
		}
		case CFileException::sharingViolation:                             
		{
			return CFileException::sharingViolation;
		}
		case CFileException::lockViolation:                                
		{
			return CFileException::lockViolation;
		}
		case CFileException::diskFull:                                   
		{
			return CFileException::diskFull;
		}
		case CFileException::endOfFile:                                      
		{
			return CFileException::endOfFile;
		}
		default:
			return -1;
	}
}

bool ParserEngine::createTempDir(const CString& strPath)
{
	//return false - error
	
	if (!CreateDirectory(strPath, NULL))
	{
		void* cstr;
		CString strErr;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
			(LPWSTR) &cstr,
			0,
			NULL
		);
		strErr.Format(_T("Obfuscate error: Cannot create a directory:\n%s\n  error: %s\n"), strPath, (TCHAR*)cstr);
		LocalFree(cstr);
		_tprintf(strErr);
		return false;
	}
	return true;
}

