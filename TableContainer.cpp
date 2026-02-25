// TableContainer.cpp : implementation file
//

#include "stdafx.h"
#include "myapp.h"
#include "TableContainer.h"
#include "TabCtn.h"
#include "ReportEdit.h"
#include "TabCtnReport.h"
#include "TabBatchReport.h"
#include "TabSecurity.h"
#include "staff.h"
#include "parent.h"
#include "myapp.h"
#include "AutoView.h"
#include "admindb.h"
#include "dbsql.h"
#include "user.h"
#include "initial.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTabCtn *pTabctn;
/////////////////////////////////////////////////////////////////////////////
// CTableContainer dialog


CTableContainer::CTableContainer(CWnd* pParent /*=NULL*/)
    : CDialog(CTableContainer::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTableContainer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_add = m_delete = m_edit = FALSE;
    TabRecord = 0;
    TableContainerMark = 1234;
}

CTableContainer::~CTableContainer()
{
    CheckMemoryError("CTableContainer");
    if(TableContainerMark==1234) {
        app->DoDllRegisterServer("mscomct2.ocx");          //For container
    }
}

void CTableContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTableContainer)
	DDX_Control(pDX, IDC_LIST1, m_ListView);
	DDX_Control(pDX, IDC_UPDOWN1, m_UpDown);
    DDX_Control(pDX, ID_RUN, m_btn1);
    DDX_Control(pDX, ID_DEL, m_btn2);
    DDX_Control(pDX, ID_NEW, m_btn3);
    DDX_Control(pDX, IDEDIT, m_btn4);
    DDX_Control(pDX, ID_SECURITY, m_btn5);
    DDX_Control(pDX, IDCANCEL, m_btn6);
    DDX_Control(pDX, IDC_EDIT3, m_btn7);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTableContainer, CDialog)
    //{{AFX_MSG_MAP(CTableContainer)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
    ON_BN_CLICKED(ID_RUN, OnClickRun)
    ON_BN_CLICKED(IDEDIT, OnEdit)
	ON_BN_CLICKED(ID_NEW, OnNew)
	ON_BN_CLICKED(ID_DEL, OnDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_BN_CLICKED(ID_SECURITY, OnSecurity)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
    ON_COMMAND(ID_RUN, OnClickRun)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    ON_BN_CLICKED(IDC_EDIT3, OnClickRun)
	ON_COMMAND(ID_CRYSTAL, OnCrystal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableContainer message handlers
BOOL CTableContainer::IsThisUserCanView(DB *df)
{
    if(UserLevel()>=99) return TRUE;        //No limit for user level > 99

    long irec;
    mrec = 0;
    long Srec = interval(&irec, user.dat[1],&userdat, 0);
    if(Srec==0){
        if(IsUserHaveGroup()==FALSE) return TRUE;
        qd(user.dat[UR_GROUP],&user);
        return IsThisUserCanView(df);
    }

    for(long rec=0; rec<Srec; rec++) {
        ridx(irec+rec, &userdat, 0);
        if(!strcmp(userdat.dat[Tcu_REFNO],df->dat[1])){
            if(userdat.dat[Tcu_ACCESS][0]==Tuu_HIDDEN) return FALSE;
            if(userdat.dat[Tcu_ACCESS][0]==Tuu_INHERITED){
                if(IsUserHaveGroup()==TRUE) {
                    qd(user.dat[UR_GROUP],&user);
                    return IsThisUserCanView(df);
                }
            }
            mrec = irec+rec;
            return TRUE;
        }
    }
    if(mrec==0) {
        if(IsUserHaveGroup()==TRUE) {
            qd(user.dat[UR_GROUP],&user);
            return IsThisUserCanView(df);
        }
    }
    return FALSE;
}

void CTableContainer::ReFresh(DB *df)
{
    unsigned long BkColor = RGB(0,0,0);
    unsigned long FgColor;

    m_ListView.DeleteAllItems();
	int j = 0;
    LV_ITEM lvi;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
    lvi.iSubItem = 0;
    for (int rec = 1; rec <= df->t->sumrecord; rec++) {
        readrec(rec,df);
        if(strcmp(df->dat[Ctn_GROUP],m_group)) continue;
        if(j % 2 ) {
            FgColor = RGB(190,255,255);
        }
        else {
            FgColor = RGB(255,255,255);
        }

        char **p = pushdf(&user);
        BOOL bFlag = IsThisUserCanView(df);
        popdf(&user,p);
        freedf(&user,p);
        if(bFlag==FALSE) continue;                  //22.1.2001

        lvi.iItem = j;
        lvi.pszText = df->dat[Ctn_NAME];
        lvi.cchTextMax = 2;
        lvi.iImage = j;
        m_ListView.InsertItem(&lvi);
        m_ListView.SetItemText(j, 0, NULL , BkColor, FgColor);
        m_ListView.SetBold(j, 0, TRUE);

        if(UserLevel()>=99) {
            m_ListView.SetItemText(j, 1, df->dat[Ctn_REPORT] , BkColor, FgColor);
            m_ListView.SetItemText(j, 2, df->dat[Ctn_FILETYPE], BkColor, FgColor);
        }
		j++;
    }
//    m_ListView.SetEnabled(3, FALSE);                //testing
//    m_ListView.SetBold(1, 1, FALSE);
}

void CTableContainer::ShowReportOption(DB *df)
{
    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.fmt = LVCFMT_LEFT;

    lvc.cx = UserLevel()>=99 ? 370: 575;
    lvc.iSubItem = 1;
    lvc.pszText = "Description";
    m_ListView.InsertColumn(0, &lvc);
    if(UserLevel()>=99) {
        lvc.cx = 100;
        lvc.pszText = "File Name";
        lvc.iSubItem = 2;
        m_ListView.InsertColumn(1, &lvc);
        lvc.cx = 100;
        lvc.pszText = "Category";
        lvc.iSubItem = 3;
        m_ListView.InsertColumn(2, &lvc);
    }

    m_ListView.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB );

	ReFresh(df);
    m_ListView.ShowWindow(SW_SHOW);
    m_ListView.SetBkColor(RGB(190,255,255));
    m_ListView.SetTextColor(RGB(90,55,55));
    OnFindTable();
}

BOOL CTableContainer::OnInitDialog()
{
    CDialog::OnInitDialog();
    TableContainerMark = 0;
	
    if(option) {
        if(option->t->sumrecord>0) {
            ShowReportOption(option);
        }
    }
	short	shBtnColor = 30;
  	m_btn1.SetIcon(IDI_RUN);
	m_btn1.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
    m_btn1.SetTooltipText(_T("Start"));
	m_btn1.DrawFlatFocus(TRUE);

//    m_btn1.SetToolTipText("Start");
//    m_btn1.SetShade(SHS_DIAGSHADE,8,20,5,RGB(55,255,55));
    m_btn2.SetToolTipText("Remove menu.");
    m_btn2.SetShade(SHS_VSHADE,8,20,5,RGB(55,55,255));
    m_btn3.SetToolTipText("Add new menu.");
    m_btn3.SetShade(SHS_HARDBUMP,8,20,5,RGB(55,55,255));
    m_btn4.SetToolTipText("Edit menu.");
    m_btn4.SetShade(SHS_SOFTBUMP,8,20,5,RGB(55,55,255));
    m_btn5.SetToolTipText("Set security.");
    m_btn5.SetShade(SHS_METAL,8,20,5,RGB(55,55,255));

//    m_btn6.SetIcon(IDI_NO3, (int)BTNST_AUTO_GRAY);
    m_btn6.SetIcon(IDI_NO3);
    m_btn6.SetTooltipText(_T("Exit to application"));
    m_btn6.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);

    shBtnColor = 128;

	shBtnColor = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTableContainer::OnEdit()
{
	if(option->rec==0) return;
    CReportEdit h1;

    h1.m_ReportTitle = option->dat[Ctn_NAME];
    h1.m_CrystalReportName = option->dat[Ctn_REPORT];
    h1.m_description = option->dat[Ctn_MODE];
    h1.m_filetype    = option->dat[Ctn_FILETYPE];
    h1.bAddMode  = FALSE;
    h1.bEditMode = TRUE;
    if(h1.DoModal()==IDOK) {
        option->mrec = option->rec;
        lset(option->dat[Ctn_NAME],h1.m_ReportTitle);
        lset(option->dat[Ctn_REPORT],h1.m_CrystalReportName);
        lset(option->dat[Ctn_MODE],h1.m_description);
        lset(option->dat[Ctn_FILETYPE],h1.m_filetype);
		fastwrt(option->rec, option);
//        option->Update();
        ReFresh(option);
    }
}

void CTableContainer::OnNew()
{
    CReportEdit h1;

    option->lsetblank();
    h1.bAddMode  = TRUE;
    h1.bEditMode = FALSE;
    h1.m_ReportTitle = option->dat[Ctn_NAME];
    if(h1.DoModal()==IDOK) {
        lset(option->dat[Ctn_NAME],h1.m_ReportTitle);
        lset(option->dat[Ctn_REPORT],h1.m_CrystalReportName);
        lset(option->dat[Ctn_MODE],h1.m_description);
        lset(option->dat[Ctn_GROUP],m_group);
        lset(option->dat[Ctn_FILETYPE],h1.m_filetype);
        if(waitflock(option)==TRUE) {
			FindAddInfoTicketNo(option);                       //22.1.2001
            long mrec = getdelrec(option);
            int  IdxItem = option->f[1].idx & 1;
            AppendIdx(mrec,option,IdxItem);
			int field[] = { 1, 0};
			sort(option,field);
			ReFresh(option);
			unflock(option);
		}
    }
}

void CTableContainer::OnDel()
{
	if(option->rec==0) return;
	
    if(MessageBox(option->dat[Ctn_NAME],"Remove",MB_ICONQUESTION | MB_YESNO)==IDNO) return;

    waitflock(option);
    int IdxItem = option->f[1].idx & 1;
    wipeidx(option->rec, option, IdxItem);
    unflock(option);

    ReFresh(option);
}

void CTableContainer::AdminDb(DB *pdf,DB *sub)
{
    if(pdf->ID==0) {
        MessageBox("ID not found");
        return;
    }
    if(pdf->m_Adodc==0){
        if(pdf->sumfield==0) {
            MessageBox("Database not open");
            return;
        }
        if(pdf->handle==0) {
			pdf->WakeUp();
        }
    }

    int flag = pdf->e->AdminDb(pdf,0);                   //25-8-1999
    if(flag==0) {
        if(sub==NULL) {
            CStaff p1(pdf->ID,pdf,pdf->f[1].idx & 0x01);
            p1.m_WinPrint=TRUE;
            p1.DoModal();
        }
        else {
            CParent p1(pdf->ID,pdf,pdf->f[1].idx & 0x01);
            p1.m_WinPrint=TRUE;
            p1.sub = sub;
            p1.IDD_SUB = sub->ID;
            p1.DoModal();
        }
    }
}

void CTableContainer::OnRun()
{
	char fldName[512];

	strcpy(fldName,m_TableReportName);
	rtrim(fldName);
    int multiple_files = instr(1,fldName,",");
    if(multiple_files) {
        OnRunMultipleFiles();
        return;
    }
    DB *pf = GetMatchFile(fldName,0);
    if(!pf) return;

    strcpy(SubTitle,option->dat[Ctn_NAME]);
    rtrim(SubTitle);
    if(dBfldLogic(option,1)) {
        if(!isblank(option,Ctn_PREF)) {
            long prec = atol(option->dat[Ctn_PREF]);
            if(prec>0) {
                app->SubProgram(prec);
                return;
            }
        }
    }

    AdminDb(pf);
}

void CTableContainer::OnOK()
{
}

void CTableContainer::SetButton(BOOL bFlag,int iHide, int iShow)
{
    GetDlgItem(ID_RUN)->EnableWindow(bFlag);
    GetDlgItem(IDC_EDIT3)->EnableWindow(bFlag);
    if(UserLevel()<99) {
        GetDlgItem(ID_SECURITY)->ShowWindow(iHide);
        GetDlgItem(ID_NEW)->ShowWindow(iHide);
        GetDlgItem(ID_DEL)->ShowWindow(iHide);
        GetDlgItem(IDEDIT)->ShowWindow(iHide);
        return;
    }
    else {
        GetDlgItem(ID_SECURITY)->ShowWindow(iShow);
        GetDlgItem(ID_SECURITY)->EnableWindow(bFlag);
    }
    if(m_add==FALSE){
		CWnd* h1 = GetDlgItem(ID_NEW);
        h1->ShowWindow(iHide);
    }
    if(m_delete==FALSE){
        CWnd* h1 = GetDlgItem(ID_DEL);
		h1->ShowWindow(iHide);
    }
    else {
        CWnd* h1 = GetDlgItem(ID_DEL);
		h1->EnableWindow(bFlag);
    }
    if(m_edit==FALSE){
        CWnd* h1 = GetDlgItem(IDEDIT);
		h1->ShowWindow(iHide);
    }
    else {
        CWnd* h1 = GetDlgItem(IDEDIT);
		h1->EnableWindow(bFlag);
    }
}

BOOL CTableContainer::OnFindTable()
{
    option->rec = 0;
	if(!m_UpDown) return FALSE;

	int nItem = m_ListView.GetNextItem(-1, LVNI_FOCUSED);
    if(UserLevel()<99) {
        m_UpDown.ShowWindow(SW_HIDE);
    }

    if(nItem<0){
        SetButton(FALSE);
        return FALSE;
    }

    str = m_ListView.GetItemText(nItem,0);
    for(long rec=1; rec<= option->t->sumrecord; rec++) {
        readrec(rec,option);
        if(!strcmp(option->dat[Ctn_NAME],str)) {
            option->rec = rec;
            if(isblank(option->dat[Ctn_MODE]))
                m_btn7.SetWindowText(option->dat[Ctn_NAME]);
            else
                m_btn7.SetWindowText(option->dat[Ctn_MODE]);
            CString str;
            str.Format("Run --> %s",option->dat[Ctn_NAME]);
			m_btn1.SetTooltipText(_T(str));

            SetWindowText(option->dat[Ctn_NAME]);
            SetButton(TRUE);
            return TRUE;
        }
    }
    SetButton(FALSE);
    return FALSE;
}

void CTableContainer::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
    OnFindTable();
}

