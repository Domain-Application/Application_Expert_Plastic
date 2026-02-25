// slip1.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "openrs.h"
#include "db_ado.h"

enum slip_do {SL_TICKET=1,SL_DATE,SL_LORRY,SL_TRAILER,SL_DO,SL_CONTNO,
              SL_SUPPLIER,SL_ADD1,SL_ADD2,SL_ADD3,SL_TRANSP,SL_DRIVER,SL_DRNAME,
              SL_NO1,SL_TIME1,SL_PT1,SL_P1,SL_Y1,SL_Y11,SL_G1,SL_M1,SL_O1,SL_W1,SL_V1,
              SL_NO2,SL_TIME2,SL_PT2,SL_P2,SL_Y2,SL_Y22,SL_G2,SL_M2,SL_O2,SL_W2,SL_V2,
              SL_NO3,SL_TIME3,SL_PT3,SL_P3,SL_Y3,SL_Y33,SL_G3,SL_M3,SL_O3,SL_W3,SL_V3,
              SL_NO4,SL_TIME4,SL_PT4,SL_P4,SL_Y4,SL_Y44,SL_G4,SL_M4,SL_O4,SL_W4,SL_V4,
              SL_NO5,SL_TIME5,SL_PT5,SL_P5,SL_Y5,SL_Y55,SL_G5,SL_M5,SL_O5,SL_W5,SL_V5,
              SL_NO6,SL_TIME6,SL_PT6,SL_P6,SL_Y6,SL_Y56,SL_G6,SL_M6,SL_O6,SL_W6,SL_V6,
              SL_NO7,SL_TIME7,SL_PT7,SL_P7,SL_Y7,SL_Y57,SL_G7,SL_M7,SL_O7,SL_W7,SL_V7,
              SL_NO8,SL_TIME8,SL_PT8,SL_P8,SL_Y8,SL_Y58,SL_G8,SL_M8,SL_O8,SL_W8,SL_V8,
              SL_WASTAGE,SL_OT,SL_NET,SL_RM,SL_REMARKS,SL_UTNAME,SL_USER,
              SL_CREDITOR,SL_DEBTOR,SL_GSTNO,SL_GSTNO2,
              SL_DODATE,SL_PTYPE,SL_PAYNAME};


void PrintTicket(FILE *ptr,char *tl)
{
    setvbuf(ptr,NULL,_IONBF,0);
    fprintf(ptr,"C!\n");
    fprintf(ptr,"t");
    fprintf(ptr,"ME%sFP",title);
    if(!isblank(profile.dat[Pro_BRANCH])) {
//        fprintf(ptr,"\nPE%sF",profile.dat[Pro_BRANCH]);
    }
    for(int i=0; i<3; i++){
        fprintf(ptr,"\n%s",profile.dat[PF_ADD1 + i]);
    }
    fprintf(ptr,"\nGST NO:%s",profile.dat[Pro_GSTNO]);
}

void CollectTicket(DB *df,DB *rep,DB *data,DB *accode,int withRA)
{
    lsetblank(rep);
    fetch(rep,df);

    qd(df->dat[D_DEBTOR], accode);
    qd(df->dat[D_TRANSP], &transp);
    qd(df->dat[D_ICNO], &driver);
    qd(df->dat[D_PTYPE], &ptype);

    lset(rep->dat[SL_SUPPLIER],accode->dat[2]);
    lset(rep->dat[SL_ADD1],accode->dat[BU_ADD1]);
    lset(rep->dat[SL_ADD2],accode->dat[BU_ADD2]);
    lset(rep->dat[SL_ADD3],accode->dat[BU_ADD3]);
    lset(rep->dat[SL_TRANSP],transp.dat[2]);
    lset(rep->dat[SL_DRNAME],driver.dat[2]);
    lset(rep->dat[SL_USER],user.dat[1]);
    int fld = GetDbFieldNo(accode,"GST_NO");
    if(fld) {
       lset(rep->dat[SL_GSTNO2],accode->dat[fld]);
    }
    lset(rep->dat[SL_GSTNO],profile.dat[Pro_GSTNO]);

    if(rep->sumfield>=SL_PAYNAME) {
        lset(rep->dat[SL_PAYNAME],ptype.dat[2]);
    }

	long i,irec;
    double net_recv=0,net_wastage=0,net_ot=0;
    double ItemAmt = 0;
    long Srec = interval(&irec,df->dat[1],data,1);
    for(i=0; i<Srec && i<8; i++) {
        int j = i * 11;
        ridx(irec+i,data,1);
        if(withRA==0) {
			if(!isblank(data->dat[Wi_PT])) {
				if(!strcmp(data->dat[Wi_PT],profile.dat[Pro_RA])) continue;
			}
        }
        qd(data->dat[Wi_PT],&prodtype);
        lset(rep->dat[SL_PT1   + j],prodtype.dat[2]);
        lset(rep->dat[SL_TIME1 + j],data->dat[Wi_TIME]);
        double m = data->val(Wi_MOISTURE);
        dtodb(m , rep, SL_M1 + j);
        net_wastage+=m;

        m = data->val(Wi_OUTTHROW);
        dtodb(m , rep, SL_O1 + j);
        net_ot+=m;

        m = data->val(Wi_AMOUNT);
        dtodb(m , rep, SL_V1 + j);
        ItemAmt += m;

        m = data->val(Wi_PRICE);
        dtodb(m , rep, SL_P1 + j);
        m = data->val(Wi_FIRST);
        dtodb(m , rep, SL_Y1 + j);
        m = data->val(Wi_SECOND);
        dtodb(m , rep, SL_Y11 + j);
        m = fabs(data->val(Wi_SECOND) - data->val(Wi_FIRST));
        dtodb(m , rep, SL_G1 + j);

        m = data->val(Wi_NETWT);
        net_recv += m;
        dtodb(m , rep, SL_W1 + j);

        dtodb(i+1 , rep, SL_NO1 + j);
    }

    dtodb(net_wastage,rep,SL_WASTAGE);
    dtodb(net_ot,rep,SL_OT);
    dtodb(net_recv,rep,SL_NET);
    dtodb(ItemAmt,rep,SL_RM);
}

