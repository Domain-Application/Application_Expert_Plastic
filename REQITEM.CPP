// Reqitem.cpp : implementation file
//

#include "stdafx.h"
#include "Reqitem.h"
#include "browse.h"
#include "LinkEdit.h"
#include "Parent.h"
#include "db_ado.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReqitem dialog


CReqitem::CReqitem(UINT IDD,DB *pdf,DB *psub,int Idx,CWnd* pParent /*=NULL*/)
    : CScrollDialog(IDD, pParent)
{
	//{{AFX_DATA_INIT(CReqitem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    NoParent   = FALSE;
    pOldDlg    = pDlg;
    sub        = psub;
    df         = pdf;
    IdxItem    = Idx;
    FirstField = 2;
    ExistRecordNo = 0;
    if(!stricmp(sub->f[2].name,"SQNO")) FirstField = 3;
    FirstJumpField = FirstField;
    TotalField   = LastField  = int(sub->sumfield);
    CurrentField = FirstField;

    m_ed          = new CLinkEdit[LastField+1];
    if(m_ed==NULL) memory_alloc_err();
    m_be          = new CButtonEdit_P[LastField+1];
    if(m_be==NULL) memory_alloc_err();
    m_dt          = new VDateTimeEdit_P[LastField+1];
    if(m_dt==NULL) memory_alloc_err();
    JumpFlag      = FALSE;
    memset( sub->ReadOnlyArray,0, (LastField+1) * sizeof(int));
    m_picutreName = "";
    HideDetailButton = TRUE;
    Status = 0;
}

CReqitem::~CReqitem()
{
    delete [] m_ed;
    delete [] m_dt;
    delete [] m_be;
    CheckMemoryError("Creqitem");
}

void CReqitem::OnDestroy()
{
    CScrollDialog::OnDestroy();
    pDlg = pOldDlg;                          //10-8-1999
}

void CReqitem::ShowFieldNo(int fld)
{
    CString s;

    s.Format("%i/%i",fld,sub->sumfield);
    m_StatusBar.SetText(s,3,0);
    m_StatusBar.SetText(sub->fieldName[fld],4,0);
    CurrentField = fld;
}

void CReqitem::DoDataExchange(CDataExchange* pDX)
{
    CScrollDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReqitem)
		// NOTE: the ClassWizard will add DDX and DDV calls here
    for(int i=FirstField; i<= TotalField; i++) {
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), IDC_STATIC1+i-1);
        if(hWnd) {
            hWnd = ::GetDlgItem(this->GetSafeHwnd(),CtrlId(i));
            if(!hWnd) continue;
            DWORD tt = GetDlgItem(CtrlId(i))->GetStyle() & WS_VSCROLL;
            if(tt==0) continue;
            if(UserLevel()) {
                tt = GetDlgItem(IDC_STATIC1+i-1)->GetExStyle() & WS_EX_ACCEPTFILES;
                if(tt || app->AutoAcceptFile){
                     DDX_Control(pDX, IDC_STATIC1+i-1, stText[i]);
                     stText[i].SetFontName("MS Sans Serif")
                        .SetTextColor(app->AutoTableFontColor)
                        .SetFontUnderline(app->AutoTableFontUnderline)
                        .SetFontItalic(app->AutoTableFontItalic)
                        .SetFontBold(app->AutoTableFontBold)
                        .SetFontSize(app->AutoTableFontSize)
                        .SetLink(TRUE,TRUE)
                        .SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HAND));
                }
            }
        }
#if _MSC_VER > 1200
        hWnd = ::GetDlgItem(this->GetSafeHwnd(), IDC_LABEL1+i-1);
        if(hWnd==NULL) {
            if(m_pCtrlCont==NULL) continue;
            COleControlSite* p = m_pCtrlCont->FindItem(IDC_LABEL1+i-1);
            if(p) {
                DDX_Control(pDX, IDC_LABEL1+i-1, cbName[i]);
            }
            continue;
        }
