// Blist.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Blist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlist dialog


CBlist::CBlist(UINT IDD,CWnd* pParent /*=NULL*/)
    : CBrowse(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlist)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CBlist::SetBrowseText()
{
    SetDlgItemText(IDC_DATE,mdate(df->dat[Bl_DATE]));
    CBrowse::SetBrowseText();
}

void CBlist::DoDataExchange(CDataExchange* pDX)
{
    CBrowse::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlist)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlist, CBrowse)
	//{{AFX_MSG_MAP(CBlist)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlist message handlers
