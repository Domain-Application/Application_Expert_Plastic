#include "stdafx.h"
#include "menu.h"
#include "Staff.h"
#include "AskPassword.h"
#include "Wb.h"

#include "CreateNewPhotoDatabase.h"

void CreateNewImageDatabase()
{
	CCreateNewPhotoDatabase h;
	//	h.AutoCreatePhotoDatabase = TRUE;
	h.DoModal();
	
}


int STCR;
enum menu {M_NORMAL=1,M_ADJ,M_CTRL,M_REPORT,M_OTHER};


void AdminDb(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CStaff p1(df->ID,df,df->f[1].idx & 0x01);
    p1.m_WinPrint = TRUE;
    p1.DoModal();
}

void ProfileData(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CStaff p1(df->ID,df,df->f[1].idx & 0x01);
    p1.m_WinPrint = TRUE;
    p1.OneRecordOnly = TRUE;
    p1.DoModal();
}

void PeriodControl(DB *df)
{
    CAskPassword p1;

    if(p1.DoModal() != IDOK) return;
    AdminDb(df);
}

void AdminTicketDb(DB *df,DB *sub)
{
    if(!stricmp(df->name,"receive") || !stricmp(df->name,"frcv")) {
        accode = &creditor;
    }
    else{
        accode = &debtor;
    }
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CWb p1(df->ID,df,df->f[1].idx & 0x01);
    p1.m_WinPrint = TRUE;
    p1.sub        = sub;
    p1.IDD_SUB    = IDD_DDATA;
    p1.DoModal();
}

int ProgramMenuNo,level;
int CMenuApp::SubProgram(int key)
{
    STCR = 0;
    ProgramMenuNo = 0;
    switch(key) {
    case M_NORMAL*10+1:
		OpenAPData();
        STCR = 1;
        Receive_Capacity();
        break;
    case M_NORMAL*10+2:
		OpenARData();
        STCR = 2;
        Despatch_Capacity();
        break;
    case M_NORMAL*10+3:
        STCR = 3;
        SingleWeighing();
        break;
    case M_ADJ*10+1:
        AdjustmentEntry();
        break;
    case M_ADJ*10+2:
		OpenAPData();
        STCR = 3;
        PurchaseInvoice();
        break;
    case M_ADJ*10+3:
		OpenARData();
        STCR = 2;
        SalesInvoice();
        break;
    case M_ADJ*10+5:
        ExportInvoiceTransactions();
        break;
    case 4:
        TableContainer("wbctn");
        break;
    case M_CTRL*10+1:
		OpenARData();
        CustomerMaintenance();
        break;
    case M_CTRL*10+2:
        OpenAPData();
        SupplierMaintenance();
        break;
    case M_CTRL*10+3:
		Prodtype();
        break;
    case M_CTRL*10+4:     
		icno.Run();
        break;		
    case M_CTRL*10+5:        
        AdminDb(&driver);
        break;		
    case M_CTRL*10+6:        
		AdminDb(&transp);
        break;		
    case M_CTRL*10+7:        
		AdminDb(&lorry);
        break;		
    case (M_CTRL*10+8) * 10 + 1:
		AdminDb(&ptype);
        break;		
    case (M_CTRL*10+8) * 10 + 2:
		AdminDb(&gst);
        break;		
    case (M_CTRL*10+8) * 10 + 3:
		AdminDb(&reason);
        break;	
    case (M_CTRL*10+8) * 10 + 4:
        AdminDb(&tender);
        break;
	case (M_CTRL*10+8) * 10 + 5:
        AdminDb(&gender);
        break;
	case (M_CTRL*10+8) * 10 + 6:
        AdminDb(&nationality);
        break;		
	case (M_CTRL*10+8) * 10 + 7:
        AdminDb(&ttype);
        break;
	case (M_CTRL*10+8) * 10 + 8:
        AdminDb(&btype);
        break;
	case (M_CTRL*10+8) * 10 + 9:
        AdminDb(&risktype);
        break;
	case (M_CTRL*10+9) * 10 + 1:
        AdminDb(&ticno);
        break;
    case M_OTHER*10+2:
        ProfileData(&profile);
        break;
    case M_OTHER*10+3:
		SetUser();
        break;
    case M_OTHER*10+4:
        BackupProcess();
        break;
    case M_OTHER*10+5:
        CreateNewImageDatabase();
        break;
    case M_OTHER*10+6:
        ResetDatabase();
        break;
    case M_OTHER*10+7:
        ExportPurchaseInvoiceTransactions();
        break;
    case M_OTHER*10+8:
        PeriodControl(&period);
        break;
    case 1001:
        FileChangeReport(&receive);
        break;
    case 1002:
        FileChangeReport(&rdata);
        break;
    case 1003:
        FileChangeReport(&despatch);
        break;
    case 1004:
        FileChangeReport(&ddata);
        break;
    case 1005:
        FileChangeReport(&swb);
        break;
    default:
         return 0;
    }
    return 1;
}

void PrintLogo(FILE *ptr,char *tl)
{
}
