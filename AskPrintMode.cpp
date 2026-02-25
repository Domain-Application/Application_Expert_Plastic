// AskPrintMode.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "AskPrintMode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int PrintMode;
BOOL    m_oneitem;
/////////////////////////////////////////////////////////////////////////////
// CAskPrintMode dialog


CAskPrintMode::CAskPrintMode(CWnd* pParent /*=NULL*/)
	: CDialog(CAskPrintMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAskPrintMode)
	m_oneitem = FALSE;
	//}}AFX_DATA_INIT
    PrintMode = 0;
}


void CAskPrintMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAskPrintMode)
	DDX_Check(pDX, IDC_CHECK1, m_oneitem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAskPrintMode, CDialog)
	//{{AFX_MSG_MAP(CAskPrintMode)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
    ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAskPrintMode message handlers

void CAskPrintMode::OnOK() 
{
    PrintMode = 1;
	CDialog::OnOK();
}

void CAskPrintMode::OnButton2() 
{
    PrintMode = 2;
	CDialog::OnOK();
	
}

void CAskPrintMode::OnButton3() 
{
    PrintMode = 3;
	CDialog::OnOK();
}

void CAskPrintMode::OnButton4()
{
    PrintMode = 4;
	CDialog::OnOK();
}
