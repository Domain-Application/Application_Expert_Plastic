#if !defined(AFX_AUTOINV_H__B6A34273_6DAE_4665_8F06_91A72ECFCDB9__INCLUDED_)
#define AFX_AUTOINV_H__B6A34273_6DAE_4665_8F06_91A72ECFCDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoInv.h : header file
//
#include "tio.h"
/////////////////////////////////////////////////////////////////////////////
// CAutoInv dialog

class CAutoInv : public CTio
{
// Construction
public:
	CAutoInv(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent = NULL);   // standard constructor
	virtual void OnPrint();
	virtual void GetInvoice(); 
	DB *main,*data;
	
// Dialog Data
	//{{AFX_DATA(CAutoInv)
	enum { IDD = IDD_AUTOINV };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoInv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoInv)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOINV_H__B6A34273_6DAE_4665_8F06_91A72ECFCDB9__INCLUDED_)
