#if !defined(AFX_RECEIVE_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
#define AFX_RECEIVE_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Receive.h : header file
//

#include "wb.h"
/////////////////////////////////////////////////////////////////////////////
// CReceive dialog

class CReceive : public CWb
{
// Construction
public:
    CReceive(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual BOOL BlockByOtherCondition();;

// Dialog Data
    //{{AFX_DATA(CReceive)
    enum { IDD = IDD_WB };
	CString	m_product;
	CString	m_ptname;
	CString	m_TripAmt;
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CReceive)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CReceive)
		afx_msg void OnButton1();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECEIVE_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
