#if !defined(AFX_ADJ_H__AEFB1E1A_FD78_43D4_9804_612D2EAB028F__INCLUDED_)
#define AFX_ADJ_H__AEFB1E1A_FD78_43D4_9804_612D2EAB028F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Adj.h : header file
//
#include "Parent.h"
/////////////////////////////////////////////////////////////////////////////
// CAdj dialog

class CAdj : public CParent
{
// Construction
public:
        CAdj(UINT IDD,DB *pdf,int Idx,CWnd* pParent = NULL);   // standard constructor
        virtual void ShowInitFrame(CMSFlexGrid *grid,DB *sub);
        virtual void ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem);

// Dialog Data
	//{{AFX_DATA(CAdj)
	enum { IDD = IDD_ADJ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdj)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdj)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJ_H__AEFB1E1A_FD78_43D4_9804_612D2EAB028F__INCLUDED_)
