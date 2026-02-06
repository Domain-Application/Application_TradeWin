// Month.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Month.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonth dialog


CMonth::CMonth(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
    : CTio(IDD,pdf,Idx,p,pParent)
{
	//{{AFX_DATA_INIT(CMonth)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMonth::DoDataExchange(CDataExchange* pDX)
{
    CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonth)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonth, CTio)
	//{{AFX_MSG_MAP(CMonth)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonth message handlers
