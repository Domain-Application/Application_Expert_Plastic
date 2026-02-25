// ExpAP.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "PickByDate.h"
#include "openrs.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPickByDate dialog


CPickByDate::CPickByDate(CWnd* pParent /*=NULL*/)
    : CDialog(CPickByDate::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPickByDate)
    m_d1 = COleDateTime::GetCurrentTime();
	m_d2 = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CPickByDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPickByDate)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_d1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_d2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPickByDate, CDialog)
    //{{AFX_MSG_MAP(CPickByDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPickByDate message handlers

void CPickByDate::OnOK()
{
	if(!isblank(df->dat[IQ_PAYDATE]))
	{
		AfxMessageBox("This invoice already paid, cannot pick ticket.");
		return;
	}
    UpdateData();

    lset(two.dat[1], m_d1);
    lset(two.dat[2], m_d2);

    CString str;
    str.Format("select * from [rdata_unbill_data] where [creditor]=%s and date>='%s' and [date]<= '%s' order by ticket",
       DbText(df, IQ_CREDITOR), two.dat[1], two.dat[2]);

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

    long xrec = viewidx(&receipt);
    if(xrec>0) {
        DoExportTicketToAR();
    }

	CDialog::OnOK();
}

void CPickByDate::DoExportTicketToAR()
{
    for(;;) {
        if(receipt.t->sumrecord<=0) break;
        readrec(1,&receipt);
        double offset = receipt.val(RE_AMOUNT);
        if(offset==0) return;

        lsetblank(sub);
        fetch(sub,&receipt);
        lset(sub->dat[1],df->dat[1]);
        double wt = receipt.val(RE_QTY) / price_unit;
        dtodb(wt, sub, Tr_QTY);
        sub->AddNew();
        deleterec(1, &receipt);
    }
}
