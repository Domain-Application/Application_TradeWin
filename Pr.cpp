// Pr.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Pr.h"
#include "comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPr dialog


CPr::CPr(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
: CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CPr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPr::DoDataExchange(CDataExchange* pDX)
{
	CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPr, CStaff)
	//{{AFX_MSG_MAP(CPr)
	ON_BN_CLICKED(IDC_FIRST, OnFirst)
	ON_BN_CLICKED(IDC_SECOND, OnSecond)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPr message handlers



BOOL CPr::OnInitDialog() 
{
	CStaff::OnInitDialog();

	return TRUE;	// Due to user not allow to enter weighing entry - 20211004 JF
	
    HWND hWnd = GetSafeHwnd();
    BOOL bOpen = SetCommThread(hWnd);
//    if(bOpen==FALSE) {
//        OnCancel();
//    }
//  Allow access entry althought no indicator signal -- ccwong 091222
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPr::SetAttribute(int flag,int fld)
{
	CStaff::SetAttribute(flag,fld);
	
	if(flag==FALSE) {
		GetDlgItem(IDC_FIRST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SECOND)->ShowWindow(SW_SHOW);
	}
	else {
		GetDlgItem(IDC_FIRST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SECOND)->ShowWindow(SW_HIDE);
	}
	
}
void CPr::ShowInformation()
{
    CStaff::ShowInformation();
    getprmet(m,df);
	
	char str[20];
    ltot(m[4],6,str);
    SetDlgItemText(IDC_NETT,putnum(str));	
}

void CPr::OnFirst() 
{
	long weight = GetCurrenctCommWeight(1);
	ltodb(weight, df, PR_SGROSS);
    _strtime(df->dat[PR_TIMEIN]);
	lset(df->dat[PR_USER1],user.dat[1]);
	ShowInformation();	
}

void CPr::OnSecond() 
{
	long weight = GetCurrenctCommWeight(1);
	ltodb(weight, df, PR_STARE);
	_strtime(df->dat[PR_TIMEOUT]);
	lset(df->dat[PR_USER2],user.dat[1]);
	ShowInformation();	
}

void CPr::OnDestroy() 
{
	CloseCommThread();	
	CStaff::OnDestroy();
	
}
