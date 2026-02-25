// menu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"

#include "MainFrm.h"
#include "menuDoc.h"
#include "NView.h"
#include "password.h"
#include "start.h"
#include "openrs.h"
#include <locale.h>
#include <initial.h>
#include "lszip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DB btype(IDD_BTYPE),risktype(IDD_RISKTYPE),ticno(IDD_TICNO);
DB  icno(IDD_ICNO),gender(IDD_GENDER),ttype(IDD_TTYPE),nationality(IDD_NTLY);
DB period(IDD_PERIOD,4);
DB *accode,prodtype(IDD_PT,2),Status(IDD_STATUS);
DB tscr(IDD_TSCR),gst,stype,itran(IDD_INV,4),tritem(IDD_INVITEM),puritran(IDD_PINV,4);
DB swb(IDD_SWB,7),*ttt,receipt(IDD_INVITEM,8);
DB lorry(IDD_LORRY,5),vtype(IDD_VTYPE);
DB driver(IDD_DRIVER,3),transp(IDD_TRANSP),tender(IDD_TENDER);
DB debtor(IDD_DEBTOR),creditor(IDD_DEBTOR),staff(IDD_STAFF);
DB receive(IDD_WB),despatch(IDD_WB);
DB ldo(IDD_DO),doitem(IDD_DOITEM),item,rdata(IDD_DDATA),ddata(IDD_DDATA);
DB prefix,area(IDD_AREA),adj(IDD_ADJ),adjitem(IDD_ADJITEM);
DB sg,glcode(IDD_GLCODE),currency,accpf,logerr;
DB reason(IDD_REASON),spitem(IDD_SPITEM),cpitem(IDD_SPITEM),ptype(IDD_PTYPE);
DB inv(IDD_INV,3),invitem(IDD_INVITEM),pinv(IDD_INV,3),pinvitem(IDD_INVITEM);
DB purinv(IDD_PINV,3),purinvitem(IDD_PINVITEM);
DB trcv(IDD_TRCV),trdata(IDD_DDATA),arinv,aroinv,apinv,apoinv,glgroup(IDD_GLGROUP,4);
DB taxcode(IDD_TAXCODE);
DB unit(IDD_UNIT,3);
DB aripay,arpaydat,apipay,appaydat;
DB ddata_unbill_data;
double price_unit=1000;
double AikhaPercent;

unsigned char CompanyNo;
/////////////////////////////////////////////////////////////////////////////
// CMenuApp initialization

char *tcompany[] = { 
	               "9A69E99AC9A9ECE92ADAB9A95ABAECB9AA0AECCA6AAA",       //EXPERT PLASTIC SDN BHD
                   ""
                    };

CMenuApp theApp;
CMenuApp::CMenuApp()
{
    IsSqlFile = TRUE;
    GetSystemProviderString();
    GetInternetFolderString();

    CompanyNo = getCompanyNo();

    int company_no = CompanyNo = 0;     

	price_unit = 1;
    if(company_no<0) company_no = 0;
    if(company_no>=3 && company_no<=8) {
        company =  tcompany[company_no];
    }
	else if (company_no==10 || company_no == 11|| company_no==12|| company_no==15){
        company =  tcompany[company_no];
	}
	else {
		company =  tcompany[company_no];
	}

    m_Local1Instance = FALSE;
	m_manifest = TRUE;

    m_TableNameStyle = 1;			//0 no change 1 upper 2 lower 3 capital first letter
	//m_sync_style = 1;		//1 new domain connect sync
    m_ReportFolder = "report\\";

}

/////////////////////////////////////////////////////////////////////////////
// CMenuApp initialization
CString CMenuApp::GetTCPIP_Server()
{
    return profile.dat[Pro_WTSERVER];
}

