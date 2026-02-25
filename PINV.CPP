// PINV.CPP : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "PINV.H"
#include "db_ado.h"
#include "openrs.h"
#include "AutoInv.h"
#include "PurPickByDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BOOL Sales_Flag;
extern void InvToAp();
/////////////////////////////////////////////////////////////////////////////
// CPurInv dialog


CPurInv::CPurInv(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
: CParent(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CPurInv)
		// NOTE: the ClassWizard will add member initialization here
	m_d1 = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	ExtraButton1 = IDC_BUTTON1;
//	ExtraButton2 = IDC_BUTTON2;
    ExtraButton3 = IDC_BUTTON3;
	ExtraButton4 = IDC_BUTTON4;
//    m_ChangeOrder = TRUE;
//    m_WinPrint = FALSE;
//    m_OpenLpt  = FALSE;
    m_LoopPrint = TRUE;
    df->m_WhileEditNoPrint = TRUE;

}


void CPurInv::DoDataExchange(CDataExchange* pDX)
{
	CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPurInv)
	DDX_DateTimeCtrl(pDX, IDC_EDIT12, m_d1);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_grid);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPurInv, CParent)
	//{{AFX_MSG_MAP(CPurInv)
	ON_BN_CLICKED(IDC_BUTTON1, OnCreateNewInvoice)
	//ON_BN_CLICKED(IDC_BUTTON2, OnAutoCreateInvoice)
	ON_BN_CLICKED(IDC_BUTTON3, OnPurPickByDate)
	ON_BN_CLICKED(IDC_BUTTON4, OnUpdatePayStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurInv message handlers

BEGIN_EVENTSINK_MAP(CPurInv, CParent)
//{{AFX_EVENTSINK_MAP(CPurInv)
ON_EVENT(CPurInv, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CPurInv::IsCannotEdit()
{
	CString strTable = "apinv" ;
	if(!stricmp(sub->name,"pinv")) {
		strTable = "arinv";
	}
	
    if(UserLevel()>=999) return FALSE;
    DB unbill;
    CString strSql;
    strSql.Format("Select top 1 * from dbo.[%s] where refno = '%s'",strTable,df->dat[1]);
    ado(&unbill,"unbill",NULL,strSql);
    long Srec = read_sumindex(0,&unbill);
    if (Srec) {		
        waitmsg("Invoice Already Post to ACC System, cannot edit");
        return TRUE;		
	}
    else{
		return FALSE;
	}
}

void CPurInv::OnDetail()
{
	CString str1,str2,strSql,strSql2,strCheck,Status;
	str1=df->dat[1];
	str2=df->dat[PIQ_PAYDATE];
	strCheck.Format("SELECT PAYSTATUS FROM puritran WHERE REFNO = '%s'",str1);
	Status=SqlText(strCheck);
	int	Result= SqlValue(strSql2);
	
	lsetblank(sub);
	sub->e->start(sub);                             //initial setting

	if(!isblank(str2))
	{
		AfxMessageBox("This invoice already paid.\nNew ticket are not able to add.");
		return;
	}
	else if(isblank(str2))
	{
		CParent::OnDetail();
	}
	ReFreshSubFile(); //          m_NeedShowDetail = TRUE;
	ShowInformation();
}

void CPurInv::ShowInformation()
{
    
    ReFreshSubFile();
    CParent::ShowInformation();

    CString str;
    str.Format("Select sum([amount]) from [%s] where [%s]=%s",
        sub->name, sub->fieldName[1], DbText(df, 1));
    double amt = SqlValue(str);
    char szTmp[20];
    dtoa(amt, szTmp, 13, 2);
    SetDlgItemText(IDC_TOTAL13, putnum0(szTmp));

	if(!isblank(df->dat[PIQ_PAYDATE]))
	{
		SetDlgItemText(IDC_BUTTON4,"Set Unpaid Payment");
	}
	else
	{
		SetDlgItemText(IDC_BUTTON4,"Set Paid Payment");
	}
	
}

void CPurInv::ShowInitFrame()
{
    fg.Initial(&m_grid,sub,2, float(1.15) );

	fg.SetTitle("No",500,1);
    fg.SetTitle("Ticket No",1200,1);
    fg.SetTitle("D/O No",1200,1);
    fg.SetTitle("Date",1200,1);
    fg.SetTitle("Description",3200,1);
    fg.SetTitle("Qty(MT)",960,7);
    fg.SetTitle("U/Price",960,7);
    fg.SetTitle("Amount (RM)",1180,7);
   // fg.SetTitle("",80);
    fg.SetTitle("Tax Code",1000);
    fg.SetTitle("Inc Tax",800);
    fg.SetTitle("SST %",800,7);
    fg.SetTitle("SST Amt",900,7);
    fg.SetTitle("Sub Total",1100,7);
}

void CPurInv::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    fg.show(rec);
    fg.SetText(sub,0,Tr_TICKET,Tr_DO,Tr_DATE,-1);
    fg.SetText(sub,Tr_NAME,Tr_QTY,Tr_PRICE,Tr_AMOUNT,-1);

    //fg.SetText("");
    fg.SetText(sub,Tr_TAXCODE,Tr_INCLUDE,Tr_PERCEN,Tr_TAXAMT,Tr_SUBTOTAL,-1);
}

int AdminDb_purinv(DB *df,int flag)
{
    PurchaseInvoice();
    return 1;
}

int tell_purinvitem(UINT nChar,int &id,int &fld,DB *df,int sq)
{
	
    switch(fld) {
    case PVI_DO:
        id = DO;
        break;
	default:
		break;
    }

    return 2;
}

int start_purinv(DB *df)
{
	lset((df->dat[PIQ_PAYSTATUS]),"Unpaid");
    df->FldDefault(2);
	return 1;
}

int purinv_initial(DB *df,int *fld)                //initial setting
{	
		fld[0] = PIQ_PAYSTATUS;
		fld[1]= PIQ_PAYDATE;
    if(atoi(user.dat[UR_LEVEL])<9) {
        fld[2] = PIV_DATE;
		fld[3] = 0;
        
    }
    return 1;
}

void init_purinv(DB *df)
{
    df->e->AdminDb = AdminDb_purinv;
    df->e->start   = start_purinv;
    df->e->initial = purinv_initial;
//    df->e->prec    = Pitran;
}

void init_purinvitem(DB *df)
{
    df->e->tell = tell_purinvitem;
}

void CPurInv::GetInvoiceInformation(DB* df)
{
    CString str;
    str.Format("select * from [ddata_unbill_data] where [debtor]=%s order by ticket",
    DbText(df, PIQ_DEBTOR));
	//AfxMessageBox(IQ_CREDITOR);
	//DbText(df, PIQ_CREDITOR));

    DB adoTmp;
    ado(&adoTmp, "", NULL, str);

    memstart(&receipt);
    double bal = 0;
    for(long rec=1; rec<= adoTmp.t->sumrecord; rec++) {
        adoTmp.read(rec);
        fetch(&receipt, &adoTmp);
        bal += atof(receipt.dat[RE_AMOUNT]);
        dtodb(bal,&receipt,RE_BAL);
        memwrt(&receipt);
    }
    memend(&receipt);
    dclose(&adoTmp);
}

void CPurInv::OnCreateNewInvoice() 
{
	if(!isblank(df->dat[PIQ_PAYDATE]))
	{
		AfxMessageBox("This invoice already paid, Cannot pick ticket.");
		return;
	}
    for(;;) {
        GetInvoiceInformation(df);
        if(receipt.t->sumrecord<=0) break;
        rec = viewidx(&receipt);
        if(rec==0) break;
        double offset = receipt.val(RE_AMOUNT);
        if(offset==0) return;

        if(SaveMainInformation()==FALSE){
            return;
        }
        mrec = appnet(sub);
        fetch(sub,&receipt);
        lset(sub->dat[1],df->dat[1]);
        double wt = receipt.val(RE_QTY) / price_unit;
        dtodb(wt, sub, Tr_QTY);
		
        sub->AddNew();
        ReFreshSubFile();
        ShowInformation();
    }
}
void CPurInv::OnUpdatePayStatus() 
{
	CString str1,str2,strSql2;
	str1=df->dat[1];
	str2=df->dat[PIQ_PAYDATE];
	strSql2.Format("SELECT COUNT(ticket) FROM tritem WHERE REFNO = '%s'",str1);
	int	Result= SqlValue(strSql2);
	if(isblank(str2))
	{
		if(AfxMessageBox("Confirm update payment status to PAID?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 )==IDYES)
		{
			if(Result>0)
			{
				lset((df->dat[PIQ_PAYSTATUS]),"Paid");
				lset((df->dat[PIQ_PAYDATE]),m_d1);
				df->Update();
			}
			else if(Result==0)
			{
				AfxMessageBox("Cannot update invoice with empty ticket");
				return;
			}	
			
		}
		
	}
	if(!isblank(str2))
	{
		if(AfxMessageBox("Confirm set payment status to UNPAID?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 )==IDYES)
		{
			lset((df->dat[PIQ_PAYSTATUS]),"Unpaid");
			lset((df->dat[PIQ_PAYDATE]),"");
			df->Update();
		}
	}
	ShowInformation();
}

void CPurInv::OnDblClickMsflexgrid1() 
{
    CString str1,str2,strSql,strSql2,strCheck,Status;
	str1=df->dat[1];
	str2=df->dat[PIQ_PAYDATE];
	strCheck.Format("SELECT PAYSTATUS FROM puritran WHERE REFNO = '%s'",str1);
	Status=SqlText(strCheck);
	int	Result= SqlValue(strSql2);

	if(!isblank(str2))
	{
		AfxMessageBox("This invoice already paid.\nPlease change to unpaid to edit the ticket");
		return;
	}
	if(isblank(str2))
	{
		CParent::OnDblClickMsflexgrid1();
	}

}
void CPurInv::OnAutoCreateInvoice() 
{
    long xrec = apinv.Search(df->dat[1],1);
    if(xrec) {
        MessageBox("Invoice existed");
        return;
    }

    InvToAp();
}

void PurchaseInvoice()
{
    Sales_Flag = FALSE;
    CPurInv p1(IDD_PINV,&puritran,1);
    p1.sub       = &tritem;
    p1.IDD_SUB   = IDD_PINVITEM;
	p1.m_WinPrint = TRUE;
    p1.m_PrintOneFormOnly = TRUE;
    p1.DoModal();

}

void CPurInv::OnPurPickByDate() 
{
    if(SaveMainInformation()==FALSE){
        return;
    }
    CPurPickByDate h1;
    h1.df  = df;
    h1.sub = sub;
    h1.DoModal();
    ReFreshSubFile();
    ShowInformation();
}
