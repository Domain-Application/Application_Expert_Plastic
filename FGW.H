#if !defined(AFX_FGW_H__31B19C0F_813A_4C0C_BFD0_2027A283CF62__INCLUDED_)
#define AFX_FGW_H__31B19C0F_813A_4C0C_BFD0_2027A283CF62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Fgw.h : header file
//

#include "ConfirmWeight.h"
#include "ComboWt.h"
/////////////////////////////////////////////////////////////////////////////
// CFgw dialog

class CFgw : public CConfirmWeight
{
// Construction
public:
    CFgw(UINT XIDD,CWnd* pParent = NULL);   // standard constructor
    virtual void OnSnapshot();
    virtual void SetSpitem();
	virtual void OnOK();
    virtual int OnGw();
	HWND m_WndCap;
    void FindLastWeight();
    long actualWeight;
    BOOL   bWtInc;
    UINT   fgkey;
    int    iRA;
    DB     *df, *sub;
    CString m_app;
    double lastwt,percent;

// Dialog Data
	//{{AFX_DATA(CFgw)
	enum { IDD = IDD_FGW };
    CComboWt m_cbProduct;
	CString	m_product;
	CString	m_moisture;
	CString	m_outthrow;
	CString	m_lastwt;
	CString	m_ptname;
	CString	m_price;
	CString	m_TripAmt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFgw)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFgw)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnEditchangeProduct();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FGW_H__31B19C0F_813A_4C0C_BFD0_2027A283CF62__INCLUDED_)
