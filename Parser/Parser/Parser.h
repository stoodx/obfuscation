#pragma once

#include "resource.h"

struct CCodeDirectories
{
	CString m_strOriginalDir;
	CString m_strTempDir;
	CStringArray m_listFiles;

	CCodeDirectories()
	{
		m_strOriginalDir = _T("");
		m_strTempDir = _T("");
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
bool CheckFilesOnObfuscation(CCodeDirectories* pDirs, CString strTypeFile); 

bool CreateTempDirs(CPtrArray& listDirs, CString strNameStartTempDir);
bool CopyCodeFiles(CPtrArray& listDirs);