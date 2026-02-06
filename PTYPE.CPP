// PTYPE.cpp : implementation file
//
#include "stdafx.h"
#include "menu.h"
#include "PTYPE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTYPE dialog
CPTYPE::CPTYPE(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CPTYPE)
	//}}AFX_DATA_INIT
}

void CPTYPE::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPTYPE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPTYPE, CParent)
    //{{AFX_MSG_MAP(CPTYPE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTYPE message handlers
void CPTYPE::ShowInformation()
{
    CParent::ShowInformation();
}

BOOL CPTYPE::OnInitDialog()
{
    CParent::OnInitDialog();

    // TODO: Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void AdminDbPTYPE(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
	
    CPTYPE p1(IDD_PTYPE,&ptype,1);
    //CPTYPE p1(df->ID,df,df->f[1].idx & 0x01);
    p1.DoModal();
}

void PrintPType(long irec,FILE *ptr,DB *df)
{
    dopen(&one,"one.scr");
    dopen(&report,"pptype.rep");
    StdPrintTableFormat(&one, df, &report, 0);
    dclose(&one);
    dclose(&report);
}

void init_ptype(DB *df)
{
//    df->e->prec		= PrintPType;
}
