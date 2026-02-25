// Inv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "openrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//

char TmpWbrep[200];
void set_debtor_variable()
{
    int tdays;

    _PL[0] = debtor.dat[BU_NAME];
    _PL[1] = debtor.dat[BU_ADD1];
    _PL[2] = debtor.dat[BU_ADD2];
    _PL[3] = debtor.dat[BU_ADD3];
    _PL[5] = debtor.dat[1];
    tdays = atoi(itran.dat[PIQ_TERMS]);
    if(tdays>0) {
        sprintf(TmpWbrep,"%3i Days",tdays);
    }
    else {
        sprintf(TmpWbrep,"%-8.8s","Cash");
    }
    _PL[6] = TmpWbrep;
    strcpy(itran.bak[IQ_DUE],mdate(itran.dat[IQ_DUE]));
    _PL[7] = itran.bak[IQ_DUE];
    _PL[8] = itran.dat[IQ_STAFF];
    _PL[11] = itran.dat[IQ_REM1];
    _PL[12] = itran.dat[IQ_REM2];
    _PL[21] = profile.dat[PF_ADD1];
    _PL[22] = profile.dat[PF_ADD2];
    _PL[23] = profile.dat[PF_ADD3];
}

void AdjustForm1MacroFeed(FILE *ptr)
{
    int feed;
/*
    feed = atoi(margin.dat[1]);
    fprintf(ptr,"2CBP\n");
    if(feed<=0 || feed>180) return;
    fprintf(ptr,"%c3%c\n2",27,feed);  */
}

void Pitran(long irec,FILE *ptr,DB *df)
{
    long Srec,i;

    Srec = interval(&irec,df->dat[1],&tritem,1);
    if(!Srec) return;
    if(CompanyNo==0x8a) {                //ks integrated waste management s/b
        dopen(&report,"ksi.rep");
    }
    else {
        dopen(&report,"pitran.rep");
    }
    memstart(&report);
    for (i=0; i<Srec; i++) {
        ridx(irec+i,&tritem,1);
		fetch(&report,&tritem);
        memwrt(&report);
    }
    memend(&report);
    qd(df->dat[PIQ_DEBTOR],&debtor);
    set_debtor_variable();
    _PL[9] = df->dat[1];
    lset(one.dat[1],df->dat[PIQ_DATE]);
    rset(one.dat[2],"34");

    ptr = fopen("lpt1","w");
    AdjustForm1MacroFeed(ptr);
    stdlayout(ptr,&one,&report);
    fprintf(ptr,"");
    fclose(ptr); 

//    stdplan(&one,&report);
    dclose(&report);
}
