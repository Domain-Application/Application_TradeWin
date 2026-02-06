// EnqBank.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "EnqBank.h"
#include "mgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnqBank dialog


CEnqBank::CEnqBank(UINT IDD,CWnd* pParent /*=NULL*/)
    : CBrowse(IDD, pParent)
{
	//{{AFX_DATA_INIT(CEnqBank)
	m_d2 = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
    bToolBarBottom = FALSE;
}

void CEnqBank::ReplaceGrid(long rec,CMSFlexGrid *m_grid,DB *df,int IdxItem)
{
    MGRID g(m_grid,df,rec);

    g.SetText(df,0,1,2,3,-1);
}

void CEnqBank::SetBrowseText()
{
    UpdateData();
    CString str;
    lset(two.dat[1],"");
    str.Format("%4.4i%2.2i%2.2i",m_d2.GetYear(),m_d2.GetMonth(),m_d2.GetDay());
    lset(two.dat[2],str);
    lset(glcode.dat[1],df->dat[1]);
    CBrowse::SetBrowseText();
}

void CEnqBank::ShowInitFrame()
{
    MGRID g(&m_grid,df,0,4);
    g.SetTitle(0);
    g.SetTitle("Bank Code",1700);
    g.SetTitle("Bank Name",5600);
    g.SetTitle("Account No",3000);
    m_grid.SetRows(8);
}

void CEnqBank::DoDataExchange(CDataExchange* pDX)
{
    CBrowse::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnqBank)
	DDX_Control(pDX, IDC_PROGRESS2, m_progress);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_d2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnqBank, CBrowse)
	//{{AFX_MSG_MAP(CEnqBank)
	ON_BN_CLICKED(IDC_BUTTON1, OnUpdate)
	ON_BN_CLICKED(IDC_BUTTON2, OnPrint)
    ON_BN_CLICKED(IDC_BUTTON3, OnEditDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnqBank message handlers

BOOL CEnqBank::OnInitDialog() 
{
    CBrowse::OnInitDialog();
	
    lset(two.dat[2],today);
    firstday(two.dat[2]);
    plusday(two.dat[2],-1);

    struct tm pst;
    sever_date(&pst,two.dat[2]);
    m_d2.SetDate(pst.tm_year,pst.tm_mon,pst.tm_mday);
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CEnqBank, CBrowse)
    //{{AFX_EVENTSINK_MAP(CEnqBank)
	ON_EVENT(CEnqBank, IDC_MSFLEXGRID1, -600 /* Click */, OnClickMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CEnqBank::OnClickMsflexgrid1() 
{
    TopRecord--;
    TotalRecords = read_sumindex(IdxItem,df);
    int  row = m_grid.GetRow();

    if(row==0) return;
    no = df->rec + row -1;
    if(no<=TotalRecords){
        long mrec = ridx(no,df,IdxItem);
        SetBrowseText();
        GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
    }
}


void BankReconciliation()
{
    if(bank.t->sumrecord==0) {
		bank.Run();
    }

    dbopen(&blist,"blist.rep",init_blist);
    dopen(&report,"bankp.rep");
    tado(&bcwac,"bcwac",init_WacBank);
    tado(&bcitem,"bcitem",init_wacitem);

    CEnqBank t1(IDD_ENQBANK);

    t1.df = &bank;
    t1.IdxItem  = 0;
    t1.EditFlag = 0;

    t1.DoModal();
    dclose(&blist);
    dclose(&report);
    dclose(&bcwac);
    dclose(&bcitem);
}
