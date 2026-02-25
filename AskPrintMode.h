#if !defined(AFX_ASKPRINTMODE_H__0C6EDC19_366C_4F6B_8B2A_3F64A975948C__INCLUDED_)
#define AFX_ASKPRINTMODE_H__0C6EDC19_366C_4F6B_8B2A_3F64A975948C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AskPrintMode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAskPrintMode dialog

class CAskPrintMode : public CDialog
{
// Construction
public:
	CAskPrintMode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAskPrintMode)
	enum { IDD = IDD_AskPrintMode };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAskPrintMode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAskPrintMode)
	virtual void OnOK();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
    afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASKPRINTMODE_H__0C6EDC19_366C_4F6B_8B2A_3F64A975948C__INCLUDED_)
