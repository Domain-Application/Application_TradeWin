// One.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "One.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COne dialog


COne::COne(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
    : CTio(IDD,pdf,Idx,p,pParent)
{
	//{{AFX_DATA_INIT(COne)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COne::DoDataExchange(CDataExchange* pDX)
{
    CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COne)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COne, CTio)
	//{{AFX_MSG_MAP(COne)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COne message handlers

void COne::show_tio()
{
}

void COne::OnSetWild()
{
    setwild(df);
    dbasedate(df->dat[1]);
    lastday(df->dat[1]);
    rset(df->dat[2],"45");
}
