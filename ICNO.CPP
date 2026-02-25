// icno.cpp : implementation file
//

#include "stdafx.h"
#include "db_ado.h"
#include "openrs.h"
#include "menu.h"
#include "icno.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DB icno_imp;
enum icno {ICNO_CODE=1,ICNO_NAME,ICNO_GENDER,ICNO_DOB,ICNO_POB,ICNO_ETHNICITY,ICNO_CITIZENSHIP,ICNO_RELIGION,
            ICNO_RADD1,ICNO_RADD2,ICNO_RADD3,
            ICNO_ADD1,ICNO_ADD2,ICNO_ADD3,
            ICNO_TOWN,ICNO_POSTCODE,ICNO_STATE,ICNO_JOIN,ICNO_EXPIRY,
            ICNO_OLDIC,ICNO_TEL,ICNO_TEL2,ICNO_FAX,ICNO_EMAIL,ICNO_BANK,ICNO_ACNO,
            ICNO_TTYPE,ICNO_PCCODE,ICNO_POSITION,ICNO_BTYPE,ICNO_EMPLOYER,
            ICNO_ALERTMSG,ICNO_RISKTYPE,ICNO_PTYPE,
			ICNO_TICNO,ICNO_NATIONALITY,ICNO_REMARKS};
/////////////////////////////////////////////////////////////////////////////
// Cicno dialog


Cicno::Cicno(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(Cicno)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    if(RIGHT_POWER !=3) {       //view only
        csImail  ="Scan IC";
        m_iMailEnabled = TRUE;
    }
	ExtraButton1 = IDC_BUTTON2;
    width = height;
}


void Cicno::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(Cicno)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Cicno, CStaff)
    //{{AFX_MSG_MAP(Cicno)
    ON_WM_DESTROY()
    ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cicno message handlers

CString Cicno::GetJpgName(int no)
{
	CString str = "";
    char szCode[5000];
	
    strcpy_s(szCode, sizeof(szCode),df->dat[1]);
	rtrim(szCode);
    if(no==1) {
        str.Format("ic\\%s.jpg",strCode);
    }
    else if(no>1) {
        str.Format("%s.%s_1.1i.jpg",df->m_strTable,szCode, no);
        str.Format("ic\\%s_%1.1i.jpg",strCode,no);
    }
    return str;
	/*
	CString str;

    str.Format("ic\\%s.jpg", df->dat[1]);
    return str;
	*/
}
CString isJpgFileLoaded;
void Cicno::ShowJpg(BOOL reDraw)
{
    if(m_showjpg==FALSE) return;
    //if(m_NeedShowDetail == FALSE) return;
    if (!reDraw) if(strCode.Compare(df->dat[1])==0) return;
	
    //strCode = df->dat[1];
	char szRefno[30];
	strcpy(szRefno,df->dat[1]);	
	replace(szRefno,"/","-");
	replace(szRefno,"/","-");
	strCode = szRefno;
    strCode.TrimRight();
    strCode.TrimLeft();
	
    CString str = GetJpgName(1);
	
    isJpgFileLoaded = m_jpg[1].Load(str);
    m_picutreName = str;
	Invalidate();	
}

void Cicno::ShowInformation()
{
    CStaff::ShowInformation();
}

void Cicno::OnInternalMail()
{
	//SetTimer(1,500,NULL);  //must delete
	//return;

	
	DeleteFile("c:\\MyKad\\mykaddata.txt");
	DeleteFile("c:\\MyKad\\mykadpicture.bmp");

	WinExec("c:\\MyKad\\mykad.exe", SW_SHOW);

	SetTimer(1,500,NULL);
}

