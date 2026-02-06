// Emode.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Emode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmode dialog

CEmode::CEmode(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CEmode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CEmode::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmode)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CEmode::ShowInformation()
{
    CStaff::ShowInformation();
    int type = atoi(df->dat[Em_PERIOD]);
    if(type<1 || type >6) type = 6;
    CheckRadioButton( IDC_RADIO1, IDC_RADIO6, IDC_RADIO1+type-1);
}

void CEmode::ReplaceRecord()
{
    CStaff::ReplaceRecord();
    int key = GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO6) - IDC_RADIO1 + 1;
    ltodb(key,df,Em_PERIOD);
}

BEGIN_MESSAGE_MAP(CEmode, CStaff)
	//{{AFX_MSG_MAP(CEmode)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
    ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_STATIC10, OnGotoNextRunningNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmode message handlers

int tell_emode(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    return 2;
}

int start_emode(DB *df)
{
    lset(df->dat[Em_TYPE],"1");
    return 1;
}

void ShowReminderErrorMessage()
{
    CString msg;
    msg.Format("Error message\n\n\nAuto reminding schedule\n1) Debit/Credit Code must assign\n2) Amount can not be zero\n3) Next remind date mus key in\n4) Remind period");
    AfxMessageBox(msg);
}

int save_emode(DB *df,int flag)
{
    if(!(df->dat[Em_REMINDER][0]=='Y' || df->dat[Em_REMINDER][0]=='T')) return 1;
    long xrec1 = qd(df->dat[Em_GLCODE1],&glcode);
    long xrec2 = qd(df->dat[Em_GLCODE2],&glcode);
    if(xrec1==0 || xrec2==0 || df->val(Em_AMT)==0) {
        ShowReminderErrorMessage();
        return 0;
    }
    if(isblank(df->dat[Em_DATE])) {
        ShowReminderErrorMessage();
        return 0;
    }
    if(!realm("1",df->dat[Em_PERIOD],"4")){
        ShowReminderErrorMessage();
        return 0;
    }
    if(!(df->dat[Em_PREFIX][0]=='Y' || df->dat[Em_PREFIX][0]=='T')) return 1;
    if(isblank(df->dat[Em_NEXTNO])) {
        CString msg;
        msg.Format("Error Message\n\nPlease key in next running no");
        AfxMessageBox(msg,MB_ICONSTOP);
        pDlg->PostMessage(WM_COMMAND,IDC_STATIC10);
        return 0;
    }
    return 1;
}

void init_emode(DB *df)
{
    df->e->tell = tell_emode;
    df->e->start = start_emode;
    df->e->save = save_emode;                     // part out
}

void SetFastModeTask()
{
    CEmode p1(IDD_EMODE,&emode,0);
    p1.DoModal();
}

void CEmode::OnRadio1() 
{
    ltodb(1,df,Em_PERIOD);
    CStaff::ShowInformation();
}

void CEmode::OnRadio2() 
{
    ltodb(2,df,Em_PERIOD);
    CStaff::ShowInformation();
}

void CEmode::OnRadio3() 
{
    ltodb(3,df,Em_PERIOD);
    CStaff::ShowInformation();
}

void CEmode::OnRadio4() 
{
    ltodb(4,df,Em_PERIOD);
    CStaff::ShowInformation();
}

void CEmode::OnRadio5() 
{
    ltodb(5,df,Em_PERIOD);
    CStaff::ShowInformation();
}

void CEmode::OnRadio6()
{
    ltodb(6,df,Em_PERIOD);
    CStaff::ShowInformation();
}

void CEmode::OnGotoNextRunningNo() 
{
     GotoDlgCtrl(GetDlgItem(IDC_EDIT10));
}
