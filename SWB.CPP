// Swb.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Swb.h"
#include "fgftp.h"
#include "PrintProgress.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwb dialog


CSwb::CSwb(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CSwb)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_WinPrint = m_OpenLpt = FALSE;
	ExtraButton1 = IDC_BUTTON1;
	ExtraButton2 = IDC_BUTTON2;
}

void CSwb::ShowInformation()
{
     CStaff::ShowInformation();
     double wt = fabs(df->val(Sw_NET) - df->val(Sw_TYPE));
     char szWeight[10];
     dtoa(wt, szWeight, 6, 0);
     SetDlgItemText(IDC_NW, putnum(szWeight));
}

void CSwb::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSwb)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSwb, CStaff)
	//{{AFX_MSG_MAP(CSwb)
	ON_BN_CLICKED(IDC_BUTTON1, OnCaptureWeight)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwb message handlers

void CSwb::OnCaptureWeight() 
{
    CFgftp h1;
    if(dBfldLogic(&profile,Pro_TCPIP)==0) {
        h1.DirectRead = TRUE;
    }
    int flag = h1.DoModal();
    if(flag==IDOK) {
        ltodb(h1.weight, df, Sw_NET);
        ShowInformation();
        return;
	}
}

enum pswb {Sl_TITLE=1,Sl_COADD1,Sl_COADD2,Sl_COADD3,Sl_COADD4,Sl_GSTNO,
           Sl_TICKET,Sl_LORRY,Sl_DATE,Sl_ACCODE,Sl_ACNAME,Sl_PT,Sl_PTNAME,
           Sl_STATUS,Sl_GST_FLAG,Sl_FIRST,Sl_SECOND,Sl_WASTAGE,Sl_NET,
           Sl_PRICE,Sl_AMT,Sl_GST,Sl_NAMT,Sl_TIMEIN,Sl_TIMEOUT,Sl_REM,
           Sl_W1,Sl_W2,Sl_Z1,Sl_Z2,Sl_PDATE,Sl_PTIME};
/*
void pswb(long irec,FILE *ptr1,DB *df)
{
    dopen(&report,"pswb.rep");
    fetch(&report,df);
    lset(report.dat[Sl_TITLE],logo.dat[Logo_NAME]);
    lset(report.dat[Sl_COADD1],logo.dat[Logo_ADD1]);
    lset(report.dat[Sl_COADD2],logo.dat[Logo_ADD2]);
    lset(report.dat[Sl_COADD3],logo.dat[Logo_ADD3]);
	lset(report.dat[Sl_COADD4],logo.dat[Logo_ADD4]);

    int flag = printerr();
    if(!(flag == 0x80 || flag == 0x90)) {
        AfxMessageBox("nothing to do");
        return;
    }
    FILE *ptr = fopen("lpt1","w");
    double w1 = df->val(Sw_NET);
    double w2 = df->val(Sw_TYPE);
    double gw = max(w1,w2);
    double tw = min(w1,w2);
    double nw = gw - tw;
    dtodb(gw, &report, Sl_FIRST);
    dtodb(tw, &report, Sl_SECOND);
    dtodb(nw, &report, Sl_NET);

    split(&report,Sl_REM, 3, df->dat[Sw_REM]);
    mprint(ptr,&report);
    fprintf(ptr,"");

    fclose(ptr);
    dclose(&report);
}

void PrintSingleWeighingTicket(long irec,FILE *ptr,DB *df)
{
    ptr = topen("lpt1");
    dopen(&report,"pswb.rep");
    fetch(&report,df);
    lset(report.dat[Sl_TITLE],logo.dat[Logo_NAME]);
    lset(report.dat[Sl_COADD1],logo.dat[Logo_ADD1]);
    lset(report.dat[Sl_COADD2],logo.dat[Logo_ADD2]);
    lset(report.dat[Sl_COADD3],logo.dat[Logo_ADD3]);
	lset(report.dat[Sl_COADD4],logo.dat[Logo_ADD4]);

    double w1 = df->val(Sw_NET);
    double w2 = df->val(Sw_TYPE);
    double gw = max(w1,w2);
    double tw = min(w1,w2);
    double nw = gw - tw;
    dtodb(gw, &report, Sl_FIRST);
    dtodb(tw, &report, Sl_SECOND);
    dtodb(nw, &report, Sl_NET);

    split(&report,Sl_REM, 3, df->dat[Sw_REM]);
    fprintf(ptr,"2C!");
    mprint(ptr,&report);
   
    fprintf(ptr,"");
   

    fclose(ptr);
    dclose(&report);
   
}
*/

