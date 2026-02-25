// ExpAP.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ExpAP.h"
#include "openrs.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpAP dialog


CExpAP::CExpAP(CWnd* pParent /*=NULL*/)
	: CDialog(CExpAP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpAP)
    m_d1 = COleDateTime::GetCurrentTime();
	m_d2 = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CExpAP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpAP)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_d1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_d2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpAP, CDialog)
	//{{AFX_MSG_MAP(CExpAP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpAP message handlers

void CExpAP::OnOK() 
{
    UpdateData();

    lset(two.dat[1], m_d1);
    lset(two.dat[2], m_d2);
    str.Format("select * from [despatch] where date >= '%s' and date <= '%s'",
        two.dat[1],two.dat[2]);
    ado(&dbToday,"despatch",init_despatch, str);

	long last = read_sumindex(1, &dbToday);
	m_progress.SetRange(1, last);
	for(long rec=1; rec<=last; rec++){
		dbToday.read(rec);
		m_progress.SetPos(rec);
		SetDlgItemText(IDC_TICKET, dbToday.dat[1]);
		DoExportTicketToAP();		
	}
    dclose(&dbToday);

	CDialog::OnOK();
}

void ExportPurchaseInvoiceTransactions()
{
	long Mrec = qd(accpf.dat[APF_PURCHASE],&glcode);
	if (Mrec<=0){		
		CString strMsg;
		strMsg.Format("Purchase G/L CODE At ACC Profile Not Found.\nPlease Check");
		waitmsg(strMsg);				
//		return;
	}

    CExpAP p1;

    p1.DoModal();
}

void CExpAP::DoExportTicketToAP()
{
    TicketToApinv(&dbToday);
}
