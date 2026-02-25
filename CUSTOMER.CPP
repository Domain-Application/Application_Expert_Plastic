// Customer.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

int CheckGlcode(char *ChkType)
{
    if(strcmp(glcode.dat[C_TYPE],ChkType)) {
        waitmsg("May be 'GL Code' incorrect");
        return 0;
    }
    return 1;
}

int tell_customer(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case 3:
        GetShortName(df,fld);
        break;
    }
    return 2;
}

int start_customer(DB *df)
{
    int fld = GetDbFieldNo(df,"TAXCODE");
    if(fld) {
        lset(df->dat[fld], "SR");
    }
    return 1;
}

int start_creditor(DB *df)
{
    int fld = GetDbFieldNo(df,"TAXCODE");
    if(fld) {
        lset(df->dat[fld], "NR");
    }
    return 1;
}

void init_customer(DB *df)
{
    df->e->start   = start_customer;
    df->e->tell    = tell_customer;
}

void init_supplier(DB *df)
{
    df->e->start   = start_creditor;
    df->e->tell    = tell_customer;
}
