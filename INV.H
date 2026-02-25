#if !defined(AFX_INV_H__FC7BD8EB_8D49_4D5D_885C_28211276FB35__INCLUDED_)
#define AFX_INV_H__FC7BD8EB_8D49_4D5D_885C_28211276FB35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Inv.h : header file
//
#include "parent.h"
/////////////////////////////////////////////////////////////////////////////
// CInv dialog

class CInv : public CParent
{
// Construction
public:
    CInv(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    void GetInvoiceInformation(DB* df);
	virtual void ShowInitFrame();
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
	virtual void ShowInformation();
	virtual BOOL IsCannotEdit();
    virtual void OnDetail();
	CString strSql;
	
	// Dialog Data
	//{{AFX_DATA(CInv)
	enum { IDD = IDD_INV };
	COleDateTime	m_d1;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInv)
	afx_msg void OnCreateNewInvoice();
	afx_msg void OnAutoCreateInvoice();
	afx_msg void OnPickByDate();
	afx_msg void OnUpdatePayStatus();
    afx_msg void OnDblClickMsflexgrid1();
    DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INV_H__FC7BD8EB_8D49_4D5D_885C_28211276FB35__INCLUDED_)
