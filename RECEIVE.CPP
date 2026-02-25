// Receive.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "Receive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReceive dialog


CReceive::CReceive(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CWb(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CReceive)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    df->m_WhileEditNoPrint = TRUE;
}

void CReceive::DoDataExchange(CDataExchange* pDX)
{
    CWb::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CReceive)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReceive, CWb)
    //{{AFX_MSG_MAP(CReceive)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CReceive::BlockByOtherCondition()
{
    CString str,strSql;

    str = profile.dat[Pro_PREFIX];
    str.TrimRight();
    str += df->dat[1];
    str.TrimRight();

    strSql.Format("select count(*) from [appaydat] where [invoice]='%s'", str);
    double dCount = SqlValue(strSql);

    if(dCount>0) {
        MessageBox("Block");
        return TRUE;
    }
    return CWb::BlockByOtherCondition();
}
/////////////////////////////////////////////////////////////////////////////
// CReceive message handlers
double m[15];
void compute_weight(DB *df)
{
    memset(m,0,sizeof(m));
    CString str;
    str.Format("select sum(netwt) from [%s] where [%s] = %s",
       df->e->chain->m_strTable, df->e->chain->fieldName[1], DbText(df,1));
    m[11] = SqlValue(str);
}

void CReceive::OnButton1()  //Brian //Last Transaction button 20230601
{		
	  DB tmpdb(IDD_BROWSE,10);
	 CString str1,str2,str3,strSql,strCount,Result;
	str1 = df->dat[D_TICKET];
	str2=rdata.dat[Wi_TICKET];
	str3=rdata.dat[Wi_SQNO];
	strSql.Format("SELECT TICKET FROM receive_completed where LORRY='%s' AND TICKET <>'%s'",df->dat[D_LORRY],df->dat[1]);
	Result= SqlText(strSql);
	strSql.Format("SELECT * FROM receive where TICKET='%s'",Result);
	/*
		select A.ticket from receive as A 
		left outer join rdata as B on A.ticket = B.ticket
		where A.date = '2023-06-02' GROUP BY A.ticket having count (B.SQNO) % 2 = 0;
			*/
			//qd(df->dat[1],&receive);
			ado(&tmpdb,"",NULL,strSql);
			long rec = viewidx(&tmpdb);
			dclose(&tmpdb);
}

void ReplaceReceiveGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);

    df->Search(df->dat[1], 1, TRUE);
    g.SetText(df,0,1,D_DATE,D_DEBTOR,D_DEBTOR | 0x100, D_LORRY,D_DO,-1);
    compute_weight(df);
    g.SetText(m[11],6,0);
    CString str;
    str.Format("select sum(amount) from [%s] where [%s]=%s",
            df->e->chain->m_strTable, df->e->chain->fieldName[1],
            DbText(df, 1));
    double amt = SqlValue(str);
    g.SetText(amt, 10, 2);
}


void SetReceiveGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,7);

    g.ratio = float(1.0);
    g.SetTitle(df,0,1,D_DATE,D_DEBTOR,-1);
    g.SetTitle("Name",3000);
    g.SetTitle(df,D_LORRY,D_DO,-1);
    g.SetTitle("Net Weight",1000,7);
    g.SetTitle("Net Amount",1000,7);
}

int WB_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = D_MODIFY;
    fld[1] = D_COMPLETED;
    if(atoi(user.dat[UR_LEVEL])<5) {
    }
    fld[2] = 0;
	return 1;
}

int tell_receive(UINT nChar,int &id,int &fld,DB *df,int sq)
{
	switch(fld) 
	{ 
    case D_LORRY:
		{
			if(sq) 
			{
			if(!strncmp(df->dat[1],"NEW",3) && !strcmp(df->name,"RECEIVE"))
			{
				CString strSql,getResult,str1;
				str1=df->dat[D_LORRY];
				strSql.Format("SELECT TICKET FROM receive_completed where LORRY='%s'",df->dat[D_LORRY]);
				getResult=SqlText(strSql);
				strSql.Format("SELECT * FROM receive where TICKET='%s'",getResult);
				lset((df->dat[D_TRAILER]),SqlText(strSql,D_TRAILER));
				lset((df->dat[D_CONTAINER]),SqlText(strSql,D_CONTAINER));
				lset((df->dat[D_DEBTOR]),SqlText(strSql,D_DEBTOR));
				lset((df->dat[D_TRANSP]),SqlText(strSql,D_TRANSP));
				lset((df->dat[D_ICNO]),SqlText(strSql,D_ICNO));
				lset((df->dat[D_TENDER]),SqlText(strSql,D_TENDER));
			}		
				long rec = qd(df->dat[fld],&lorry);
				if(rec>0) 
				{
					fetch(df,&lorry);
				}
			}
			break;
		}
	default:
		break;
    }
    return 2;
}

