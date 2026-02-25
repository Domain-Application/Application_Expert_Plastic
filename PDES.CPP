// slip1.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"


enum pdes {SL_TICKET=1,SL_DATE,SL_LORRY,SL_TRAILER,SL_DO,SL_CONTNO,
           SL_SUPPLIER,SL_ADD1,SL_ADD2,SL_ADD3,SL_TRANSP,SL_DRIVER,SL_DRNAME,
           SL_TIME1,SL_PT1,SL_P1,SL_Y1,SL_Y11,SL_G1,SL_M1,SL_O1,SL_W1,SL_V1,
           SL_TIME2,SL_PT2,SL_P2,SL_Y2,SL_Y22,SL_G2,SL_M2,SL_O2,SL_W2,SL_V2,
           SL_TIME3,SL_PT3,SL_P3,SL_Y3,SL_Y33,SL_G3,SL_M3,SL_O3,SL_W3,SL_V3,
           SL_TIME4,SL_PT4,SL_P4,SL_Y4,SL_Y44,SL_G4,SL_M4,SL_O4,SL_W4,SL_V4,
           SL_TIME5,SL_PT5,SL_P5,SL_Y5,SL_Y55,SL_G5,SL_M5,SL_O5,SL_W5,SL_V5,
           SL_TIME6,SL_PT6,SL_P6,SL_Y6,SL_Y56,SL_G6,SL_M6,SL_O6,SL_W6,SL_V6,
           SL_TIME7,SL_PT7,SL_P7,SL_Y7,SL_Y57,SL_G7,SL_M7,SL_O7,SL_W7,SL_V7,
           SL_TIME8,SL_PT8,SL_P8,SL_Y8,SL_Y58,SL_G8,SL_M8,SL_O8,SL_W8,SL_V8,
           SL_WASTAGE,SL_OT,SL_NET,SL_RM,SL_REMARKS,SL_UTNAME,SL_USER};


void PRintDespatchSlip(long irec,FILE *ptr,DB *df)
{
    if(CompanyNo==7 || CompanyNo==8) {
         PRintDespatchSlipAikha(irec,ptr,df);
         return;
    }
#ifdef _DEBUG
		ptr = fopen("slip.txt","w");
#else
		ptr = fopen("lpt1","w");
#endif //_DEBUG

    if(CompanyNo==4) {
        dopen(&report,"pdes_cf.rep");
    }
    else {
        dopen(&report,"pdes.rep");
    }
    CollectTicket(df,&report,&ddata,&debtor,0);
    PrintTicket(ptr,title);
    mprint(ptr,&report);
    fprintf(ptr,"");
	dclose(&report);

    fclose(ptr);
#ifdef _DEBUG
    junction("slip.txt",0);
#else
#endif //_DEBUG
}
