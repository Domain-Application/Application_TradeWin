// menu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "wacitem.h"
#include "menu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWacitem::CWacitem(UINT IDD,DB *psub,CWnd* pParent /*=NULL*/)
    : CChild(IDD, psub, pParent)
{
	//{{AFX_DATA_INIT(CWacitem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CWacitem::DoDataExchange(CDataExchange* pDX)
{
    CChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWacitem)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWacitem, CChild)
	//{{AFX_MSG_MAP(CWacitem)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWacitem message handlers
void CWacitem::ShowInitFrame()
{
     CChild::ShowInitFrame();
}

void CWacitem::ReplaceChildGrid(long rec)
{
    CChild::ReplaceChildGrid(rec);
}

void CWacitem::Refresh()
{
    Srec = sub->t->sumrecord;
    m_grid.SetRedraw(FALSE);
    m_grid.SetRows(1);
    m_grid.SetRows(Srec+2);
	dr_amt = cr_amt = 0;
    for(rec=1; rec<=Srec; rec++) {
        readrec(rec, sub);
        m_grid.SetRow(rec);
        ReplaceChildGrid(rec);
    }
	m_grid.SetRedraw(TRUE);
    ShowChildTotal();
}

BOOL CWacitem::IsBatchSaveConditionOK()
{
    if(fabs(dr_amt-cr_amt)>0.001){
        waitmsg(not_bal_msg);
        return FALSE;
    }
    return TRUE;
}

void CWacitem::ShowChildTotal()
{
    char str[20];
    dtoa(dr_amt, str, 12, 2);
    SetDlgItemText(IDC_TOTAL3,putnum0(str));

    dtoa(cr_amt, str, 12, 2);;
    SetDlgItemText(IDC_TOTAL4,putnum0(str));
}
