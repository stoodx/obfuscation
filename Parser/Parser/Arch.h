#pragma once
#include "afxcoll.h"

// CArch command target

class CArch : public CObject
{
public:
	DECLARE_SERIAL(CArch)

	CArch();
	virtual ~CArch();

	int writeArch(CString strPath, CPtrArray* pArr);
	int readArch(CString strPath, CPtrArray* pArr);
private:
	int getFileErrorArch(CFileException *e);
	void Serialize(CArchive &ar);
	CPtrArray* m_pArr;
};


