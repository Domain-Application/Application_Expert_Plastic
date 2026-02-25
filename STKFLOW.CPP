// stkflow.cpp : implementation file
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

DB stkflow(IDD_ITEM,9);
void ViewStockFlow(DB *df)
{
//    if(isblank(df->dat[1])) return;

    CString strSql;

    strSql.Format("select * from [stkflow] where [pt]=%s order by [date],[refno]",
		DbText(df,1));

    stkflow.DefaultOrder = FALSE;

    stkflow.rep_field = (UserLevel()<9)  ? 5 : 5;
    dopen(&stkflow,"stkflow.rep");

    OpenRs  p1(strSql);
    p1.ToDBF(&stkflow);
    AutoDebitCreditBalance(&stkflow,3);
    AutoDebitCreditBalance(&stkflow,7);
    viewidx(&stkflow);
    dclose(&stkflow);
}