void PrintSwbTicket(FILE *ptr,DB *df,char *rep)
{
    dopen(&report,rep);
    fetch(&report,df);
    lset(report.dat[Sl_TITLE],logo.dat[Logo_NAME]);
    lset(report.dat[Sl_COADD1],logo.dat[Logo_ADD1]);
    lset(report.dat[Sl_COADD2],logo.dat[Logo_ADD2]);
    lset(report.dat[Sl_COADD3],logo.dat[Logo_ADD3]);
	lset(report.dat[Sl_COADD4],logo.dat[Logo_ADD4]);
    lset(report.dat[Sl_GSTNO],profile.dat[Pro_GSTNO]);
	lset(report.dat[Sl_ACNAME], df->dat[Sw_ACNAME]);
	/*
    if(df->name[0]=='S' || df->name[0]=='s') {
        qd(df->dat[Sw_CREDITOR], &debtor);
        lset(report.dat[Sl_ACNAME], debtor.dat[2]);
    }
    else {
        qd(df->dat[Sw_CREDITOR], &creditor);
        lset(report.dat[Sl_ACNAME], creditor.dat[2]);
    }
	*/
	double w1 = df->val(Sw_NET);
	double w2 = df->val(Sw_TYPE);
//	double wastage = df->val(Sw_WASTAGE);
	double nw = fabs(w1-w2) ;//- wastage;
	if(!w1 || !w2) nw = 0;
	
   // dtodb(nw, &report, Sl_NET);

	
    double gw = max(w1,w2);
    double tw = min(w1,w2);
    double nw2 = gw - tw;
    dtodb(gw, &report, Sl_FIRST);
    dtodb(tw, &report, Sl_SECOND);
    dtodb(nw2, &report, Sl_NET);

	
	
    split(&report,Sl_REM, 3, df->dat[Sw_REM]);
    fprintf(ptr,"2C!");
    mprint(ptr,&report);
    fprintf(ptr,"");
    dclose(&report);
}

void PrintSingleWeighingTicket(long irec,FILE *ptr,DB *df)
{
    
    int preview = 0;
    ptr = fopen("lpt1","w");
    if(!ptr) {
        preview = 1;
        ptr = fopen("pswb.txt","w");
    }

        PrintSwbTicket(ptr,df,"pswb_inv.dbf");
    
	
    fclose(ptr);
    if( preview ) {
        junction("pswb.txt",0);
    }
}

int start_swb(DB *df)
{
    dbasedate(df->dat[Sw_DATE]);
    _strtime(df->dat[Sw_TIME]);
    

//	lset(df->dat[Sw_TAXCODE],"SR");
    qd(df->dat[Sw_TAXCODE],&taxcode);
	
    double percen = taxcode.val(3);
    int fld = GetDbFieldNo(&accpf,"GST_DATE");
    if(fld>0) {
        if(strcmp(df->dat[Sw_DATE], accpf.dat[fld])<0) {
            percen = 0;
        }
    }
    dtodb(percen, df, Sw_PERCEN);
    return 1;
}

int swb_AdminDb(DB *df,int flag)
{
    CSwb p1(df->ID,df,df->f[1].idx & 0x01);
    p1.m_WinPrint = FALSE;
    p1.DoModal();
    return 1;
}

int tell_swb(UINT nChar,int &flag,int &fld,DB *df,int sq)
{

	switch(fld) {
    case Sw_TAXCODE:
		if(sq){
			int fld2 = GetDbFieldNo(&accpf,"GST_DATE");
			if(fld2>0) {
                if(strcmp(df->dat[Sw_DATE], accpf.dat[fld2])>0) {
                    qd(df->dat[Sw_TAXCODE], &taxcode);
					double percen = taxcode.val(3);
                    dtodb(percen, df, Sw_PERCEN);
				}
			}	
		}
        break;
    }
    double total = df->val(Sw_CHARGE);
	
    double percen = df->val(Sw_PERCEN);
    double taxamt = 0;
    double subtotal = total;
    if(df->Boolean(Sw_INCLUSIVE)) {
        taxamt = total * percen / (100 + percen);
    }
    else {
        taxamt = total * percen / 100;
    }
    dtodb(taxamt, df, Sw_TAXAMT);
    if(!df->Boolean(Sw_INCLUSIVE)) {
        subtotal = total + df->val(Sw_TAXAMT);
    }
    dtodb(subtotal,df,Sw_SUBTOTAL);
    return 2;
}

void init_swb(DB *df)
{
    df->e->AdminDb = swb_AdminDb;
//    df->e->prec    = PrintSingleWeighingTicket;
    df->e->tell    = tell_swb;
    df->e->start   = start_swb;
}

void SingleWeighing()
{
    swb_AdminDb(&swb,0);
}

void CSwb::OnButton2() 
{
    CFgftp h1;
    if(dBfldLogic(&profile,Pro_TCPIP)==0) {
        h1.DirectRead = TRUE;
    }
    int flag = h1.DoModal();
    if(flag==IDOK) {
        ltodb(h1.weight, df, Sw_TYPE);
        ShowInformation();
        return;
	}
}
