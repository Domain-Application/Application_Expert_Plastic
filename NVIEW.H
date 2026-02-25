// NView.h : interface of the CNView class
//
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INCLUDES()
//}}AFX_INCLUDES

#if !defined(AFX_NView_H__01A41CA5_3A95_9713_AA5A_118DB73CFB3E__INCLUDED_)
#define AFX_NView_H__01A41CA5_3A95_9713_AA5A_118DB73CFB3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "menuDoc.h"
#include "cctvview.h"

#define timer_video 1
#define timer_weight 2

class CNView : public CCctvView
{
public: // create from serialization only
    CNView();
    void ShowInitFrame();
    void ShowInformation();
    void SetActiveStatus(DB *df,long rec);
	BOOL busy123;
    DB   *df;

    DECLARE_DYNCREATE(CNView)

public:
    //{{AFX_DATA(CNView)
    enum { IDD = IDD_MENU_FORM };
	UINT	m_days;
	//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNView)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:

protected:

// Generated message map functions
public:
	afx_msg void OnTimer(UINT nIDEvent);
    //{{AFX_MSG(CNView)
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NView_H__01A41CA5_3A95_9713_AA5A_118DB73CFB3E__INCLUDED_)
