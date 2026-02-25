#if !defined(AFX_TRCV_H__E54A44AD_458C_443B_9431_F735F41C0BEB__INCLUDED_)
#define AFX_TRCV_H__E54A44AD_458C_443B_9431_F735F41C0BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Trcv.h : header file
//

#include "parent.h"
/////////////////////////////////////////////////////////////////////////////
// CTrcv dialog

class CTrcv : public CParent
{
// Construction
public:
    CTrcv(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
    virtual void ShowInitFrame(CMSFlexGrid *grid,DB *sub);
    virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);
    virtual void OnPrint();

// Dialog Data
	//{{AFX_DATA(CTrcv)
        // NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrcv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrcv)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRCV_H__E54A44AD_458C_443B_9431_F735F41C0BEB__INCLUDED_)
