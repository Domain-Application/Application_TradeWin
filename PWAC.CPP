// pwac.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"
#include "Wac.h"

void SetPwacTitle(DB *df)
{
    for(uint i=1;i<=df->sumfield;i++) {
        _PL[i] = df->dat[i];
    }
}

void PrintVoucher(long irec,FILE *ptr,DB *df)
{
    long Srec = interval(&irec,df->dat[1],df->e->chain,1);
    ropen(&report,"pjv.rep");
    lset(one.dat[1],df->dat[I_DATE]);
    SetPwacTitle(df);
    memstart(&report);
    fetch_set(&report,df->e->chain);
    for(long i=0;i<Srec;i++) {
        ridx(irec+i,df->e->chain,1);
        if(df->e->chain->val(Wi_AMT)==0) continue;
        fetch_rec(&report,df->e->chain);
        cr_amt = dr_amt = 0;
        if(df->e->chain->dat[Wi_DC][0]=='D') {
            dr_amt = df->e->chain->val(Wi_AMT);
        }
        else {
            cr_amt = df->e->chain->val(Wi_AMT);
        }
        dtodb(dr_amt,&report,3);
        dtodb(cr_amt,&report,4);
        memwrt(&report);

        if(df->e->chain->val(Wi_GST)) {
            qd(df->e->chain->dat[Wi_T],&Gst_type);
            qd(Gst_type.dat[Vat_GLCODE3],&glcode);

            lset(report.dat[1],glcode.dat[1]);
            lset(report.dat[2],glcode.dat[2]);
            cr_amt = dr_amt = 0;
            if(df->e->chain->dat[Wi_DC][0]=='D') {
                dr_amt = df->e->chain->val(Wi_GST);
            }
            else {
                cr_amt = df->e->chain->val(Wi_GST);
            }
            dtodb(dr_amt,&report,3);
            dtodb(cr_amt,&report,4);
            memwrt(&report);
        }
    }
    memend(&report);

    rset(one.dat[2],"42");
    if(report.t->sumrecord<9) {
        rset(one.dat[2],"9");
    }
    _PL[20] = user.dat[2];
    SetPaperSize(ptr);
    stdlayout(ptr,&one,&report);
    fprintf(ptr,"");                   //Issued form feed
    dclose(&report);
}