#endif // _MSC_VER > 1200
    }
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReqitem, CScrollDialog)
	//{{AFX_MSG_MAP(CReqitem)
    ON_BN_CLICKED(ID_DEL, OnDel)
	ON_BN_CLICKED(IDINSERT, OnInsert)
	ON_COMMAND(ID_DUPLICATE, OnDuplicate)
	ON_COMMAND(ID_GOTO_RECNO, OnGotoFirstField)
	ON_COMMAND(IDEND, OnEnd)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
    ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
    ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
    ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
    ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
    ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
    ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReqitem message handlers

void CReqitem::OnPaint()
{
    CWnd* previewWnd = GetDlgItem(IDC_PREVIEW);
    if(NULL == previewWnd){
        CDialog::OnPaint();
        return;
    }

    CRect     PreviewRect;
    previewWnd->GetWindowRect(PreviewRect);
    ScreenToClient(&PreviewRect);

	CPaintDC dc(this); // device context for painting

    CSize xy = m_jpg.GetSize(&dc);
    double m_ratio = (PreviewRect.right - PreviewRect.left) / double(xy.cx);
    m_jpg.Draw(&dc,CPoint(PreviewRect.left,PreviewRect.top),m_ratio);
}

void CReqitem::OnClickComboBox(int fld)
{
    if(fld<1 || fld>= int(sub->sumfield)) return;

    sub->IsScroll = 1;
    strcpy(sub->old[fld],sub->dat[fld]);
    (*sub->e->In) (sub, fld);                //ooi 8.6.2006
    pf       = GetList(sub, fld, TRUE);
    int q    = strcmp(sub->old[fld],sub->dat[fld]);
    int id   = UNDO;
    UINT nChar = 0;
    int flag = (*sub->e->tell)(nChar,id,fld,sub,q);
    sub->IsScroll = 0;
    if(flag==2) {
        ShowInformation();
    }
    else {
        ShowFieldContent(fld);
    }
}

int CReqitem::OnTellMe(UINT nChar,int &id,int &fld)
{
	CString str;
    CParent *p = (CParent *) GetParent();

    if(NoParent==FALSE) {
        p->ChildTellBefore(this,fld);
    }
    ReplaceFieldContent(fld);
    (*sub->e->Relationships)(sub);
    int flag = (*sub->e->tell)(nChar,id,fld,sub,0);
    if(flag) {
        if(fld==1) {
            if(nChar==VK_ESCAPE) {
                return 0;
            }
            return 0;
        }
        else {
            int q = strcmp(sub->old[fld],sub->dat[fld]);
            DWORD tt = GetDlgItem(CtrlId(fld))->GetStyle() & WS_VSCROLL;
            if(tt){
                pf = GetList(sub,fld);
                if(pf) {
                    if(pf->rec==0){
                        ShowFieldContent(fld);
                        return 0;
                    }
                    q = strcmp(sub->old[fld],sub->dat[fld]);
                }
            }
            else {
                pf = GetMatchTable(fld,sub);
                if(pf) {
                    qd(sub->dat[fld],pf);
                }
            }
            if(q) {
                if(pf) {
                    DWORD tt = GetDlgItem(CtrlId(fld))->GetExStyle() & WS_EX_ACCEPTFILES;
                    if(tt){
                        char tmpField[1024];
                        strcpy(tmpField,sub->dat[1]);
                        fetch_ACCEPTFILES(sub,pf);
                        strcpy(sub->dat[1],tmpField);
                        ShowInformation();
                    }
                }
            }
            q = strcmp(sub->old[fld],sub->dat[fld]);
            flag = (*sub->e->tell)(nChar,id,fld,sub,q);
        }
    }
    if(NoParent==FALSE) {
        p->ChildTellAfter(this,fld);
    }
    if(flag==2) {
        ShowInformation();
    }
    else {
        ShowFieldContent(fld);
    }
	return 1;
}

void CReqitem::ShowFieldContent(int i)
{
    DBUpdateData(this,sub,i,FALSE);

#if _MSC_VER > 1200
    HWND hWnd = ::GetDlgItem(pDlg->GetSafeHwnd(), IDC_LABEL1 + i - 1);
    if(hWnd==NULL) {
         if(m_pCtrlCont==NULL) return;
         COleControlSite* p = m_pCtrlCont->FindItem(IDC_LABEL1+i-1);
         if(p) {
             cbName[i].SetWindowText(strLabel);
         }
    }
#endif // _MSC_VER > 1200
}