void PrintStoreReceipt(long irec,FILE *ptr,DB *df,int withRA)
{
	if(!ptr) {
		return;
	}

    CollectTicket(df,&report,&rdata,&creditor,withRA);
    PrintTicket(ptr,title);
    mprint(ptr,&report);


}

void ExportApInvoiceItem(DB *df,DB *data)
{
    long Srec,irec,rec;
    double total=0;

    CString strRem = "";
    Srec = interval(&irec,df->dat[1],data,1);
    for(rec=0; rec<Srec; rec++) {

        ridx(irec+rec,data,1);
        double amt = data->val(Wi_SUBTOTAL);
        if(amt==0) continue;
        double tax = data->val(Wi_TAXAMT);

		lsetblank(&apoinv);
        fetch(&apoinv, data);
		lset(apoinv.dat[1], apinv.dat[1]);

        long xrec = qd(data->dat[Tr_PT], &prodtype);
        if(xrec) {
			strRem.TrimRight();
            if(strRem.GetLength()>0) strRem += ",";
            strRem += prodtype.dat[1];
            lset(apoinv.dat[Oiv_GLCODE],prodtype.dat[PT_GLCODE_PURCHASE]);
        }
        else {
            lset(apoinv.dat[Oiv_GLCODE],accpf.dat[PF_PURCHASE]);
        }
        xrec = qd(apoinv.dat[Oiv_GLCODE],&glcode);
		if(xrec==0) {
            lset(apoinv.dat[Oiv_GLCODE],accpf.dat[PF_PURCHASE]);
			xrec = qd(apoinv.dat[Oiv_GLCODE],&glcode);
		}
        lset(apoinv.dat[Oiv_NAME],glcode.dat[2]);
        dtodb(amt, &apoinv, Oiv_AMT);
        dtodb(tax, &apoinv, Oiv_VAT);
        double qty = data->val(Wi_NETWT);
        dtodb(qty, &apoinv, Oiv_QTY);
        if(qty) {
            dtodb(amt / qty, &apoinv, Oiv_PRICE);
        }
        if(data->Boolean(Wi_INCLUDE)) {
            lset(apoinv.dat[Oiv_INCLUDE],"T");
        }
        else {
            lset(apoinv.dat[Oiv_INCLUDE],"F");
        }

        apoinv.AddNew();
        total += amt;
    }
    dtodb(total, &apinv, EIV_TOTAL);
    lset(apinv.dat[EIV_REM], strRem);
    apinv.SqlUpdate(FALSE);
}

