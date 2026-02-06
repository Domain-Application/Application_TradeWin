#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double INV_total,INV_pay,INV_cr,INV_rc,INV_bal;
double InvoiceRealBalance(DB *df,char *text,char *date)
{
    INV_total = df->val(IV_TOTAL);
/*
    INV_pay   = date_tally(text, paydat, PY_INVOICE, PY_TOTAL, "DATE", date);
    INV_cr    = date_tally(text, crdat, CN_INVOICE, CN_TOTAL, "DATE", date);
    INV_rc    = date_tally(text, rcdat, PY_INVOICE, PY_TOTAL, "DATE", date);
    double bal = INV_bal = INV_total - INV_pay - INV_cr + INV_rc;
*/
    int  p1 = (STCR == St_CR) ? 0 : 1;
    double total = RunStoreProcedure(TRUE,"dt_invbal",
        ADOCG::adVarChar,"invno",text,
        ADOCG::adDate,"date",date,
        ADOCG::adInteger,"type",p1,
        0);
	
    INV_bal = INV_total - total;
    char tmp[20];
    dtoa(INV_bal,tmp,17,2);
    return atof(tmp);
}

double InvoiceBalance(DB *df,char *text)
{
    return InvoiceRealBalance(df,text,today);
    INV_total = df->val(IV_TOTAL);
    INV_pay   = tally(text, paydat, PY_INVOICE, PY_TOTAL);
    INV_cr    = tally(text, crdat, CN_INVOICE, CN_TOTAL);
    INV_rc    = tally(text, rcdat, PY_INVOICE, PY_TOTAL);

    double bal = INV_bal = INV_total - INV_pay - INV_cr + INV_rc;
    char tmp[20];
    dtoa(bal,tmp,17,2);
    return atof(tmp);
}

double ContraInvoiceBalance(DB *df,char *text,BOOL bArFlag)
{
    return InvoiceBalance(df,text);
}
