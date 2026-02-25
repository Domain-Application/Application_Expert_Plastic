// DDATA.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Wb.h"
#include "mgrid.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int openrs_rows;
/////////////////////////////////////////////////////////////////////////////
//

void FindWBOutthrowWt(DB *df,int i)
{
    if(df->val(Wi_SECOND)==0){
        dtodb(0,df,Wi_NETWT);
        return;
    }
    double wt,percent,wt2;

    wt = fabs(df->val(Wi_FIRST) - df->val(Wi_SECOND));
    percent = df->val(i-1);
    if(percent>0) {
        wt2 = wt * percent / 100;
        if(df->val(i)==0) {
            dtodb(wt2,df,i);
        }
    }
    wt2 = wt - df->val(Wi_MOISTURE) - df->val(i);
    dtodb(wt2,df,Wi_NETWT);
}

void FindWBMoistWt(DB *df,int i)
{
    if(df->val(Wi_SECOND)==0){
        dtodb(0,df,Wi_NETWT);
        return;
    }
    double wt,percent,wt2;

    wt = fabs(df->val(Wi_FIRST) - df->val(Wi_SECOND));
    percent = df->val(i-1);
    if(percent>0) {
        wt2 = wt * percent / 100;
        if(df->val(i)==0) {
            dtodb(wt2,df,i);
        }
    }
    wt2 = wt - df->val(Wi_OUTTHROW) - df->val(i);
    dtodb(wt2,df,Wi_NETWT);
}

void FindPecen(DB *df)
{
    qd(df->dat[Wi_TAXCODE], &taxcode);
    double percen = taxcode.val(3);

    int fld = GetDbFieldNo(&accpf,"GST_DATE");
    if(fld>0) {
        if(strcmp(df->l->ParentFile->dat[D_DATE], accpf.dat[fld])<0) {
            percen = 0;
        }
    }
    dtodb(percen, df, Wi_PERCEN);
}

void FindTaxCode(DB *df)
{
    long xrec = qd(df->dat[Wi_PT], &prodtype);
    if(xrec==0) return;

    if(STCR == 1) {
        qd(df->l->ParentFile->dat[D_DEBTOR], accode);
        int fld = GetDbFieldNo(accode,"TAXCODE");
        if(fld) {
            lset(df->dat[Wi_TAXCODE], accode->dat[fld]);
            if(isblank(df->dat[Wi_TAXCODE])) {
                lset(df->dat[Wi_TAXCODE], "NR");
            }
        }
    }
    else {
        lset(df->dat[Wi_TAXCODE], prodtype.dat[PT_TAXCODE2]);
    }
    qd(df->dat[Wi_TAXCODE], &taxcode);
    double percen = taxcode.val(3);

    int fld = GetDbFieldNo(&accpf,"GST_DATE");
    if(fld>0) {
        if(strcmp(df->l->ParentFile->dat[D_DATE], accpf.dat[fld])<0) {
            percen = 0;
        }
    }
    dtodb(percen, df, Wi_PERCEN);
}

void FindSpecialPrice(DB *df,int fld)
{
    CString str;

    long xrec = qd(df->dat[Wi_PT], &prodtype);
    if(xrec==0) return;

    double price = df->val(fld);
    if(price== prodtype.val(PT_BUY) || price== prodtype.val(PT_SALE)
        || price==0) {

        if(df->name[0]=='D') {
            str.Format("select [price] from [cpitem] where [accode]=%s and [pt]=%s",
                DbText(&despatch, D_DEBTOR), DbText(&prodtype,1));
        }
        else {
            str.Format("select [price] from [spitem] where [accode]=%s and [pt]=%s",
                DbText(&receive, D_DEBTOR), DbText(&prodtype,1));
        }
        price = SqlValue(str);
        if(price==0) {
             if(df->name[0]=='D') {
                 price = prodtype.val(PT_SALE);
             }
             else {
                 price = prodtype.val(PT_BUY);
             }
        }
        dtodb(price, df, fld);
    }
}

