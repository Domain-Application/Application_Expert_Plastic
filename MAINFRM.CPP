// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "menu.h"
#include "MainFrm.h"
#include "Staff.h"
#include "about.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

extern void CheckUserRight(DB *user,int key);
IMPLEMENT_DYNCREATE(CMainFrame, CMyFrame)

BEGIN_MESSAGE_MAP(CMainFrame, CMyFrame)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_INITMENU()
    ON_COMMAND(ID_USER, OnUser)
    ON_COMMAND(ID00051, OnPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    CString str = profile.dat[Pro_BRANCH];

    str.TrimLeft();
    str.TrimRight();
    if(str.GetLength()>0) {
        m_System.Format("%s (%s)",title, str);
    }
    else {
        m_System.Format("%s",title);
    }
    strcpy(title,m_System);

    if(dBfldLogic(&profile,Pro_TCPIP)==0) {
        bCommOpen = TRUE;
    }
    app->m_auto_purge_logfiles_after_days = 14;
}

void CMainFrame::OnOpenOtherFiles()
{
    dopen(&one,"one.scr");
    dopen(&two,"two.scr");
}

void CMainFrame::OnOpenSettingFiles()
{
    if(dBfldLogic(&profile,Pro_TCPIP)==0) {
        HWND hWnd = GetSafeHwnd();
        BOOL bOpen = SetCommThread(hWnd,FALSE);
        if(bOpen==FALSE) {
        }
    }
}


