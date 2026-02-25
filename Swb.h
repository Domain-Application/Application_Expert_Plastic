#if !defined(AFX_SWB_H__037B581D_E487_4DA6_A667_A9D57D04DA51__INCLUDED_)
#define AFX_SWB_H__037B581D_E487_4DA6_A667_A9D57D04DA51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Swb.h : header file
//
#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// CSwb dialog

class CSwb : public CStaff
{
// Construction
public:
    CSwb(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    void ShowInformation();

// Dialog Data
	//{{AFX_DATA(CSwb)
	enum { IDD = IDD_SWB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSwb)
	afx_msg void OnCaptureWeight();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWB_H__037B581D_E487_4DA6_A667_A9D57D04DA51__INCLUDED_)