void ExportArInvoiceItem(DB *df,DB *data)
{
    long Srec,irec,rec;
    double total=0;
	
    CString strRem = "";
    Srec = interval(&irec,df->dat[1],data,1);
    for(rec=0; rec<Srec; rec++) {
		
        ridx(irec+rec,data,1);
        double amt = data->val(Wi_SUBTOTAL);
        if(amt==0) continue;
        double tax = data->val(Wi_TAXAMT);
		
		lsetblank(&aroinv);
        fetch(&aroinv, data);
		lset(aroinv.dat[1], arinv.dat[1]);
		
        long xrec = qd(data->dat[Tr_PT], &prodtype);
        if(xrec) {
			strRem.TrimRight();
            if(strRem.GetLength()>0) strRem += ",";
            strRem += prodtype.dat[1];
            lset(aroinv.dat[Oiv_GLCODE],prodtype.dat[PT_GLCODE_PURCHASE]);
        }
        else {
            lset(aroinv.dat[Oiv_GLCODE],accpf.dat[PF_PURCHASE]);
        }
        xrec = qd(aroinv.dat[Oiv_GLCODE],&glcode);
		if(xrec==0) {
            lset(aroinv.dat[Oiv_GLCODE],accpf.dat[PF_PURCHASE]);
			xrec = qd(aroinv.dat[Oiv_GLCODE],&glcode);
		}
        lset(aroinv.dat[Oiv_NAME],glcode.dat[2]);
        dtodb(amt, &aroinv, Oiv_AMT);
        dtodb(tax, &aroinv, Oiv_VAT);
        double qty = data->val(Wi_NETWT);
        dtodb(qty, &aroinv, Oiv_QTY);
        if(qty) {
            dtodb(amt / qty, &aroinv, Oiv_PRICE);
        }
        if(data->Boolean(Wi_INCLUDE)) {
            lset(aroinv.dat[Oiv_INCLUDE],"T");
        }
        else {
            lset(aroinv.dat[Oiv_INCLUDE],"F");
        }
		
        aroinv.AddNew();
        total += amt;
    }
    dtodb(total, &arinv, EIV_TOTAL);
    lset(arinv.dat[EIV_REM], strRem);
    arinv.SqlUpdate(FALSE);
}
void TicketToApinv(DB *df)
{
    BOOL flag = dBfldLogic(df,D_CASHBILL);
    //if(flag) return;

    char key[25];

    strcpy(key,apinv.dat[1]);
    lset(key,profile.dat[Pro_PREFIX]);
    lset(&key[1],df->dat[1]);

    long xrec = apinv.Search(key,1);
    if(xrec){
        CString str;
        str.Format("select * from appaydat where invoice = %s",
           DbText(&apinv, 1, key));
        BOOL flag = IsSQLRSExisted(str);
        if(flag) return;

        str.Format("select * from apcrdat where invoice = %s",
           DbText(&apinv, 1, key));
        flag = IsSQLRSExisted(str);
        if(flag) return;

        str.Format("delete from aroinv where [%s] = %s",
            aroinv.fieldName[1],DbText(&apinv, 1, key));
        RunSql(str);

        str.Format("delete from apinv where [%s] = %s",
            apinv.fieldName[1],DbText(&apinv, 1, key));
        RunSql(str);
    }

    lsetblank(&apinv);
    fetch(&apinv, df);
    lset(apinv.dat[1],key);
    lset(apinv.dat[EIV_CUSTOMER],df->dat[D_DEBTOR]);
    lset(apinv.dat[EIV_TT],"I");
    lset(apinv.dat[EIV_CURRENCY],"RM");
    lset(apinv.dat[EIV_REM],"Purchase");
    dtodb(1, &apinv, EIV_RATE);
    apinv.AddNew();
    ExportApInvoiceItem(df, &ddata);
}

void TicketToArinvNew(DB *df)
{
    BOOL flag = dBfldLogic(df,D_CASHBILL);
    //if(flag) return;
	
    char key[25];
	
    strcpy(key,arinv.dat[1]);
    lset(key,profile.dat[Pro_PREFIX]);
    lset(&key[1],df->dat[1]);
	
    long xrec = arinv.Search(key,1);
    if(xrec){
        CString str;
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
    lset(arinv.dat[EIV_CUSTOMER],df->dat[D_CREDITOR]);
    lset(arinv.dat[EIV_TT],"I");
    lset(arinv.dat[EIV_CURRENCY],"RM");
    lset(arinv.dat[EIV_REM],"Sales");
    dtodb(1, &arinv, EIV_RATE);
    arinv.AddNew();
    ExportApInvoiceItem(df, &rdata);
}
void PRintReceiveSlip(long irec,FILE *ptr,DB *df)
{
#ifdef _DEBUG
    ptr = fopen("slip.txt", "w");
#endif //_DEBUG
    if(CompanyNo==4 || CompanyNo==6) {
        dopen(&report,"slip_cf.rep");
        PrintStoreReceipt(irec,ptr,df,1);
        fprintf(ptr,"");
        dclose(&report);
    }
    else if(CompanyNo==7 || CompanyNo==8) {
         PRintReceiveSlipAikha(irec,ptr,df);
    }
    else {
        dopen(&report,"slip_do.rep");
        PrintStoreReceipt(irec,ptr,df,0);
        fprintf(ptr,"");
        dclose(&report);

        int fld = GetDbFieldNo(&accpf,"GST_DATE");
        if(fld>0) {
            if(strcmp(df->dat[D_DATE], accpf.dat[fld])<0) {
                fld = 0;
            }
        }
        if(fld>0) {
            PRintReceiveSlipKS(irec,ptr,df);
        }
        else {
			dopen(&report,"slip.rep");
            PrintStoreReceipt(irec,ptr,df,1);
            fprintf(ptr,"");
            dclose(&report);
        }
    }
#ifdef _DEBUG
    fclose(ptr);
    junction("slip.txt", 0);
#endif //_DEBUG

    long Srec = interval(&irec,df->dat[1],&rdata,1);
    if(Srec>=1) {
        TicketToArinvNew(df);
    }
}
