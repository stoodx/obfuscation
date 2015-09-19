// Parser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
			_tprintf(_T("***** Obfuscation *****\n"));
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
					_tprintf(_T("!!!Obfuscate operation!!!\n\n"));
					nRetCode = Obfuscate(strCurrentPath);
				case _T('R'):
				case _T('r'):
					_tprintf(_T("!!!Restore operation!!!\n\n"));
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

	return nRetCode;
}


int Obfuscate(CString strRootPath)
{
	//return:
	//0 - normal
	//1 - error

	CPtrArray listDirs;
	int nSizeDir = 0;
	int nResult = 1; //for error 

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
		_tprintf(_T("Obfuscate operation - Done!\n\n"));
	}
	else
		nResult = 1;

	//clean the dir list 
	nSizeDir = listDirs.GetSize();
	for (int i = 0; i < nSizeDir; i++)
	{
		pDirs = (CCodeDirectories*)listDirs.GetAt(i);
		delete pDirs;
	}
	listDirs.RemoveAll();

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
		bool result[2] = {false};
		result[0] = CheckFilesOnObfuscation(pDirs,  _T("*.C*"));
		result[1] = CheckFilesOnObfuscation(pDirs,  _T("*.h*"));
	}

	return true;
}

bool CheckFilesOnObfuscation(CCodeDirectories* pDirs, CString strTypeFile)
{
	//return true - we found files
	ASSERT(pDirs && strTypeFile.GetLength() > 0);

	return true;
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

bool CopyCodeFiles(CPtrArray& listDirs)
{
	//return false - error
	ASSERT(listDirs.GetSize() > 0);

	return true;
}