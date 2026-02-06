// Ciq.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include <afxtempl.h>
#include "Ciq.h"
#include "One.h"
#include "Ipay.h"
#include "Cr.h"
#include "ViewCiq.h"
#include "mgrid.h"
#include "Tso.h"
#include "Debtor.h"
#include "openrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCiq dialog

CCiq::CCiq(UINT IDD,CWnd* pParent /*=NULL*/)
    : CBrowse(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCiq)
	m_date = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
    select_records = 0;
    bToolBarBottom = FALSE;
    m_SHOWMAXIMIZED = TRUE;
    SupportStoreProcedure = FALSE;
}

void CCiq::DoDataExchange(CDataExchange* pDX)
{
    CBrowse::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCiq)
	DDX_Control(pDX, IDC_LIST1, m_salesman);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress2);
	DDX_Control(pDX, IDC_PROGRESS3, m_progress3);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCiq, CBrowse)
	//{{AFX_MSG_MAP(CCiq)
    ON_BN_CLICKED(ID_PRINT, OnPrinter)
	ON_BN_CLICKED(IDC_BUTTON2, OnStatement)
    ON_BN_CLICKED(ID_VIEW_DETAILS, OnViewDetails)
	ON_BN_CLICKED(IDC_BUTTON1, OnGO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCiq message handlers
void CCiq::ReplaceGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);

    g.SetText(df,0,1,2,CU_TEL0,CU_CONTACT,-1);
}

void CCiq::CreateToolBarAndStatusBar()
{
	RECT rect;
    int  button_no = 0;

    if(bToolBarBottom==TRUE) {
        VERIFY(m_ToolBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM , rect, this, 0));
    }
    else {
        VERIFY(m_ToolBar.Create(WS_CHILD | WS_VISIBLE | CCS_TOP , rect, this, 0));
    }

    SetButton(button_no,1,IDCANCEL,"Esc");
    SetButton(button_no,9,ID_CHANGE,"Change");
    SetButton(button_no,10,ID_SEARCH,"Search");
    SetButton(button_no,11,ID_NEXT,"Next");
    SetButton(button_no,18,IDOK,"View on screen");
    SetButton(button_no,8,ID_PRINT,"Print");
    SetButton(button_no,13,IDC_BUTTON2,"Statement");
    SetButton(button_no,21,ID_VIEW_DETAILS,"Details");

    m_ToolBar.AddButtons(button_no,m_Buttons);
    m_ToolBar.AddBitmap(button_no,IDR_TOOLBAR_EDIT);
	CSize sizeButton(16,16);
	m_ToolBar.SetBitmapSize(sizeButton);
	CSize sizeBitmap(16,15);
	m_ToolBar.SetBitmapSize(sizeBitmap);
	m_ToolBar.AutoSize();
    m_ToolBar.HideButton(ID_NEXT,TRUE);
    if(EditFlag==1) {
        if(atoi(user.dat[UR_LEVEL])>5) {
            return;
        }
    }
    m_ToolBar.HideButton(ID_ADDEDIT,TRUE);
}	

void CCiq::ShowInitFrame()
{
    MGRID g(&m_grid,df,0,5);

    g.ratio = float(1.2);
    g.SetTitle("No",500);
    g.SetTitle("Code No",900);
    g.SetTitle("Name",4500);
    g.SetTitle("         Tel. No",1200);
    g.SetTitle("Contact Person",3200);
    m_grid.SetRows(15);
}

