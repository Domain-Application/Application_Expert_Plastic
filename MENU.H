// menu.h : main header file for the MENU application
//

#if !defined(AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_)
#define AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "mycctv.h"       // main symbols
#include <Tan.h>       // main symbols
//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
extern DB  taxcode;
extern DB  unit,icno;

extern int STCR;
extern void OpenARData();
extern void SetSlipTitle(DB *scr,DB *df);
extern CString LastFileName2(const char *szDirPath);
extern void OpenAPData();
extern void SetSubTotal(DB *df);
extern void SetWindowLocation(CDialog *p,int it1);
extern void init_creditor(DB *df);
extern void FindTaxCode(DB *df);
extern void PRintDespatchSlipAikha(long irec,FILE *ptr,DB *df);
extern void PRintReceiveSlipAikha(long irec,FILE *ptr,DB *df);
extern void FileChangeReport(DB *df);
extern unsigned char CompanyNo;
extern int ClientRequest(const char *server_name,char *command,CString &errmsg);
extern long tcpWeight;
extern unsigned char CompanyNo;
extern double price_unit;
extern char *TcpBuffer;
extern void TicketToApinv(DB *df);
extern void TicketToArinvNew(DB *df);
extern void PrintTicket(FILE *ptr,char *tl);
extern void init_icno(DB *df);
extern void init_tritem(DB *df);
extern void Pitran(long irec,FILE *ptr,DB *df);
extern void CollectTicket(DB *df,DB *rep,DB *data,DB *accode,int withRA);
extern void PublicWeighing();
extern void ExportInvoiceTransactions();
extern void ExportPurchaseInvoiceTransactions();
extern void ExportSalesInvoiceTransactions();
extern void Prodtype();
extern void SalesInvoice();
extern void PurchaseInvoice();
extern void PRintReceiveSlipKS(long irec,FILE *ptr,DB *df);
extern void init_inv(DB *df);
extern void init_invitem(DB *df);
extern void init_purinv(DB *df);
extern void init_purinvitem(DB *df);
extern void SetItemCost();
extern void ResetDatabase();
extern void CustomerMaintenance();
extern void SupplierMaintenance();
extern void ViewStockFlow(DB *df);
extern void FindProductPrice(DB *df);
extern double AikhaPercent;
extern void AdjustmentEntry();
extern void SingleWeighing();
extern void Receive_Capacity();
extern void OtherReceive_Capacity();
extern void init_adj(DB *df);
extern void init_adjitem(DB *df);
extern void Despatch_Capacity();
extern void PRintReceiveSlip(long irec,FILE *ptr,DB *df);
extern void PRintDespatchSlip(long irec,FILE *ptr,DB *df);
extern void ProfileData(DB *df);
extern void FindWBMoistWt(DB *df,int i);
extern void FindWBOutthrowWt(DB *df,int i);
extern int  PViewFlag;
extern void init_stdlist(DB *df);
extern void SetWBGridRows(CMSFlexGrid *grid);
extern void PrintLoadingUnloadingForm(long irec,FILE *ptr,DB *df);
extern void DetailGrossTareNetListingReport(DB *df);

extern void ViewContractTicketListing();

extern void init_pr(DB *df);
extern void init_prdata(DB *df);
extern void init_customer(DB *df);
extern void init_supplier(DB *df);
extern void init_ddata(DB *df);
extern void init_item(DB *df);
extern void init_swb(DB *df);
extern void init_do(DB *df);
extern void init_doitem(DB *df);
extern void init_accode(DB *df);
extern void init_first(DB *df);
extern void init_vtype(DB *df);
extern void init_receive(DB *df);
extern void init_despatch(DB *df);
extern void pslip1(long irec,FILE *ptr,DB *df);
extern int tscr_flag(DB *df);
extern void SetStdCompanyLogo();
extern long GwstWeight,ActualWeight;
extern void init_vtype(DB *df);
extern void compute_weight(DB *df);
extern void init_lorry(DB *df);
extern void init_vtype(DB *df);
extern void init_gst(DB *df);
extern void init_product(DB *df);
extern void init_driver(DB *df);
extern void init_accode(DB *df);
extern long weight1,weight2;
extern void PrintLogo(FILE *ptr,char *tl);

