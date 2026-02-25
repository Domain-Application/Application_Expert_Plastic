// Fgw.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "Fgw.h"
#include "Wb.h"
#include "vfw.h"

#pragma comment (lib, "vfw32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int openrs_rows;
int oldfg_key = 0;
/////////////////////////////////////////////////////////////////////////////
// CFgw dialog


CFgw::CFgw(UINT IDD,CWnd* pParent /*=NULL*/)
    : CConfirmWeight(IDD, pParent)
{
	//{{AFX_DATA_INIT(CFgw)
	m_product = _T("");
	m_moisture = _T("");
	m_outthrow = _T("");
	m_lastwt = _T("");
	m_ptname = _T("");
	m_price = _T("");
	m_TripAmt = _T("");
	//}}AFX_DATA_INIT
    lastwt = 0;
    fgkey = oldfg_key;
    percent = AikhaPercent;
    DirectRead = FALSE;
    m_app = "v1.bat";
}

void CFgw::DoDataExchange(CDataExchange* pDX)
{
    CConfirmWeight::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFgw)
    DDX_Control(pDX, IDC_PRODUCT, m_cbProduct);
	DDX_CBString(pDX, IDC_PRODUCT, m_product);
	DDX_CBString(pDX, IDC_COMBO1, m_moisture);
	DDX_CBString(pDX, IDC_COMBO2, m_outthrow);
	DDX_Text(pDX, IDC_LASTWT, m_lastwt);
	DDX_Text(pDX, IDC_STATIC_PT, m_ptname);
	DDX_Text(pDX, IDC_EDIT2, m_price);
	DDX_Text(pDX, IDC_EDIT3, m_TripAmt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFgw, CConfirmWeight)
	//{{AFX_MSG_MAP(CFgw)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_EDITCHANGE(IDC_PRODUCT, OnEditchangeProduct)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CFgw::OnDestroy()
{
    CConfirmWeight::OnDestroy();
}

LRESULT CALLBACK EXPORT ErrorCallbackProc(HWND hWnd,int nErrID,LPSTR lpErrorText)
{
	if(nErrID==0)
		return true;
	AfxMessageBox(lpErrorText,MB_OK|NULL);
	return true;
}

LRESULT FAR PASCAL StatusCallbackProc(HWND hWnd,int nID,LPSTR lpStatusText)
{
	static int CurrentID;
	if(nID==IDS_CAP_END)
	{
		if((CurrentID==IDS_CAP_STAT_VIDEOAUDIO)||(CurrentID==IDS_CAP_STAT_VIDEOONLY))
			return true;
	}
	CurrentID=nID;
	return (LRESULT)true;
}

LRESULT FAR PASCAL VideoCallbackProc(HWND hWnd,LPVIDEOHDR lpVHdr)
{
//	((CMainFrame *)AfxGetMainWnd())->m_dibinfo.bitmapheader.biSizeImage =lpVHdr->dwBytesUsed ;
//	memcpy(((CMainFrame *)AfxGetMainWnd())->m_dibinfo.buffer +((CMainFrame *)AfxGetMainWnd())->m_dibinfo.VideoFormatSize,lpVHdr->lpData,lpVHdr->dwBytesUsed);
//	(((CMainFrame *)AfxGetMainWnd())->GetActiveView())->InvalidateRect(NULL,FALSE);
	return (LRESULT)true;
}

/////////////////////////////////////////////////////////////////////////////
// CFgw message handlers