BOOL CCiq::OnInitDialog() 
{
    CBrowse::OnInitDialog();

    CheckRadioButton( IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
    dbasedate(one.dat[1]);
	struct tm pst;
    sever_date(&pst,one.dat[1]);
    m_date.SetDate(pst.tm_year,pst.tm_mon,pst.tm_mday);
    UpdateData(FALSE);

    long mrec = ridx(df->rec,df,IdxItem);
    select_records = mrec;
    if(profile.dat[PF_MULTI_CURR][0]=='F') {
         GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_CIQ_CURRENCY_FRAME)->ShowWindow(SW_HIDE);
    }
    m_salesman.AddString("**All**");
    for(long rec=1; rec<= staff.t->sumrecord; rec++) {
        readrec(rec,&staff);
/*        if( staff.dat[Stf_POSITION][0] != 'S' ) continue;*/
        m_salesman.AddString(staff.dat[1]);
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCiq::FindCustomerBalance()
{
    if(st1.t->sumrecord>0) {
        if(st1.rec<0) st1.rec = 1;
        for(;;) {
            st1.rec = viewidx_ciq(&st1,IDD_VIEWCIQ);
            if(st1.rec ==0) break;
            ViewCIQ();
        }
    }
}

BEGIN_EVENTSINK_MAP(CCiq, CBrowse)
    //{{AFX_EVENTSINK_MAP(CCiq)
	ON_EVENT(CCiq, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CiqstateTitle()
{
    lset(logo.dat[Logo_SELECT1],customer->dat[ENAME]);
    lset(logo.dat[Logo_SELECT2],customer->dat[ADD1]);
    lset(logo.dat[Logo_SELECT3],customer->dat[ADD2]);
    lset(logo.dat[Logo_RTITLE],customer->dat[ADD3]);
    lset(logo.dat[Logo_DATE1],tso.dat[1]);
    lset(logo.dat[Logo_DATE2],tso.dat[2]);
    lset(logo.dat[Logo_ACCODE],customer->dat[CU_CODE]);
    lset(logo.dat[Logo_USER],customer->dat[STAFF]);
    qd(customer->dat[STAFF],&staff);
    lset(logo.dat[Logo_USERNAME],staff.dat[2]);
    writerec(1,&logo);
}

void CCiq::ViewCsutomerDetail(long rec,int mode)
{
    if(rec==0 || rec>customer->t->sumrecord) return;

    ridx(BrowseRecords,customer,IdxItem);
	UpdateData();
    int key = GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2);
    setwild(&tso);
    lset(tso.dat[Ts_CUSTOMER1],customer->dat[1]);
    lset(tso.dat[Ts_CUSTOMER2],customer->dat[1]);
    lset(tso.dat[Ts_CONVERT],"N");
    lset(tso.dat[Ts_CONVERT],"N");
    if(key==IDC_RADIO1) {
        lset(tso.dat[Ts_CONVERT],"Y");
    }
    lset(one.dat[1],m_date);

    lset(tso.dat[2],one.dat[1]);
    rset(tso.dat[Ts_TYPE],"1");
    if(mode==2) {
        lset(tso.dat[1],one.dat[1]);
        firstday(tso.dat[1]);
    }
    else {
        tso.val(1,"");
    }
    st1.t->sumrecord = 0;
    pDlg = this;
    m_p1 = &m_progress2;
    m_p2 = &m_progress3;
    clock_t start = clock();
    GetAnalysisBalance(GetAuditorStateData);
    m_p1 = m_p2 = NULL;
    clock_t finish = clock();
    double duration = (double)(finish - start) / CLOCKS_PER_SEC;
    str.Format(" %f seconds.", duration );
    SetDlgItemText(IDC_TYPE,"Spend Time");
    SetDlgItemText(IDC_MSG,str);
    ComputeOverFourMonthAgeing();

    if(st1.t->sumrecord==0){
		str.Format("Customer:%s\n\nno items to show in this view",customer->dat[2]);
		MessageBox(str,"Data not found",MB_ICONSTOP);
		return;
	}
    if(mode==0) {
        st1.rec = st1.t->sumrecord - 15;
        st1.f[St_DATE].idx = 0;

        FindCustomerBalance();
        return;
    }
    if(mode==2) {
        CString msg;
        msg.Format("Customer ID:%s\nName:%s\n\nClick Yes --> Print this statement only\nClick No  --> Print All statement",
            customer->dat[1],customer->dat[2]);
        int flag = MessageBox(msg,"option",MB_ICONQUESTION | MB_YESNOCANCEL);
        if(flag==IDYES) {
            DB st2;
            flag = MessageBox("I print it with recycle paper ?","option",MB_ICONQUESTION | MB_YESNOCANCEL);
            if(flag==IDYES) {
                 ropen(&st2,"st4.rep");
                 rset(tso.dat[Ts_PAGE],"45");
            }
            else {
                 ropen(&st2,"st2.rep");
				 rset(tso.dat[Ts_PAGE],"29"); //OOI
            }
            dupcpy(&st2,&st1);
            stdplan(&tso,&st2);
            dclose(&st2);
        }
        else if(flag==IDNO) {
            CTso t1(IDD_TSO,&tso);
            t1.gst = GetAuditorStateData;
            t1.bSetWild = FALSE;
            t1.DoModal();
        }
    }
    else {
        RunSql("Truncate table [ciq]");
        fetch_set(&ciq,&st1);
        readrec(1,&logo);
        lset(two.dat[1],profile.dat[PF_DATE]);
        lset(two.dat[2],tso.dat[2]);
        long last = st1.t->sumrecord;
        for(long rec=1; rec<=last; rec++) {
            readrec(rec,&st1);
            fetch_rec(&ciq,&st1);
//            lset(ciq.dat[1],logo.dat[1]);
            lset(ciq.dat[CIQ_PERIOD1],tso.dat[1]);
            lset(ciq.dat[CIQ_PERIOD2],tso.dat[2]);
            lset(ciq.dat[CIQ_ACCODE],customer->dat[1]);
            ciq.SqlUpdate(TRUE);
        }
        CiqstateTitle();
        if((STCR == St_CR)) {
            PrintRPT_withScr(&two,"ciq_ar.rpt",TRUE);
        }
        else {
            PrintRPT_withScr(&two,"ciq_ap.rpt",TRUE);
        }
    }
}

void CCiq::OnOK() 
{
    select_records = df->mrec;
    ViewCsutomerDetail(select_records,0);
}

void CCiq::OnPrinter() 
{
    select_records = df->mrec;
    ViewCsutomerDetail(select_records,1);
}

void CCiq::OnDblClickMsflexgrid1() 
{
    TopRecord--;
    TotalRecords = read_sumindex(IdxItem,df);
	int  row = m_grid.GetRow();

	if(row==0) return;
    no = df->rec + row -1;
    if(no<=TotalRecords){
        long mrec = ridx(no,df,IdxItem);
        if((*df->e->IsRecordInValid)(no,df,IdxItem)==TRUE) {        //30-3-2000
            lsetblank(df);
            return;
        }
        if(IdxItem<=1) {
            BrowseRecords = select_records = mrec;
            ViewCsutomerDetail(select_records,0);
			SetBrowseText();
			GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
            return;
        }
        BrowseRecords = df->rec+row-1;
        no = ntx(1,TotalRecords,mrec,df);
        SetBrowseText();
        GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
	}
}

void CCiq::OnClickMsflexgrid1() 
{
    select_records = 0;
    TopRecord--;
    TotalRecords = read_sumindex(IdxItem,df);
	int  row = m_grid.GetRow();

	if(row==0) return;
    no = df->rec + row -1;
    if(no<=TotalRecords){
        long mrec = ridx(no,df,IdxItem);
        select_records = df->mrec;
        BrowseRecords = df->rec+row-1;
	}
    SetBrowseText();
    GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
}

void CCiq::OnStatement() 
{
    select_records = df->mrec;
    ViewCsutomerDetail(select_records,2);
}

void CCiq::OnViewDetails()
{
    long mrec = df->mrec;
    long irec = df->irec;
    long rec = df->rec;

    AdminAccodeDb(customer);

    df->mrec = mrec;
    df->irec = irec;
    df->rec  = rec;
}

void CCiq::OnGO() 
{
    int nCount = m_salesman.GetSelCount();
	CArray<int,int> myArray;

	myArray.SetSize(nCount);
    m_salesman.GetSelItems(nCount, myArray.GetData());
    CString rString,strWhere= " where ",strStaff,strSql;

    long last = myArray.GetSize();
    if(last<1) {
        return;
    }
    BOOL bSelectAll=FALSE;
    for (int i=0;i < last; i++){
		int nIndex = myArray.ElementAt(i);
        if(nIndex>0) {
            m_salesman.GetText( nIndex, rString );
            lset(staff.dat[1], rString);
            strStaff.Format(" [staff]='%s'",staff.dat[1]);
        }
        else {
            bSelectAll = TRUE;
            break;
        }
        if(i>0) strWhere += " or ";
        strWhere += strStaff;
	}	
    if(bSelectAll) {
        strSql.Format("select * from [%s] ", df->m_strRsName);
    }
    else {
        strSql.Format("select * from [%s] %s", df->m_strRsName,strWhere);
    }
    strSql += " order by [code] ";

    df->m_strCmdText = strSql;
    df->ReOpen();
    TotalRecords = read_sumindex(IdxItem,df);
    ShowInitFrame();
    //Refresh();
	Refresh(15,0,1);
    GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
}
