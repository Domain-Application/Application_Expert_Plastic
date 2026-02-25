// PR.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

int tell_prdata(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
	CString str;
	double price=0;
	double wastage=0;
	double netwt = 0;
				
    switch(fld) {
    case Wi_DATE:
        break;
    case Wi_TIME:
        if(isblank(df->dat[fld])){
            _strtime(df->dat[fld]);
        }
        break;	
    case Wi_FIRST:
	case Wi_SECOND:
	case Wi_WASTAGE:
    case Wi_NETWT:		
        wastage = atol(df->dat[Wi_WASTAGE]);
        netwt = labs(atol(df->dat[Wi_SECOND]) - atol(df->dat[Wi_FIRST]))
            - wastage;
        dtodb(netwt, df, Wi_NETWT);
        break;		
    case Wi_AMOUNT:
        flag = DO;
        break;
    }
    
    double total = df->val(Wi_PRICE) * df->val(Wi_NETWT);
    total += df->val(Wi_TRIPAMT);
    dtodb(total,df,Wi_AMOUNT);
    return 2;
}

int start_prdata(DB *df)
{
    dbasedate(df->dat[Wi_DATE]);
    _strtime(df->dat[Wi_TIME]);
    return 1;
}

int prdata_save(DB *df,int flag)
{
    if(df->val(Wi_NETWT)==0) {
        if(AfxMessageBox("Zero weight\nAre you want to acceipt ?",MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2)==IDNO) {
            lsetblank(df);
            return 0;
        }
    }
	
    return 1;
}

void init_prdata(DB *df)
{
    df->e->tell  = tell_prdata;
    df->e->start = start_prdata;
    df->e->save  = prdata_save;
}

extern int start_receive(DB *df);
extern char *WbFieldName(DB *df,int fld);
extern void SetWBGridRows(CMSFlexGrid *grid);
void init_pr(DB *df)
{
	df->e->prec = PRintDespatchSlip;
    df->e->SetGridRows   = SetWBGridRows;
    df->e->start     = start_receive;
    df->e->LongFieldName = WbFieldName;
}
