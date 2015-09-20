// Parser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char* g_strSearchMacros = "ENCRYPT(x)";
const char* g_strSearchString = "ENCRYPT(";

// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			_tprintf(_T("\n***** Obfuscation *****\n"));
			if (argc != 2)
			{
err:
				_tprintf(_T("Error No start parameters\n\nUsage\n Parser <operation>\n\nWhere is\n  operation:\n    o - parse code files and obfuscation them\n    r - restore original codes\n\n"));
				_tprintf(_T("\nExample:\n\nParser c\n\n1. Create the temp directory: temp_original\n2. Copy all codes files from the original directory to the temp directory\n3. Obfuscate codes files in the original directory\n\n"));
				_tprintf(_T("Parser r\n\n1. Delete all the code obfuscated files in the original directory\n2. Move the original code from the temp directory to the original directory\n3. Delete the temporary directory\n\n"));
			}
			else
			{
				TCHAR strCurrentPath[MAX_PATH] = {0};
				GetCurrentDirectory(MAX_PATH, strCurrentPath);
				TCHAR ch =  argv[1][0];
				switch(ch) 
				{
				case _T('O'):
				case _T('o'):
					_tprintf(_T("Obfuscate operation.\n"));
					nRetCode = Obfuscate(strCurrentPath);
					break;
				case _T('R'):
				case _T('r'):
					_tprintf(_T("Restore operation.\n"));
					nRetCode = Restore(strCurrentPath);
					break;
				default:
					_tprintf(_T("Invalid parameter: %s\n\n"), argv[1]);
					goto err; //no parameters
					break;
				}
			}
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	_tprintf(_T("Obfuscated finish: %s\n\n"), nRetCode == 1 ? L"Error" : L"Normal");
	return nRetCode;
}


