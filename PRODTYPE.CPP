// Prodtype.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Prodtype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProdtype dialog


CProdtype::CProdtype(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CProdtype)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProdtype::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProdtype)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProdtype, CStaff)
	//{{AFX_MSG_MAP(CProdtype)
	ON_BN_CLICKED(IDC_BUTTON1, OnStockFlow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProdtype message handlers

void CProdtype::OnStockFlow() 
{
    ViewStockFlow(df);
}

void Prodtype()
{
    CProdtype p1(IDD_PT,&prodtype,1);
    p1.DoModal();
}
