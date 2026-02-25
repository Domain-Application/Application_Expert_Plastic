// Trcv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Trcv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrcv dialog


CTrcv::CTrcv(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CTrcv)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTrcv::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrcv)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrcv, CParent)
	//{{AFX_MSG_MAP(CTrcv)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrcv message handlers

void CTrcv::OnPrint()
{
    lset(df->dat[D_COMPLETED],"Y");
    long xrec = dseek(df->dat[1],df,IdxItem,MF);        //force update
    if(xrec) {
        long mrec = ridxrec(xrec, df, IdxItem);
        wrtfield(mrec,D_COMPLETED,df);
    }
    m_NeedShowDetail = TRUE;
    ShowInformation();
    CParent::OnPrint();
    m_NeedShowDetail = TRUE;
    ShowInformation();
}

void CTrcv::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    fg.Initial(grid,sub,7,1.16);
    fg.SetTitle("No",285,0);
    fg.SetTitle("Date",890,1);
    fg.SetTitle("Time",700,1);
    fg.SetTitle("1st Wt",650,7);
    fg.SetTitle("2nd Wt",650,7);
    fg.SetTitle("Gross Wt",720,7);
    fg.SetTitle("Product",650,1);
    fg.SetTitle("Description",2000,1);
    fg.SetTitle("Wastage",700,7);
    fg.SetTitle("Nett Wt",700,7);
    fg.SetTitle("U/Price",720,7);
    fg.SetTitle("Trip",800,7);
    fg.SetTitle("Amount",800,7);
}

void CTrcv::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    fg.show(rec);

    fg.SetText(sub,0,Wi_DATE,Wi_TIME,Wi_FIRST,Wi_SECOND,-1);
    double gw = fabs( sub->val(Wi_FIRST) - sub->val(Wi_SECOND));
    char tmp[10];
    dtoa(gw, tmp, 6, 0);
    fg.SetText(putnum(tmp));
    fg.SetText(sub,Wi_PT,Wi_PT | 0x100,Wi_WASTAGE,
        Wi_NETWT,Wi_PRICE,Wi_TRIPAMT,Wi_AMOUNT,-1);
}

void OtherReceive_Capacity()
{
	trcv.e->chain = &trdata;
	trdata.e->parent = &trcv;
    CTrcv p1(IDD_TRCV,&trcv,1);
    p1.sub     = &trdata;
    p1.IDD_SUB = IDD_DDATA;
    p1.m_WinPrint=FALSE;
    p1.ReqitemStatus = 2;
    p1.m_PreviewCrystalReport = FALSE;
    p1.m_PrintOneFormOnly = TRUE;
    p1.m_UseInternalSortFields = FALSE;
    p1.DoModal();
}
