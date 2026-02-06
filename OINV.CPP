// Oinv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

void compute_oinv_amt(DB *df)
{
    double total=0;

    long rec = glcode.Search(df->dat[Oiv_GLCODE],1);
    if(rec==0) {
        dtodb(0,df,Oiv_AMT);
        dtodb(0,df,Oiv_T);
        dtodb(0,df,Oiv_VAT);
    }
    return;
    qd(df->dat[Oiv_T],&Gst_type);
    double rate = Gst_type.val(3);
    double Tax  = df->val(Oiv_AMT) * rate / 100;
    dtodb(Tax,df,Oiv_VAT);
}

int tell_oinv(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
	double amt;

    switch(fld) {
    case Oiv_AMT:
        amt = df->val(Oiv_QTY) * df->val(Oiv_PRICE) / price_unit;
        if(amt) {
            dtodb(amt , df , fld);
        }
        break;
    case Oiv_GLCODE:
        if(sq) {
            lset(df->dat[Oiv_NAME],glcode.dat[2]);
        }
        break;
    case Oiv_T:
    case Oiv_VAT:
        if(bTaxInclude == FALSE) {
            flag = DO;
        }
        break;
    }
    compute_oinv_amt(df);
    return 2;
}

int save_oinv(DB *df,int flag)
{
    CString str;
    long xrec = qd(df->dat[Oiv_GLCODE],&glcode);
    if(xrec==0) {
        double price = df->val(Oiv_AMT);
        if(price) {
            str.Format("Error message\nPlease key in glcode");
            AfxMessageBox(str);
            return 0;
        }
    }
    else {
        double price = df->val(Oiv_AMT);
        if(price==0) {
            str.Format("Warning\n%s\nAmount is zero",glcode.dat[2]);
            AfxMessageBox(str);
            return 0;
        }
    }
    double vat = df->val(Oiv_VAT);
    if(vat>0) {
        xrec = qd(df->dat[Oiv_T],&Gst_type);
        if(xrec==0) {
            str.Format("Error Message\n%s\nVat Amount %s\nBut tax code not found",df->dat[Oiv_VAT]);
            AfxMessageBox(str);
            return 0;
        }
        xrec = qd(Gst_type.dat[Vat_GLCODE1],&glcode);
        if(xrec==0) {
            str.Format("Error Message\n\nVat Amount %s\nBut tax on sales g/l code not found",df->dat[Oiv_VAT]);
            AfxMessageBox(str);
            return 0;
        }
        xrec = qd(Gst_type.dat[Vat_GLCODE2],&glcode);
        if(xrec==0) {
            str.Format("Error Message\n\nVat Amount %s\nBut tax on purchase g/l code not found",df->dat[Oiv_VAT]);
            AfxMessageBox(str);
            return 0;
        }
    }
    return 1;
}

int start_oinv(DB *df)
{
    if(df->t->sumrecord>0) return 1;
    if(STCR == St_CR) {
        lset(df->dat[Oiv_GLCODE],profile.dat[PF_SALES]);
    }
    else {
        lset(df->dat[Oiv_GLCODE],profile.dat[PF_PURCHASE]);
    }
    long xrec = qd(df->dat[Oiv_GLCODE],&glcode);
    glcode.rec = xrec;
    lset(df->dat[Oiv_NAME],glcode.dat[2]);
    return 1;
}

void init_oinv(DB *df)
{
    df->e->tell  = tell_oinv;
    df->e->start = start_oinv;
    df->e->save  = save_oinv;                     // part out
}
