#pragma once
#include "afxcoll.h"

// CArch command target

class CArch : public CObject
{
public:
	DECLARE_SERIAL(CArch)

	CArch();
	virtual ~CArch();

	int WriteArch(CString strPath, CPtrArray* pArr);
	int ReadArch(CString strPath, CPtrArray* pArr);
private:
	int FileErrorArch(CFileException *e);
	void Serialize(CArchive &ar);
	CPtrArray* m_pArr;
};


