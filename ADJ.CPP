// Adj.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Adj.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdj dialog


CAdj::CAdj(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CAdj)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    df->m_CannotAdd = TRUE;
}


void CAdj::DoDataExchange(CDataExchange* pDX)
{
        CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdj)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdj, CParent)
	//{{AFX_MSG_MAP(CAdj)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdj message handlers

void CAdj::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    fg.Initial(grid,sub,2,grid_ratio);
    fg.SetTitle("No",500);
    fg.SetTitle("Item Code",2000);
    fg.SetTitle("Description",3000);
    fg.SetTitle("Quantity",1000,7);
    fg.SetTitle("U/price",1200,7);
    fg.SetTitle("Amount",1400,7);
}

void CAdj::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    fg.show(rec);
    fg.SetText(sub,NULL,ADJ_ITEM,ADJ_ITEM | 0X100,ADJ_QTY,ADJ_PRICE,ADJ_AMOUNT,-1);
}

int IsAdjStable(DB *df)
{
    long xrec = dseek(df->dat[1],&adjitem,ADJ_REFNO,MF);

    if(xrec>0) {
        return 1;
    }
    return 0;
}

int tell_adj(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case Aj_DATE:
        if(IsAdjStable(df)) {
            flag = DO;
            break;
        }
        break;
    }
    return 2;
}

void ReplaceAdjGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,NULL,Aj_REFNO,Aj_DATE,Aj_REASON,Aj_REASON | 0x100,-1);
}

void SetAdjGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,2);
    g.SetTitle(df,0,Aj_REFNO,Aj_DATE,Aj_REASON,-1);
    g.SetTitle("Name",4300);
}

int Adj_AdminDb(DB *df,int flag)
{
    AdjustmentEntry();
    return 1;
}

int start_adj(DB *df)
{
    dbasedate(df->dat[Aj_DATE]);
    return 1;
}

void init_adj(DB *df)
{
    df->e->start      = start_adj;
    df->e->tell       = tell_adj;
    df->e->AdminDb    = Adj_AdminDb;
    df->e->chain      = &adjitem;
    df->e->rep        = ReplaceAdjGrid;
    df->e->GridTitle  = SetAdjGridTitle;
}

void AdjustmentEntry()
{     
    CAdj p1(IDD_ADJ,&adj,1);
    p1.sub        = &adjitem;
    p1.IDD_SUB    = IDD_ADJITEM;
    p1.m_OpenLpt  = FALSE;
    p1.DoModal();
}

void CAdj::OnDestroy() 
{
	CParent::OnDestroy();
	
}
