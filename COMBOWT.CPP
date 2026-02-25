// ComboWt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "ComboWt.h"
#include "Fgw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboWt

CComboWt::CComboWt()
{

}

CComboWt::~CComboWt()
{
}


BEGIN_MESSAGE_MAP(CComboWt, CComboBox)
	//{{AFX_MSG_MAP(CComboWt)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboWt message handlers

UINT CComboWt::OnGetDlgCode() 
{
    return CComboBox::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void CComboWt::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
 	CFgw *h = (CFgw *) GetParent();
    if(nChar==VK_ESCAPE) {
        CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
        h->OnCancel();
        return;
    }
    else if(nChar==VK_RETURN) {
        CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
//        h->NextDlgCtrl();
        h->OnOK();
        return;
    }
    else if(nChar == VK_PRIOR) {
        CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
        h->PrevDlgCtrl();
        return;
    }
    else if(nChar == VK_NEXT) {
        CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
        h->NextDlgCtrl();
        return;
    }
    int ShiftKeyStatus = GetKeyState(VK_SCROLL);
    if(ShiftKeyStatus<=0) {
        h->percent = 0;
        h->fgkey = 0;
    }
    else {
        if(nChar>=VK_F1 && VK_F2<=VK_F10) {
            h->percent = double (nChar + 1 - VK_F1);
            h->fgkey = 0;
        }
        else if(nChar==VK_SPACE) {
            h->percent = 0;
            h->fgkey = 0;
        }
        else {
        }
    }
	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CComboWt::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CFgw *h = (CFgw *) GetParent();
    int ShiftKeyStatus = GetKeyState(VK_SCROLL);
	if(ShiftKeyStatus>0) {
        if(nChar>='A' && nChar<='Z') {
            h->fgkey = nChar;
        }
        if(nChar>=1 && nChar<=26) {
            if(!(nChar==8 || nChar==13)) {
                h->fgkey = nChar + 'A' - 1;
            }
        }
    }
	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}
