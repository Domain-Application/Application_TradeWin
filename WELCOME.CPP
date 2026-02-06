// Welcome.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Welcome.h"
#include "VehicleInYard.h"
#include "ShowBal.h"
#include "PopBal.h"
#include "Wcard.h"
#include "NoLorry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWelcome dialog


CWelcome::CWelcome(CWnd* pParent /*=NULL*/)
	: CDialog(CWelcome::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWelcome)
	m_pt = _T("");
	//}}AFX_DATA_INIT
    WaitingFire = busy = FALSE;
    iCounter = 0;
	ExistRecordNo = 0;
    m_tuid = "";
	lset(tng.dat[1],"");
}


void CWelcome::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcome)
	DDX_Control(pDX, IDC_CLOCKFRAME, m_Clock);
	DDX_Text(pDX, IDC_EDIT4, m_pt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWelcome, CDialog)
	//{{AFX_MSG_MAP(CWelcome)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcome message handlers

void CWelcome::OnDestroy() 
{
    KillTimer(1);
	CDialog::OnDestroy();
}

BOOL CWelcome::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Clock.Start(IDB_CLOCKST_PANE, IDB_CLOCKST_BIG, IDB_CLOCKST_SMALL);
    this->ShowWindow(SW_SHOWMAXIMIZED);

    CString t;
    t.Format("Welcome to %s - Please scan your smart card.", title);
    SetWindowText(t);

    SetTimer(1,500,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWelcome::OnTimer(UINT nIDEvent) 
{
    if(busy) return;
    busy = TRUE;
	CDialog::OnTimer(nIDEvent);

    int flag = ClientRequest(WeightServer,port1,errmsg);
    if(flag==-1) {
        tcpWeight = 0;
    }
    current_weight = tcpWeight;
    char tmp[20];
    sprintf(tmp,"%6ld",tcpWeight);
    SetDlgItemText(IDC_LABEL1, putnum0(tmp));
    if(ExistRecordNo) {
        ltodb(current_weight,df,SECOND);
    }
    else {
        ltodb(current_weight,df,FIRST);
        ltodb(0,df,SECOND);
    }
    SetDlgItemText(IDC_EDIT10, df->dat[FIRST]);
    SetDlgItemText(IDC_EDIT11, df->dat[SECOND]);

    if(tcpWeight<100){
        CDialog::OnOK();
    }
    else {
        OnWaitingFire();
    }
    busy = FALSE;
}

void CWelcome::OnWaitingFire()
{
    if(current_weight<1000) return;
    iCounter++;
    if(iCounter % 2) return;

    if( !OnScanCardID(m_tuid)){
        return;
    }
    if( strcmp(tng.dat[1], m_tuid)==0) return;

    lsetblank(df);
    lset(tng.dat[1], m_tuid);
    long xrec = tng.Search(tng.dat[1], 1, TRUE);
    if(xrec==0){
        int state = rf_M1_authentication2(0, 0x60,0, k[ 0 ]);
        if(state){
            lset(df->dat[TNG], m_tuid);

            CWcard h;
            h.showType = 1;
            h.df  = df;
            h.wb  = wb;
            h.msg = "This is unformatted card.\n\nPlease use other card.";
            h.DoModal();
            return;
        }
        if( !OnScanCardID(m_tuid)){
            return;
        }
        BOOL flag = ReadCardKey();
        if(!flag) return;
        if(strcmp(MCardKey,CardKey)) {
            CWcard h;
            h.showType = 1;
            h.df  = df;
            h.wb  = wb;
            h.msg = "\nPlease use other card or register first.";
            h.DoModal();
            return;
        }
        lsetblank(&tng);
        lset(tng.dat[1], m_tuid);
        ReadCardInfo(&tng,1);
        tng.AddNew();
    }
    xrec = qd(tng.dat[Tng_ACCODE],&accode);
    if(xrec==0) {
        UpdateACInfo(xrec);
        xrec = qd(tng.dat[Tng_ACCODE],&accode);
    }

    SetDlgItemText(IDC_EDIT6,tng.dat[1]);
    SetDlgItemText(IDC_EDIT1,tng.dat[Tng_VEHICLE]);
    SetDlgItemText(IDC_EDIT2, accode.dat[1]);
    SetDlgItemText(IDC_EDIT3, accode.dat[2]);
    keyMifare d1;

    d1.code = atol(tng.dat[1]);
    unsigned char Snr[20];
    Snr[0] = d1.ch[0];
    Snr[1] = d1.ch[1];
    Snr[2] = d1.ch[2];
    Snr[3] = d1.ch[3];
    CString s = ExchangeHexToString(Snr,4);
    SetDlgItemText(IDC_HEXDATA, s);

    double camt = profile.val(Pro_MINCARDAMT);

    if(!CardBalance(amt)) return;

    str.Format("%9.2f", amt);
    SetDlgItemText(IDC_EDIT7,str);
    rf_light(0,2);

    if( (amt < camt)  ) {
        ShowFundNoEnough();
        return;
    }

    lset(df->dat[TNG], tng.dat[1]);
    ExistRecordNo =  df->Search(df->dat[TNG],TNG, TRUE);
    if(ExistRecordNo==0) {
        lset(df->dat[TNG], tng.dat[1]);
        lset(df->dat[LORRY], tng.dat[Tng_VEHICLE]);
        lset(df->dat[ACCODE], tng.dat[Tng_ACCODE]);
        lset(df->dat[TRANSP], tng.dat[Tng_TRANSP]);
        lset(df->dat[PT], tng.dat[Tng_PT]);
        ltodb(current_weight,df,FIRST);
        dbasedate(df->dat[DATE_IN]);
        dbasedate(df->dat[T_DATE]);
        strtime(df->dat[TIMEIN]);
        dtodb(amt,df,"CARDBAL");
        long xrec = qd(tng.dat[Tng_PT],&prodtype);
        if(xrec) {
            SaveFirstWeight();
        }
    }
    else {
        dbasedate(df->dat[T_DATE]);
        strtime(df->dat[TIMEOUT]);
        ltodb(current_weight,df,SECOND);
        compute_weight(df);

        long pValue = df->val(NAMT) * 100;
        int state = rf_M1_decrement(0, 1, pValue);

        lset(df->dat[USER2],user.dat[1]);
        if(current_comport_no==1) {
            lset( df->dat[WB2], "1");
        }
        else {
            lset( df->dat[WB2], "2");
        }
        lset( df->dat[COMPUTER2], app->m_strComputerName);
        long mrec = appnet(wb);

        fetch(wb,df);
        wb->GetTicketNo(1);
		wb->AddNew();

        wb->m_PrintOneFormOnly = TRUE;
        wb->m_PreviewCrystalReport = TRUE;
//        wb->PrintOneFormOnly("wb.rpt");

        CString s;
        s.Format("delete from [first] where [%s]='%s'",
            df->fieldName[1],df->dat[1]);
        RunSql(s);

		this->ShowWindow(SW_HIDE);
        CPopBal h;
        h.df  = df;
        h.wb  = wb;
        h.msg = "SILA KELUAR";
        h.str = str;
        h.DoModal();

        CDialog::OnOK();
		this->ShowWindow(SW_SHOW);

    }
    WaitingFire = FALSE;
}

void CWelcome::OnOK()
{
	UpdateData();

    if( isblank(df->dat[LORRY]) ) {
        CNoLorry h;
        h.DoModal();
        m_pt = "";
        UpdateData(FALSE);
        return;
    }

	lset(tng.dat[1], m_pt);
    long xrec = tng.Search(tng.dat[1], 1, TRUE);
    if(xrec==0) return;
    xrec = qd(tng.dat[Tng_PT],&prodtype);
    if(xrec==0) return;
    SetDlgItemText(IDC_EDIT4, prodtype.dat[1]);
    SetDlgItemText(IDC_EDIT5, prodtype.dat[2]);

    lset(df->dat[PT], tng.dat[Tng_PT]);
    if(ExistRecordNo==0) {
        SaveFirstWeight();
    }
}

void CWelcome::SaveFirstWeight()
{
    df->GetTicketNo(1);
    double price = prodtype.val(PT_PRICE);
    dtodb(price, df, PRICE);
    lset(df->dat[UNIT],prodtype.dat[PT_UNIT]);

    lset(df->dat[USER1],user.dat[1]);
    if(current_comport_no==1) {
        lset( df->dat[WB1], "1");
    }
    else {
        lset( df->dat[WB1], "2");
    }
    lset( df->dat[COMPUTER1], app->m_strComputerName);

    long tw = tng.lval(Tng_TAREWT);
    ltodb(tw, df, SECOND);
    compute_weight(df);
    if( amt < m[6] ) {
        ShowFundNoEnough();
        return;
    }

    ltodb(0, df, SECOND);
    compute_weight(df);
    df->AddNew();

    CShowBal h;
    h.showType = 2;
    h.df  = df;
    h.wb  = wb;
    h.str = str;
    h.DoModal();

    CDialog::OnOK();
}

void WelcomeScreen()
{
    CVehicleInYard p;

    p.df  = &first;
    p.wb = &receive;
	p.DoModal();
}

void CWelcome::ShowFundNoEnough()
{
    lset(df->dat[TNG], tng.dat[1]);
    lset(df->dat[LORRY], tng.dat[Tng_VEHICLE]);
    lset(df->dat[ACCODE], tng.dat[Tng_ACCODE]);
    lset(df->dat[TRANSP], tng.dat[Tng_TRANSP]);

    CShowBal h;
    h.showType = 1;
    h.df  = df;
    h.wb  = wb;
    h.str = str;
    h.DoModal();

    CDialog::OnOK();
}
