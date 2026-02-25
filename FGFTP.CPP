// Fgftp.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Fgftp.h"
#include "Wb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int oldfg_key;
/////////////////////////////////////////////////////////////////////////////
// CFgftp dialog


CFgftp::CFgftp(CWnd* pParent /*=NULL*/)
    : CConfirmWeight(CFgftp::IDD, pParent)
{
    //{{AFX_DATA_INIT(CFgftp)
	//}}AFX_DATA_INIT
    DirectRead = FALSE;
}

void CFgftp::DoDataExchange(CDataExchange* pDX)
{
    CConfirmWeight::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFgftp)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFgftp, CConfirmWeight)
    //{{AFX_MSG_MAP(CFgftp)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFgftp message handlers

int CFgftp::OnGw()
{
    if(DirectRead) {
        weight = GetCurrenctCommWeight(1);
    }
    else {
/*
        char szText[100];
        if(IndicatorNo == 1) {
            qmove(ptr,0);
        }
        else {
            qmove(ptr,7);
        }
        qread( ptr,szText,6);
        weight = atol(szText);  */

        CString errmsg,str,WeightServer;
        WeightServer = profile.dat[Pro_WTSERVER];
        WeightServer.TrimRight();
        int flag = ClientRequest(WeightServer,"W1",errmsg);
        weight = tcpWeight;
    }
#ifdef _DEBUG
//    weight  = 19000;
#endif //_DEBUG

    char tmp[80];
    sprintf(tmp,"%6.6ld kg",weight);
	m_wt.SetText(tmp);
    return TRUE;
}

