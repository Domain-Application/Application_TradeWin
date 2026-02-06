// Ptentry.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Ptentry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPtentry dialog


CPtentry::CPtentry(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CPtentry)
	m_cadj = 0.0;
	m_sadj = 0.0;
	//}}AFX_DATA_INIT
}


void CPtentry::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPtentry)
	DDX_Text(pDX, IDC_CADJ, m_cadj);
	DDX_Text(pDX, IDC_SADJ, m_sadj);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPtentry, CParent)
    //{{AFX_MSG_MAP(CPtentry)
    ON_WM_DESTROY()
    ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnViewStockFlow)
	ON_BN_CLICKED(IDC_PROCESS, OnProcess)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPtentry message handlers
void CPtentry::ShowInformation()
{
    CParent::ShowInformation();
}


BOOL CPtentry::OnInitDialog()
{
    CParent::OnInitDialog();

    // TODO: Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPtentry::SetAttribute(int flag,int fld)
{
	CParent::SetAttribute(flag,fld);
	
	if(flag==FALSE) {
		GetDlgItem(IDC_PROCESS)->ShowWindow(SW_SHOW);
	}
	else {
		GetDlgItem(IDC_PROCESS)->ShowWindow(SW_HIDE);
	}
}

void CPtentry::OnDestroy()
{
    CParent::OnDestroy();

    // TODO: Add your message handler code here

}

void CPtentry::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    CParent::OnTimer(nIDEvent);
}

void CPtentry::ShowDetail()
{
    CParent::ShowDetail();
}

void CPtentry::OnDetail()
{
    CParent::OnDetail();
}

void CPtentry::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    fg.Initial(grid,sub,2,grid_ratio);
    fg.SetTitle("No",400);
    fg.SetTitle("Code",1000);
	fg.SetTitle("Product Grade Description",4000);
	fg.SetTitle("Purchase Price",1500,7);
	fg.SetTitle("Sales Price",1500,7);
}

void CPtentry::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    fg.show(rec);
    fg.SetText(sub,NULL,3,3 | 0x100,4,5,-1);
}


void AdminDbPtentry(DB *df,DB *sub)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CPtentry p1(df->ID,df,df->f[1].idx & 0x01);
	p1.sub = sub;
	p1.NonDupArray[0] = PTSB_PRODGRADE;
    p1.DoModal();
}


void CPtentry::OnViewStockFlow() 
{
	ViewStockFlow(df);	
}

void CPtentry::OnProcess() 
{
	CString str;
	UpdateData();
	if (m_cadj!=0 || m_sadj!=0){
		str.Format("Do you want to adjust value %9.5f on Purchase Price\r\nand %9.5f on Sales Price",m_cadj,m_sadj);
		if(AfxMessageBox(str, MB_ICONQUESTION | MB_YESNO)==IDYES){
			str.Format("Update PTSUB set COST=COST+%9.5f,SALES=SALES+%9.5f where PART='%s'",m_cadj,m_sadj,df->dat[1]);
			RunSql(str);
			str.Format("Update PT set LASTUPDATE=getdate(),C_ADJ=%9.5f,S_ADJ=%9.5f where PART='%s'",m_cadj,m_sadj,df->dat[1]);
			RunSql(str);
			df->Search(df->dat[1],1,TRUE);
			ReFreshSubFile();
			ShowInformation();
			SetDlgItemText(IDC_CADJ,"0");
			SetDlgItemText(IDC_SADJ,"0");
		}
	}
	else{
		AfxMessageBox("Zero value on both Purchase and Sales adjustment column.\r\nNot allowed to proceed.");
		return;
	}


}

void init_ptentry(DB *df)
{

}