int start_receive(DB *df)
{
    dbasedate(df->dat[D_DATE]);
    _strtime(df->dat[D_TIME]);
    lset(df->dat[D_CASHBILL],"Y");
    lset(df->dat[D_RA],"Y");
    return 1;
}


int receive_save(DB *df,int flagi)
{
    return 1;
}
int DeleteReceive(long irec,DB *df,int IdxItem) //23062023 Brian Delete ticket alert
{
	df->ReOpen();
	lsetblank(&tritem);
	long xrec = tritem.Search(df->dat[1],Tr_TICKET);
	
	if(xrec>0)
	{
		AfxMessageBox("The invoice is exist. Press ok to continue." );
		if(AfxMessageBox("Confirm delete ticket?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 )==IDYES)
		{
			return Delete(irec,df,IdxItem);
		}
	}
	else
	{
		return Delete(irec,df,IdxItem);
	}
}

char *WbFieldName(DB *df,int fld)
{
    strcpy(DateStringFormat,DbFieldName(df,fld));
    return DateStringFormat;
}

void SetWBGridRows(CMSFlexGrid *grid,DB *df)
{
    grid->SetRows(15);
}

void init_wb(DB *df)
{
    df->e->SetGridRows   = SetWBGridRows;
    df->e->rep       = ReplaceReceiveGrid;
    df->e->GridTitle = SetReceiveGridTitle;
    df->e->tell      = tell_receive;
    df->e->start     = start_receive;
    df->e->save      = receive_save;
	df->e->Delete	 = DeleteReceive;
    df->e->initial   = WB_initial;
  //  df->e->LongFieldName = WbFieldName;
	
	
}

void Receive_Capacity()
{
    CReceive p1(IDD_WB,&receive,1);
    p1.sub     = &rdata;
    p1.IDD_SUB = IDD_DDATA;
    p1.m_WinPrint=FALSE;
    p1.ReqitemStatus = 2;
    p1.m_PreviewCrystalReport = FALSE;
    p1.m_PrintOneFormOnly = TRUE;
    p1.bWtInc = FALSE;
    p1.m_UseInternalSortFields = FALSE;
    p1.period = &period;
    p1.fldPeriod       = D_DATE;
    p1.Prd_D1          = Prd_D1;
    p1.Prd_D2          = Prd_D2;
    p1.Prd_STATUS      = Prd_STATUS;
    p1.iRA = Pro_RA;
    p1.DoModal();
}

int Receive_AdminDb(DB *df,int flag)
{
    Receive_Capacity();
    return 1;
}


BOOL AUTOChangePrefix2(DB *df)
{
    CString str,s;
	
    int year,month,day,week;
    gdate(&year,&month,&day,&week);
	
    s.Format("SR%2.2i%2.2i-", year-2000, month);
	
	
	str.Format("select count(*) from ticketno where [prefix]='%s' and [RECORDSET]='%s'",
		s,df->m_strTicketNo);
	
    int no = SqlValue(str);
    if(no>0) return FALSE;
	
    str.Format("update [ticketno] set [prefix]='%s', [NEXT NO]=1 where [RECORDSET]='%s'", 
		s,df->m_strTicketNo);
	RunSql(str);
	
	
	
	return TRUE;
}


void init_receive(DB *df)
{
    init_wb(df);
//    df->e->prec = PRintReceiveSlip;
    df->e->AdminDb  = Receive_AdminDb;
	df->e->chain = &rdata;
//	df->e->ChangePrefix = AUTOChangePrefix2;	
}
