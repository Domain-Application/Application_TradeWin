// GetWt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "GetWt.h"
#include "GwEdit.h"
#include "mgrid.h"
#include "AskPassword.h"
#include "comm.h"
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
    m_WtFlag= TRUE;
	//{{AFX_DATA_INIT(CGetWt)
	//}}AFX_DATA_INIT
    weight = 0;

    m_Status = m_zero = FALSE;
    hcount = 0;
    if (!m_ok.LoadBitmaps("OKU", "OKD", "OKF"))
	{
		TRACE("Failed to load bitmaps for buttons\n");
		MessageBox("Failed to load bitmaps for buttons\n");
		AfxThrowResourceException();
	}
    if (!m_cancel.LoadBitmaps("CANCELU", "CANCELD", "CANCELF"))
	{
		TRACE("Failed to load bitmaps for buttons\n");
		MessageBox("Failed to load bitmaps for buttons\n");
		AfxThrowResourceException();
	}
}

void CGetWt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetWt)
	DDX_Control(pDX, IDC_TRAFFIC, m_traffic);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_TEXTBOX1, m_wt);
	DDX_Text(pDX, IDC_EDIT1, m_vehicle);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_grid);
	//}}AFX_DATA_MAP
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
    SetTimer(1,100,NULL);

    HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_SCALE);
	this->SetIcon(m_hIcon, TRUE);			// Set big icon
    SetGridTitle();
    ShowGridContent();
    lsetblank(df);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGetWt::DisplayIndicatorType(char LeadChar,int i)
{
}

int CGetWt::OnGw()
{
    if( m_Status == FALSE) {
        SetDlgItemText(IDC_EDIT2,"Unstable");
    }
    else {
        SetDlgItemText(IDC_EDIT2,"stable");
    }
    if( m_zero == FALSE) {
        SetDlgItemText(IDC_EDIT3,"No");
    }
    else {
        SetDlgItemText(IDC_EDIT3,"Yes");
    }

    weight = GetCurrenctCommWeight(1);

#ifdef _DEBUG
    int ScrollKeyStatus = GetKeyState(VK_SCROLL);
    weight = ( ScrollKeyStatus) ? 17000: 12000;
	IsWBZeroWt = TRUE;
#endif

	ActualWeight = weight;
    if (strcmp(df->m_strTable,"wds")==0){   // ccwong 20100714 only receiving
		if((weight>1000 && GwstWeight>0) || GwstWeight<0) {
			weight -= GwstWeight;
		}
	}	
	char tmp[80];
    if(weight==0) {
        sprintf(tmp,"00,000",weight);
    }
    else {
        char szTmp[80];
        sprintf(szTmp,"%6ld",weight);
        strcpy(tmp,putnum(szTmp));
    }
	m_wt.SetText(tmp);

    return TRUE;
}

void CGetWt::OnOK() 
{
	CString str;
	/* 30/4/2015
	if(weight <= profile.val(PF_MINIMUM)) {
		str.Format("Current Weigh must > %s kg",profile.dat[PF_MINIMUM]);
		MessageBox(str);
        return;
	}
	*/
    if(m_CanConfirm==FALSE) {
        str = "Error Message\n\n";
        if(m_Status == FALSE) {
            str += "Indicator weight unstable\n";
        }
        if(m_zero == FALSE) {
            str += "Weight indicator must need start with zero during weighing process.";
        }
        MessageBox(str);
        return;
    }
    GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
    GetDlgItemText(IDC_EDIT1,m_vehicle);
    if(weight==0) {
        OnGw();
    }
    KillTimer(1);
    CDialog::OnOK();
}

void CGetWt::OnCancel() 
{
	KillTimer(1);
	CDialog::OnCancel();
}

void CGetWt::OnTimer(UINT nIDEvent) 
{
    int flag = TRUE;

    if(m_WtFlag == TRUE) {
        m_WtFlag = FALSE;
        flag = OnGw();
        m_WtFlag = TRUE;
    }
	CDialog::OnTimer(nIDEvent);
    if(flag==FALSE) {
        OnCancel();
    }
    if(hcount>11) hcount = 0;
    m_Status = FALSE;
    if(weight==hwt[0]) {
        m_Status = TRUE;
    }
  //  m_zero = ::ZeroWeight1;
	
	if (IsWBZeroWt){
		m_zero = TRUE;
	}
	else{
		m_zero = FALSE;
	}

    hwt[0] = weight;

    flag = FALSE;
//    if(weight > 500) {
        if(stkpf.dat[PRO_ZERO][0]=='N' && stkpf.dat[PRO_STABLE][0]=='N') {
            flag = TRUE;
        }
        else if(stkpf.dat[PRO_ZERO][0]=='Y' && stkpf.dat[PRO_STABLE][0]=='N') {
            flag = m_zero;
        }
        else if(stkpf.dat[PRO_ZERO][0]=='N' && stkpf.dat[PRO_STABLE][0]=='Y') {
            flag = m_Status;
        }
        else if(stkpf.dat[PRO_ZERO][0]=='Y' && stkpf.dat[PRO_STABLE][0]=='Y') {
            if(m_Status && m_zero) flag = TRUE;
        }
//    }
	
    m_CanConfirm = flag;
    if(flag == TRUE) {
        HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_TRAFFIC_GREEN);
        m_traffic.SetIcon(m_hIcon);
	}
    else {
        HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_TRAFFIC_RED);
        m_traffic.SetIcon(m_hIcon);
	}
}

BEGIN_EVENTSINK_MAP(CGetWt, CDialog)
    //{{AFX_EVENTSINK_MAP(CGetWt)
	ON_EVENT(CGetWt, IDC_MSFLEXGRID1, -600 /* Click */, OnClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CGetWt, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CGetWt::OnClickMsflexgrid1() 
{
    long mrec,last = read_sumindex(0,df);
	int  row = m_grid.GetRow();

	if(row==0) return;
    int no = row;
    if(no<=last){
        mrec = ridx(no,df,0);
        SetDlgItemText(IDC_EDIT1,df->dat[DP_LORRY]);
	}
	else {
		MessageBox("Out of range");
	}
    GotoDlgCtrl(GetDlgItem(CtrlId(1)));
}

void CGetWt::SetGridTitle()
{
    MGRID g(&m_grid,df,0,2);

    g.ratio = float(1.2);
    g.SetTitle("No",400,7);
    g.SetTitle("Vehicle No",1000);
    g.SetTitle("Date",900,4);
    g.SetTitle("Time In",900,4);
    g.SetTitle("Wt.In(Kg)",750,7);
}

void CGetWt::SetActiveStatus(long rec)
{
    MGRID g(&m_grid,df,rec);
	if(!stricmp(df->name,"wpr_new")){
		g.SetText(df,0,PR_LORRY,PR_DATE,PR_TIMEIN,PR_PGROSS,-1);
	}
	else{
		g.SetText(df,0,DP_LORRY,DP_DATE,DP_TIMEIN,DP_PGROSS,-1);
	}
    
}

void CGetWt::ShowGridContent()
{
    long srec = read_sumindex(0,df);

    m_grid.SetRows(srec+1);
    for(long rec=1; rec<=srec; rec++) {
        readrec(rec,df);
        m_grid.SetRow(rec);
        SetActiveStatus(rec);
    }
    CString str;
    str.Format("%ld",srec);
    SetDlgItemText(IDC_TOTAL_VEHICLE_IN_YARD,str);
}

void CGetWt::OnDblClickMsflexgrid1() 
{
    OnClickMsflexgrid1();
    OnOK();
}
