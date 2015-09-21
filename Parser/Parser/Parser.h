#pragma once

#include "resource.h"

struct CCodeDirectories
{
	CString m_strOriginalDir;
	CStringArray m_listFiles;
	bool m_bTempDirCreated;

	CCodeDirectories()
	{
		m_strOriginalDir = _T("");
		m_bTempDirCreated = false;
	}

	~CCodeDirectories()
	{
		if (m_listFiles.GetSize() > 0)
			m_listFiles.RemoveAll();
	}
};

int Obfuscate(CString strRootPath);
int Restore(CString strRootPath);
int FindSubDirs(CPtrArray& listDirs);
bool FindCodesFiles(CPtrArray& listDirs);
bool FindFileByType(CCodeDirectories* pDirs, CString strTypeFile); 
int ParseFiles(CCodeDirectories* pDirs);
int ParseFile(CString strPath, CString strFilename, bool& bTempDirCreated);
int FileError(CFileException *e);
CStringA Encoder(CStringA strText);
bool CreateTempDir(CString strPath);
