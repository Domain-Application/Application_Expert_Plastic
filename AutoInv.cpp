// AutoInv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "AutoInv.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoInv dialog


CAutoInv::CAutoInv(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
: CTio(IDD,pdf,Idx,p,pParent)

{
	//{{AFX_DATA_INIT(CAutoInv)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAutoInv::DoDataExchange(CDataExchange* pDX)
{
	CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoInv)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoInv, CTio)
	//{{AFX_MSG_MAP(CAutoInv)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoInv message handlers

void CAutoInv::GetInvoice() 
{
	CString strTable,strSql;
	DB tinv;
	if(!stricmp(main->name,"pinv")) {
		strTable = "dsinv";
	}
	else{
		strTable = "dpinv";
	}
	
    strSql.Format("select * from [%s] where [accode]='%s' and [date] <= %s and [DO] IS NULL order by ticket",
		strTable,main->dat[IV_ACCODE], DbText(df,1,df->dat[1]));

	ado(&tinv,"",NULL,strSql);
    long Srec = read_sumindex(0,&tinv);
	for(long rec=1;rec<=Srec;rec++) {
		readrec(rec,&tinv);
        lset(data->dat[1], main->dat[1]);
        data->Find_SequenceNo();
        lset(data->dat[VI_DO], tinv.dat[8]);	//PRIMARY KEY
        rset(data->dat[VI_QTY], tinv.dat[5]);
        rset(data->dat[VI_AMOUNT], tinv.dat[6]);
        data->AddNew();        
    }
	dclose(&tinv);
}


void CAutoInv::OnPrint() 
{
    ReplaceRecord();
    m_ToolBar.HideButton(IDCANCEL,TRUE);
    m_ToolBar.HideButton(ID_PRINT,TRUE);
    if(CollectData==NULL) {
        BeginWaitCursor();
		GetInvoice();
        EndWaitCursor();
        OnOK();
        return;
    }

}


