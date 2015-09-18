#pragma once

#include "resource.h"

struct CCodeDirectories
{
	CString m_strOriginalDir;
	CString m_strTempDir;

	CCodeDirectories()
	{
		m_strOriginalDir = _T("");
		m_strTempDir = _T("");
	}
};

int Obfuscate(CString strRootPath);
int Restore(CString strRootPath);
int FindSubDirs(CPtrArray& listDirs);
bool CreateTempDirs(CPtrArray& listDirs, CString strNameStartTempDir);
bool CopyCodeFiles(CPtrArray& listDirs);