// Spitem.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Spitem.h"
#include "PrintDlg.h"
#include "mgrid.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpitem dialog

CSpitem::CSpitem(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CSpitem)
	//}}AFX_DATA_INIT
}

void CSpitem::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpitem)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpitem, CParent)
	//{{AFX_MSG_MAP(CSpitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSpitem::OnInitDialog()
{
    CParent::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CSpitem message handlers

void CSpitem::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    fg.Initial(grid,sub,2);
    fg.SetTitle("No",400);
    fg.SetTitle("Item Code",1400);
    fg.SetTitle("Description",4000);
    fg.SetTitle("U/Price",900,7);
}

void CSpitem::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    fg.show(rec);
    fg.SetText(sub,NULL,Sp_ITEM,Sp_ITEM | 0x100,Sp_PRICE, -1);
}

void CSpitem::OnReqitemInitDialog(CDialog *p,int status)
{
    CString str,s =  "MT";
    if(price_unit==1) s = "kg";
    str.Format("Price (RM)/%s", s);
    p->SetDlgItemText(IDC_PRICE,str);
}

BEGIN_EVENTSINK_MAP(CSpitem, CParent)
    //{{AFX_EVENTSINK_MAP(CSpitem)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CustomerMaintenance()
{
    CSpitem p1(IDD_DEBTOR,&debtor,1);
    p1.sub       = &cpitem;
    p1.IDD_SUB   = IDD_SPITEM;
    p1.DoModal();
}

void SupplierMaintenance()
{
    CSpitem p1(IDD_DEBTOR,&creditor,1);
    p1.sub       = &spitem;
    p1.IDD_SUB   = IDD_SPITEM;
    p1.DoModal();
}

void ChangePrice(DB *df,int fld)
{
}

void FindProductPrice(DB *df)
{
    CString str;
    str.Format("select [price] from [spitem] where [pt]='%s' and [accode]='%s'",
        df->dat[Wi_PT],df->e->parent->dat[D_DEBTOR]);

    double price = SqlValue(str);

//    dtodb(price, df, Wi_PRICE1);

}