void CReqitem::ShowInformation()
{
    for(int i=FirstField; i<=LastField; i++ ) {
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(i));
        if(hWnd==NULL) continue;               //08-04-2011
        ShowFieldContent(i);
	}
    sub->e->ShowInformation(this,sub,0);

    CWnd* previewWnd = GetDlgItem(IDC_PREVIEW);
    if(NULL == previewWnd) return;

    CParent *p = (CParent *) GetParent();

    CString str = p->GetJpgName();
    if(str.GetLength()==0) return;
    if(m_picutreName.CompareNoCase(str)==0) return;

    m_jpg.Load(str);
	Invalidate();
    m_picutreName = str;
}

void CReqitem::SetLimitText(int i,int len)
{
    char chMode = sub->mode(i);

    if(chMode=='D') {
        m_ed[i].SetLimitText(10);
    }
    else if(chMode=='L') {
    }
    else if(chMode=='T') {
        m_dt[i].SetLimitText(20);
    }
    else if(chMode=='B') {
        m_ed[i].SetLimitText(30);
    }
    else {
        m_ed[i].SetLimitText( len );
    }
}

void CReqitem::EditFieldSetting(int i)
{
    char chMode = sub->mode(i);

    if(chMode=='D') {
        m_ed[i].SubclassDlgItem(CtrlId(i),this);
    }
    else if(chMode=='T') {
        m_dt[i].SubclassDlgItem(CtrlId(i),this);
    }
    else if(chMode=='L') {
        m_be[i].SubclassDlgItem(CtrlId(i),this);
    }
    else if(chMode=='B') {
        m_ed[i].SubclassDlgItem(CtrlId(i),this);
    }
    else {
        m_ed[i].SubclassDlgItem(CtrlId(i),this);
        DWORD stype = GetDlgItem(CtrlId(i))->GetStyle() & ES_MULTILINE;
        if(stype==0) {
            SetLimitText(i,sub->fieldLength[i]);
        }
    }
}

void CReqitem::InitialEditFieldSetting()
{
    for(int i=FirstField; i<= LastField ; i++) {
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(i));
        if(hWnd==NULL) continue;               //08-04-2011
        EditFieldSetting(i);
	}
}

void CReqitem::SetButton(int button_no,int button_sq,int IdNo,CString string)
{
	m_Buttons[button_no].iBitmap = button_sq;
	m_Buttons[button_no].idCommand = IdNo;
	m_Buttons[button_no].fsState = TBSTATE_ENABLED;
	m_Buttons[button_no].fsStyle = TBSTYLE_BUTTON;
	m_Buttons[button_no].dwData = 0;

	int nStringLength = string.GetLength() + 1;
	TCHAR * pString = string.GetBufferSetLength(nStringLength);
	pString[nStringLength] = 0;

	VERIFY((m_Buttons[button_no].iString = m_ToolBar.AddStrings(pString)) != -1);

	string.ReleaseBuffer();
}

void CReqitem::CreateToolBarAndStatusBar()
{
	RECT rect;
	rect.top = rect.left = 0;
	rect.right = 100;
	rect.bottom = 20;
    m_StatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS | SBT_TOOLTIPS, rect, this, 0);

    int strPartDim[]= { 25,150, 200, 250, 340, 390, -1};
    m_StatusBar.SetParts(7,strPartDim);
    m_StatusBar.SetText( sub->name,1,0);
    m_StatusBar.SetText( "File",0,0);
    m_StatusBar.SetText( "Field No",2,0);
    m_StatusBar.SetText( "Esc: Exit",6,0);
	m_StatusBar.FlashWindow(TRUE);

	VERIFY(m_ToolBar.Create(WS_CHILD | WS_VISIBLE | CCS_TOP | TTS_ALWAYSTIP , rect, this, 0));
	
    SetButton(0,0,IDOK,"Save");
    SetButton(1,1,IDCANCEL,"Esc");
    SetButton(2,12,ID_DUPLICATE,"Duplicate");
    SetButton(3,6,ID_DEL,"Delete");
    SetButton(4,11,IDINSERT,"Insert");
    SetButton(5,13,IDDETAIL, "More");

	m_ToolBar.SetRows(3,TRUE,&rect);

    m_ToolBar.AddButtons(6,m_Buttons);
    m_ToolBar.AddBitmap(11,IDR_TOOLBAR_EDIT);
	CSize sizeButton(16,19);
	m_ToolBar.SetBitmapSize(sizeButton);
	CSize sizeBitmap(16,15);
	m_ToolBar.SetBitmapSize(sizeBitmap);
	m_ToolBar.AutoSize();
}	