void CTableContainer::OnPreviewReport(BOOL bInternalFilter, BOOL exportToExcel)
{

    if(app->GetReportFilter(option,bInternalFilter)==FALSE) {
        CTabCtnReport h1;
        h1.option = option;
        h1.InternalFilter = bInternalFilter;
        if( !strncmp(option->dat[Ctn_PREF],"Excel", 5)) {
            h1.m_ExportToWinExcel = TRUE;
        }
        else if( !strncmp(option->dat[Ctn_PREF],"Word", 4)) {
            h1.m_ExportToWinWord = TRUE;
        }
        h1.DoModal();
    }
}

char *szObjectName;
void CTableContainer::OnRunObject()
{
    strcpy(SubTitle,option->dat[Ctn_NAME]);
    rtrim(SubTitle);
    if(!isblank(option,Ctn_PREF)) {
        long prec = atol(option->dat[Ctn_PREF]);
        if(prec>0) {
            szObjectName = new char [50];
            strcpy(szObjectName,option->dat[Ctn_REPORT]);
            rtrim(szObjectName);
            app->SubProgram(prec);
            delete [] szObjectName;
            return;
        }
    }
}

extern void (*wbSetup) (DB *);
void CTableContainer::OnWeighingSetup()
{
    (*wbSetup) (option);
}