extern DB sg,glcode,currency,aripay,arpaydat,apipay,appaydat;
extern DB prefix,scheme,area,rdata,ddata,swb,adj,adjitem,reason;
extern DB *ttt,Status,stype,*accode,itran,tritem,puritran;
extern DB despatch,receive,prodtype,driver,gst,logo;
extern DB bal,one,user,login,report,two,lorry,vtype;
extern DB pr,fpr,profile,porla,transp,tender,gender,nationality,ttype,btype,risktype,ticno;
extern DB debtor,creditor,staff,cpitem,spitem,ptype;
extern DB inv,invitem,pinv,pinvitem,arinv,aroinv,apinv,apoinv;
extern DB logerr,accpf,trcv,trdata,pr,prdata,receipt,period;
extern DB purcinv,purcinvitem;

enum despatch {D_TICKET=1,D_LORRY,D_TRAILER, D_CONTAINER,
               D_DATE,D_TIME,D_DEBTOR,D_TRANSP,D_ICNO,D_TENDER,
               D_DO,D_DODATE,
               D_REM,D_REM2,D_COMPLETED, D_MODIFY, D_CASHBILL, D_RA,
               D_PTYPE, D_CHEQUE, D_CQ_DATE};
enum rdata {Wi_TICKET=1,Wi_SQNO,Wi_DATE,Wi_TIME,Wi_FIRST,Wi_SECOND,
            Wi_PT,Wi_BALES,Wi_MP,Wi_MOISTURE,Wi_OP,Wi_OUTTHROW,Wi_NETWT,
            Wi_PRICE,Wi_TRIPAMT,Wi_AMOUNT,
            Wi_TAXCODE,Wi_INCLUDE,Wi_PERCEN,Wi_TAXAMT,Wi_SUBTOTAL,
            Wi_W1,Wi_W2,Wi_Z1,Wi_Z2};

enum tscr {Da_D1=1,Da_D2,Da_TIME1,Da_TIME2,Da_ACCODE,Da_LORRY,Da_VTYPE,Da_PT,
           Da_TRANSP,Da_STAFF,Da_DRIVER,Da_STATUS,Da_REJECT,Da_PAGE};

enum pt  {PT_ITEM=1,PT_DES,PT_CAT,PT_UNIT,
            PT_GLCODE_PURCHASE,PT_GLCODE_SALE,
            PT_GLCODE_CASHPURCHASE,PT_GLCODE_CASHSALE,
            PT_BUY,PT_SALE,PT_REM,
            PT_TAXCODE1, PT_TAXCODE2};

enum  debtor { BU_CODE=1,BU_NAME,BU_SNAME,BU_ADD1,BU_ADD2,BU_ADD3,BU_TEL0,BU_TEL1,
BU_RESTEL,BU_HTEL,BU_FAX,BU_EMAIL,BU_AREA,BU_TYPE,BU_CURRENCY,
BU_STAFF,BU_CONTACT,
BU_GLCODE,BU_GLCODE2,BU_SCHEME,BU_PREFIX,BU_TERMS,
BU_LIMIT,BU_BAL,BU_ODATE,BU_STATEMENT,BU_ZERO,
BU_REM,BU_REM2,
BU_CUST_BAL};

enum two {Tw_D1=1,Tw_D2,Tw_PAGE};
enum profile {Pro_CODE=1,Pro_ADD1,Pro_ADD2,Pro_ADD3,
              Pro_PREFIX,Pro_WT,Pro_TCPIP,Pro_WTSERVER,
              Pro_BRANCH,Pro_RA,Pro_RA2,Pro_FACTOR,
              Pro_GSTNO,Pro_GST_DTE,Pro_GSTVER,Pro_ALERT21RULE,Pro_21RULE,
              Pro_Snapshot,Pro_Photo_Folder,Pro_Export_Pdf_Folder,Pro_IC_Folder,
              };

