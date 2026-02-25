#if !defined(AFX_FGFTP_H__31B19C0F_813A_4C0C_BFD0_2027A283CF62__INCLUDED_)
#define AFX_FGFTP_H__31B19C0F_813A_4C0C_BFD0_2027A283CF62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Fgftp.h : header file
//

#include "ConfirmWeight.h"
/////////////////////////////////////////////////////////////////////////////
// CFgftp dialog

class CFgftp : public CConfirmWeight
{
// Construction
public:
    CFgftp(CWnd* pParent = NULL);   // standard constructor
    virtual int OnGw();

// Dialog Data
    //{{AFX_DATA(CFgftp)
    enum { IDD = IDD_FGFTP };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFgftp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
    //{{AFX_MSG(CFgftp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FGFTP_H__31B19C0F_813A_4C0C_BFD0_2027A283CF62__INCLUDED_)
