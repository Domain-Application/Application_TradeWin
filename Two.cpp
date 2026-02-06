// Two.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Two.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTwo dialog


CTwo::CTwo(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
    : CTio(IDD,pdf,Idx,p,pParent)
{
	//{{AFX_DATA_INIT(CTwo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTwo::DoDataExchange(CDataExchange* pDX)
{
    CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTwo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTwo, CTio)
	//{{AFX_MSG_MAP(CTwo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTwo message handlers

void CTwo::show_tio()
{
}

void CTwo::OnSetWild()
{
 /*   readrec(1,&period);
    lset(two.dat[1],period.dat[2]);
    readrec(12,&period);
    lset(two.dat[2],period.dat[3]);
    rset(two.dat[3],"45");*/
    setwild(df);
    dbasedate(df->dat[1]);
    firstday(df->dat[1]);
    rset(df->dat[3],"40");
}