enum profile_acc {PF_COMPANY_NO=1,PF_ADD1,PF_ADD2,PF_ADD3,PF_TAXID,
              PF_DATE,PF_DEBTOR,
              PF_SALES,PF_CREDITOR,PF_PURCHASE,PF_PC,PF_BANK,PF_BC,PF_APF,PF_PF,
              PF_VAT,PF_AR_CGL,PF_AP_CGL,PF_CURRENCY,
              PF_ARPAY,PF_APPAY,PF_GLWAC,PF_ALIGN,PF_RPRINTER,
              PF_AR,PF_AP,PF_GL,PF_TAX,PF_GLTAX,PF_PREVIEW,PF_CUSTOMER_BANK,
              PF_TEMP_REC,PF_TAX_NO,PF_ETA_DATE,PF_PCCODE,
              PF_CUST_PO,PF_SUPP_INV,PF_COLLECT_BY,PF_PAID_BY,PF_PAYTYPE,
              PF_EMODE,PF_MULTI_CURR,PF_SCHEME,PF_TT,PF_PREFIX,
              PF_SHOW_OVER_CREDIT_LIMIT,PF_AREA,PF_SALESMAN,PF_PD_CHQ,
              PF_CONTRA,PF_RET_CHQ_RECORDS,PF_HOLD_PAYMENT,PF_PERIOD,
              PF_CN_CATEGORY,PF_MULTI_USER,PF_CLIENT_SERVER,PF_SERVER_NAME,
              };

enum glcode {C_CODE=1,C_DESCRIPT,C_GLGROUP,C_TYPE,CBAL};
enum lorry {Lo_LORRY=1,Lo_TRANSP,Lo_DRIVER,Lo_TARE,Lo_BLACKLIST,Lo_REMARKS,Lo_REMARK2,Lo_REMARK3,Lo_ICNO};
enum swb {Sw_TICKET=1,Sw_DATE,Sw_TIME,Sw_ACNAME,Sw_LORRY,Sw_NET,
Sw_TYPE,Sw_CHARGE,Sw_TAXCODE,Sw_INCLUSIVE,Sw_PERCEN,Sw_TAXAMT,Sw_SUBTOTAL,Sw_REM};

enum logo {Logo_CODE=1,Logo_NAME,Logo_ADD1,Logo_ADD2,Logo_ADD3,Logo_ADD4,
           Logo_DATE,Logo_DATE1,Logo_DATE2,Logo_TIME1,Logo_TIME2,
           Logo_SELECT1,Logo_SELECT2,Logo_SELECT3,Logo_RTITLE,Logo_GROUP,
           Logo_USER,Logo_USERNAME,Logo_PROGRAM_ID,Logo_CONDITION,Logo_PRINT_RC};
enum spitem  {Sp_CUSTOMER=1,Sp_SQNO,Sp_ITEM,Sp_PRICE};
enum adj     {Aj_REFNO=1,Aj_DATE,Aj_REASON,Aj_REMARKS};
enum adjitem {ADJ_REFNO=1,ADJ_SQNO,ADJ_ITEM,ADJ_QTY,
              ADJ_PRICE,ADJ_AMOUNT,ADJ_PKKEY};
enum inv {IV_INVOICE=1,IV_DATE,IV_ACCODE,IV_TERMS,IV_YOUR_REP,IV_OUR_REF,IV_REMARKS,
			IV_OTHER_GLCODE,IV_OTHERAMT};
enum invitem {VI_INVOICE=1,VI_SQNO,VI_DO,VI_QTY,VI_AMOUNT};

enum purinv {PIV_INVOICE=1,PIV_DATE,PIV_ACCODE,PIV_TERMS,PIV_YOUR_REP,PIV_OUR_REF,PIV_REMARKS,
             PIV_OTHER_GLCODE,PIV_OTHERAMT};
enum purinvitem {PVI_INVOICE=1,PVI_SQNO,PVI_DO,PVI_QTY,PVI_AMOUNT};

enum logerr {LGR_DATE=1,LGR_CASHIER,LGR_ACCESS,LGR_TICKET,LGR_TIME,LGR_PASS_TYPE,LGR_PROGRAM};
enum tritem {Tr_TICKET=3,Tr_DO,Tr_DATE,Tr_VEHICLE,
             Tr_PT,Tr_NAME,Tr_QTY,Tr_PRICE,Tr_AMOUNT,
             Tr_TAXCODE,Tr_INCLUDE,Tr_PERCEN,Tr_TAXAMT,Tr_SUBTOTAL,
             Tr_KEY,Tr_PKKEY};
enum itran {IQ_REFNO=1,IQ_DATE,IQ_DUE,IQ_CREDITOR,IQ_STAFF,
            IQ_TERMS,IQ_TAX, IQ_REM1,IQ_REM2,IQ_PYTYPE,IQ_PAYSTATUS,IQ_PAYDATE};