int Obfuscate(CString strRootPath)
{
	//return:
	//0 - normal
	//1 - error

	CPtrArray listDirs;
	int nSizeDir = 0;
	int nResult = 0; //no error 

	//save the rood dir to the list
	CCodeDirectories* pDirs =  new CCodeDirectories;
	ASSERT(pDirs);
	pDirs->m_strOriginalDir = strRootPath + _T("\\");
	listDirs.Add(pDirs);
	//Find all subdirs in root
	FindSubDirs(listDirs);
	//find codes files
	if (FindCodesFiles(listDirs))
	{
		nSizeDir = listDirs.GetSize();
		for (int i = 0; i < nSizeDir; i++)
		{
			pDirs = (CCodeDirectories*)listDirs.GetAt(i);
			int nRet = ParseFiles(pDirs);
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
	//else
	//	nResult = 1;

	//clean the dir list 
	nSizeDir = listDirs.GetSize();
	if (nSizeDir)
	{
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
			_tprintf(_T("Obfuscate operation - Done!\nObfuscated files number: %i\n"), nFilesNumber);
		}
	}
	return nResult;
}

int Restore(CString strRootPath)
{
	//return:
	//0 - normal
	//1 - error

	int nResult = 0;
	return nResult;
}

int FindSubDirs(CPtrArray& listDirs)
{
	//return size of dirs in root
	ASSERT(listDirs.GetSize() > 0);

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


bool FindCodesFiles(CPtrArray& listDirs)
{
	//return false - error
	int nSize = listDirs.GetSize();
	ASSERT(nSize > 0);
	for (int i = 0; i < nSize; i++)
	{
		CCodeDirectories* pDirs = (CCodeDirectories*) listDirs.GetAt(i);
		bool result[4] = {false};
		result[0] = FindFileByType(pDirs,  _T("*.h"));
		result[1] = FindFileByType(pDirs,  _T("*.hpp"));
		result[2] = FindFileByType(pDirs,  _T("*.c"));
		result[3] = FindFileByType(pDirs,  _T("*.cpp"));

		if (!result[1] && !result[0]  && !result[2]  && !result[3])
		{
			delete pDirs;
			listDirs.RemoveAt(i);
			nSize = listDirs.GetSize();
			if (nSize == 0)
			{
				_tprintf(_T("Obfuscate error: No codes files.\n"));
				return false;
			}
			i--;
		}
	}
	nSize = listDirs.GetSize();
	if (nSize == 0)
	{
		_tprintf(_T("Obfuscate error: No codes files.\n"));
		return false;
	}
	else
		return true;
}

bool FindFileByType(CCodeDirectories* pDirs, CString strTypeFile)
{
	//return true - we found files
	ASSERT(pDirs && strTypeFile.GetLength() > 0);
	CString strCurrentDir;
	CStringArray directoryArray;
	CFileFind finder;

	strCurrentDir = pDirs->m_strOriginalDir;
	SetCurrentDirectory(strCurrentDir);

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


int ParseFiles(CCodeDirectories* pDirs)
{
	//return:
	//-1 - error
	//0 - no files
	//>0 - number of files, the files were parsed

	ASSERT(pDirs);

	int nSize = pDirs->m_listFiles.GetSize();
	if (!nSize)
	{
		_tprintf(_T("No files for obfuscation in %s\n"), pDirs->m_strOriginalDir);
		return 0;
	}
	_tprintf(_T("Parsing files in %s\n"), pDirs->m_strOriginalDir);
	for (int i = 0; i < nSize; i++)
	{
		int nRet = ParseFile(pDirs->m_strOriginalDir, pDirs->m_listFiles.GetAt(i), pDirs->m_bTempDirCreated);
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
		_tprintf(_T("No files for obfuscation in %s\n"), pDirs->m_strOriginalDir);
		return 0;
	}	
	return nSize;
}

int ParseFile(CString strPath, CString strFilename, bool& bTempDirCreated)
{
	//return:
	//-1 - error
	//0 - file is not for obfuscation
	//>0 - file length, file was obfuscated.

	CFileException e;
	CFile fileOriginal;
	CString strFullPath;
	int nFileLength = 0;
	char* pBufFileOriginal = NULL; 

	if (strPath.GetLength() == 0 || strFilename.GetLength() == 0)
		return -1;

	strFullPath = strPath + strFilename;
	if (!fileOriginal.Open(strFullPath, CFile::modeRead, &e))
	{
		_tprintf(_T("Cannot open the file: %s\nError=%i\n"), strFilename, FileError(&e));
		return -1;
	}
	nFileLength = (int)fileOriginal.GetLength();
	if (nFileLength == 0)
	{
		fileOriginal.Close();
		_tprintf(_T("The file: %s has the null length.\n"), strFilename);
		return 0;
	}
	pBufFileOriginal = new char[nFileLength + 1];
	ASSERT(pBufFileOriginal);
	memset(pBufFileOriginal, 0, nFileLength + 1);
	fileOriginal.SeekToBegin(); 
	fileOriginal.Read(pBufFileOriginal, nFileLength);
	fileOriginal.Close();

	delete [] pBufFileOriginal;
	_tprintf(_T("Check the file: %s\n"), strFilename);

	return 0;
}

int FileError(CFileException *e)
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


bool CreateTempDirs(CPtrArray& listDirs, CString strNameStartTempDir)
{
	//return false - error
	int nSize = listDirs.GetSize();
	ASSERT(nSize > 0 && strNameStartTempDir.GetLength() > 0);

	for (int i = 0; i < nSize; i++)
	{
		CCodeDirectories* pDirs = (CCodeDirectories*) listDirs.GetAt(i);
		CString strDir = pDirs->m_strOriginalDir + strNameStartTempDir;
		strDir += _T("\\");
		if (!CreateDirectory(strDir, NULL))
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
			strErr.Format(_T("Error: Cannot create a directory:\n%s\nerror: %s\n\n"), strDir, (TCHAR*)cstr);
			LocalFree(cstr);
			_tprintf(strErr);
			return false;
		}
		pDirs->m_strTempDir = strDir;
	}

	return true;
}

