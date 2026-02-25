//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_SPITEM_H__47194F42_45C2_11D4_9E54_00606767E971__INCLUDED_)
#define AFX_SPITEM_H__47194F42_45C2_11D4_9E54_00606767E971__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Spitem.h : header file
//
#include "Parent.h"
#include "coolbtn.h"

/////////////////////////////////////////////////////////////////////////////
// CSpitem dialog

class CSpitem : public CParent
{
// Construction
public:
    CSpitem(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void OnReqitemInitDialog(CDialog *p,int status);
    virtual void ShowInitFrame(CMSFlexGrid *grid,DB *sub);
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
    BOOL OnInitDialog();

// Dialog Data
	//{{AFX_DATA(CSpitem)
    enum { IDD = IDD_DEBTOR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpitem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpitem)
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPITEM_H__47194F42_45C2_11D4_9E54_00606767E971__INCLUDED_)