BOOL CFgw::OnInitDialog() 
{


    int ShiftKeyStatus = GetKeyState(VK_SCROLL);
    if(ShiftKeyStatus<=0) {
        percent = 0;
    }
	CConfirmWeight::OnInitDialog();


    int IdxItem = prodtype.f[1].idx & 1;
    long last = read_sumindex(IdxItem,&prodtype);
	
    FindLastWeight();
    char szTmp[20];
    dtoa(lastwt, szTmp, 6, 0);
    m_lastwt = putnum(szTmp);
    UpdateData(FALSE);
    if(lastwt==0) {
        GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
        last = 0;
        m_cbProduct.AddString("**None**");
    }
	for(long rec=1; rec<=last; rec++) {
        ridx(rec, &prodtype , IdxItem);
        m_cbProduct.AddString(prodtype.dat[1]);
	}
	UINT SW = (bWtInc==TRUE) ? SW_HIDE : SW_SHOW;
	GetDlgItem(IDC_COMBO1)->ShowWindow(SW);
	GetDlgItem(IDC_COMBO2)->ShowWindow(SW);
	GetDlgItem(IDC_EDIT2)->ShowWindow(SW);
	GetDlgItem(IDC_EDIT3)->ShowWindow(SW);
	for(int i=0; i<6; i++) {
		GetDlgItem(IDC_STATIC1+i)->ShowWindow(SW);
	}
    if(price_unit==1){
        SetDlgItemText(IDC_STATIC5, "(RM / kg)");
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFgw::FindLastWeight()
{
    CString str;

    str.Format("select top 1 [second] from [%s] where [%s]=%s and (pt <> '%s' or pt is null) order by [sqno] desc",
        sub->name, sub->fieldName[1], DbText(df,1),profile.dat[iRA]);

    double wt = SqlValue(str);

    lastwt = long(wt);
    if(lastwt) return;

    str.Format("select top 1 [first] from [%s] where [%s]=%s and (pt <> '%s' or pt is null) order by [sqno] desc",
        sub->name, sub->fieldName[1], DbText(df,1),profile.dat[iRA]);

    wt = SqlValue(str);

    lastwt = long(wt);
}

void CFgw::OnButton1() 
{
	long rec = viewidx(&prodtype);
	if(rec>0) {
        m_product = prodtype.dat[1];
        m_ptname  = prodtype.dat[2];
        SetSpitem();
	}
}


void CFgw::SetSpitem()
{
    CString str;
    double price = 0;

    if(bWtInc==TRUE) {
        str.Format("select [price] from [cpitem] where [accode]=%s and [pt]=%s",
            DbText(&despatch, D_DEBTOR), DbText(&prodtype,1));
        price = SqlValue(str);
        if(openrs_rows==0) {
            price = prodtype.val(PT_SALE);
        }
    }
    else {
        str.Format("select [price] from [spitem] where [accode]=%s and [pt]=%s",
            DbText(&receive, D_DEBTOR), DbText(&prodtype,1));
        price = SqlValue(str);
        if(openrs_rows==0) {
            price = prodtype.val(PT_BUY);
        }
    }
    m_price.Format("%9.2f",price);
    UpdateData(FALSE);
}

int CFgw::OnGw()
{
    if(DirectRead) {
        weight = GetCurrenctCommWeight(1);
    }
    else {

        CString errmsg,str,WeightServer;
        WeightServer = profile.dat[Pro_WTSERVER];
        WeightServer.TrimRight();
        int flag = ClientRequest(WeightServer,"W1",errmsg);
        weight = tcpWeight;
    }
    actualWeight = weight;
    int ShiftKeyStatus = GetKeyState(VK_SCROLL);
    if(ShiftKeyStatus<=0) {
        percent = 0;
        fgkey = 0;
    }
#ifdef _DEBUG
//    weight  = 19000;
#endif //_DEBUG
    if(weight >= profile.val(Pro_WT)) {
        if(bWtInc==FALSE) {                         //Receiving Only
            long iPercent = StoreProcedure("dt_dwPercent", "1", 1);
            long iWeight  = StoreProcedure("dt_dwWeight", "1", 1);
            if(iPercent>0 && iWeight==-1) {
                percent = iPercent / 100;
                weight  *= 1 - (percent/100);
                weight  = radix(weight);
			}
            else if(iPercent==-1 && iWeight>10) {
                weight  -= iWeight;
                if(weight<0) weight = 0;
			}
            else {
                oldfg_key = fgkey;
                if(fgkey==0) {
                    AikhaPercent = percent;
                    weight  *= 1 - (percent/100);
                    weight  = radix(weight);
                }
                else {
                    int idx = 2 + fgkey - 'A';
                    weight  -= gst.val(idx);
                    if(weight<0) weight = 0;
                }
            }
        }
    }

    char tmp[80];
    if(weight != -99) {
        sprintf(tmp,"%6.6ld kg",weight);
    }
    else {
        sprintf(tmp,"No Wt.");
    }
	m_wt.SetText(tmp);
    return TRUE;
}

void CFgw::OnEditchangeProduct() 
{
	lset(prodtype.dat[1],m_product);
	qd(prodtype.dat[1],&prodtype);
    m_ptname = prodtype.dat[2];
    SetSpitem();
}

void CFgw::OnSnapshot()
{
}

void CFgw::OnOK() 
{
    UpdateData();

    if(weight == -99) {
        MessageBox("\nWeight signal lost\n\nPlease check indicator/cable/computer com port!!\n","Signal Lost",MB_ICONSTOP);
        return;
    }
    if(lastwt==0) {
        if( weight < 10) {
            MessageBox("\nPlease double confirm your weight!!\n","--- Weight too low ---");
            return;
        }
        OnSnapshot();
        CConfirmWeight::OnOK();
        return;
    }
    if(bWtInc==TRUE) {
        if(weight < lastwt) {
            if(MessageBox("Goods Despatch\nSecond weight must greater than first weigh.\n\nClick yes to exit\nClick no to continue",
                "Incorrect weighing step",MB_ICONQUESTION | MB_YESNO)==IDYES) return;
        }
    }
    else {
        if(weight > lastwt) {
            if(MessageBox("Goods receiving\nSecond weight must less than first weigh.\n\nClick yes to exit\nClick no to continue",
                "Incorrect weighing step",MB_ICONQUESTION | MB_YESNO)==IDYES) return;
        }
    }
	lset(prodtype.dat[1],m_product);
    long xrec = qd(prodtype.dat[1],&prodtype);
    if(xrec==0) {
        MessageBox("Please key in product","Data Incompleted");
        return;
    }
    if(atof(m_price)==0) {
        SetSpitem();
    }
    OnSnapshot();
	CConfirmWeight::OnOK();
}
