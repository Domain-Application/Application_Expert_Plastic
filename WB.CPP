// Wb.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "Wb.h"
#include "mgrid.h"
#include "myjpg.h"
#include "Fgw.h"
#include "FastSearch.h"
#include "AskPrintMode.h"
#include "TakePhoto.h"

#define TotalCameras 2

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int PrintMode =1;
/////////////////////////////////////////////////////////////////////////////
// CWb dialog


CWb::CWb(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CWb)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_Wizard   = TRUE;
    m_WinPrint = FALSE;
    m_OpenLpt  = TRUE;
    b_AskContinue = TRUE;
    df->m_WhileEditNoPrint = TRUE;
    iRA = 0;
    m_jpg = "final.jpg";
	//df->m_CannotAdd = TRUE;
	jpgkey = 0;
    csPrint2  ="Preview";
    m_Print2 = TRUE;
}

void CWb::OnButton2()
{
	CString szDirPath = df->dat[1];

	
	if(df->name[0]=='R' || df->name[0]=='r') {
		szDirPath.Format("jpg\\receiving\\%s",df->dat[1]);
	}
	else {
		szDirPath.Format("jpg\\despatch\\%s",df->dat[1]);
	}
	
	
	szDirPath.TrimRight();
    OpenFolderDoc(szDirPath);
}

void CWb::OnDecimalAdjustment()
{
    if(!iRA) return;

    BOOL flag = dBfldLogic(df,D_RA);
    if(flag==FALSE) return;

    long xrec = qd(profile.dat[iRA], &prodtype);
    if(xrec==0) return;

    str.Format("select sum(amount) from [%s] where [%s]=%s and pt <> '%s'",
        sub->name, sub->fieldName[1], DbText(df, 1),profile.dat[iRA]);
    double amt = SqlValue(str);
    if(amt<0.5) return;

    long intamt1 = long(amt) * 100;

    double ame = amt * 100;
    long intamt2 = long(ame);
	long intamt = intamt2 - intamt1;
    double adjamt = 0;
    if(intamt >= profile.lval(Pro_FACTOR)) {
        adjamt = long(amt) + 1 - amt;
    }
    else {
        adjamt = long(amt) - amt;
    }

    str.Format("select count(*) from [%s] where [%s]=%s and pt = '%s'",
        sub->name, sub->fieldName[1], DbText(df, 1) ,profile.dat[iRA]);
    double no = SqlValue(str);
    if(no) {
        str.Format("delete from [%s] where [%s]=%s and pt = '%s'",
             sub->name, sub->fieldName[1], DbText(df, 1) ,profile.dat[iRA]);
        RunSql(str);
    }
    lsetblank(sub);
    lset(sub->dat[1], df->dat[1]);
    lset(sub->dat[Wi_PT], profile.dat[iRA]);
    dtodb(adjamt , sub, Wi_AMOUNT);
    if(sub->val(Wi_AMOUNT)) {
        sub->AddNew();
    }
}

void CWb::HideNoUseButtonMenu()
{
    CParent::HideNoUseButtonMenu();
    HideButton(ID_WIZARD,TRUE);
}

void CWb::ShowSaveButton()
{
    CParent::ShowSaveButton();
    HideButton(ID_WIZARD,FALSE);
}
/*

BOOL CWb::BlockByOtherCondition()
{
	int fld = GetDbFieldNo(df,"PRINTED");
	if(fld>0) {
		if(df->Boolean(fld)==TRUE) {
			if(UserLevel()<99) {
				MessageBox("block");
				return TRUE;
			}
		}
	}
	return CStaff::BlockByOtherCondition();
}
 
*/
/*
void CWb::OnPrint()
{
	CString str;

	str.Format("update [%s] set printed=1 where [%s] = %s", 
		df->m_strTable, df->fieldName[1], DbText(df, 1));
	RunSql(str);

    m_NeedShowDetail = TRUE;
    ShowInformation();
    CParent::OnPrint();
    m_NeedShowDetail = TRUE;
    ShowInformation();
}*/

