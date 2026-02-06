// TimeFrm.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "TimeFrm.h"
#include "db_ado.h"
#include "comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DB tmpFrm;
/////////////////////////////////////////////////////////////////////////////
// CTimeFrm dialog

CTimeFrm::CTimeFrm(CWnd* pParent /*=NULL*/)
: CDialog(CTimeFrm::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTimeFrm)
	m_wrange = _T("1000");
	//}}AFX_DATA_INIT
    type = 0;
	confirmWeight = 0;
    msg = "";
    strVehicle = "Unknown vehicle";
}

void CTimeFrm::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTimeFrm)
	DDX_Control(pDX, IDC_MSCHART1, m_chart);
	DDX_Text(pDX, IDC_EDIT1, m_wrange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeFrm, CDialog)
    //{{AFX_MSG_MAP(CTimeFrm)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTimeFrm message handlers
BOOL CTimeFrm::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString s;
    s.Format("select max(wt) from [logAlarmData] where id=%i", key);
    maxWt = SqlInt(s);

    ShowChart();

    char tmp[100];
   // SetDlgItemText(IDC_MaxTime, cMaxTime1);

    SetDlgItemText(IDC_LORRY, strVehicle);

    if(maxWt==0) {
        OnCancel();
        return FALSE;
    }
    sprintf(tmp,"%6ld", maxWt);
    SetDlgItemText(IDC_MaxWt, putnum0(tmp) );

    sprintf(tmp,"%6ld", confirmWeight);
    SetDlgItemText(IDC_CW, putnum0(tmp) );

    long var = maxWt - confirmWeight;
    sprintf(tmp,"%ld", var);
    SetDlgItemText(IDC_VAR, putnum0(tmp));

    s.Format("select top 1 [DATE] from [logAlarmData] where wt = %i and id=%i",
        maxWt,key);
    s = SqlText( s );
    SetDlgItemText(IDC_MaxDate, s);
    SetDlgItemText(IDC_MSG, msg);

    this->ShowWindow(SW_SHOWMAXIMIZED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeFrm::OnOK()
{

	CDialog::OnOK();
}

void CTimeFrm::OnCancel()
{
	CDialog::OnCancel();
}

void CTimeFrm::ShowChart()
{
	m_chart.SetStacking(TRUE);
    m_chart.SetRowCount(1);
    m_chart.SetChartType(3);
    m_chart.SetShowLegend(FALSE);
	m_chart.SetAutoIncrement(TRUE);

    CString str;
    int bw = maxWt - atoi(m_wrange);
    if(type == 0) {
		SetDlgItemText(IDC_ADD, "");
        str.Format("select [DATE],wt from [logAlarmData] where id=%i order by pkkey",key);
        m_chart.SetRowLabel("Kg");
        m_chart.SetTitleText("Weighing Chart");
    }
    else {
        str.Format("+ %iKg", bw);
		SetDlgItemText(IDC_ADD, str);
        m_chart.SetRowLabel(str);
		str.Format("Weighing Chart (+ %iKg)", bw);
        m_chart.SetTitleText( str );
        str.Format("select [DATE],wt from [logAlarmData] where id=%i and wt >= %i order by pkkey",
            key, bw);
 
    }
    ado(&tmpFrm,"",NULL,str);

    long last = tmpFrm.t->sumrecord;
	m_chart.SetRow(1);

    m_chart.SetColumnCount(last);
    for(long rec=1; rec<=last; rec++) {
        readrec(rec,&tmpFrm);
        m_chart.SetColumn(rec);
        m_chart.SetColumnLabel( &tmpFrm.dat[1][11] );
        m_chart.SetRow(1);
        m_chart.SetRowLabel("Kg");

        long w = tmpFrm.lval(2) ;
        char tmp[10];
        if(type == 1) {
            w -= bw;
        }
        ltot(w, 6, tmp);
        m_chart.SetData(tmp);
    }
	
    dclose(&tmpFrm);

	m_chart.Refresh();
	m_chart.RedrawWindow();
	m_chart.ShowWindow(SW_SHOW);
    short i1 = m_chart.GetColumnLabelIndex();
    CString s1 = m_chart.GetColumnLabel();

    int kk = 0;
}

void CTimeFrm::OnButton1() 
{
    type = 0;
    ShowChart();
	
}

void CTimeFrm::OnButton2() 
{
    UpdateData();
    type = 1;
    ShowChart();
}

DB violation;
void ViewViolation()
{
    violation.m_fldDateTime = 1;
    CString s;

    s.Format("select [date],vname1 as Violation_Description,vehicle,ticketWt,pkkey from logalarm where [date]>='%s' order by [date] desc",dbasedate(today));
    ado(&violation,"LogAlarm",NULL,s);

	for(;;) {
		long xrec = viewidx(&violation);
		if(xrec==0) break;
		int key = violation.IntKey();
		
		CTimeFrm h1;
		h1.key = key;
        h1.msg = violation.dat[2];
        h1.strVehicle = violation.dat[3];
		h1.confirmWeight = violation.lval(4);
		h1.DoModal();
	}

    dclose(&violation);
}
