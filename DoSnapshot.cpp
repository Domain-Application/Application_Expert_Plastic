// DoSnapshot.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "DoSnapshot.h"
#include "vfw.h"

#pragma comment (lib, "vfw32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoSnapshot dialog


CString LastFileName(const char *szDirPath)
{
	CString s;

	for(int i=1; i<100; i++) {
		s.Format("%s\\%3.3i.jpg",szDirPath,i);
		if(IsFileExisted(s)==FALSE) break;
	}

	return s;
}


CDoSnapshot::CDoSnapshot(CWnd* pParent /*=NULL*/)
	: CDialog(CDoSnapshot::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDoSnapshot)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_app = "capture.bat";
    iCounter = 5;
    m_jpg = "final.jpg";
    busy = FALSE;
}


void CDoSnapshot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDoSnapshot)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDoSnapshot, CDialog)
	//{{AFX_MSG_MAP(CDoSnapshot)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoSnapshot message handlers

void CDoSnapshot::OnTimer(UINT nIDEvent) 
{
    if(busy == TRUE) return;
    busy = TRUE;
    if(nIDEvent==2) {
        iCounter--;
	    m_progress.SetPos(200 - iCounter);
        if(iCounter>0){
            busy = FALSE;
            return;
        }

        KillTimer(2);

        DeleteFile(m_jpg);
        DeleteFile("snapz.dib");
        capGrabFrame(m_WndCap);
        capFileSaveDIB (m_WndCap, "snapz.dib");
        Sleep(50);
        iCounter=500;

        int flag = WinExec(m_app, SW_HIDE);
        if(flag==0) {
            MessageBox("The system is out of memory or resources.");
            CDialog::OnCancel();
            busy = FALSE;
            return;
        }
        else if(flag==ERROR_BAD_FORMAT) {
            MessageBox("The .EXE file is invalid (non-Win32 .EXE or error in .EXE image).");
            CDialog::OnCancel();
            busy = FALSE;
            return;
        }
        else if(flag==ERROR_FILE_NOT_FOUND) {
            MessageBox("The specified file was not found.");
            CDialog::OnCancel();
            busy = FALSE;
            return;
        }
        else if(flag==ERROR_PATH_NOT_FOUND) {
            MessageBox("The specified path was not found.");
            CDialog::OnCancel();
            busy = FALSE;
            return;
        }

        SetTimer(1,10,NULL);
        busy = FALSE;
        return;
    }

    iCounter--;
    if(iCounter<=0) {
        OnCancel();
        busy = FALSE;
        return;
    }

    m_progress.SetPos(500 - iCounter);

    if(IsFileExisted(m_jpg)) {

		CString s,path ;

		Sleep(50);
		if(df->name[0]=='R' || df->name[0]=='r') {
			path.Format("jpg\\receiving\\%s",df->dat[1]);
		}
		else {
			path.Format("jpg\\despatch\\%s",df->dat[1]);
		}

		path.TrimRight();

		CreateDirectory( path,NULL );

		filename = LastFileName( path );
		filename.TrimRight();

        BOOL flag = MoveFile( m_jpg, filename);

        OnOK();
        busy = FALSE;
        return;
    }
	
    busy = FALSE;
	CDialog::OnTimer(nIDEvent);
}

void CDoSnapshot::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CDoSnapshot::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDoSnapshot::OnDestroy() 
{
    capPreview(m_WndCap,FALSE);          //TESTING
    capCaptureAbort(m_WndCap);
    capDriverDisconnect(m_WndCap);

    CDialog::OnDestroy();
}

BOOL CDoSnapshot::OnInitDialog() 
{
	CDialog::OnInitDialog();


	m_WndCap=capCreateCaptureWindow((LPSTR)"Video Capture Window",WS_CHILD|WS_VISIBLE,0,0,640,480,
		GetSafeHwnd() ,0);
	capDriverConnect(m_WndCap,0);
//	capDriverGetCaps(m_WndCap,&m_caps,sizeof(CAPDRIVERCAPS));
	
	capPreviewRate(m_WndCap,1000/100);//TESTING
    capPreview(m_WndCap,TRUE);          //TESTING

    m_progress.SetRange32(1,iCounter);
    SetTimer(2,10,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
