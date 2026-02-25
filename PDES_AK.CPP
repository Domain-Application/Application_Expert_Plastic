// slip1.cpp : implementation file
//

#include "stdafx.h"
#include "db_ado.h"
#include "openrs.h"
#include "menu.h"

enum slip {SL_TIME=1,SL_PTNAME,SL_FIRST,SL_SECOND,SL_MOISTURE,
           SL_OUTTHROW,SL_NETWT,SL_PRICE,SL_AMOUNT};
enum slipmain {Sm_CONAME=1,Sm_COADD1,Sm_COADD2,Sm_COADD3,Sm_TICKET,
     Sm_LORRY,Sm_DATE,Sm_TIME,Sm_CREDITOR,Sm_NAME,Sm_ADD1,Sm_ADD2,
     Sm_ADD3,Sm_TRANSP,Sm_TNAME,Sm_DRIVER,Sm_DNAME,Sm_DO,Sm_REM,Sm_REM2,
     Sm_GSTNO,Sm_GSTNO2,
     Sm_TAXAMT,Sm_NONTAX,Sm_GSTAMT,Sm_TOTALQTY,Sm_TOTALAMT,
     Sm_PAGE};

extern void SetSlipTitle(DB *scr,DB *df);
void ExportArInvoiceItem(DB *df,DB *data, int autozero)
{
    long Srec,irec,rec;
    double total=0;
	
    CString strRem = "";
    Srec = interval(&irec,df->dat[1],data,1);
    for(rec=0; rec<Srec; rec++) {
        ridx(irec+rec,data,1);
        double amt = data->val(Wi_AMOUNT);
        if(amt==0) continue;
		
        lsetblank(&aroinv);
        fetch(&aroinv, data);
        lset(aroinv.dat[1], arinv.dat[1]);
		
        long xrec = qd(data->dat[Tr_PT], &prodtype);
        if(xrec) {
			strRem.TrimRight();
            if(strRem.GetLength()>0) strRem += ",";
            strRem += prodtype.dat[1];
            lset(aroinv.dat[Oiv_GLCODE],prodtype.dat[PT_GLCODE_CASHSALE]);
        }
        else {
            lset(aroinv.dat[Oiv_GLCODE],accpf.dat[PF_SALES]);
        }
        xrec = qd(aroinv.dat[Oiv_GLCODE],&glcode);
		if(xrec==0) {
            lset(aroinv.dat[Oiv_GLCODE],accpf.dat[PF_SALES]);
            xrec = qd(aroinv.dat[Oiv_GLCODE],&glcode);
		}
        lset(aroinv.dat[Oiv_NAME],glcode.dat[2]);
        dtodb(amt, &aroinv, Oiv_AMT);
        double qty = data->val(Wi_NETWT);
        dtodb(qty, &aroinv, Oiv_QTY);
        aroinv.AddNew();
        total += amt;
    }
    if(autozero) {
        lsetblank(&aroinv);
        lset(aroinv.dat[1], arinv.dat[1]);
		
        lset(aroinv.dat[Oiv_GLCODE],accpf.dat[PF_PC]);
        qd(aroinv.dat[Oiv_GLCODE],&glcode);
        lset(aroinv.dat[Oiv_NAME],glcode.dat[2]);

        dtodb(total * -1, &aroinv, Oiv_AMT);
        aroinv.AddNew();
        total = 0;
    }
    dtodb(total, &arinv, EIV_TOTAL);
    lset(arinv.dat[EIV_REM], "Sales Invoice");
    arinv.SqlUpdate(FALSE);
}