BOOL Cicno::OnInitDialog()
{
    CStaff::OnInitDialog();

	int fld_width = GetDbFieldNo(&logo,"IC_WIDTH");
	int fld_height = GetDbFieldNo(&logo,"IC_HEIGHT");
    if(fld_width && fld_height) {
        width = logo.lval(fld_width);
        height = logo.lval(fld_height);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void Cicno::OnDestroy()
{
    CStaff::OnDestroy();

    // TODO: Add your message handler code here

}


DB tmp_icno;
void Cicno::OnImportData(DB *pf, char *data,int dwSize)
{
    char text[1024];

    memset(text, 0, sizeof(text));
    int j = 0, fld=1;
    for(int i=0; i<dwSize; i++) {
        char ch = data[i];
        text[j++] = ch;
        if(ch==9) {
            text[j++] = 0;
            if(pf->f[fld].mode=='D') {
                Cdate(pf->dat[fld], text);
			}
			else {
                lset(pf->dat[fld], text);
			}
			memset(text, 0, sizeof(text));
			fld++;
			if(fld> pf->sumfield) break;
            j = 0;
        }
    }

    tado(&tmp_icno,"icno");

    fetch(df,pf);
    lset(tmp_icno.dat[1], pf->dat[1]);
    long xrec = qd(tmp_icno.dat[1], &tmp_icno);
    if(xrec==0) {
        df->AddNew();
    }
    else {
        fetch(&tmp_icno, df);
        tmp_icno.SqlUpdate();
    }
	
    dclose(&tmp_icno);

}

void Cicno::OnTimer(UINT nIDEvent)
{
    CStaff::OnTimer(nIDEvent);

    if(nIDEvent != 1) return;

    if(!IsFileExisted("c:\\MyKad\\mykaddata.txt")) return;

	CString icpicture= "c:\\MyKad\\mykadpicture.bmp";


    if(!IsFileExisted(icpicture)) {

		icpicture="c:\\MyKad\\mykad-bmp.bmp";

		if(!IsFileExisted(icpicture))

			return;
	}
	

    KillTimer(1);

    char lpBuffer[1024];


    HANDLE handle = bopen("c:\\MyKad\\mykaddata.txt");

    int dwSize = GetFileSize (handle, NULL);
    if(dwSize < 1024) {
        qread( handle, lpBuffer , dwSize);
        lset(df->dat[1], lpBuffer);
        long xrec = df->Search(df->dat[1], 1, TRUE);
        if(xrec>0) {
			ShowInformation();
			CloseHandle(handle);
			return;
			CString icPicture;
			icPicture.Format("ic\\%s.jpg", df->dat[1]);
			icPicture.Replace(" ",""); 
			
			
			if(!IsFileExisted(icPicture)){
				
				
				UINT flag = GetCurrentDirectory(1000,lpBuffer);
				
				CString newfile2;
				newfile2.Format("%s\\ic\\%s.jpg", lpBuffer, df->dat[1]);
				//MoveFile("c:\\MyKad\\mykadpicture.bmp", newfile);
				Dib2Jpg(icpicture, newfile2);
				
				
                int fld2 = GetDbFieldNo(&profile,"WEBPHOTOFOLDER");
				if(fld2) {
                    CString pFolder2 = profile.dat[fld2];
					pFolder2.TrimRight();
					pFolder2 += df->dat[1];
					pFolder2 += ".jpg";
					
					CopyFile(newfile2, pFolder2, FALSE);
				}
				
				strCode = "";     
			}
			
            CloseHandle(handle);
			ShowInformation();
			ShowJpg(1);
            OnImportData(&icno_imp,lpBuffer,dwSize);
            ChangeToEditMode( 2 );
            GotoEdit( 2 );
            
		
			CloseHandle(handle);
			
			return;
        }
        OnImportData(&icno_imp,lpBuffer,dwSize);
    }
    CloseHandle(handle);

    UINT flag = GetCurrentDirectory(1000,lpBuffer);
	
    CString newfile;
    newfile.Format("%s\\ic\\%s.jpg", lpBuffer, df->dat[1]);
    //MoveFile("c:\\MyKad\\mykadpicture.bmp", newfile);
    Dib2Jpg(icpicture, newfile);
	

    int fld = GetDbFieldNo(&profile,"WEBPHOTOFOLDER");
    if(fld) {
        CString pFolder = profile.dat[fld];
        pFolder.TrimRight();
        pFolder += df->dat[1];
        pFolder += ".jpg";

        CopyFile(newfile, pFolder, FALSE);
    }

    strCode = "";           //redraw picture
    ShowInformation();
    ShowJpg(1);
    ChangeToEditMode( 2 );
    GotoEdit( 2 );
}


void AdminDbicno(DB *df)
{
    if(df->ID==0) {                           
        AfxMessageBox("Database not open");   
        return;                               
    }

    tado(&icno_imp,"icno_imp");
    Cicno p1(df->ID,df,df->f[1].idx & 0x01);
    p1.DoModal();    
	
    dclose(&icno_imp);
}

int icno_AdminDb(DB *df,int flag)
{


    AdminDbicno(df);

    return 1;
}

int tell_icno(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    return 1;
}

int start_icno(DB *df)
{
	
    return 1;
}

int initial_icno(DB *df,int *fld)
{

	
    return 1;
}

BOOL IsValidPhoneNumber(const char *telno)
{
	char tel[1000];

	char ch1 = telno[0];
	char ch2 = telno[1];
	int offset = 0;
	if(ch1=='+') offset = 1;
	if(ch1=='0' && ch2=='0') offset = 2;

	strcpy(tel, &telno[offset]);
	trim(tel);
    BOOL flag = IsDigitsText(tel);
    if(!flag) return FALSE;

    int len = countstr(tel);
    if(len<10) return FALSE;

    return TRUE;
}

int save_icno(DB *df,int flag)
{
    return 1;
}

void Relationships_icno(DB *df)
{
}

CString GetItemJpgFileLocation(DB *pf)
{
    CString str = "";
	if(pf->m_JpgFolder.GetLength()<=0) return str;

    str.Format("ic\\%s.jpg", pf->dat[1]);
    return str;
}

void init_icno(DB *df)
{
    df->e->AdminDb = icno_AdminDb;
    df->e->tell = tell_icno;
    df->e->start = start_icno;
    df->e->initial = initial_icno;
    df->e->save = save_icno;
    df->e->Relationships = Relationships_icno;
    df->e->GetJpgName = GetItemJpgFileLocation;
	df->m_JpgFolder = "ic";
	df->TwinField=26;
    df->NodeNo = 55;
}

void Cicno::OnSnapshotDoc() 
{
}

void Cicno::OnViewDoc() 
{
}

void Cicno::OnHistoryRecords() 
{
	
}

void Cicno::OnButton2() 
{
}

void Cicno::OnPaint()
{
	KB v;

	if(m_ToolBar) {
		v.code = m_ToolBar.GetButtonSize();
		if(v.code != buttonSize.code) {
			CSize c;
			c.cx = buttonSize.sh[0];
			c.cy = buttonSize.sh[1];
			m_ToolBar.SetButtonSize( c );
		}
	}

    if(m_showjpg==FALSE) {
		CDialog::OnPaint();
        return;
    }


	CPaintDC dc(this); // device context for painting
    for(int i=1; i<=8;i++) {
		CWnd* previewWnd = GetDlgItem(IDC_PREVIEW+i-1);
        if(!previewWnd) continue;

		if(width>0 && height>0) {
			PreviewRect[i].right = PreviewRect[i].left + width;
			PreviewRect[i].bottom = PreviewRect[i].top + height;
		}


		m_jpg[i].Draw(&dc,PreviewRect[i].left,PreviewRect[i].top,
                   PreviewRect[i].Width(),PreviewRect[i].Height());
    }
}
