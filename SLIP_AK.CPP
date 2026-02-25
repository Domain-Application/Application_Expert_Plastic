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


void SetSlipTitle(DB *scr,DB *df)
{
    fetch(scr,df);

    DB *pf = GetMatchTable(D_DEBTOR,df);
    if(pf) {
        qd(df->dat[D_DEBTOR],pf);
        lset(scr->dat[Sm_NAME],pf->dat[2]);
        lset(scr->dat[Sm_ADD1],pf->dat[BU_ADD1]);
        lset(scr->dat[Sm_ADD2],pf->dat[BU_ADD2]);
        lset(scr->dat[Sm_ADD3],pf->dat[BU_ADD3]);

        int fld = GetDbFieldNo(pf,"GST_NO");
        if(fld) {
           lset(scr->dat[Sm_GSTNO2],pf->dat[fld]);
        }
    }
    qd(df->dat[D_TRANSP],&transp);
    lset(scr->dat[Sm_TNAME],transp.dat[2]);
    qd(df->dat[D_ICNO],&driver);
    lset(scr->dat[Sm_DNAME],driver.dat[2]);
    lset(scr->dat[Sm_CONAME],logo.dat[Logo_NAME]);
    lset(scr->dat[Sm_COADD1],logo.dat[Logo_ADD1]);
    lset(scr->dat[Sm_COADD2],logo.dat[Logo_ADD2]);
    lset(scr->dat[Sm_COADD3],logo.dat[Logo_ADD3]);
    lset(scr->dat[Sm_GSTNO],profile.dat[Pro_GSTNO]);
    rset(scr->dat[Sm_PAGE],"6");
}

void PRintReceiveSlipAikha(long irec,FILE *ptr,DB *df)
{
    long Srec = interval(&irec,df->dat[1],&rdata,1);
    if(Srec==0){
        int flag = AfxMessageBox("No data found\n\nDo you want to print?",MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
        if(flag==IDNO) return;
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
        dopen(&report,"slip_gst.rep");
    }
    else {
        dopen(&report,"slip_ak.rep");
    }

    memstart(&report);
    fetch_set(&report,&rdata);

    double tax1, tax2, qty, amt, gst;
    tax1=tax2=qty=amt=gst=0;
    for(long i=0;i<Srec;i++) {
        ridx(irec+i,&rdata,1);
        fetch_rec(&report,&rdata);
        qd(rdata.dat[Wi_PT],&prodtype);
        lset(report.dat[SL_PTNAME],prodtype.dat[2]);
        memwrt(&report);
        if(rdata.val(Wi_TAXAMT)==0) {
           tax1 += rdata.val(Wi_AMOUNT);
        }
        else {
           tax2 += rdata.val(Wi_AMOUNT);
        }
        qty  += rdata.val(Wi_NETWT);
        gst  += rdata.val(Wi_TAXAMT);
        amt  += rdata.val(Wi_SUBTOTAL);

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
    dclose(&report);
    dclose(&main);

    fclose(ptr);
#ifdef _DEBUG
    junction("slip.txt",0);
#else
#endif //_DEBUG
}