void TicketToArinv(DB *df)
{
    BOOL flag = dBfldLogic(df,D_CASHBILL);
    if(!flag) return;
    long xrec = qd(df->dat[D_PTYPE], &ptype);
    if(xrec==0) return;

    CString str;
    str.Format("select sum(amount) from ddata where [ticket] = %s",
       DbText(df, 1));
    double total = SqlValue(str);
    if(total==0) return;

    char key[250];

    strcpy(key,arinv.dat[1]);
    lset(key,profile.dat[Pro_PREFIX]);
    lset(&key[1],df->dat[1]);

    xrec = arinv.Search(key,1);
    if(xrec){
        str.Format("select * from arpaydat where invoice = %s",
           DbText(&arinv, 1, key));
        BOOL flag = IsSQLRSExisted(str);
        if(flag) return;

        str.Format("select * from arcrdat where invoice = %s",
           DbText(&arinv, 1, key));
        flag = IsSQLRSExisted(str);
        if(flag) return;

        str.Format("delete from aroinv where [%s] = %s",
            aroinv.fieldName[1],DbText(&arinv, 1, key));
        RunSql(str);

        str.Format("delete from arinv where [%s] = %s",
            arinv.fieldName[1],DbText(&arinv, 1, key));
        RunSql(str);
    }

    lsetblank(&arinv);
    fetch(&arinv, df);
    lset(arinv.dat[1],key);
    lset(arinv.dat[EIV_CUSTOMER],df->dat[D_DEBTOR]);
    lset(arinv.dat[EIV_TT],"I");
    lset(arinv.dat[EIV_CURRENCY],"RM");
    lset(arinv.dat[EIV_REM],"Cash Sales");
    dtodb(1, &arinv, EIV_RATE);
    arinv.AddNew();
    if(isblank(df->dat[D_CHEQUE])) {
//        ExportArInvoiceItem(df, &ddata, 1);
//        return;
    }

    ExportArInvoiceItem(df, &ddata, 0);
    total = arinv.val(EIV_TOTAL);
    if(total==0) return;

    lsetblank(&aripay);
    fetch(&aripay, &arinv);
    aripay.dat[1][0] = 'P';
//    FindAddInfoTicketNo(&aripay,1);
    dtodb(total, &aripay, IP_TOTAL);
    lset(aripay.dat[IP_PAYTYPE], df->dat[D_PTYPE]);
    lset(aripay.dat[IP_GLCODE], ptype.dat[4]);
    lset(aripay.dat[IP_CHEQUE], df->dat[D_CHEQUE]);
    lset(aripay.dat[IP_CQ_DATE], df->dat[D_CQ_DATE]);
    lset(aripay.dat[IP_REMARKS], "Payment Receipt");
    aripay.AddNew();

    lsetblank(&arpaydat);
    lset(arpaydat.dat[1], aripay.dat[1]);
    lset(arpaydat.dat[PY_INVOICE], arinv.dat[1]);
    lset(arpaydat.dat[PY_DATE], arinv.dat[EIV_DATE]);
    dtodb(total, &arpaydat, PY_TOTAL);
    arpaydat.AddNew();
}

void PRintDespatchSlipAikha(long irec,FILE *ptr,DB *df)
{
    long Srec = interval(&irec,df->dat[1],&ddata,1);
    if(Srec==0){
        int flag = AfxMessageBox("No data found\n\nDo you want to print?",MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
        if(flag==IDNO) return;
    }
    BOOL flag = dBfldLogic(df,D_CASHBILL);
    if(flag) {
        long xrec = qd(df->dat[D_PTYPE], &ptype);
        if(xrec==0) {
            AfxMessageBox("Error Message\n\nPlease key in type of payment.");
            return;
        }
        if(!isblank(df->dat[D_CHEQUE])) {
            if(isblank(df->dat[D_CQ_DATE])) {
                AfxMessageBox("Error Message\n\nPlease key in cheque date.");
                return;
            }
        }
        CString str;
        str.Format("select sum(amount) from [%s] where [ticket] = %s",
           df->e->chain->name, DbText(df, 1));
        double total = SqlValue(str);
        if(total==0){
            AfxMessageBox("Error Message\n\nPlease key in unit price.");
            return;
        }
    }

#ifdef _DEBUG
		ptr = fopen("slip.txt","w");
#else
		ptr = fopen("lpt1","w");
#endif //_DEBUG
    DB main;

    dopen(&main,"slipmain.rep");
    int fld = GetDbFieldNo(&accpf,"GST_DATE");
    if(fld>0) {
        if(strcmp(df->dat[D_DATE], accpf.dat[fld])<0) {
            fld = 0;
        }
    }
    if(fld>0) {
        dopen(&report,"pdes_gst.rep");
    }
    else {
        dopen(&report,"pdes_ak.rep");
    }

    memstart(&report);
    fetch_set(&report,&ddata);

    double tax1, tax2, qty, amt, gst;
    tax1=tax2=qty=amt=gst=0;
    for(long i=0;i<Srec;i++) {
        ridx(irec+i,&ddata,1);
        fetch_rec(&report,&ddata);
        qd(ddata.dat[Wi_PT],&prodtype);
        lset(report.dat[SL_PTNAME],prodtype.dat[2]);
        memwrt(&report);
        if(ddata.val(Wi_TAXAMT)==0) {
           tax1 += ddata.val(Wi_AMOUNT);
        }
        else {
           tax2 += ddata.val(Wi_AMOUNT);
        }
        qty  += ddata.val(Wi_NETWT);
        gst  += ddata.val(Wi_TAXAMT);
        amt  += ddata.val(Wi_SUBTOTAL);

    }
    memend(&report);

    SetSlipTitle(&main,df);
    dtodb(tax1, &main, Sm_NONTAX);
    dtodb(tax2, &main, Sm_TAXAMT);
    dtodb(gst , &main, Sm_GSTAMT);
    dtodb(qty, &main, Sm_TOTALQTY);
    dtodb(amt, &main, Sm_TOTALAMT);

    fprintf(ptr,"2C!");
    stdlayout(ptr,&main,&report);
    fprintf(ptr,"");
//    TicketToArinv(df);

    dclose(&report);
    dclose(&main);

    fclose(ptr);
#ifdef _DEBUG
    junction("slip.txt",0);
#else
#endif //_DEBUG
}
