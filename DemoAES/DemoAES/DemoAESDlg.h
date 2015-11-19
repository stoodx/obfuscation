
// DemoAESDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CDemoAESDlg dialog
class CDemoAESDlg : public CDialogEx
{
// Construction
public:
	CDemoAESDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEMOAES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

protected:
	CString m_strDecode;
	CString m_strEncode;
	CString m_strText;
};
