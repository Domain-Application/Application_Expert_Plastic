// Inv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int  IsGlcodeExists(DB *df)
{
    long Srec,irec;

    long rec = qd(accpf.dat[PF_SALES], &glcode);
    if(rec==0) {
        AfxMessageBox("Profile\n\nG/L Not found");
        return 0;
    }

    Srec = interval(&irec,df->dat[1],&tritem,1);
    for(rec=0; rec<Srec; rec++) {
        ridx(irec+rec,&tritem,1);
        double total = tritem.val(Tr_AMOUNT);
        if(total==0) continue;

        long xrec = qd(tritem.dat[Tr_PT], &prodtype);
        long yrec = qd(prodtype.dat[PT_GLCODE_SALE],&glcode);
        if(yrec==0) {
            char szTmp[80];
            sprintf(szTmp , "G/L Not found\n\nProduct : %s",prodtype.dat[1]);
            AfxMessageBox(szTmp);
            return 0;
        }
    }
    return 1;
}

void ExportArInvoiceItem(DB *df)
{
	CString strSql,str = "select pt,name,price, sum(qty) AS QTY, sum(subtotal),sum(taxamt),taxcode,included,percen from tritem where refno = %s GROUP BY PT,name,price,taxcode,included,percen";
	strSql.Format(str, DbText(df,1));

	ado(&report, "tritem", NULL, strSql);
    double total=0;

    long Srec = report.t->sumrecord;
    for(long rec=1; rec<=Srec; rec++) {
		report.read(rec);
        double amt = report.val(5);
        if(amt==0) continue;
        double tax = report.val(6);
		
        lsetblank(&aroinv);
        fetch(&aroinv, &report);

        long xrec = qd(report.dat[1], &prodtype);
        if(xrec) {
            lset(aroinv.dat[Oiv_GLCODE],prodtype.dat[PT_GLCODE_SALE]);
        }
        else {
            lset(aroinv.dat[Oiv_GLCODE],accpf.dat[PF_SALES]);
        }
        lset(aroinv.dat[1], df->dat[1]);
        lset(aroinv.dat[Oiv_NAME], report.dat[2]);
        dtodb(amt, &aroinv, Oiv_AMT);
        dtodb(tax, &aroinv, Oiv_VAT);
        if(report.Boolean(8)) {
            lset(aroinv.dat[Oiv_INCLUDE],"T");
        }
        else {
            lset(aroinv.dat[Oiv_INCLUDE],"F");
        }
        aroinv.AddNew();
        total += amt;
    }
    dclose(&report);

	CString cmdstr;

    cmdstr.Format("update arinv set amount='%f' from arinv where refno='%s'",
        total,df->dat[1]);
    runsql(cmdstr);
}

void InvToAr()
{
    CString cmdstr;

    if(IsGlcodeExists(&itran)==0) return;

    char key[12];

	/*
    strcpy(key,arinv.dat[1]);
    lset(key,itran.dat[1]);
	
    long xrec = arinv.Search(key,1);
    if(xrec) return;

    lsetblank(&arinv);
    fetch(&arinv,&itran);
	
	
    lset(arinv.dat[1],key);
    lset(arinv.dat[EIV_TT],"I");
    lset(arinv.dat[EIV_CURRENCY],"RM");
    lset(arinv.dat[EIV_REM],"Sales");
    dtodb(1, &arinv, EIV_RATE);
    arinv.AddNew();
	*/
	cmdstr.Format("insert into arinv(REFNO,DATE,DEBTOR,TT,CURRENCY,PARTICULAR,RATE) values ('%s','%s','%s','I','RM','SALES','1')",
		itran.dat[1],itran.dat[2],itran.dat[4]);
	RunSql(cmdstr);


	
    ExportArInvoiceItem(&itran);
    AfxMessageBox("****Export completed****");
	
	
}
