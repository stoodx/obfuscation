// Arch.cpp : implementation file
//

#include "stdafx.h"
#include "Parser.h"
#include "Arch.h"
#include "Parser.h"

const TCHAR* g_strSerialFilename = _T("Serial_obfuscate.obf");

// CArch
IMPLEMENT_SERIAL(CArch, CObject, 1)

CArch::CArch()
{
	m_pArr = NULL;
}

CArch::~CArch()
{
}


// CArch member functions
bool CArch::WriteArch(CString strPath, CPtrArray* pArr)
{
	ASSERT(pArr);

	CFileException e;
	CFile f;
	if (strPath.GetLength() == 0)
		return false;

	m_pArr = pArr;

	CString strFullPath = strFullPath + g_strSerialFilename;

	if (!f.Open(strFullPath, CFile::modeCreate | CFile::modeWrite | 
		CFile::typeBinary, &e))
	{
		FileErrorArch(&e);
		return false;
	}
	CArchive ar(&f, CArchive::store);
	Serialize(ar);
	f.Close();
	return true;

}

int CArch::FileErrorArch(CFileException *e)
{
		switch(e->m_cause)
		{
		case CFileException::none:
		{
			return CFileException::none;
		}
		case CFileException::genericException : 
		{
			_tprintf(_T("Arch file error: genericException\n"));
			return CFileException::genericException;
		}
		case CFileException::fileNotFound:     
		{
			_tprintf(_T("Arch file error: fileNotFound\n"));
			return CFileException::fileNotFound;
		}
		case CFileException::badPath:       
		{
			_tprintf(_T("Arch file error: badPath;\n"));
			return CFileException::badPath;
		}
		case CFileException::tooManyOpenFiles:           
		{
			_tprintf(_T("Arch file error: tooManyOpenFiles\n"));
			return CFileException::tooManyOpenFiles;
		}
		case CFileException::accessDenied:              
		{
			_tprintf(_T("Arch file error: accessDenied\n"));
			return CFileException::accessDenied;
		}
		case CFileException::invalidFile:                 
		{
			_tprintf(_T("Arch file error: invalidFile\n"));
			return CFileException::invalidFile;
		}
		case CFileException::removeCurrentDir:                  
		{
			_tprintf(_T("Arch file error: removeCurrentDir\n"));
			return CFileException::removeCurrentDir;
		}
		case CFileException::directoryFull:                    
		{
			_tprintf(_T("Arch file error: directoryFull\n"));
			return CFileException::directoryFull;
		}
		case CFileException::badSeek:                       
		{
			_tprintf(_T("Arch file error: adSeek\n"));
			return CFileException::badSeek;
		}
		case CFileException::hardIO:                          
		{
			_tprintf(_T("Arch file error: hardIO\n"));
			return CFileException::hardIO;
		}
		case CFileException::sharingViolation:                             
		{
			_tprintf(_T("Arch file error: sharingViolation\n"));
			return CFileException::sharingViolation;
		}
		case CFileException::lockViolation:                                
		{
			_tprintf(_T("Arch file error: lockViolation\n"));
			return CFileException::lockViolation;
		}
		case CFileException::diskFull:                                   
		{
			_tprintf(_T("Arch file error: diskFull\n"));
			return CFileException::diskFull;
		}
		case CFileException::endOfFile:                                      
		{
			_tprintf(_T("Arch file error: endOfFile\n"));
			return CFileException::endOfFile;
		}
		default:
		{
			_tprintf(_T("Arch file error: unknown\n"));
			return -1;
		}
	}
}

void CArch::Serialize(CArchive &ar)
{
	ASSERT(m_pArr);
	
	int nSize = 0;
	int nFilesNumber = 0;
	if(ar.IsStoring())
	{//backup
		nSize = m_pArr->GetSize();
		if(nSize == 0)
			return;
		ar << nSize;
		for(int i = 0; i < nSize; i++)
		{
			CCodeDirectories* pDirs = (CCodeDirectories*)m_pArr->GetAt(i);
			ar << pDirs->m_strOriginalDir;
			nFilesNumber = pDirs->m_listFiles.GetSize();
			ar << nFilesNumber;
			for (int j = 0; j < nFilesNumber; j++)
			{
				CString strFileName = pDirs->m_listFiles.GetAt(j);
				ar << strFileName;
			}
		}
	}
	else
	{//restore
		ar >> nSize;
		if(nSize == 0)
			return;
		for(int i = 0; i < nSize; i++)
		{
			CCodeDirectories* pDirs = new CCodeDirectories;
			ASSERT(pDirs);
			ar >> pDirs->m_strOriginalDir;
			ar >> nFilesNumber;
			for (int j = 0; j < nFilesNumber; j++)
			{
				CString strFileName;
				ar >> strFileName;
				pDirs->m_listFiles.Add(strFileName);
			}
			m_pArr->Add(pDirs);
		}
	}
}