void CTableContainer::OnClickRun()
{
    if(option->rec <= option->t->sumrecord && option->rec>=1) {
        readrec(option->rec,option);
        char **p = pushdf(&user);
        IsThisUserCanView(option);
        popdf(&user,p);
        freedf(&user,p);
        RIGHT_POWER &= 0x0000ff20;
        if(mrec>0) {
            if(dBfldLogic(&userdat,Tcu_LOCKSKIP)) {
                RIGHT_POWER |= RP_SKIPLOCK;
            }
            if(dBfldLogic(&userdat,Tcu_SEARCH)) {
                RIGHT_POWER |= RP_SEARCH;
            }
            if(dBfldLogic(&userdat,Tcu_DELETE)) {
                RIGHT_POWER |= RP_DELETE;
            }
            if(dBfldLogic(&userdat,Tcu_CANNOTPRINT)) {
                RIGHT_POWER |= RP_CANNOTPRINT;
            }
            if(dBfldLogic(&userdat,Tcu_PRINT)) {
                RIGHT_POWER |= RP_PRINT_NEED_PASSWORD;
            }
            if(dBfldLogic(&userdat,Tcu_DUPLICATE)) {
                RIGHT_POWER |= RP_DUPLICATE;
            }
            if(dBfldLogic(&userdat,Tcu_NAVIGATE)) {
                RIGHT_POWER |= RP_NAVIGATE;
            }
            if(userdat.dat[Tcu_ACCESS][0]==Tuu_VIEWONLY){
                RIGHT_POWER |= RP_VIEWONLY;
            }
            else if(userdat.dat[Tcu_ACCESS][0]==Tuu_ADDONLY){
                RIGHT_POWER |= RP_NEWONLY;
            }
            else if(userdat.dat[Tcu_ACCESS][0]==Tuu_EDITONLY){
                RIGHT_POWER |= RP_EDITONLY;
            }
        }

        m_TableReportName = option->dat[Ctn_REPORT];
        char mode = option->dat[Ctn_FILETYPE][0];
        if(mode=='T') {             // Transaction/Table/Object
            OnRun();
        }
        else if(mode=='E') {        // Exe files  8-8-2001
            OnRunExe();
        }
        else if(mode=='O') {        // Object
            OnRunObject();
        }
        else if(mode=='R') {        // Report
            OnPreviewReport();
        }
        else if(mode=='S') {        // Simply Dialog
            OnSimplyDialog();
        }
        else if(mode=='Q') {        // SQL Store Procedure query
            OnStoreProcedure();
        }
        else if(mode=='I') {        // Report (Internal Filter)
            OnPreviewReport(TRUE) ;
        }
        else if(mode=='B') {        // Batch Report
            OnBatchReport() ;
        }
        else if(mode=='H') {        // HTML/ASP
            OnHTML() ;
        }
        else if(mode=='W') {
            OnWeighingSetup() ;
        }
    }
    SetActiveWindow();
}

