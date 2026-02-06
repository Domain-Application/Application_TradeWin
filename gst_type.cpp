// bank.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

int save_bank(DB *df,int flag)
{
    CString str;
    long xrec = qd(df->dat[1],&glcode);
    if(xrec==0) {
        str.Format("Error message\nBank code must same as g/l code");
        AfxMessageBox(str);
        return 0;
    }
    return 1;
}

void init_bank(DB *df)
{
    df->e->save = save_bank;                     // part out
}
