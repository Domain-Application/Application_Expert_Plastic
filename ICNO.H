#if !defined(AFX_ICNO_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
#define AFX_ICNO_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// icno.h : header file
//

#include "staff.h"
/////////////////////////////////////////////////////////////////////////////
// Cicno dialog

class Cicno : public CStaff
{
// Construction
public:
    Cicno(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    CString GetJpgName(int no);
	void Cicno::ShowJpg(BOOL reDraw);
    virtual void ShowInformation();
    virtual void OnInternalMail();
    void OnImportData(DB *df,char *data,int dwSize);
    virtual void OnPaint();
    int width;
    int height;

// Dialog Data
    //{{AFX_DATA(Cicno)
    enum { IDD = IDD_ICNO };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(Cicno)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(Cicno)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSnapshotDoc();
	afx_msg void OnViewDoc();
	afx_msg void OnHistoryRecords();
	afx_msg void OnButton2();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_ICNO_H__79BD2FB0_67E7_4871_8330_07406B8CEF44__INCLUDED_)