void CTableContainer::OnHTML()
{
	 char htmlFile[80];
	 strcpy(htmlFile,m_TableReportName);
     rtrim(htmlFile);
     DoViewHTML(htmlFile);
}

void CTableContainer::OnBatchReport()
{
    CTabBatchReport h1;

    h1.option = option;
    h1.DoModal();
}

void RunBatchOnSqlStatement(char *szMasterFile, BOOL bPrintSupport)
{
    rtrim(szMasterFile);

    BOOL bOpen = FALSE;
    DB *df = GetMatchFile(szMasterFile,0);
    if(!df) {
        char szFile[512];
        sprintf(szFile,"%s.scr",szMasterFile);
        df = new DB;
        bOpen = TRUE;
        dopen(df,szFile);
    }
    if(df->ID==0) {
        UINT IDD = IDD_TIO_TWO;
        if(df->f[1].mode=='D') {
            if(df->f[2].mode!='D') {
                IDD = IDD_TIO_ONE;
            }
        }
        else {
            IDD = IDD_TESTVIEW;
        }
        df->ID = IDD;
    }
    int flag = df->e->AdminDb(df,0);                   //15-2-2001
    if(flag==0) {
        CDBSql t1(df, df->ID);
        t1.name = szMasterFile;
        t1.bPrintSupport = bPrintSupport;
        t1.DoModal();
    }

    if(bOpen) {
        dclose(df);
        delete df;
    }
}

