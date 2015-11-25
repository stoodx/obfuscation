// Parser.cpp : Defines the entry point for the console application.
//

#ifndef _GOOGLE_TEST

#include "stdafx.h"
#include "ParserEngine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define __NO__OBFUSCATION


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			_tprintf(_T("Not init MFC.\n"));
			return 1;
		}
	}

	_tprintf(_T("\n***** Obfuscation *****\n"));
	if (argc != 2)
	{
err:
		_tprintf(_T("Error No start parameters\n\nUsage\n Parser <operation>\n\nWhere is\n  operation:\n    o - parse code files and obfuscation them\n    r - restore original codes\n\n"));
		_tprintf(_T("\nExample:\n\nParser c\n\n1. Create the temp directory: temp_original\n2. Copy all codes files from the original directory to the temp directory\n3. Obfuscate codes files in the original directory\n\n"));
		_tprintf(_T("Parser r\n\n1. Delete all the code obfuscated files in the original directory\n2. Move the original code from the temp directory to the original directory\n3. Delete the temporary directory\n\n"));
		nRetCode = 1;
	}
	else
	{
		ParserEngine parser;		
		TCHAR ch =  argv[1][0];
		switch(ch) 
		{
		case _T('O'):
		case _T('o'):
			_tprintf(_T("Obfuscate operation.\n"));
			nRetCode = parser.obfuscate();
			break;
		case _T('R'):
		case _T('r'):
			_tprintf(_T("Restore operation.\n"));
			nRetCode = parser.restore();
			break;
		default:
			_tprintf(_T("Invalid parameter: %s\n\n"), argv[1]);
			goto err; //no parameters
			break;
		}
	}

	_tprintf(_T("\nObfuscated finish: %s\n\n"), nRetCode == 1 ? L"Error" : L"Normal");
	return nRetCode;
}

#else

#include "stdafx.h"
#include "gtest\gtest.h"

int main(int argc, char* argv[])
{
	HMODULE hModule = ::GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			_tprintf(_T("Not init MFC.\n"));
			return 1;
		}
	}

	::testing::InitGoogleTest(&argc, argv);
	int nRes = RUN_ALL_TESTS();
	system("pause");
	return nRes;
}
#endif // _GOOGLE_TEST