void CReqitem::SetReadOnlyField(int fld)
{
    if(fld>=FirstField && fld<=LastField) {

        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(fld));
        if(hWnd==NULL) return;               //08-04-2011
        char chMode = sub->mode(fld);
        if(chMode=='L') {
            m_be[fld].EnableWindow(FALSE);
        }
        else {
            HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), IDC_EDIT1+fld-1);
            if(hWnd!=NULL) {
//                BOOL bVisible = GetDlgItem(CtrlId(fld))->IsWindowVisible( );
//                if(bVisible==FALSE) {
                if(chMode=='T') {
                    m_dt[fld].SetReadOnly(TRUE);
                }
                else {
                    m_ed[fld].SetReadOnly(TRUE);
                }
            }
        }
    }
}


void CReqitem::SetAttribute(int flag,int fld)
{
    int kflag = flag ? FALSE : TRUE;
    for(int i=FirstField; i<=LastField; i++ ) {
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(i));
        if(hWnd==NULL) continue;               //08-04-2011
        char chMode = sub->mode( i );
        if(chMode=='L') {
            m_be[i].EnableWindow(kflag);
        }
        else if(chMode=='T') {
            BOOL bVisible = GetDlgItem(CtrlId(i))->IsWindowVisible( );
            if(bVisible==TRUE) {
                m_dt[i].SetReadOnly(flag);
            }
        }
        else {
            BOOL bVisible = GetDlgItem(CtrlId(i))->IsWindowVisible( );
            if(bVisible==TRUE) {
            }
            m_ed[i].SetReadOnly(flag);
        }
    }
}

void CReqitem::SetReadOnlyFields()
{
    int fld,i;

    TRACE("Initial Edit Status : %s\n",df->name);
    SetAttribute(FALSE,TRUE);
    for(i=0; fld = sub->ReadOnlyArray[i]; i++) {
        SetReadOnlyField(fld);
    }
    for(i=FirstField; i<= LastField ; i++) {                    //1.5.2003
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(i));
        if(hWnd==NULL) continue;               //08-04-2011
        if( sub->fieldAttribute[i]  & adFldUpdatable) {
             SetReadOnlyField(i);
        }
    }
}

/******************************************************************************
* USE DEFINE FIELD ENABLE/DISABLE                                             *
******************************************************************************/
void CReqitem::AddExtraUserLevelReadOnlyField()
{
    int fld,i,j,level;

    for(i=j=0; fld = sub->ReadOnlyArray[i]; i++) {
    }
    j = i;
    for(i=FirstField; i<= LastField ; i++) {
        if(ExistRecordNo) {
            level = sub->f[i].msi & 0x0f;
        }
        else {
            level = sub->f[i].cal & 0x0f;
        }
        if(UserLevel()<level) {
            sub->ReadOnlyArray[j++] = i;
        }
	}
    sub->ReadOnlyArray[j++] = 0;
}

void CReqitem::HideField()
{
    if(!sub->m_Adodc) return;
    for(int i=FirstField; i<= LastField ; i++) {
        int level = sub->f[i].blk;
        if(UserLevel()<level) {
            HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(i));
            if(hWnd==NULL) continue;               //08-04-2011
            ::HideDataField(this,i);
        }
	}
}

void CReqitem::ShowInitFrame()
{
}

