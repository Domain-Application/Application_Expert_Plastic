#if !defined(AFX_ExpAR_H__49329EA2_6CF1_4683_8B8F_CC9C2DD23753__INCLUDED_)
#define AFX_ExpAR_H__49329EA2_6CF1_4683_8B8F_CC9C2DD23753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpAP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpAR dialog

class CExpAR : public CDialog
{
	// Construction
public:
	void DoExportTicketToAR();
	CExpAR(CWnd* pParent = NULL);   // standard constructor
    DB dbToday;
    CString str;
	
	// Dialog Data
	//{{AFX_DATA(CExpAR)
	enum { IDD = IDD_EXEAP };
	CProgressCtrl	m_progress;
    COleDateTime    m_d1;
	COleDateTime	m_d2;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpAR)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CExpAR)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ExpAR_H__49329EA2_6CF1_4683_8B8F_CC9C2DD23753__INCLUDED_)