void CMenuApp::Profile()
{
    AikhaPercent = 0;


    SetRegistryKey(_T("ks-win server"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
}

BOOL CMenuApp::Domain()
{
	//

    CStart p1;
    p1.InitialFlag = 0x0004 | INIT_ROUND |
                    INIT_CANNOTCREATENEWITEM |
                    INIT_ADO_USER |
                    INIT_ADO_PROGRAM_PASSWORD |
                    INIT_LOCAL_REPORT | INIT_ADO_USER_WITH_CRYPT | 
                    INIT_ENCRYPT_PROGRAM_PASSWORD;
    p1.DefaultUser = m_defaultUser;
    p1.DoModal();
    if(p1.rec==0) return FALSE;
    UserLogin(p1.UserID);
    CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
        IDR_MAINMENU,
		RUNTIME_CLASS(CMenuDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CNView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

    HideStaffMenu = 99990;
    m_useDropDownBtnInsteadSrollbar = TRUE;
#ifdef _DEBUG
#else
    level_Adoarange_Advance_Button = 99990;
#endif //_DEBUG

	
	return TRUE;
}


//DB logo;
void CMenuApp::SetStdCompanyLogo()
{
    fetch(&logo, &profile);
    lset(logo.dat[Logo_NAME],title);
    lset(logo.dat[Logo_CODE],profile.dat[Pro_CODE]);
    lset(logo.dat[Logo_ADD1],profile.dat[Pro_ADD1]);
    lset(logo.dat[Logo_ADD2],profile.dat[Pro_ADD2]);
    lset(logo.dat[Logo_ADD3],profile.dat[Pro_ADD3]);
    lset(logo.dat[Logo_PROGRAM_ID],Pref);
}

void CMenuApp::OpenAllDataFile()
{
    if (ado(&accpf,"profile")){
        readrec(1,&accpf);
    }
    if(ado(&profile,"wbprofile")) {
		profile.ID = IDD_PROFILE;
        readrec(1,&profile);
        ado(&logo,"logo");
        readrec(1,&logo);
        SetStdCompanyLogo();
        logo.SqlUpdate();
    }
    strBranchTable = "branch";

    ado(&icno,"icno",init_icno);
    ado(&taxcode,"taxcode");
    ado(&unit,"unit");
    tado(&swb,"swb",init_swb);
    tado(&rdata,"rdata",init_ddata);
    tado(&receive,"receive",init_receive);
    tado(&ddata,"ddata",init_ddata);
    tado(&despatch,"despatch",init_despatch);
	tado(&trcv,"trcv",init_receive);
    tado(&trdata,"trdata",init_ddata);	

    ado(&prodtype,"pt");
    ado(&lorry,"lorry");
    ado(&driver,"driver");
    ado(&transp,"transp");
    ado(&tender,"tender");
    ado(&staff,"staff");
    ado(&area,"area");
	ado(&gender,"gender");
	ado(&nationality,"nationality");
	ado(&ttype,"ttype");
	ado(&btype,"btype");
	ado(&risktype,"risktype");
	ado(&ticno,"ticno");
    if(ado(&gst,"gst")) {
        readrec(1,&gst);
    }
    ado(&stype,"stype");
	ado(&ptype,"ptype");
    ado(&Status,"status");
    ado(&debtor,"debtor",init_customer);
    ado(&creditor,"creditor",init_supplier);
    tado(&adj,"adj",init_adj);
    tado(&adjitem,"adjitem",init_adjitem);
    tado(&spitem,"spitem");
    tado(&cpitem,"cpitem");
    ado(&reason,"reason");
	ado(&glcode,"glcode");
    tado(&inv,"inv",init_inv);
    tado(&invitem,"invitem",init_invitem);
	tado(&purinv,"purcinv",init_purinv);
    tado(&purinvitem,"purcinvitem",init_purinvitem);
	tado(&pinv,"pinv",init_inv);
    tado(&pinvitem,"pinvitem",init_invitem);
	tado(&logerr,"logerr");

    tado(&itran,"itran",init_inv);
	tado(&puritran,"puritran",init_purinv);
    tado(&tritem,"tritem",init_tritem);
    tado(&arinv,"arinv");
    tado(&aroinv,"aroinv");
    tado(&apinv,"apinv");
    tado(&apoinv,"apoinv");

    dopen(&receipt,"receipt.rep");
    ado(&period,"period_wb");
    ado(&glgroup,"glgroup");

    tado(&aripay,"aripay");
    tado(&arpaydat,"arpaydat");
    tado(&apipay,"apipay");
    tado(&appaydat,"appaydat");

}

void CMenuApp::SetDBRelations()
{
    itran.m_PrintOneFormOnly = TRUE;
    itran.m_PreviewCrystalReport = TRUE;
    itran.m_LpView = "lp_itran";

	puritran.m_PrintOneFormOnly = TRUE;
    puritran.m_PreviewCrystalReport = TRUE;
    puritran.m_LpView = "lp_puritran";

    receive.m_pdf = despatch.m_pdf = TRUE;

    receive.m_PrintOneFormOnly = 
    swb.m_PrintOneFormOnly = 
    despatch.m_PrintOneFormOnly = TRUE;

    receive.m_PreviewCrystalReport = 
    despatch.m_PreviewCrystalReport = FALSE;

    swb.m_PreviewCrystalReport = TRUE;

    itran.m_PrintOneFormOnly = 
    itran.m_PreviewCrystalReport = TRUE;
	puritran.m_PrintOneFormOnly = 
    puritran.m_PreviewCrystalReport = TRUE;

    debtor.IdIcon   = IDI_LOGOFF;
    creditor.IdIcon   = IDI_LOGOFF;
    lorry.IdIcon    = IDI_EMPTY_VEHICLE;
    profile.IdIcon  = IDI_CONFIGURATION;
    user.IdIcon  = IDI_CHAIN;
	prodtype.IdIcon  = IDI_ICON_PRODUCTS;
	driver.IdIcon  = IDI_ICON_DRIVER;
    tender.IdIcon  = IDI_ICON_LOCATION;
    transp.IdIcon  = IDI_ICON_TRANSPORTER;
}

void OpenARData()
{
	taxcode.m_strCmdText.Format("select * from TAXCODE where TAX_TYPE='O' and ISNULL(INACTIVE,0)<>1");
	taxcode.ReOpen();
    accode = &debtor;
}

void OpenAPData()
{
	taxcode.m_strCmdText.Format("select * from TAXCODE where TAX_TYPE='I' and ISNULL(INACTIVE,0)<>1");
	taxcode.ReOpen();
    accode = &creditor;
}

void SetWindowLocation(CDialog *p,int it1)
{

//    int screen_width = app->longWindowWidth;

    RECT rect,rectOld;
	
    p->GetWindowRect(&rectOld);
    p->GetWindowRect(&rect);
	
	p->ClientToScreen(&rect);

    int width = rect.right - rect.left;
    rect.left = it1;
    rect.right = it1 + width;

    p->MoveWindow(&rect);

}


CString LastFileName2(const char *szDirPath)
{
	CString s;
	
	for(int i=1; i<100; i++) {
		s.Format("%s\\%3.3i.jpg",szDirPath,i);
		if(IsFileExisted(s)==FALSE) break;
	}
	
	return s;
}


void CMenuApp::SetTitleColor()
{
    app->DialogIcon = IDI_SCALE;
    if(STCR == 1) {
        app->DialogIcon2 = IDI_R;
        int r = 255;
        int g = 0;
        int b = 0;
        app->clrFrom = r  + g * 256 + b* 256 *256;
        app->clrTo   = RGB(255,255,255);
        app->ChangeDialogCaptionColor = TRUE;
        app->ChangeTabCtnCaptionColor = TRUE;
    }
    else if(STCR == 2) {
        app->DialogIcon2 = IDI_D;
        int r = 0;
        int g = 0;
        int b = 155;
        app->clrFrom = r  + g * 256 + b* 256 *256;
        app->clrTo   = RGB(255,255,255);
        app->ChangeDialogCaptionColor = TRUE;
        app->ChangeTabCtnCaptionColor = TRUE;
    }
    else if(STCR == 3) {
        app->DialogIcon2 = IDI_P;
        int r = 30;
        int g = 205;
        int b = 30;
        app->clrFrom = r  + g * 256 + b* 256 *256;
        app->clrTo   = RGB(255,255,255);
        app->ChangeDialogCaptionColor = TRUE;
        app->ChangeTabCtnCaptionColor = TRUE;
    }
    else {
        app->ChangeDialogCaptionColor = FALSE;
        app->ChangeTabCtnCaptionColor = FALSE;
    }
}
