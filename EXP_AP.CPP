// Exe_ap.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Two.h"
#include "Logerr.h"

extern DB   accinv,oinv;
extern int  exp_invFlag;
enum pinvpt {W1_INVOICE=1,W1_PT,W1_AMOUNT};

int  IsPurcGlcodeExists(DB *df)
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

void ExportPurchaseInvoiceItem(DB *df)
{
	CString strSql;
    strSql.Format("Select sum([amount]) from [pinvitem] where invoice = '%s'",df->dat[1]);
    double total = SqlValue(strSql);

    lsetblank(&oinv);
    lset(oinv.dat[1],df->dat[1]);
    long Mrec = qd(accpf.dat[APF_PURCHASE],&glcode);
	if (Mrec>0){
		lset(oinv.dat[Oiv_GLCODE],accpf.dat[APF_PURCHASE]);
	}
	else{
		CString strMsg;
		strMsg.Format("Invoice:%s Purchase G/L CODE Not Found.\nPlease Check",df->dat[1]);
		waitmsg(strMsg);				
	}
	//lset(oinv.dat[Oiv_GLCODE],"P8001");
    //lset(oinv.dat[Oiv_NAME],"Purchase");
	qd(oinv.dat[Oiv_GLCODE],&glcode);
    lset(oinv.dat[Oiv_NAME],glcode.dat[2]);
    dtodb(total,&oinv,Oiv_AMOUNT);
    oinv.AddNew();

    if (!isblank(df->dat[IV_OTHER_GLCODE])){
		lsetblank(&oinv);
		lset(oinv.dat[1],df->dat[1]);
		lset(oinv.dat[Oiv_GLCODE],df->dat[IV_OTHER_GLCODE]);
		qd(oinv.dat[Oiv_GLCODE],&glcode);
		lset(oinv.dat[Oiv_NAME],glcode.dat[2]);
		rset(oinv.dat[Oiv_AMOUNT],df->dat[IV_OTHERAMT]);
		oinv.AddNew();
	}
	total += df->val(IV_OTHERAMT);
    dtodb(total,&accinv,EIV_TOTAL);
    accinv.Update();
}

BOOL TRansferToAP(char *key,DB *df)
{
	long xrec,mrec;
	
    lset(key,df->dat[1]);
    lsetblank(&accinv);
    fetch(&accinv,df);
    lset(accinv.dat[1],key);
    lset(accinv.dat[EIV_DEBTOR],df->dat[IV_ACCODE]);

    qd(df->dat[IV_ACCODE],&debtor);
    lset(accinv.dat[EIV_PREFIX],debtor.dat[BU_PREFIX]);
    lset(accinv.dat[EIV_STAFF],debtor.dat[BU_STAFF]);
    lset(accinv.dat[EIV_TERMS],df->dat[IV_TERMS]);
    lset(accinv.dat[EIV_PARTICULAR],"PURCHASE");
    lset(accinv.dat[EIV_SG],"IV");
    lset(accinv.dat[EIV_CURR],"RM");
	dtodb(1,&accinv,EIV_RATE);
    accinv.AddNew();
    ExportPurchaseInvoiceItem(df);
    return 1;
}

void DoExportPurchaseInvoice(DB *main,DB *data)
{
	char key[20];
    CString strSql,strCmd;
    DB adoTmp;
    strcpy(key,accinv.dat[1]);
	
    strSql.Format("SELECT * FROM [%s] WHERE  (invoice NOT in (SELECT [refno] FROM %s)) ",
        pinv.m_strTable,accinv.name);
    strCmd.Format("and [date]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("and [date]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
    strCmd.Format(" order by %s",pinv.fieldName[1]);
    strSql+=strCmd;
    ado(&adoTmp,"adoTmp",NULL,strSql);
    long Srec = read_sumindex(0,&adoTmp);
    for(long rec=1;rec<=Srec; rec++){
        readrec(rec,&adoTmp);
        lset(key,adoTmp.dat[1]);
        if (TRansferToAP(key,&adoTmp)){
        }
        else{
            CString strMsg;
            strMsg.Format("Invoice %s Transfer to AP Not Sucessful",key);
            waitmsg(strMsg);
        }
	}
	dclose(&adoTmp);
	
}

void put_exp_ap()
{

    FLASH = 0;
    exp_invFlag = 0;
    DoExportPurchaseInvoice(&pinv,&pinvitem);
    waitmsg("Export Completed");
    FLASH = 1;
}

void ExportPurchaseInvoiceTransactions2()
{
	long Mrec = qd(accpf.dat[APF_PURCHASE],&glcode);
	if (Mrec<=0){		
		CString strMsg;
		strMsg.Format("Purchase G/L CODE At ACC Profile Not Found.\nPlease Check");
		waitmsg(strMsg);				
		return;
	}

	tado(&accinv,"apinv");
	tado(&oinv,"apoinv");

    CLogerr p1;
    p1.level = 9;
    p1.m_purpose = "CONFIRM EXPORT INVOICE TO AP";
    p1.df = &accinv;
    int wflag = p1.DoModal();
    if(wflag==IDOK) {

       CTwo t1(IDD_TWO,&two,0,put_exp_ap);
	   t1.Status = 2;
       t1.DoModal();

    }
    dclose(&accinv);
    dclose(&oinv);
}
void ExportApInvoiceItem(DB *df)
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
void InvToAp()
{
    CString cmdstr;

    if(IsPurcGlcodeExists(&puritran)==0) return;

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


	
    ExportApInvoiceItem(&puritran);
    AfxMessageBox("****Export completed****");
	
	
}