void CWb::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    fg.Initial(grid,sub,2, float(1.14) );
    fg.SetTitle("No.",315,0);
    fg.SetTitle("Date",950,1);
    fg.SetTitle("Time",740,1);
    fg.SetTitle("Bales",540,7);
    fg.SetTitle("1st Wt",690,7);
    fg.SetTitle("2nd Wt",690,7);
    fg.SetTitle("Gross Wt",800,7);
    fg.SetTitle("Product",790,1);
    fg.SetTitle("Description",1800,1);
   // fg.SetTitle("%",345,7);
  //  fg.SetTitle("Moisture",735,7);  //new (hide % and moisture) Brian 20230530
  //  fg.SetTitle("%",405,7);
    fg.SetTitle("Out throw",800,7);
    fg.SetTitle("Nett Wt",735,7);
    fg.SetTitle("U/Price",690,7);
    fg.SetTitle("Trip",600,7);
    fg.SetTitle("Amount",800,7);
   // fg.SetTitle("",80);
    fg.SetTitle("Tax Code",900);
    fg.SetTitle("Inc Tax",800);
    fg.SetTitle("%",500,7);
    fg.SetTitle("SST",650,7);
    fg.SetTitle("Sub Total",900,7);
}

void CWb::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    fg.show(rec);

    fg.SetText(sub,0,Wi_DATE,Wi_TIME,Wi_BALES,-1);

    if(sub->val(Wi_SECOND)==0 && sub->val(Wi_FIRST)>0 && sub->val(Wi_W1)==0) {
        fg.UserColor = RGB(255,0,200);
        fg.UseUserColor = TRUE;
    }
    fg.SetText(sub,Wi_FIRST,-1);
    fg.UseUserColor = FALSE;

    if(sub->val(Wi_SECOND)>0 && sub->val(Wi_W2)==0) {
        fg.UserColor = RGB(255,0,100);
        fg.UseUserColor = TRUE;
    }
    fg.SetText(sub,Wi_SECOND,-1);
    fg.UseUserColor = FALSE;

    double gw = fabs( sub->val(Wi_FIRST) - sub->val(Wi_SECOND));
    char tmp[10];
    dtoa(gw, tmp, 6, 0);
    fg.SetText(putnum(tmp));
   /* fg.SetText(sub,Wi_PT,Wi_PT | 0x100,Wi_MP,Wi_MOISTURE,Wi_OP,Wi_OUTTHROW,Wi_NETWT,
      Wi_PRICE,Wi_TRIPAMT,Wi_AMOUNT,-1);*/ //old one
	fg.SetText(sub,Wi_PT,Wi_PT | 0x100,Wi_OUTTHROW,Wi_NETWT,  //new (hide % and moisture) Brian 20230530
      Wi_PRICE,Wi_TRIPAMT,Wi_AMOUNT,-1);
    fg.SetText("");
    fg.SetText(sub,Wi_TAXCODE,Wi_INCLUDE,Wi_PERCEN,Wi_TAXAMT,Wi_SUBTOTAL,-1);
}

void CWb::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWb)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CWb::ShowInformation()
{
    CParent::ShowInformation();

	CString pszPath,Result,strSql1,strSql2;
	
	if(df->name[0]=='R' || df->name[0]=='r') {
		pszPath.Format("jpg\\receiving\\%s",df->dat[1]);
	}
	else {
		pszPath.Format("jpg\\despatch\\%s",df->dat[1]);
	}
	

	pszPath.TrimRight();
	if(IsFolderExist(pszPath)) {
        GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
	}
	else {
        GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	}

   int fld = GetDbFieldNo(df,"NEWUSER");
   if(fld) {
	   SetDlgItemText(IDC_USER, df->dat[fld]);
   }
	   
	
   if(df->name[0]=='D') {
	   str.Format("select top 1 refno from tritem where ticket = '%s'", df->dat[1]);
	   SetDlgItemText(IDC_NAME1, SqlText(str));
	   strSql1.Format("select top 1 refno from tritem where ticket = '%s'", df->dat[1]);
		Result=SqlText(strSql1);
	   strSql2.Format("select PAYSTATUS from puritran where REFNO = '%s'", Result);
	   SetDlgItemText(IDC_NAME2, SqlText(strSql2));
	  
	   return;
   }

   if(df->name[0]=='R') {
	   str.Format("select top 1 refno from tritem where ticket = '%s'", df->dat[1]);
	   SetDlgItemText(IDC_NAME1, SqlText(str));
	   strSql1.Format("select top 1 refno from tritem where ticket = '%s'", df->dat[1]);
		Result=SqlText(strSql1);
	   strSql2.Format("select PAYSTATUS from itran where REFNO = '%s'", Result);
	   SetDlgItemText(IDC_NAME2, SqlText(strSql2));
	   return;
   }	

    GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
    char key[25];

    strcpy(key,apinv.dat[1]);
    lset(key,profile.dat[Pro_PREFIX]);
    lset(&key[1],df->dat[1]);

    rtrim(key);
    long xrec = apinv.Search( key , 1);
    if(xrec) {
        SetDlgItemText(IDC_ACINV, key);
        str.Format("select sum(amt) from apoinv where [refno]='%s'", key);
        double amt = SqlValue(str);
        dtoa(amt , key, 10, 2);
        SetDlgItemText(IDC_ACAMT, putnum0(key));
        double var = Double(fm.acc[Wi_AMOUNT] - amt);
        dtoa( var , key, 10, 2);
        SetDlgItemText(IDC_ACVAR, putnum0(key));
        if(var) {
             GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
        }
    }
	else {
        SetDlgItemText(IDC_ACINV, "");
        SetDlgItemText(IDC_ACAMT, "");
        SetDlgItemText(IDC_ACVAR, "");
	}
}

