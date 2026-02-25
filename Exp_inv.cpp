// exp_inv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Logerr.h"
#include "Two.h"

DB   accinv,oinv;
int  exp_invFlag;

enum invpt {W1_INVOICE=1,W1_PT,W1_AMOUNT};

void ExportInvoiceItem(DB *df)
{
	CString strSql;
    strSql.Format("Select sum([amount]) from [invitem] where invoice = '%s'",df->dat[1]);
    double total = SqlValue(strSql);
	
    lsetblank(&oinv);
    lset(oinv.dat[1],df->dat[1]);
	long Mrec = qd(accpf.dat[APF_SALES],&glcode);
	if (Mrec>0){
		lset(oinv.dat[Oiv_GLCODE],accpf.dat[APF_SALES]);
	}
	else{
		CString strMsg;
		strMsg.Format("Invoice:%s Sales G/L CODE Not Found.\nPlease Check",df->dat[1]);
		waitmsg(strMsg);				
	}
    //lset(oinv.dat[Oiv_GLCODE],"S9001");
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
	/*
	double pcu_total=0,total;
	
	DB adoTmp;
	CString strSql;
	strSql.Format("Select * from [invpt] where invoice = '%s'",df->dat[1]);
    ado(&adoTmp,"adoTmp",NULL,strSql);
    long Srec = read_sumindex(0,&adoTmp);
    if (Srec) {		
		for(long rec=1; rec<=Srec; rec++) {
			readrec(rec,&adoTmp);
			lsetblank(&oinv);
			lset(oinv.dat[1],df->dat[1]);
			dtodb(rec,&oinv,Oiv_SQNO);
			qd(adoTmp.dat[W1_PT],&prodtype);
			lset(oinv.dat[Oiv_NAME],prodtype.dat[2]);
			pcu_total += adoTmp.val(W1_AMOUNT);
			total = adoTmp.val(W1_AMOUNT);
			dtodb(total,&oinv,Oiv_AMOUNT);
			long Mrec = qd(accpf.dat[APF_SALES],&glcode);
			if (Mrec>0){
				lset(oinv.dat[Oiv_GLCODE],accpf.dat[APF_SALES]);
			}
			else{
				CString strMsg;
				strMsg.Format("Invoice:%s Product:%s Purchase G/L CODE Not Found.\nPlease Check",df->dat[1],adoTmp.dat[W1_PT]);
				waitmsg(strMsg);				
			}
			
			oinv.AddNew();
		}			
	}
	dclose(&adoTmp);
    dtodb(pcu_total,&accinv,EIV_TOTAL);
    accinv.Update();
	*/

}

BOOL TRansferToAR(char *key,DB *df)
{
	long xrec,mrec;
	
    lset(key,df->dat[1]);
    xrec = accinv.Search(key,1);
    if(xrec==0) {
        lsetblank(&accinv);
		fetch(&accinv,df);
		lset(accinv.dat[1],key);
		lset(accinv.dat[EIV_DEBTOR],df->dat[IV_ACCODE]);

		qd(df->dat[IV_ACCODE],&debtor);
		lset(accinv.dat[EIV_PREFIX],debtor.dat[BU_PREFIX]);
		lset(accinv.dat[EIV_STAFF],debtor.dat[BU_STAFF]);
		lset(accinv.dat[EIV_TERMS],df->dat[IV_TERMS]);
		lset(accinv.dat[EIV_PARTICULAR],"SALES");
		lset(accinv.dat[EIV_SG],"IV");
		lset(accinv.dat[EIV_CURR],"RM");	
		dtodb(1,&accinv,EIV_RATE);
        accinv.AddNew();   
		ExportInvoiceItem(df);
		return 1;		
    }
	return 0;
}

void DoExportInvoice(DB *main,DB *data)
{
    char key[20];
    CString strSql,strCmd;
    DB adoTmp;
    strcpy(key,accinv.dat[1]);

//    strSql.Format("SELECT * FROM [%s] WHERE  (NOT EXISTS (SELECT * FROM %s)) ",inv.m_strTable,accinv.name);
    strSql.Format("SELECT * FROM [%s] WHERE  (invoice NOT in (SELECT [refno] FROM %s)) ",
                    inv.m_strTable,accinv.name);
    strCmd.Format("and [date]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("and [date]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
    strCmd.Format(" order by %s",inv.fieldName[1]);
    strSql+=strCmd;
    ado(&adoTmp,"adoTmp",NULL,strSql);
    long Srec = read_sumindex(0,&adoTmp);
    if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
            lset(key,adoTmp.dat[1]);
            long xrec =  inv.Search(key,1,TRUE);
            if (xrec){
                if (TRansferToAR(key,&adoTmp)){
                }
				else{
					CString strMsg;
					strMsg.Format("Invoice %s Transfer to AR Not Sucessful",key);
					waitmsg(strMsg);
				}
            }
		}		
	}
	dclose(&adoTmp);
}

void put_exp_inv()
{

    FLASH = 0;
    exp_invFlag = 0;
    DoExportInvoice(&inv,&invitem);

    waitmsg("Export Completed");
    FLASH = 1;
}

void ExportInvoiceTransactions()
{
	long Mrec = qd(accpf.dat[APF_SALES],&glcode);
	if (Mrec<=0){		
		CString strMsg;
		strMsg.Format("Sales G/L CODE At ACC Profile Not Found.\nPlease Check");
		waitmsg(strMsg);				
		return;
	}

    tado(&accinv,"arinv");
    tado(&oinv,"aroinv");

    CLogerr p1;
    p1.level = 9;
    p1.m_purpose = "CONFIRM EXPORT INVOICE TO AR";
    p1.df = &accinv;
    int wflag = p1.DoModal();
    if(wflag==IDOK) {

       CTwo t1(IDD_TWO,&two,0,put_exp_inv);
	   t1.Status = 2;
       t1.DoModal();

    }
    dclose(&accinv);
    dclose(&oinv);
}