void CTableContainer::OnSqlStatement()
{
    char szMasterFile[512];
    strcpy(szMasterFile,option->dat[3]);
    RunBatchOnSqlStatement(szMasterFile,TRUE);
}

void CTableContainer::OnSimplyDialog()
{
    strcpy(SubTitle,option->dat[2]);
    char mode = option->dat[Ctn_FILETYPE][1];
    if(mode=='Q') {
        OnSqlStatement();
        return;
    }
	char fldName[512];

	strcpy(fldName,m_TableReportName);
	rtrim(fldName);
    int multiple_files = instr(1,fldName,",");
    if(multiple_files) {
        OnRunMultipleSimplyDialog();
        return;
    }

    char szMasterFile[512];
    strcpy(szMasterFile,option->dat[3]);
    rtrim(szMasterFile);

    ::AdminDb(szMasterFile,TRUE);
}

BEGIN_EVENTSINK_MAP(CTableContainer, CDialog)
    //{{AFX_EVENTSINK_MAP(CTableContainer)
	ON_EVENT(CTableContainer, IDC_UPDOWN1, 2 /* DownClick */, OnDownClickUpdown1, VTS_NONE)
	ON_EVENT(CTableContainer, IDC_UPDOWN1, 3 /* UpClick */, OnUpClickUpdown1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTableContainer::OnDownClickUpdown1() 
{
    if( option->rec ==0 ) return;

    long DownRec = 0;
    long UpRec = option->rec;
    for (int rec = UpRec + 1; rec <= option->t->sumrecord; rec++) {
        readrec(rec,option);
        if(strcmp(option->dat[Ctn_GROUP],m_group)) continue;
        DownRec = rec;
        break;
    }
    if(DownRec==0) return;
    backup(option);
    readrec(UpRec,option);
    writerec(DownRec,option);
    restore(option);
    writerec(UpRec,option);
    option->rec = UpRec;
    ReFresh(option);
}

void CTableContainer::OnUpClickUpdown1() 
{
    if( option->rec ==0 ) return;
	
    long DownRec = option->rec;
    long UpRec = 0;
    for (int rec = DownRec - 1; rec >= 1; rec--) {
        readrec(rec,option);
        if(strcmp(option->dat[Ctn_GROUP],m_group)) continue;
        UpRec = rec;
        break;
    }
    if(UpRec==0) return;

    backup(option);
    readrec(DownRec,option);
    writerec(UpRec,option);
    restore(option);
    writerec(DownRec,option);
    option->rec = DownRec;
    ReFresh(option);
}

void CTableContainer::OnRunMultipleFiles()
{
	char fldName[512];

	strcpy(fldName,m_TableReportName);
	rtrim(fldName);
    int multiple_files = instr(1,fldName,",");
    fldName[multiple_files-1] = 0;
    DB *pf = GetMatchFile(fldName,0);
    if(!pf) return;

	char subName[512];
    strcpy(subName,&fldName[multiple_files]);
	rtrim(subName);
    DB *sub = GetMatchFile(subName,0);
    if(!sub) return;

    strcpy(SubTitle,option->dat[Ctn_NAME]);
    rtrim(SubTitle);
    if(dBfldLogic(option,1)) {
        if(!isblank(option,Ctn_PREF)) {
            long prec = atol(option->dat[Ctn_PREF]);
            if(prec>0) {
                app->SubProgram(prec);
                return;
            }
        }
    }

    AdminDb(pf,sub);
}

void CTableContainer::OnSecurity() 
{
    CTabSecurity h1;

    h1.option = option;
    h1.DoModal();
}

BOOL CTableContainer::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN) {
		TRACE("\nPreTranslateMessage %i %ld",pMsg->message,pMsg->lParam);	
		if(pMsg->wParam==VK_RETURN) {
            PostMessage(WM_COMMAND,ID_RUN);
		}
		else if(pMsg->wParam==VK_ESCAPE) {
		}
        else if(pMsg->wParam==VK_RIGHT) {
            PostMessage(WM_COMMAND,ID_RUN);
		}
        else if(pMsg->wParam==VK_LEFT) {
            PostMessage(WM_COMMAND,ID_RUN);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CTableContainer::OnStoreProcedure()
{
    char szMasterFile[512];
    strcpy(szMasterFile,option->dat[3]);
    rtrim(szMasterFile);

    BOOL bOpen = FALSE;
    DB *df = GetMatchFile(szMasterFile,0);
    if(!df) {
        char szFile[512];
        sprintf(szFile,"%s.scr",szMasterFile);
        df = new DB;
        bOpen = TRUE;
        dopen(df,szFile);
    }
    if(df->ID==0) {
        UINT IDD = IDD_TIO_TWO;
        if(df->f[1].mode=='D') {
            if(df->f[2].mode!='D') {
                IDD = IDD_TIO_ONE;
            }
        }
        else {
            IDD = IDD_TESTVIEW;
        }
        df->ID = IDD;
    }
    int flag = df->e->AdminDb(df,0);                   //15-2-2001
    if(flag==0) {
        CDBSql t1(df, df->ID);
        t1.name = szMasterFile;
        t1.IsSqlStoreProcedure = TRUE;
        t1.CleanAfterUse = TRUE;
        t1.DoModal();
    }

    if(bOpen) {
        dclose(df);
        delete df;
    }
}

void CTableContainer::OnRunExe()
{
    char szMasterFile[512];
    strcpy(szMasterFile,option->dat[3]);
    rtrim(szMasterFile);

    WinExec(szMasterFile,SW_SHOW);
}

void CTableContainer::OnRunMultipleSimplyDialog()
{
	char fldName[512];

	strcpy(fldName,m_TableReportName);
	rtrim(fldName);
    int multiple_files = instr(1,fldName,",");
    fldName[multiple_files-1] = 0;
    DB pf,sub;
    tado(&pf,fldName);

	char subName[512];
    strcpy(subName,&fldName[multiple_files]);
	rtrim(subName);
    tado(&sub,subName);

    strcpy(SubTitle,option->dat[Ctn_NAME]);
    rtrim(SubTitle);

    CAutoView p1(IDD_AUTOVIEW,&pf,1);
    p1.sub     = &sub;
    p1.IDD_SUB = IDD_REQVIEW;
    p1.DoModal();

    dclose(&pf);
    dclose(&sub);
}

void CTableContainer::OnCancel()
{
	CDialog::OnCancel();
    CTabCtn *p = (CTabCtn *) GetParent();
    pTabctn->OnCancel();
}

void CTableContainer::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
    OnFindTable();
}

void CTableContainer::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
    PostMessage(WM_COMMAND,ID_RUN);
	*pResult = 0;
}

void CTableContainer::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem = m_ListView.GetNextItem(-1, LVNI_FOCUSED);
	POINT pt;
	
	m_ListView.GetItemPosition(nItem, &pt);
	UINT uFlags;
	int nItem2 = m_ListView.HitTest(pt, &uFlags);
	
	
	
	*pResult = 0;
    OnFindTable();

    CRect Rect;
    GetDlgItem(IDC_LIST1)->GetWindowRect(&Rect);
    CMenu menu;

    BOOL flag = menu.LoadMenu(IDR_TABCTN);
	if(flag) {
        CMenu*p = (CMenu*) menu.GetSubMenu(0);

        p->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
            Rect.left + pt.x, Rect.top + pt.y,
            this);
//		p->DestroyMenu();
	}
}

void CTableContainer::OnCrystal() 
{
    CString lpFile = option->dat[Ctn_REPORT];
    lpFile.TrimRight();
    HINSTANCE flag = ShellExecute(
        NULL,
        "OPEN",
        lpFile,
        NULL,
        NULL,
        SW_SHOW
        );
}
