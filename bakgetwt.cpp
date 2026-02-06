// GetWt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "GetWt.h"
#include "GwEdit.h"
#include "Indicator.h"
#include "AskPassword.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetWt dialog


CGetWt::CGetWt(CWnd* pParent /*=NULL*/)
	: CDialog(CGetWt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetWt)
	m_vehicle = _T("");
	//}}AFX_DATA_INIT
    weight = 0;
}


void CGetWt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetWt)
	DDX_Control(pDX, IDC_MSCOMM1, m_comm);
	DDX_Control(pDX, IDC_TEXTBOX1, m_wt);
	DDX_Text(pDX, IDC_EDIT1, m_vehicle);
	//}}AFX_DATA_MAP
}

int CGetWt::GetIndicatorType(int k)
{
    int   handle,no;
    char  tmp[20];

    handle = bopen("menu.ini");
    qmove(handle, 0x10);
    if(k==0) {
       qread(handle, tmp, 1);
    }
    else {
       tmp[0] = k;
       write(handle, tmp, 1);
    }
    close(handle);
    no = int(tmp[0]);
    if(no<1 || no>3) no = 1;
    return no;
}

BEGIN_MESSAGE_MAP(CGetWt, CDialog)
	//{{AFX_MSG_MAP(CGetWt)
	ON_BN_CLICKED(IDC_GW, OnGw)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetWt message handlers

BOOL CGetWt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_gwedit.SubclassDlgItem(IDC_EDIT1,this);
    m_comm.SetPortOpen(TRUE);
    SetTimer(1,500,NULL);// set 500 second to capture the weight

    IndicatorType = GetIndicatorType();
    OnGw();
    if(atoi(user.dat[UR_LEVEL])<999) {
        GetDlgItem(IDC_COMMANDBUTTON1)->ShowWindow(SW_HIDE);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGetWt::GetIndicatorWeight(CString str,char *tmp,int i)
{
    for(int k=1; k<=7; k++) {
        tmp[k-1] = str[i+k];
    }
    tmp[k-1] = 0;
}

void CGetWt::OnGw() 
{
	char tmp[80];
    CString Dtext;
	
/*	ActualWeight = 8000;
    weight = 8000 - GwstWeight;
    ltot(weight,6,tmp);
    Dtext.Format("%s kg",putnum(tmp));
    m_wt.SetText(Dtext);
	return;	
*/
	COleVariant propVal;
	
    propVal     = m_comm.GetInput() ;
	CString str = propVal.bstrVal;
    int flag,i,len   = str.GetLength();


    if(len<=5) return;

	int  k;
    for(flag=0,i=len-1; i>=0; i--) {
		k = 0;
        if(IndicatorType==1 && str[i]==char(2) && i+9<len) k = 1;
		if(IndicatorType==2 && str[i]==char(32) && i+9<len) k = 2;
		if(IndicatorType==3 && str[i]==char(2) && i+9<len) k = 3;
		if(k) {
            GetIndicatorWeight(str,tmp,i);
            flag = 1;
			break;
		}
	}
    if(flag==0) return;
	
    weight = ActualWeight = atol(tmp)/100;
    if((weight>1000 && GwstWeight>0) || GwstWeight<0) {
        weight -= GwstWeight;
    }
	ltot(weight,6,tmp);
    Dtext.Format("%s kg",putnum(tmp));
    m_wt.SetText(Dtext);
	return;	
}

void CGetWt::OnOK() 
{
    GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
    GetDlgItemText(IDC_EDIT1,m_vehicle);
    if(weight==0) {
        OnGw();
    }
    KillTimer(1);
    m_comm.SetPortOpen(FALSE);
    CDialog::OnOK();
}

void CGetWt::OnCancel() 
{
	KillTimer(1);
    m_comm.SetPortOpen(FALSE);
	CDialog::OnCancel();
}

void CGetWt::OnTimer(UINT nIDEvent) 
{
	OnGw();
	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CGetWt, CDialog)
    //{{AFX_EVENTSINK_MAP(CGetWt)
	ON_EVENT(CGetWt, IDC_COMMANDBUTTON1, -600 /* Click */, OnClickCommandbutton1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CGetWt::OnClickCommandbutton1() 
{
    CAskPassword p1;

    if(p1.DoModal()==IDOK) {

        CIndicator p2;
        CString str = m_comm.GetSettings();
        p2.m_setup  = str;
        p2.no       = GetIndicatorType();
        int flag = p2.DoModal();
        if(flag==IDOK) {
            m_comm.SetSettings(p2.m_setup);
			str = m_comm.GetSettings();
			MessageBox(str);
            GetIndicatorType(p2.no);
			MessageBox("Updated");
            OnCancel();
        }
    }
}