void SetSubTotal(DB *df)
{
/*    long xrec = qd(df->dat[Wi_UNIT], &unit);
    if(!xrec) {
        lset(df->dat[Wi_UNIT], prodtype.dat[PT_UNIT]);
        xrec = qd(df->dat[Wi_UNIT], &unit);
    }*/

    if(price_unit==0) price_unit = 1000;
    double total = df->val(Wi_PRICE) * df->val(Wi_NETWT) / price_unit;
    total += df->val(Wi_TRIPAMT);
    dtodb(total,df,Wi_AMOUNT);

    double percen = df->val(Wi_PERCEN);
    double taxamt = 0;
    total = df->val(Wi_AMOUNT);
    double subtotal = total;
    if(df->Boolean(Wi_INCLUDE)) {
        taxamt = total * percen / (100 + percen);
    }
    else {
        taxamt = total * percen / 100;
    }
    dtodb(taxamt, df, Wi_TAXAMT);
    if(!df->Boolean(Wi_INCLUDE)) {
        subtotal = total + df->val(Wi_TAXAMT);
    }
    dtodb(subtotal,df,Wi_SUBTOTAL);
}

int tell_ddata(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case Wi_DATE:
        break;
    case Wi_TIME:
        if(isblank(df->dat[fld])){
            _strtime(df->dat[fld]);
        }
        break;
    case Wi_MOISTURE:
        FindWBMoistWt(df,fld);
        break;
    case Wi_OUTTHROW:
        FindWBOutthrowWt(df,fld);
        break;
    case Wi_PT:
        if(sq) {
            FindTaxCode(df);
        }
    case Wi_PRICE:
        FindSpecialPrice(df,Wi_PRICE);
        break;
    case Wi_TAXCODE:
		if(sq){
            FindPecen(df);
		}
		break;
    case Wi_TAXAMT:
    case Wi_AMOUNT:
    case Wi_SUBTOTAL:
        flag = DO;
        break;
    }
    SetSubTotal(df);
    return 2;
}

int start_ddata(DB *df)
{
    dbasedate(df->dat[Wi_DATE]);
    _strtime(df->dat[Wi_TIME]);
//    lset(df->dat[Wi_UNIT],"kg");
    return 1;
}

int ddata_save(DB *df,int flag)
{
	lsetblank(&tritem);
	long drec = tritem.Search(df->dat[1],Tr_TICKET);
	
	if(drec>0)
	{
		AfxMessageBox("The invoice is exist. Weighing process cannot be conducted" );
		return 0;
	}
	else
	{
		return 1;
	}
    if(df->val(Wi_NETWT)==0) {
        if(AfxMessageBox("Zero weight\nAre you want to accept ?",MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2)==IDNO) {
            lsetblank(df);
            return 0;
        }
    }
    long xrec = qd(df->dat[Wi_TAXCODE], &taxcode);
    if(xrec==0) {
        for(int i= Wi_TAXCODE; i<=Wi_TAXAMT; i++) {
            fillblank( i, df);
        }
    }
    return 1;
}
int DeleteTicketData(long irec,DB *df,int IdxItem) //23062023 Brian Delete ticket alert
{
	df->ReOpen();
	lsetblank(&tritem);
	long xrec = tritem.Search(df->dat[1],Tr_TICKET);

	if(xrec>0)
	{
		AfxMessageBox("The invoice for this ticket has already paid. Ticket cannot be deleted.");
		return 0;	
	}
	else
	{
		if(AfxMessageBox("Confirm Delete Ticket?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 )==IDYES)
		{
			return Delete(irec,df,IdxItem);
		}
	}
}

int ddata_ShowInformation(CDialog *pDlg,DB *df,int flag)
{
    double netwt = fabs(df->val(Wi_SECOND) -  df->val(Wi_FIRST));
    if(df->val(Wi_SECOND)==0){
        netwt = 0;
    }
    char szTmp[20];
    ltot(netwt, 6, szTmp);
    pDlg->SetDlgItemText(IDC_NETWT, putnum0(szTmp));
    return 0;
}

void init_ddata(DB *df)
{
    df->e->tell  = tell_ddata;
    df->e->start = start_ddata;
    df->e->save  = ddata_save;
	df->e->Delete  = DeleteTicketData;
    df->e->ShowInformation  = ddata_ShowInformation;
}