BOOL CReqitem::OnInitDialog() 
{
    CScrollDialog::OnInitDialog();
    pDlg = this;

	HWND flag; 
	int i = 3;
    for(; i<= LastField; i++) {
        flag = GetDlgItem(CtrlId(i))->GetSafeHwnd();
        if(!flag) break;
    }

	
    sub->ReadOnlyMask();
    CParent *p = (CParent *) GetParent();
    if(NoParent==FALSE) {
        if(p->SaveFlag == CANNOTSAVE) {                       //6.5.2006
            if(sub->CanEditReqItem==FALSE) {
                Status  = 0x00ff;
            }
        }
        LastField = p->FindChildTotalFields(sub,(i-1));
    }
    CreateToolBarAndStatusBar();
    InitialEditFieldSetting();
    backup(sub);
    lset(sub->dat[1],df->dat[1]);
    if(FirstField>=3 && NoParent==FALSE) {
		p->Find_SequenceNo(df,sub);
	}
    for(i = FirstField; i<= LastField; i++) {
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(i));
        if(hWnd==NULL) continue;               //08-04-2011
        DWORD tt = GetDlgItem(CtrlId(i))->GetStyle() & WS_VSCROLL;
        if(tt){
            int v = sub->f[i].ext & 0x80;
            if(!v){
                DB *pf = GetMatchTable(i,sub);
                if(pf) {
                     sub->f[i].ext |= 0x80;
                     continue;
                }
				if( sub->DBDataType != DBDataType_DBASE) {         //27.9.2008
					BOOL flag = IsAdoTableExisted(sub->fieldName[i]);
	                if(flag) sub->f[i].ext |= 0x80;
				}
            }
        }
    }
    if(NoParent==FALSE) {
        p->OnReqitemInitDialog(this,ExistRecordNo);
    }
    (*sub->e->Relationships)(sub);                        // 14.5.2001
    if( ExistRecordNo==0 ) {
        CWnd *p = GetDlgItem(IDEND);
        if(p){
            p->ShowWindow(SW_HIDE);
        }
    }
    ShowInitFrame();
	ShowInformation();

    if(Status==0) {
        m_ToolBar.HideButton(ID_DUPLICATE,TRUE);
//      m_ToolBar.HideButton(ID_DEL,TRUE);
    }
    else if(Status==1) {
        m_ToolBar.HideButton(IDINSERT,TRUE);
    }
    else if(Status==2) {
        m_ToolBar.HideButton(ID_DUPLICATE,TRUE);
        m_ToolBar.HideButton(IDINSERT,TRUE);
    }
    else if(Status==4) {
        m_ToolBar.HideButton(ID_DUPLICATE,TRUE);
        m_ToolBar.HideButton(IDINSERT,TRUE);
        m_ToolBar.HideButton(ID_DEL,TRUE);
    }

    if( ExistRecordNo == 0) {
        m_ToolBar.HideButton(ID_DEL,TRUE);
        m_ToolBar.HideButton(ID_DUPLICATE,TRUE);
        m_ToolBar.HideButton(IDINSERT,TRUE);
    }
    if(HideDetailButton) m_ToolBar.HideButton(IDDETAIL,TRUE);

    SetControl();
    if(NoParent==FALSE) {
        p->ChildTellBefore(this,1);
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CReqitem::IsSaveConditionOk(DB *df) //04/01/2000
{
    if(NoParent==FALSE) {
        CParent *p = (CParent *) GetParent();
        if(p->IsDuplicateRecord(ExistRecordNo,df,IdxItem)==TRUE) {     //14/6/2000
            return 0;
        }
    }

    int flag = db_save(df,1);
    if(flag==0) return 0;
    if(df->e->save != db_save) {
        flag = (*df->e->save)(df,1);
        if(flag==0) return 0;
    }
    return 1;
}

void CReqitem::OnOK() 
{
//    ReplaceRecord();
    ReplaceFieldContent(CurrentField);
    int flag = IsSaveConditionOk(sub);
    if(flag==0) return;
    lset(sub->dat[1],df->dat[1]);
    sub->l->ExistRecordNo = ExistRecordNo;
    if((*sub->l->Save)(sub)==1){
        CScrollDialog::OnOK();
    }
}

void CReqitem::OnLastField()
{
     OnOK();
}

void CReqitem::OnCancel() 
{
    if(NoParent==FALSE) {
        CParent *p = (CParent *) GetParent();
        if( !(p->SaveFlag == OLDFILE || p->SaveFlag == NEWFILE) ) {
            CScrollDialog::OnCancel();
            return;
        }
    }
    sub->e->BeforeSave(sub);
    if(!isblank(sub,3)) {
        if(istwin(sub)==0) {                           //change only
            int flag = MessageBox("Save IT !","Ask ?",MB_YESNOCANCEL | MB_ICONQUESTION);
            if(flag == IDYES) {
                if(IsSaveConditionOk(sub)) {
                    OnOK();
                }
                return;
            }
            else if(flag == IDCANCEL) {
                return;
            }
        }
    }
    CScrollDialog::OnCancel();
}

void CReqitem::ReplaceFieldContent(int i)
{
    DBUpdateData(this,sub,i);
}

void CReqitem::AutoDate(DB *sub,int fld,int &nStartChar,int &nEndChar,char *date)
{
    AutoAdjDate(this,sub,fld,nStartChar,nEndChar,date);
}

int CReqitem::IsValideDate(DB *df,int fld)
{
    return IsValidDbData(this,df,fld);
}

void CReqitem::ReplaceRecord()
{
    for(int i=FirstField; i<=LastField; i++ ) {
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(i));
        if(hWnd==NULL) continue;               //08-04-2011
        ReplaceFieldContent(i);
	}
}

