// adjprodtype.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"

int tell_adjprodtype(UINT nChar,int &flag,int &fld,DB *df,int sq)
{

    switch(fld) {
    case ADJ_AMOUNT:
         flag = DO;
         break;
    }
    double bal = df->val(ADJ_QTY) * df->val(ADJ_PRICE) ;
    dtodb(bal,df,ADJ_AMOUNT);
    return 2;
}

void init_adjitem(DB *df)
{
    df->e->tell = tell_adjprodtype;
}
