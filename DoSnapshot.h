#if !defined(AFX_DOSNAPSHOT_H__4A98A83F_A4CF_4CC4_8410_DEA188BFB866__INCLUDED_)
#define AFX_DOSNAPSHOT_H__4A98A83F_A4CF_4CC4_8410_DEA188BFB866__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoSnapshot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDoSnapshot dialog

class CDoSnapshot : public CDialog
{
// Construction
public:
	CDoSnapshot(CWnd* pParent = NULL);   // standard constructor
    CString filename;
    DB *df;
	HWND m_WndCap;
    CString m_app,m_jpg;
    BOOL busy;
    int iCounter;

// Dialog Data
	//{{AFX_DATA(CDoSnapshot)
	enum { IDD = IDD_DoSnapshot };
	CProgressCtrl	m_progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoSnapshot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDoSnapshot)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOSNAPSHOT_H__4A98A83F_A4CF_4CC4_8410_DEA188BFB866__INCLUDED_)
