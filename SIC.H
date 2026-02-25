#if !defined(AFX_SIC_H__CA32D9DD_EFE3_4A6E_9B1C_250565661DD5__INCLUDED_)
#define AFX_SIC_H__CA32D9DD_EFE3_4A6E_9B1C_250565661DD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSic dialog

class CSic : public CDialog
{
// Construction
public:
	CSic(CWnd* pParent = NULL);   // standard constructor
    void OnSetCost();
    void OnGetCost();
    double GetFIFOAMT(double ReqQty);
    DB   tmp,FifoTmp,outTmp;
    CString strSql,FileName,str;

// Dialog Data
	//{{AFX_DATA(CSic)
	enum { IDD = IDD_SIC };
	CProgressCtrl	m_p2;
	CProgressCtrl	m_p1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSic)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIC_H__CA32D9DD_EFE3_4A6E_9B1C_250565661DD5__INCLUDED_)
