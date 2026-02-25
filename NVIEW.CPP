// NView.cpp : implementation of the CNView class
//

#include "stdafx.h"

#include "MENU.h"
#include "NView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define timer_import_ri 123
#define timer_showinfo 133
extern DB ddata_unbill_data;
extern void init_ddata_unbill_data(DB *df);``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_RestartCameras, OnRestartCameras)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNView construction/destruction

CNView::CNView()
    : CCctvView(app->IDD_DOMVIEW)
{
    //{{AFX_DATA_INIT(CNView)
    m_days = profile.lval(Pro_ALERT21RULE);
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	TotalCameras =  0;
	busy123 = FALSE;
}

void CNView::DoDataExchange(CDataExchange* pDX)
{
    CDomView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNView)
	DDX_Text(pDX, IDC_EDIT1, m_days);
	//}}AFX_DATA_MAP
}

void CNView::OnInitialUpdate()
{
    CCctvView::OnInitialUpdate();


    SetTimer(timer_video,250,NULL);

    ShowInitFrame();
    ShowInformation();

	SetTimer(timer_import_ri,60 * 1000,NULL);

	SetTimer(timer_showinfo, 5 * 1000, NULL);
}

void CNView::ShowInitFrame()
{
}

void CNView::SetActiveStatus(DB *first,long rec)
{
}

void CNView::ShowInformation()
{
    if(m_swGrid) return;
    m_swGrid = TRUE;

    m_swGrid = FALSE;
}


void CNView::OnTimer(UINT nIDEvent)
{
     if(nIDEvent==timer_import_ri) {
        if(busy123) return;

        busy123 = TRUE;

        busy123 = FALSE;
    }
	 else if(nIDEvent==timer_showinfo) {
		 ShowInformation();
	 }

    CCctvView::OnTimer(nIDEvent);
}

void CNView::OnButton2()
{
	char today[20];
	
	UpdateData();
	space_str(today, 8);
	dbasedate(today);
	plusday(today, m_days * -1);
	
	CString s;
	s.Format("select * from ddata_unbill_data where date < '%s'", today);
    
    ado(&ddata_unbill_data,"ddata_unbill_data",init_ddata_unbill_data);
	
	ddata_unbill_data.m_CanSort = TRUE;
    ddata_unbill_data.DBEnqflag = 2;   //group
    long xrec = ddata_unbill_data.browse(FALSE);
	if(xrec) {
//		MessageBox(ddata_unbill_data.dat[1]);
	}
	
    dclose(&ddata_unbill_data);
	
	
}
