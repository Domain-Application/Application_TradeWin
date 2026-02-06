// menu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CheckIssuedChequeNo(int fld,int flag,DB *df)
{
    flag = DO;
    if(isblank(df->dat[fld])) return 1;
    if(strcmpNI(df->dat[fld],"CASH",4)==0) return 1;

    long rec = df->Search(df->dat[fld],fld);
    if(rec==0) return 1;

    CString msg;
    msg.Format("select refno from [%s] where [cheque] = %s",
        df->m_strTable,DbText(df, fld));
    CString tmp = SqlText(msg);
	if(tmp.CompareNoCase(df->dat[1])==0) return 1;

    msg.Format("select [date] from [%s] where [cheque] = %s",
        df->m_strTable,DbText(df, fld));
    CString strDate = SqlText(msg);
	
    msg.Format("Cheque have been issued in voucher no:%s (%s)\n\nAccept it ?",
		tmp, mdate(strDate) );
    if ( AfxMessageBox(msg,MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES ) return 1;
    pDlg->PostMessage(WM_COMMAND,ID_GOTO_NEXT);
    return 1;
}
