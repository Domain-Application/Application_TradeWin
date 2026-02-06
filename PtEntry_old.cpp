// PtEntry.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "PtEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPtEntry dialog


CPtEntry::CPtEntry(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
: CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CPtEntry)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPtEntry::DoDataExchange(CDataExchange* pDX)
{
	CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPtEntry)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPtEntry, CStaff)
	//{{AFX_MSG_MAP(CPtEntry)
	ON_BN_CLICKED(IDC_BUTTON1, OnViewStockFlow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPtEntry message handlers

void AdminDb_Product(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
	
    CPtEntry p1(df->ID,df,df->f[1].idx & 0x01);
	p1.DoModal();
	
}

void CPtEntry::OnViewStockFlow() 
{
	ViewStockFlow(df);	
}
