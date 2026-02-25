// tritem.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"
#include "openrs.h"

extern BOOL Purchase_Flag,Rounding_Flag,Sales_Flag;

int tell_tritem(UINT nChar,int &flag,int &fld,DB *df,int sq)
{

	DB *customer;
	
	customer= &debtor;

    switch(fld) {
    case Tr_PT:
        if(sq) {
            qd(df->dat[fld],&prodtype);
            lset(df->dat[Tr_NAME], prodtype.dat[2]);
            rset(df->dat[Tr_PRICE], prodtype.dat[PT_SALE]);

					
			int fld2 = GetDbFieldNo(&accpf,"GST_DATE");
			if(fld2>0) {
				if(strcmp(df->l->ParentFile->dat[IQ_DATE], accpf.dat[fld2])>0) {

                    if(STCR==1) {
                        qd(df->e->parent->dat[PIQ_DEBTOR],customer);
                        int fld3 = GetDbFieldNo(customer,"TAXCODE");
                        lset(df->dat[Tr_TAXCODE],customer->dat[fld3]);
                    }
                    else {
                        lset(df->dat[Tr_TAXCODE], prodtype.dat[PT_TAXCODE2]);
                    }
					
					qd(df->dat[Tr_TAXCODE], &taxcode);
					double percen = taxcode.val(3);
					dtodb(percen, df, Tr_PERCEN);

				}
			}
        }
        break; 
		
	case Tr_TAXCODE:
		if(sq){
			int fld2 = GetDbFieldNo(&accpf,"GST_DATE");
			if(fld2>0) {
				if(strcmp(df->l->ParentFile->dat[IQ_DATE], accpf.dat[fld2])>0) {
				
					qd(df->dat[Tr_TAXCODE], &taxcode);
					double percen = taxcode.val(3);
					dtodb(percen, df, Tr_PERCEN);
					
				}
			}	
		}
		break;
    }

    double qty = df->val(Tr_QTY);
    if(price_unit==1) {
        qty *= 1000;
    }
    double amt = qty * df->val(Tr_PRICE);

    long intamt1 = long(amt) * 100;

    double ame = amt * 100;
    long intamt2 = long(ame);
	long intamt = intamt2 - intamt1;
    double adjamt = 0;
/*    if(Rounding_Flag) {
        if(intamt >= profile.lval(Pro_FACTOR)) {
            adjamt = long(amt) + 1 - amt;
        }
        else {
            adjamt = long(amt) - amt;
        }
    }*/

    double total = amt + adjamt;
    dtodb(total,df,Tr_AMOUNT);

    double subtotal = total;
    double percen = df->val(Tr_PERCEN);
    double taxamt = total * percen / 100;
    if(df->Boolean(Tr_INCLUDE)) {
        taxamt = total * percen / (100 + percen);
    }
    dtodb(taxamt, df, Tr_TAXAMT);
    if(!df->Boolean(Tr_INCLUDE)) {
        subtotal = total + df->val(Tr_TAXAMT);
    }
    dtodb(subtotal,df,Tr_SUBTOTAL);
	

    return 2;
}

int DeleteTicket(long irec,DB *df,int IdxItem) //21062023 Brian Delete ticket alert
{
	df->ReOpen();
	CString strSql,Result,strSql2,strSql3,strSql4,strSql5,Purcheck,Salcheck,ref1,ref2;
	strSql.Format("SELECT REFNO from tritem where TICKET='%s'",df->dat[3]);
	Result=SqlText(strSql);
	strSql2.Format("SELECT PAYSTATUS from puritran where REFNO='%s'",Result);
	Purcheck=SqlText(strSql2);
	strSql3.Format("SELECT PAYSTATUS from itran where REFNO='%s'",Result);
	Salcheck=SqlText(strSql3);
	strSql4.Format("SELECT REFNO from puritran where REFNO='%s'",Result);
	ref1=SqlText(strSql4);
	strSql5.Format("SELECT REFNO from itran where REFNO='%s'",Result);
	ref2=SqlText(strSql5);

	if(strcmp(Result,ref1))
	{

		if(Salcheck=="Unpaid")
		{
			if(AfxMessageBox("Confirm Delete Ticket?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 )==IDYES)
			{
				return Delete(irec,df,IdxItem);
			}
		}
		if(Salcheck=="Paid")
		{
			AfxMessageBox("The invoice for this ticket has already paid. Ticket cannot be deleted.");
			return 0;
		}
	}
	if(strcmp(Result,ref2))
	{
		
		if(Purcheck=="Unpaid")
		{
			if(AfxMessageBox("Confirm Delete Ticket?",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 )==IDYES)
			{
				return Delete(irec,df,IdxItem);
			}
		}
		if(Purcheck=="Paid")
		{
			AfxMessageBox("The invoice for this ticket has already paid. Ticket cannot be deleted.");
			return 0;
		}
		
	}
}
int save_ticket(DB *df,int flag)
{

	if(isblank(df->dat[3]))
	{
		AfxMessageBox("Ticket without ticket no cannot be added");
		return 0;
	}
    return 1;
}
int tritem_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = Tr_SUBTOTAL;
    fld[1] = Tr_PERCEN;
    fld[2] = Tr_TAXAMT;
    return 1;
}

void init_tritem(DB *df)
{
    df->e->initial = tritem_initial;
    df->e->tell = tell_tritem;
	df->e->save = save_ticket;
	df->e->Delete  = DeleteTicket;
}