enum puritran {PIQ_REFNO=1,PIQ_DATE,PIQ_DUE,PIQ_DEBTOR,PIQ_STAFF,
            PIQ_TERMS,PIQ_TAX, PIQ_REM1,PIQ_REM2,PIQ_PYTYPE,PIQ_PAYSTATUS,PIQ_PAYDATE};

enum arinv {EIV_BILLNO=1,EIV_INVOICE,EIV_PONO,EIV_DATE,EIV_ETA,EIV_CUSTOMER,
            EIV_TYPE,EIV_STAFF,EIV_PREFIX,EIV_TERMS,
            EIV_CURR,EIV_RATE,EIV_PCCODE,EIV_DEPT,EIV_REM,EIV_HOLD_PAY,EIV_REASON,EIV_TOTAL,
            EIV_BANK,EIV_CHEQUE,EIV_CQ_DATE,EIV_BANKIN,EIV_POB_JOBFILE,EIV_CONTRAREF};
#define EIV_TT       EIV_TYPE
#define EIV_CURRENCY EIV_CURR
#define EIV_DEBTOR EIV_CUSTOMER
#define EIV_SG EIV_TYPE
#define EIV_PARTICULAR EIV_REM

enum oinv {Oiv_REFNO=1,Oiv_SQNO,Oiv_GLCODE,Oiv_NAME,
           Oiv_QTY,Oiv_PRICE,Oiv_AMT,Oiv_ANA,Oiv_ACODE,Oiv_PCCODE,
           Oiv_DEPT,Oiv_FA,Oiv_T,Oiv_PERCEN,Oiv_VAT,
           Oiv_INCLUDE,Oiv_JOBKEY,Oiv_JOBGRP};
#define Oiv_AMOUNT Oiv_AMT
#define D_CREDITOR D_DEBTOR
enum accpf   {APF_COMPANY_NO=1,APF_ADD1,APF_ADD2,APF_ADD3,APF_TAXID,APF_DATE,
              APF_DEBTOR,APF_SALES,APF_CREDITOR,APF_PURCHASE,APF_PC,
              APF_BANK,APF_BC,APF_APF,APF_PF,APF_VAT,
              APF_GLCODE11,APF_GLCODE12,APF_CURRENCY,APF_RPRINTER,
              APF_TAX,APF_PREVIEW,APF_CUST_BANK,APF_TEMP_REC,APF_TAX_NO,
              APF_ETA_DATE,APF_PCCODE,APF_CUST_PO,APF_SUPP_INV,APF_COLLECT_BY,
              APF_PAID_BY,APF_PAYTYPE,APF_EMODE,APF_MULTI_CURR,APF_SCHEME,
              APF_TT,APF_PREFIX,APF_SW_OVERCL,APF_AREA,APF_SALESMAN,APF_PD_CHQ,
              APF_CONTRA,APF_RET_CHQ,APF_HOLD_PAY,APF_MULTI_USER,APF_CLI_SVR,
              APF_SERVER};
enum receipt {RE_TICKET=1,RE_DO,RE_DATE,RE_VEHICLE,
              RE_PT,RE_NAME,RE_AMOUNT,RE_BAL,RE_QTY,RE_PRICE};
enum  paydat  {PY_BILL=1,PY_SQNO,PY_INVOICE,PY_DATE,PY_TOTAL,PY_GL};
enum  ipay    {IP_BILL=1,IP_CUSTOMER,IP_RECEIPT,IP_DATE,IP_STAFF,IP_CURR,IP_RATE,
               IP_PAYTYPE,IP_BANK2,IP_CHEQUE,IP_CQ_DATE,
               IP_GLCODE,IP_TOTAL,IP_RM,IP_BC,IP_REMARKS,
               IP_BANKIN,IP_RETURN,IP_CGL,IP_VATNO,IP_DISAMT,IP_PAYER,IP_FOREIGN_AMT,IP_FOREIGN_RATE,IP_CGL_DATE};
#define IP_CURRENCY IP_CURR
#define IP_BILLNO   IP_BILL
/////////////////////////////////////////////////////////////////////////////
// CMenuApp:
// See menu.cpp for the implementation of this class
//

class CMenuApp : public CMyCctvApp
{
public:
	CMenuApp();
    virtual BOOL Domain();
    virtual void Profile();
	virtual void OpenAllDataFile();
    virtual void SetStdCompanyLogo();
    virtual int SubProgram(int key);
    virtual CString GetTCPIP_Server();
    virtual void SetTitleColor();
    virtual void SetDBRelations();
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_)
