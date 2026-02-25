// Wb.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "Wb.h"
#include "mgrid.h"
#include "Fgw.h"
#include "FastSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern void init_wb(DB *df);
/////////////////////////////////////////////////////////////////////////////
// CWb dialog


void Despatch_Capacity()
{
    CWb p1(IDD_DESPATCH,&despatch,1);
    p1.sub     = &ddata;
    p1.IDD_SUB = IDD_DDATA;
    p1.m_WinPrint=FALSE;
    p1.ReqitemStatus = 2;
    p1.m_PreviewCrystalReport = FALSE;
    p1.m_PrintOneFormOnly = TRUE;
    p1.bWtInc = TRUE;
    p1.m_UseInternalSortFields = FALSE;
    p1.period = &period;
    p1.fldPeriod       = D_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    p1.iRA = Pro_RA2;
    p1.DoModal();
}

int Despatch_AdminDb(DB *df,int flag)
{
    Despatch_Capacity();
    return 1;
}

int start_despatch(DB *df)
{
    dbasedate(df->dat[D_DATE]);
    _strtime(df->dat[D_TIME]);
    lset(df->dat[D_CASHBILL],"N");
    lset(df->dat[D_RA],"Y");
    return 1;
}
int DeleteDespatch(long irec,DB *df,int IdxItem) //23062023 Brian Delete ticket alert
{
	df->ReOpen();
	lsetblank(&tritem);
	long xrec = tritem.Search(df->dat[1],Tr_TICKET);
	
	if(xrec>0)
	{
		AfxMessageBox("The invoice is exist. Press ok to continue." );
		if(AfxMessageBox("Confirm delete ticket?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 )==IDYES)
		{
			return Delete(irec,df,IdxItem);
		}
	}
	else
	{
		return Delete(irec,df,IdxItem);
	}
}
void init_despatch(DB *df)
{
    init_wb(df);
    df->e->AdminDb  = Despatch_AdminDb;
    df->e->start    = start_despatch;
    df->e->prec		= PRintDespatchSlip;
	df->e->Delete	= DeleteDespatch;
	df->e->chain	= &ddata;
}
