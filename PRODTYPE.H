#if !defined(AFX_PRODTYPE_H__1735AF28_608C_4F9E_8DD4_F3BCD41675BF__INCLUDED_)
#define AFX_PRODTYPE_H__1735AF28_608C_4F9E_8DD4_F3BCD41675BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Prodtype.h : header file
//

#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CProdtype dialog

class CProdtype : public CStaff
{
// Construction
public:
    CProdtype(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProdtype)
	enum { IDD = IDD_PT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProdtype)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProdtype)
	afx_msg void OnStockFlow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODTYPE_H__1735AF28_608C_4F9E_8DD4_F3BCD41675BF__INCLUDED_)
