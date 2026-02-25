#if !defined(AFX_WB_H__8B172581_7B26_11D3_B370_00001CD5F6A7__INCLUDED_)
#define AFX_WB_H__8B172581_7B26_11D3_B370_00001CD5F6A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Wb.h : header file
//
#include "parent.h"
/////////////////////////////////////////////////////////////////////////////
// CWb dialog

class CWb : public CParent
{
// Construction
public:
    CWb(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void OnPressPrint2();
    virtual void OnPrintTicket();
    virtual BOOL OnInitDialog();
    virtual void OnSnapshot();
    virtual void OnDecimalAdjustment();
    virtual void OnSubModifyCompleted();
    virtual void OnReqitemInitDialog(CDialog *p,int status);
    virtual void OnSearch();
    virtual void OnJpg2Db(DB *df,int wno);
    virtual void ShowInformation();
    virtual void ShowInitFrame(CMSFlexGrid *grid,DB *sub);
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
    virtual void OnPrint();
    virtual void OnDetail();
    virtual void HideNoUseButtonMenu();
    virtual void ShowSaveButton();
    virtual void OnWizard();
    virtual void OnButton2();
	virtual void OnPDF();
//	virtual BOOL BlockByOtherCondition();
	CString strPhoto[8];
    BOOL busy;
    int iRA,jpgkey;
    CString str,m_jpg;
    BOOL   bWtInc;

// Dialog Data
	//{{AFX_DATA(CWb)
	enum { IDD = IDD_WB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWb)
	afx_msg void OnExport();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WB_H__8B172581_7B26_11D3_B370_00001CD5F6A7__INCLUDED_)