BEGIN_MESSAGE_MAP(CWb, CParent)
	//{{AFX_MSG_MAP(CWb)
//	ON_BN_CLICKED(IDC_BUTTON1, OnExport)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWb message handlers

void CWb::OnDetail()
{
	lsetblank(&tritem);
	long xrec = tritem.Search(df->dat[1],Tr_TICKET);
	if(xrec>0)
	{
		AfxMessageBox("The invoice is exist. Weighing process cannot be conducted" );
		return;
	}
     if(strcmp(df->dat[D_DATE],today)) {
          if(MessageBox("Do you want to continue ?","Date error",MB_ICONQUESTION | MB_YESNO)==IDNO) return;
     }

     lsetblank(sub);
     sub->e->start(sub);                             //initial setting


     UINT IDD = IDD_FGW;
     CFgw h1(IDD);

     h1.df = df;
     h1.sub = sub;
     h1.iRA  = iRA;
     h1.bWtInc = bWtInc;

     if(dBfldLogic(&profile,Pro_TCPIP)==0) {
         h1.DirectRead = TRUE;
     }
     int flag = h1.DoModal();
     if(flag==IDCANCEL){
         return;
     }
	 
     if(NewFlag == NEW_MODE) {
         if(SaveNewFileOnly()==FALSE) return;
         SaveFlag = OLDFILE;
         NewFlag  = ADDEDIT_MODE;
     }
     lsetblank(sub);
     sub->e->start(sub);                             //initial setting
	 fillblank(2,sub); // if not fillblank the SQNO will set arcordingly b'cos if SQNO have value it will not autoset
     lset(sub->dat[1],df->dat[1]);
     dbasedate(sub->dat[Wi_DATE]);
     _strtime(sub->dat[Wi_TIME]);
     if(h1.lastwt>0) {
        lset(sub->dat[Wi_PT],prodtype.dat[1]);
        dtodb(h1.lastwt, sub, Wi_FIRST);
        ltodb(h1.weight, sub, Wi_SECOND);
        ltodb(h1.weight, sub, Wi_W2);
        ltodb(h1.actualWeight, sub, Wi_Z2);
        double moisture = atof(h1.m_moisture);
        dtodb(moisture, sub, Wi_MP);
        double op = atof(h1.m_outthrow);
        dtodb(op, sub, Wi_OP);
        double price = atof(h1.m_price);
        dtodb(price, sub, Wi_PRICE);

        double tamt = atof(h1.m_TripAmt);
        dtodb(tamt, sub, Wi_TRIPAMT);
        FindTaxCode(sub);
        FindWBMoistWt(sub,Wi_MOISTURE);
        FindWBOutthrowWt(sub,Wi_OUTTHROW);
        double total = sub->val(Wi_PRICE) * sub->val(Wi_NETWT) / price_unit;
        total += sub->val(Wi_TRIPAMT);

		double adjamt;
		
		if (CompanyNo==9){
			adjamt =  0;		
		}
		else{
			adjamt = RoundingAdj( total);
		}

		//double adjamt = RoundingAdj( total);
        dtodb(total + adjamt,sub,Wi_AMOUNT);
        SetSubTotal(sub);
     }
     else {
         ltodb(h1.weight, sub, Wi_FIRST);
         ltodb(h1.weight, sub, Wi_W1);
         ltodb(h1.actualWeight, sub, Wi_Z1);
     }

     waitflock(sub);
     Find_SequenceNo(df,sub);
     save_entry(NONEXIST,IdxItem,0,sub);
     unflock(sub);
     (*sub->l->compute)(sub->l->flag,sub,TRUE);
     OnDecimalAdjustment();
     OnSnapshot();

     ReFreshSubFile(); //          m_NeedShowDetail = TRUE;
     ShowInformation();
}

