// Sic.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "Sic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DB sic;
/////////////////////////////////////////////////////////////////////////////
// CSic dialog


CSic::CSic(CWnd* pParent /*=NULL*/)
	: CDialog(CSic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSic)
	DDX_Control(pDX, IDC_PROGRESS2, m_p2);
	DDX_Control(pDX, IDC_PROGRESS1, m_p1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSic, CDialog)
	//{{AFX_MSG_MAP(CSic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSic message handlers

BOOL CSic::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSic::OnOK() 
{
    long last = read_sumindex(1,&prodtype);
    m_p1.SetRange32(1,last);
    for(long rec=1; rec<=last; rec++) {
//    for(long rec=3; rec<=3; rec++) {
        prodtype.read(rec);
        m_p1.SetPos(rec);
        OnSetCost();
    }
	CDialog::OnOK();
}

void CSic::OnGetCost()
{
    strSql.Format("select * from [incost] where pt = %s order by [date],[pkkey]",
        DbText(&prodtype,1));

    tmp.DefaultOrder = FALSE;
    ado(&tmp,"",NULL,strSql);

    FileName.Format("incost.rep");
    unlink(FileName);
    copyst(&tmp, FileName);
    dopen(&FifoTmp, FileName);
    long last = read_sumindex(1,&tmp);
    memstart(&FifoTmp);
    fetch_set(&FifoTmp, &tmp);
    for(long rec=1; rec<=last; rec++) {
        adoFastRead(rec,&tmp);
        fetch_rec(&FifoTmp, &tmp);
        memwrt(&FifoTmp);
    }
    dclose(&tmp);
    memend(&FifoTmp);
}

double CSic::GetFIFOAMT(double ReqQty)
{
    if(ReqQty==0) return 0;
    long f1 = 1;
    long f2 = FifoTmp.t->sumrecord;
    if(f1==0 || f2==0) return 0;

    double amt = 0;

    for(long rec=f1; rec<=f2; rec++) {
        readrec(rec,&FifoTmp);
        if(FifoTmp.dat[0][0] == '*') continue;
        double in_qty = FifoTmp.val(3);
        if(in_qty == 0) continue;
        double price = FifoTmp.val(4) / FifoTmp.val(3);
        if(in_qty >= ReqQty) {
            amt += ReqQty * price;
            in_qty -= ReqQty;
            dtodb(in_qty, &FifoTmp, 3);
            if(in_qty==0) {
                FifoTmp.dat[0][0] = '*';
            }
			fastwrt(rec,&FifoTmp);
            str.Format("insert into [dcost] values (%i,%i,%9.2f,%9.2f)",
               atoi(outTmp.dat[2]),atoi(FifoTmp.dat[2]),
               ReqQty,ReqQty * price );
            RunSql(str);
            break;
        }
        else {
            ReqQty -= in_qty;
            amt += in_qty * price;
            dtodb(0, &FifoTmp, 3);
			fastwrt(rec,&FifoTmp);
            str.Format("insert into [dcost] values (%i,%i,%9.2f,%9.2f)",
               atoi(outTmp.dat[2]),atoi(FifoTmp.dat[2]),
               in_qty,in_qty * price );
            RunSql(str);
        }
    }
    return amt;
}

void CSic::OnSetCost()
{
    OnGetCost();

    strSql.Format("select * from [outqty] where [pt] = %s order by [date],[pkkey]",
        DbText(&prodtype,1));

    outTmp.DefaultOrder = FALSE;
    ado(&outTmp,"",NULL,strSql);
    long last = read_sumindex(1,&outTmp);
    m_p2.SetRange32(1,last);
    for(long rec=1; rec<=last; rec++) {
        adoFastRead(rec,&outTmp);
        m_p2.SetPos(rec);
        double ReqQty = outTmp.val(3);
        GetFIFOAMT(ReqQty);
    }
    dclose(&outTmp);

    dclose(&FifoTmp);
}

void SetItemCost()
{
    CSic p1;

    p1.DoModal();
}
