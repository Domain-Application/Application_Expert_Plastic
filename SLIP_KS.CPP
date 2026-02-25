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


void PRintReceiveSlipKS(long irec,FILE *ptr,DB *df)
{
    long Srec = interval(&irec,df->dat[1],&rdata,1);
    if(Srec==0){
        int flag = AfxMessageBox("No data found\n\nDo you want to print?",MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
        if(flag==IDNO) return;
    }

    DB main;

    dopen(&main,"slipmain.rep");
    dopen(&report,"slip_ks.rep");

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

}
