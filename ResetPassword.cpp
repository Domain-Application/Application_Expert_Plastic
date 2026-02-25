// ResetPassword.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "ResetPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResetPassword dialog


CResetPassword::CResetPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CResetPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResetPassword)
	m_password = _T("");
	//}}AFX_DATA_INIT
}

void CResetPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResetPassword)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_EDIT1, m_password);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResetPassword, CDialog)
	//{{AFX_MSG_MAP(CResetPassword)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResetPassword message handlers

void CResetPassword::RunSql(CString str)
{
    m_progress.StepIt();
    BOOL flag = ::RunSql(str);
    if(flag==FALSE) {
        int len = str.Find("delete from");
        if(len>=0) {
            char szText[1024];
            strcpy(szText,str);
            replace(szText,"delete from","delete from");
            ::RunSql(szText);
		}
    }
}

void CResetPassword::OnOK() 
{
    UpdateData();

    if(m_password.Compare("JOYCE-ACC")) {
        MessageBox("Incorret password");
        return;
    }
    m_progress.SetRange(1,50);

    str = "delete from [DCOST]";
    RunSql(str);
    str = "delete from [DDATA]";
    RunSql(str);
    str = "delete from [RDATA]";
    RunSql(str);
    str = "delete from [RECEIVE]";
    RunSql(str);
    str = "delete from [DESPATCH]";
    RunSql(str);
    str = "delete from [SWB]";
    RunSql(str);
    str = "delete from [ADJITEM]";
    RunSql(str);
    str = "delete from [ADJ]";
    RunSql(str);
    str = "delete from [SPITEM]";
    RunSql(str);
    str = "delete from [CPITEM]";
    RunSql(str);
    str = "delete from [PT]";
    RunSql(str);
    str = "delete from [DRIVER]";
    RunSql(str);
    str = "delete from [TRANSP]";
    RunSql(str);
	str = "delete from [PTYPE]";
    RunSql(str);
	str = "delete from [INVITEM]";
    RunSql(str);
	str = "delete from [INV]";
    RunSql(str);
	str = "delete from [PINVITEM]";
    RunSql(str);
	str = "delete from [PINV]";
    RunSql(str);

    str = "update [ticketno] set [next no] = 1 ";
    RunSql(str);
	
    MessageBox("Reset database completed");

	CDialog::OnOK();
}

void ResetDatabase()
{
    CResetPassword h1;

    h1.DoModal();
}

void CResetPassword::ImportGlgroup(char *source,char *dest, BOOL checkExist)
{
    CString name;
    name = app->internetFolder + source;

    dopen(&dbf, name);

    tado(&mdb,dest);
	int IdxItem = dbf.f[1].idx & 1;
    long last = read_sumindex(IdxItem,&dbf);
    m_progress.SetRange32(1,last);

    fetch_set(&mdb, &dbf);
    for(long rec=1; rec<=last; rec++) {
        ridx(rec,&dbf,IdxItem);
        m_progress.SetPos(rec);
        if(checkExist) {
            long xrec = mdb.Search(dbf.dat[1], 1);
            if(xrec) continue;
        }
        SetDlgItemText(IDC_MSG, dbf.dat[1]);

        fetch_rec(&mdb, &dbf);
        lset(mdb.dat[1], dbf.dat[1]);
        if(checkExist) {
            mdb.SqlUpdate(TRUE);
        }
        else {
            mdb.AddNew();
        }
    }

    int fld = GetDbFieldNo(&mdb, "CURRENCY");
    if(fld) {
        str.Format("Update [%s] set [currency] = 'RM' where [currency] is null",
           mdb.m_strTable);
        RunSql(str);
        str.Format("Update [%s] set [rate] = 1 where [rate] is null",
           mdb.m_strTable);
        RunSql(str);
    }

    dclose(&dbf);
    dclose(&mdb);
}