void CWb::OnJpg2Db(DB *df,int i)
{
	CString str, file;

	if(jpgkey==0) {
		jpgkey = app->CreateNewJpgFile();
	}
	
    app->m_SQLServerName = app->Find_SQLServerName();
    app->m_SQLDatabase   = app->Find_SQLDatabaseName();
    app->m_SQLUserID     = app->Find_SQLUserID();
    app->m_SQLPassword   = app->Find_SQLPassword();

	CString jpg = strPhoto[i];
    int flagPhoto = 0;

	ltodb(jpgkey, df, "JPGKEY");


    CString fldName;
	fldName.Format("jpg%2.2i",i + 11);
	CMyJpg p;
	
	int flag = p.textcopy("jpgnew", fldName, jpgkey , jpg, 'I');
	if(flag==0) {
//		str.Format("insert into logJpg2Db (jpgkey,no,imagesize,len) values (%i,%i,%i,%i)", jpgkey,i,__imagefilesize,__imagefilelen);
//		runsql(str);
	}
}

void CWb::OnSnapshot()
{
     if(!profile.Boolean(Pro_Snapshot)) return;

	 int i;
	 CString path;

     jpgkey=0;
     for(i=0; i<TotalCameras; i++) {
         BOOL flag = app->OnSnapshot(1+i, m_jpg,50,255);

         if(df->name[0]=='R' || df->name[0]=='r') {
             path.Format("jpg\\receiving\\%s",df->dat[1]);
         }
         else {
             path.Format("jpg\\despatch\\%s",df->dat[1]);
         }

         path.TrimRight();

         CreateDirectory( path,NULL );

         CString filename = LastFileName2( path );
         filename.TrimRight();

         flag = MoveFile( m_jpg, filename);
         if(flag) {
             strPhoto[i] = filename;
         }
         OnJpg2Db(sub,i);
     }
	 sub->SqlUpdate();			//update jpgkey

}

void CWb::OnSearch()
{
    CFastSearch h1;
    h1.df = df;
    int flag = h1.DoModal();
    if(flag==IDC_BUTTON1) {
        CStaff::OnSearch();
    }
    else if(flag==IDOK) {
        ShowInformation();
    }
}

void CWb::OnWizard()
{
    CParent::OnDetail();

    OnDecimalAdjustment();
    ReFreshSubFile();
    ShowInformation();
}

void CWb::OnReqitemInitDialog(CDialog *p,int status)
{
    CString s =  "MT";
    if(price_unit==1) s = "kg";
    str.Format("Unit Price (RM)/%s", s);
    p->SetDlgItemText(IDC_PRICE, str);
}

BOOL CWb::OnInitDialog()
{
    CParent::OnInitDialog();
    SetWindowLocation( this , 0);
    return TRUE;
}

//Date 12-11-2004
void CWb::OnSubModifyCompleted()
{
    if( sub->val(Wi_SECOND) != atof(sub->bak[Wi_SECOND]) ){
        SetModifyMarkProcess(D_MODIFY,df,"*");
    }
    else if( sub->val(Wi_FIRST) != atof(sub->bak[Wi_FIRST]) ){
        SetModifyMarkProcess(D_MODIFY,df,"#");
    }
    else if( (sub->val(Wi_MOISTURE) != atof(sub->bak[Wi_MOISTURE])) ||
             (sub->val(Wi_MP) != atof(sub->bak[Wi_MP])) ){
        SetModifyMarkProcess(D_MODIFY,df,"m");
    }
    else if( (sub->val(Wi_OP) != atof(sub->bak[Wi_OP])) ||
             (sub->val(Wi_OUTTHROW) != atof(sub->bak[Wi_OUTTHROW])) ){
        SetModifyMarkProcess(D_MODIFY,df,"o");
    }
    else if( sub->val(Wi_PRICE) != atof(sub->bak[Wi_PRICE]) ) {
        SetModifyMarkProcess(D_MODIFY,df,"@");
    }
    OnDecimalAdjustment();
    df->Update();
}

void CWb::OnExport() 
{
    BOOL flag = dBfldLogic(df,D_CASHBILL);
    if(flag) return;

    TicketToApinv(df);
	TicketToArinvNew(df);
    ShowInformation();
}

