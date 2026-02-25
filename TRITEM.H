#if !defined(AFX_TRITEM_H__FC7BD8EB_8D49_4D5D_885C_28211276FB35__INCLUDED_)
#define AFX_TRITEM_H__FC7BD8EB_8D49_4D5D_885C_28211276FB35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Inv.h : header file
//
#include "parent.h"
/////////////////////////////////////////////////////////////////////////////
// CPurInv dialog

class CTritem : public CParent
{
	// Construction
public:
    CTritem(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    
	virtual void ShowInformation();
	CString strSql;
	
	// Dialog Data
	//{{AFX_DATA(CPurInv)

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPurInv)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CTritem)
	afx_msg void DeleteTicket();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINV_H__FC7BD8EB_8D49_4D5D_885C_28211276FB35__INCLUDED_)
