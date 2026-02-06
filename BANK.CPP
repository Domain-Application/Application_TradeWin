// Gst_type.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

int save_Gst_type(DB *df,int flag)
{
    if(df->val(Vat_TAX)==0) return 1;

    CString str;
    long xrec = qd(df->dat[Vat_GLCODE1],&glcode);
    if(xrec==0) {
        str.Format("Warning message\nplease key in tax on sales g/l code");
        AfxMessageBox(str);
    }
    xrec = qd(df->dat[Vat_GLCODE2],&glcode);
    if(xrec==0) {
        str.Format("Warning message\nplease key in tax on purchase g/l code");
        AfxMessageBox(str);
    }
    xrec = qd(df->dat[Vat_GLCODE3],&glcode);
    if(xrec==0) {
        str.Format("Warning message\nplease key in tax on expense g/l code");
        AfxMessageBox(str);
    }
    return 1;
}

void init_Gst_type(DB *df)
{
    df->e->save = save_Gst_type;                     // part out
}