void CResetPassword::ImportInvData(char *source,char *dest)
{
    CString name;
    name = app->internetFolder + source;
    dopen(&dbf, name);

    tado(&mdb,dest);
    long last = read_sumindex(0,&dbf);
    m_progress.SetRange32(1,last);

    for(long rec=1; rec<=last; rec++) {
        readrec(rec,&dbf);
        if(dbf.dat[0][0]=='*') continue;
        m_progress.SetPos(rec);
        SetDlgItemText(IDC_MSG, dbf.dat[1]);

        double j= 1;
        int Ivv_GLCODE1 = GetDbFieldNo(&dbf, "PT1");
        int Ivv_GLCODE7 = GetDbFieldNo(&dbf, "PT7");
        int Ivv_DATE = GetDbFieldNo(&dbf, "DATE");
        for(int i=Ivv_GLCODE1; i<=Ivv_GLCODE7; i+=10) {
            long xrec = qd(dbf.dat[i], &prodtype);
            if(xrec==0) continue;

            lset(mdb.dat[1], dbf.dat[1]);
            dtodb(j++,&mdb,2);

            lset(mdb.dat[Wi_PT], dbf.dat[i]);
            lset(mdb.dat[Wi_TIME], dbf.dat[i-2]);
            lset(mdb.dat[Wi_DATE], dbf.dat[Ivv_DATE]);

            double w1 = dbf.val(i-1);
            double w2 = dbf.val(i+9);
            dtodb(w1,&mdb,Wi_FIRST);
            dtodb(w2,&mdb,Wi_SECOND);

            for(int k=2; k<=5; k++) {
                double q = dbf.val(i+k);
                dtodb(q,&mdb,Wi_MP+k-2);
            }

            double qty = dbf.val(i+6);
            dtodb(qty,&mdb,Wi_NETWT);

            double price = dbf.val(i+1);
            dtodb(price,&mdb,Wi_PRICE);

            double amt = dbf.val(i+7);
            dtodb(amt,&mdb,Wi_AMOUNT);
            mdb.SqlUpdate(TRUE);
        }
    }

    dclose(&dbf);
    dclose(&mdb);
}


void CResetPassword::OnButton1() 
{
	int nID = MessageBox("Do you want to import data?","Question",MB_ICONQUESTION | MB_YESNO);
	if(nID==IDNO){
		OnCancel();
		return;
	}
    UpdateData();

    if(m_password.Compare("JOYCE-ACC")) {
        MessageBox("Incorret password");
        return;
    }

    ImportGlgroup("cpitem.dat","cpitem",FALSE);
    ImportGlgroup("spitem.dat","spitem",FALSE);

    ImportGlgroup("transp.dat","transp");
    ImportGlgroup("driver.dat","driver");
    ImportGlgroup("pt.dat","pt");
    ImportGlgroup("lorry.dat","lorry");
    ImportGlgroup("wb.dat","receive");
    ImportInvData("wb.dat","rdata");

    ImportGlgroup("dr.dat","despatch");
    ImportInvData("dr.dat","ddata");

    ImportGlgroup("itran.dat","itran");
	ImportGlgroup("puritran.dat","puritran");
    ImportGlgroup("tritem.dat","tritem",FALSE);
    FindTritemKey();
	
	MessageBox("****Import completed****");
    CDialog::OnOK();
}

void CResetPassword::FindTritemKey()
{
    ado(&mdb,"tritem");

    long last = read_sumindex(1,&mdb);
    m_progress.SetRange32(1,last);

    for(long rec=1; rec<=last; rec++) {
        mdb.read(rec);
        m_progress.SetPos(rec);
        SetDlgItemText(IDC_MSG, mdb.dat[1]);
        double wt = mdb.val(Tr_QTY) * price_unit;
        str.Format("select [pkkey] from [ddata] where ticket = '%s' and pt='%s' and [netwt]=%6.0f",
           mdb.dat[Tr_TICKET], mdb.dat[Tr_PT] , wt);
        double iKey = SqlValue(str);
		dtodb(iKey, &mdb, Tr_KEY);
		if(iKey==0) {
            str.Format("select [pkkey] from [ddata] where ticket = '%s' and [netwt]=%6.0f",
               mdb.dat[Tr_TICKET], wt);
            iKey = SqlValue(str);
            dtodb(iKey, &mdb, Tr_KEY);
            if(iKey==0) {
                str.Format("Invoice No: %s", mdb.dat[1]);
                MessageBox(str, "key not found");
                continue;
            }
		}
        mdb.SqlUpdate(FALSE);
    }
    dclose(&mdb);
}