void CReqitem::OnDel()
{
    EndDialog(ID_DEL);
}

void CReqitem::OnInsert() 
{
	EndDialog(IDINSERT);
}

void CReqitem::OnEnd()
{
    EndDialog(IDEND);
}

void CReqitem::OnDuplicate() 
{
    EndDialog(ID_DUPLICATE);
}

int CReqitem::GetPreviousField(int fld)
{
    BOOL bVisible=FALSE;
    do {
        if(fld==FirstField) fld = LastField+1;
        fld--;
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(fld));
        if(hWnd==NULL) continue;               //08-04-2011
        DWORD tt = GetDlgItem(CtrlId(fld))->GetExStyle() & WS_EX_TRANSPARENT;
        if(tt){
            continue;
		}
        bVisible = GetDlgItem(CtrlId(fld))->IsWindowVisible( );
    } while (bVisible==FALSE);
    DWORD tt = GetDlgItem(CtrlId(fld))->GetStyle() & ES_READONLY;
    if(tt==0) return fld;
    return GetPreviousField(fld);
}

BOOL CReqitem::IsLastField(int fld)
{
    while(TRUE) {
        if(fld>=LastField) return TRUE;
        fld++;
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(fld));
        if(hWnd==NULL) continue;               //08-04-2011
        DWORD tt = GetDlgItem(CtrlId(fld))->GetExStyle() & WS_EX_TRANSPARENT;
        if(tt){
            continue;
		}
        if( GetDlgItem(CtrlId(fld))->IsWindowVisible( ) == TRUE) {
            tt = GetDlgItem(CtrlId(fld))->GetStyle() & ES_READONLY;
            if(tt==0) break;
        }
    }
    return FALSE;
}

int CReqitem::GetNextField(int fld)
{
    BOOL bVisible=FALSE;
    do {
        if(fld>=LastField) fld = FirstField - 1;
        fld++;
        HWND hWnd = ::GetDlgItem(this->GetSafeHwnd(), CtrlId(fld));
        if(hWnd==NULL) continue;               //08-04-2011
        DWORD tt = GetDlgItem(CtrlId(fld))->GetExStyle() & WS_EX_TRANSPARENT;
        if(tt){
            continue;
		}
        bVisible = GetDlgItem(CtrlId(fld))->IsWindowVisible( );
    } while (bVisible==FALSE);
    DWORD tt = GetDlgItem(CtrlId(fld))->GetStyle() & ES_READONLY;
    if(tt==0) return fld;
    return GetNextField(fld);
}

void CReqitem::OnGotoFirstField() 
{
    if(JumpFlag==FALSE) {
        GotoEdit(FirstField);
    }
    else {
        GotoEdit(FirstJumpField);
    }
}

void CReqitem::OnButton(UINT IDC)
{
    if(NoParent) return;

    CParent *p = (CParent *) GetParent();
    if(!p) return;

    p->OnSubPressButton(IDC);
    p->OnSubPressButton(IDC,this);
    ShowInformation();
    SetFocus();
}

void CReqitem::OnButton1() 
{
    OnButton(IDC_BUTTON1);
}

void CReqitem::OnButton2() 
{
    OnButton(IDC_BUTTON2);
}