void CWb::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==99) {
        if(busy == TRUE) return;
        busy = TRUE;
        if(IsFileExisted(m_jpg)) {
            KillTimer(99);
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

            CString filename = LastFileName2( path );
            filename.TrimRight();

            BOOL flag = MoveFile( m_jpg, filename);

            busy = FALSE;
            return;
        }

        busy = FALSE;
	}
	CParent::OnTimer(nIDEvent);
}

void CWb::OnPDF()
{
	CString pdf_path,file;
	
	file.Format("%s_MIX.RPT",df->name);
	pdf_path = profile.dat[Pro_Export_Pdf_Folder];
	pdf_path.TrimRight();
	if(isblank(pdf_path)) {
		df->m_PreviewCrystalReport = TRUE;
		df->PrintOneFormOnly(file);
		df->m_PreviewCrystalReport = FALSE;
		return;
	}
	
    df->strWinWordFileName.Format("%s\\%s",pdf_path,df->dat[1]);
    df->strWinWordFileName.TrimRight();
    df->strWinWordFileName += ".pdf";
    DeleteFile(df->strWinWordFileName);
    BOOL flag = app->ToPDF(df, file, FALSE);
	if(!flag) {
		MessageBox("Can not create pdf file");
		return;
	}
    HINSTANCE flag2 = ShellExecute(
        NULL,
        "OPEN",
        df->strWinWordFileName,
        NULL,
        NULL,
        SW_SHOW
        );
	
}

void CWb::OnPrintTicket()
{
    char ch = df->name[0];

	if(CompanyNo == 15) {
		PrintMode =1;
	}
	else {
		CAskPrintMode h;
		UINT ID = h.DoModal();
		if(ID==IDCANCEL) return;
	}

	CString str;

    if(df->m_hasPrintButton) {
        str.Format("update [%s] set printed=1 where [%s] = %s",
            df->m_strTable, df->fieldName[1], DbText(df, 1));
        RunSql(str);
    }

    m_NeedShowDetail = TRUE;
    ShowInformation();

    if( PrintMode ==1 || PrintMode ==3) {
        CString file = (STCR == 2) ? "report\\despatch.rpt" : "report\\receive.rpt";
        df->PrintOneFormOnly(file);
    }
    if( PrintMode ==2 || PrintMode ==3) {
        CString file = (STCR == 2) ? "report\\despatch_inv.rpt" : "report\\receive_sr.rpt";
        df->PrintOneFormOnly(file);
    }
    if(df->autoaddprintcounter && df->m_hasPrintButton) {
        df->IncPrinterCounter();
        if(STCR==1) {
            TicketToApinv(df);
			TicketToArinvNew(df);
        }
    }

    m_NeedShowDetail = TRUE;
    ShowInformation();
}
/*
int DeleteArinv_wb(long irec,DB *df,int IdxItem)
{
	
    char invno[30];
    strcpy(invno,arinv.dat[1]);
    lset(invno,df->dat[1]);
	long xrec =0,xrec2=0;
	if(!stricmp(df->name,"apinv_wb")) {
		xrec = apinv.Search(invno,1); //acc\ap\invoice	
		xrec2 = arinv_sv.Search(invno,IVSV_PINVOICE);
		if(xrec2 >0)
		{
			CString str;
			str.Format("Error Message\n\nCannot delete Ticket No: %s\nbecause this ticket exist in invoice.\n",df->dat[1]);
			AfxMessageBox(str);
			return 0;
		}
		
	}
	else{
		xrec = arinv.Search(invno,1); 
	}
    
	if(xrec>0) {
		if (UserLevel()>=998){ 			
			if(AfxMessageBox("This invoice already post to ACC\nConfirm Delete?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 )==IDYES){
				return Delete(irec,df,IdxItem);
			}
		}
        CString str;
        str.Format("Error Message\n\nCannot delete Invoice No: %s\nbecause this invoice already Post to ACC.\n",df->dat[1]);
        AfxMessageBox(str);
		return 0;
    }
    else {
        return Delete(irec,df,IdxItem);
    }
}
*/
void CWb::OnPrint()
{
    df->m_hasPrintButton = TRUE;
    OnPrintTicket();
}

void CWb::OnPressPrint2()
{
	int tmp = df->m_PreviewCrystalReport;
	df->m_PreviewCrystalReport = TRUE;
    df->m_hasPrintButton = TRUE;
    OnPrintTicket();
	df->m_PreviewCrystalReport = tmp;
}
