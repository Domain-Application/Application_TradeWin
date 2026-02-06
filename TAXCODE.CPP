// taxcode.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWb dialog

void SetSubTotal(DB *df)
{
    double total = df->val(IVWB_AMOUNT);
	
    double percen = df->val(IVWB_PERCEN);
    double taxamt = 0;
	
    double subtotal = total;
    if(df->Boolean(IVWB_INCLUDE)) {
        taxamt = total * percen / (100 + percen);
    }
    else {
        taxamt = total * percen / 100;
    }
    dtodb(taxamt, df, IVWB_TAXAMT);
    if(!df->Boolean(IVWB_INCLUDE)) {
        subtotal = total + df->val(IVWB_TAXAMT);
    }
    dtodb(subtotal,df,IVWB_SUBTOTAL);
}

void FindPercen(DB *df)
{

    qd(df->dat[IVWB_TAXCODE], &taxcode);
    double percen = taxcode.val(3);

    int fldDate = GetDbFieldNo(df,"DATE");
    int fld = GetDbFieldNo(&stkpf,"GST_DATE");
    if(fld>0) {
        if(strcmp(df->dat[fldDate], stkpf.dat[fld])<0) {
            percen = 0;
        }
    }
    dtodb(percen, df, IVWB_PERCEN);
}

void FindTaxCode(DB *df, BOOL Purchase_Flag)
{
	if( strcmp(df->dat[IVWB_DATE],txtSST_StartDate)<0 ) { // ooi 21/08/2018 SST
		if(Purchase_Flag) {
			lset(df->dat[IVWB_TAXCODE], accode.dat[ACCODE_TAXCODE]);
			if (isblank(df->dat[IVWB_TAXCODE])){
				lset(df->dat[IVWB_TAXCODE], "NR");
			}
		}
		else {
			lset(df->dat[IVWB_TAXCODE], "SR");        //sales
		}
	}
	else{
		lset(df->dat[IVWB_TAXCODE], accode.dat[ACCODE_TAXCODE]);
	}
	FindPercen(df);
}