void CReqitem::OnButton3() 
{
    OnButton(IDC_BUTTON3);
}

void CReqitem::OnButton4() 
{
    OnButton(IDC_BUTTON4);
}

void CReqitem::OnButton5()
{
    OnButton(IDC_BUTTON5);
}

void CReqitem::OnButton6()
{
    OnButton(IDC_BUTTON6);
}

void CReqitem::OnButton7()
{
    OnButton(IDC_BUTTON7);
}

void CReqitem::OnButton8()
{
    OnButton(IDC_BUTTON8);
}

void CReqitem::OnButton9()
{
    OnButton(IDC_BUTTON9);
}

void CReqitem::OnButton10()
{
    OnButton(IDC_BUTTON10);
}

void CReqitem::OnHitF2()
{
}

void CReqitem::OnHitF3()
{
}

void CReqitem::OnHitF4()
{
}

void CReqitem::OnHitF5()
{
}

void CReqitem::OnHitF6()
{
}

void CReqitem::GotoEdit(int fld)
{
    if(fld==0) {
        fld = CurrentField;
    }
    CurrentField = fld;
    if(isblank(sub->dat[fld]) && NoParent==FALSE) {
         CParent *p = (CParent *) GetParent();
         if( sub->mode(fld)=='T' &&
              (p->SaveFlag == OLDFILE || p->SaveFlag == NEWFILE) ) {
              sub->FldDefault(fld);
         }
    }
    ShowFieldContent(fld);
    ShowFieldNo(fld);
    (*sub->e->In) (sub, fld);
    GotoDlgCtrl(GetDlgItem( CtrlId(fld) ));
}

void CReqitem::OnStatic()
{
    int ShiftKeyStatus = GetKeyState(VK_SHIFT);
    if(ShiftKeyStatus<0){
        CDlgEdit::OnStatic();
        return;
    }
    AutoTable(this,STATIC_IDD,sub);
}

void CReqitem::SetControl()
{
	if((Status & 0x00ff) == 0x00ff  ||
        (*sub->e->IsRecordInLocked)(ExistRecordNo,sub,IdxItem)==TRUE) {     //ENQUIRY MODE (24/5/2000)
        m_ToolBar.HideButton(ID_DUPLICATE,TRUE);
        m_ToolBar.HideButton(ID_DEL,TRUE);
        m_ToolBar.HideButton(IDOK,TRUE);
        m_ToolBar.HideButton(IDINSERT,TRUE);
        int j=0;
        for(uint i=3; i<= sub->sumfield; i++,j++) {
            sub->ReadOnlyArray[j] = i;
        }
        sub->ReadOnlyArray[j] = 0;
        Status = 0x00ff;
    }
    else {
        sub->e->initial(sub,sub->ReadOnlyArray);              // 1/3/2000
        AddExtraUserLevelReadOnlyField();
        AddDeafaultReadOnlyField();
    }
    SetReadOnlyFields();
    HideField();
    PostMessage(WM_COMMAND, ID_GOTO_RECNO, (LPARAM)0);
    if(FirstJumpField>LastField) FirstJumpField = FirstField;
    if(FirstJumpField<FirstField) FirstJumpField = FirstField;
    int ScrollKeyStatus = GetKeyState(VK_SCROLL);
    if(ScrollKeyStatus==0) {
        m_StatusBar.ShowWindow(SW_HIDE);
    }
}

void CReqitem::AddDeafaultReadOnlyField()
{
    int fld,i,j,level;

    for(i=j=0; fld = sub->ReadOnlyArray[i]; i++) {
    }
    j = i;
    for(i=FirstField; i<= TotalField ; i++) {
        if( sub->fieldName[i].CompareNoCase("created_by")==0 ||
            sub->fieldName[i].CompareNoCase("created_on")==0 ||
            sub->fieldName[i].CompareNoCase("updated_by")==0 ||
            sub->fieldName[i].CompareNoCase("updated_on")==0) {

            if(j>= TotalField) break;
            if( IsReadOnlyExisted( sub, i )==FALSE) {
                sub->ReadOnlyArray[j++] = i;
            }
        }
	}
    sub->ReadOnlyArray[j++] = 0;
}
