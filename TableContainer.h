//{{AFX_INCLUDES()
#include "updown.h"
//}}AFX_INCLUDES
#if !defined(AFX_TABLECONTAINER_H__6F6DC241_A2ED_11D4_A637_C46FCBF1030A__INCLUDED_)
#define AFX_TABLECONTAINER_H__6F6DC241_A2ED_11D4_A637_C46FCBF1030A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableContainer.h : header file
//
#include "resource.h"
#include "tan.h"
#include "xShadeButton.h"
#include "BtnST.h"
#include "XListCtrl.h"
#include "labelControl.h"
enum tabctn {Ctn_REFNO=1,Ctn_NAME,Ctn_REPORT,Ctn_MODE,Ctn_PREF,Ctn_GROUP,Ctn_FILETYPE};
/////////////////////////////////////////////////////////////////////////////
// CTableContainer dialog

class CTableContainer : public CDialog
{
// Construction
public:
    CTableContainer(CWnd* pParent = NULL);   // standard constructor
    ~CTableContainer();
    void OnStoreProcedure();
    void OnRunMultipleSimplyDialog();
    BOOL IsThisUserCanView(DB *df);
    void OnSqlStatement();
    void OnSimplyDialog();
    void OnRunMultipleFiles();
    void OnHTML();
    void AdminDb(DB *pdf,DB *sub=NULL);
    virtual void ShowReportOption(DB *df);
    virtual void ReFresh(DB *df);
    virtual void OnRun();
    virtual void OnRunObject();
    virtual void OnRunExe();
    virtual void OnPreviewReport(BOOL bInternalFilter = FALSE, BOOL exportToExcel = FALSE);
    virtual void OnBatchReport();
    virtual void SetButton(BOOL bFlag,int iHide = SW_HIDE, int iShow = SW_SHOW);
    virtual void OnWeighingSetup();
    BOOL OnFindTable();
    DB *option;
    CString m_group,str;
    int TotalPage,CurrentPage;
    long mrec,TabRecord,TableContainerMark;
    virtual void OnCancel();
    virtual void OnOK();
    BOOL m_add,m_delete,m_edit;

// Dialog Data
    //{{AFX_DATA(CTableContainer)
	enum { IDD = IDD_TABLE_CONTAINER };
    CXListCtrl   m_ListView;
    CString m_TableReportName;
	CUpDown	m_UpDown;
    CButtonST m_btn1;
    CxShadeButton m_btn2;
    CxShadeButton m_btn3;
    CxShadeButton m_btn4;
    CxShadeButton m_btn5;
    CButtonST     m_btn6;
    CLabelControl m_btn7;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTableContainer)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
    //{{AFX_MSG(CTableContainer)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickRun();
	afx_msg void OnEdit();
	afx_msg void OnNew();
	afx_msg void OnDel();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDownClickUpdown1();
	afx_msg void OnUpClickUpdown1();
	afx_msg void OnSecurity();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCrystal();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLECONTAINER_H__6F6DC241_A2ED_11D4_A637_C46FCBF1030A__INCLUDED_)
