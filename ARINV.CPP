// Arinv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Arinv.h"
#include "mgrid.h"
#include "Tap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArinv dialog



CArinv::CArinv(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CArinv)
	m_customer = _T("");
	m_salesman = _T("");
	m_invno = _T("");
	//}}AFX_DATA_INIT
    b_AskContinue = TRUE;
    if(STCR == St_CR) {
        ProduresID = 1;
    }
    else {
        ProduresID = 2;
    }
}

void CArinv::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CArinv)
    DDX_Text(pDX, IDC_ACCODE, m_customer);
    DDX_Text(pDX, IDC_STAFF, m_salesman);
	DDX_Control(pDX, IDC_ADJGRID, m_agrid);
    DDX_Control(pDX, IDC_PAYGRID, m_pgrid);
    DDX_Text(pDX, IDC_INVNO, m_invno);
	DDX_Control(pDX, IDC_BAL1, m_balance);
	DDX_Control(pDX, IDC_CGRID, m_cgrid);
	DDX_Control(pDX, IDC_RCGRID, m_rgrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CArinv, CParent)
    //{{AFX_MSG_MAP(CArinv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArinv message handlers

void CArinv::ShowAdjGrid()
{
    MGRID g(&m_agrid,sub,0,2);
    g.SetTitle("Adj No",1300);
    g.SetTitle("Date",1200,4);

    CString tmp;
    tmp.Format("Adj. Amt(%s)",inv->dat[IV_CURRENCY]);
    g.SetTitle(tmp,1500,7);
}

void CArinv::ShowPayGrid()
{
    MGRID g(&m_pgrid,sub,0,3);
    g.SetTitle("Voucher #",1300);
    g.SetTitle("Date",1200,4);
    CString tmp;
    tmp.Format("Pay. Amt(%s)",inv->dat[IV_CURRENCY]);
    g.SetTitle(tmp,1500,7);
}

void CArinv::ShowRCGrid()
{
    MGRID g(&m_rgrid,sub,0,3);
    g.SetTitle("Voucher #",1300);
    g.SetTitle("Date",1200,4);
    CString tmp;
    tmp.Format("Ret. Amt(%s)",inv->dat[IV_CURRENCY]);
    g.SetTitle(tmp,1500,7);
}

void CArinv::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    ShowAdjGrid();
    ShowPayGrid();
    ShowRCGrid();

    MGRID g(grid,sub,0,4);
    if(bTaxInclude == TRUE) {
        g.SetCols(6);
    }

    g.SetTitle("No",400);
    g.SetTitle("G/L Code",1000);

    CString text;    
    if(bTaxInclude == TRUE) {
        g.SetTitle("Description",3500);
        g.SetTitle("T",200,7);
        g.SetTitle("Vat",900,7);
        if(STCR == St_CR) {
            text.Format("Sales Amt.");
        }
        else {
            text.Format("Purchase Amt");
        }
        g.SetTitle(text,1200,7);
    }
    else {
        g.SetTitle("Description",3200);
        g.SetTitle("Qty",1100,7);
        if(STCR == St_CR) {
            text.Format("Sales Amt.(%s)",df->dat[IV_CURRENCY]);
        }
        else {
            text.Format("Purchase Amt.");
        }
        g.SetTitle(text,1600,7);
    }
}

void CArinv::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    MGRID g(m_grid,sub,rec);
    if(bTaxInclude == TRUE) {
        g.SetText(sub,0,Oiv_GLCODE,Oiv_NAME,Oiv_T,Oiv_VAT,-1);
        double total = sub->val(Oiv_AMT) + sub->val(Oiv_VAT);
        g.SetText(total,12,2);
    }
    else {
        g.SetText(sub,0,Oiv_GLCODE,Oiv_NAME,Oiv_QTY,Oiv_AMT,-1);
    }
}

void CArinv::ReplacePayGrid(long rec,long irec,CMSFlexGrid *m_pgrid,DB *data,int IdxItem)
{
    MGRID g(m_pgrid,data,rec+1);
    m_pgrid->SetRow(rec+1);
    g.SetText(PY_BILL);
    qd(data->dat[PY_BILL],ipay);
    g.SetText(ipay,IP_DATE);
    g.SetText(PY_TOTAL);
}

void CArinv::ReplaceAdjGrid(long rec,long irec,CMSFlexGrid *m_agrid,DB *data,int IdxItem)
{
    MGRID g(m_agrid,data,rec+1);
    m_agrid->SetRow(rec+1);
    g.SetText(CN_BILL);
    qd(data->dat[CN_BILL],cr);
    g.SetText(cr,Cr_DATE);
    g.SetText(CN_TOTAL);
}

void CArinv::ReplaceRCGrid(long rec,long irec,CMSFlexGrid *grid,DB *data,int IdxItem)
{
    MGRID g(grid,data,rec+1);
    grid->SetRow(rec+1);
    g.SetText(PY_BILL);
    qd(data->dat[PY_BILL],rc);
    g.SetText(rc,IP_DATE);
    g.SetText(PY_TOTAL);
}

void CArinv::ShowPaymentDetail()
{
    pay_total = 0;
    Srec = interval(&irec,df->dat[1],paydat,PY_INVOICE);
    m_pgrid.SetRows(Srec+1);
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec,paydat,PY_INVOICE);
        ReplacePayGrid(rec,irec,&m_pgrid,paydat,IdxItem);
        pay_total += paydat->val(PY_TOTAL);
    }
    dtoa(pay_total,str,12,2);
    SetDlgItemText(IDC_PAY,putnum0(str));
}

void CArinv::ShowAdjustmentDetail()
{
    adj_total = 0;
    Srec = interval(&irec,df->dat[1],crdat,CN_INVOICE);
    m_agrid.SetRows(Srec+1);
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec,crdat,CN_INVOICE);
        ReplaceAdjGrid(rec,irec,&m_agrid,crdat,IdxItem);
        adj_total += crdat->val(CN_TOTAL);
    }
    dtoa(adj_total,str,12,2);
    SetDlgItemText(IDC_ADJ,putnum0(str));
}

void CArinv::ShowRCDetail()
{
    RC_total = 0;
    Srec = interval(&irec,df->dat[1],rcdat,PY_INVOICE);
    m_rgrid.SetRows(Srec+1);
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec,rcdat,PY_INVOICE);
        ReplacePayGrid(rec,irec,&m_rgrid,rcdat,IdxItem);
        RC_total += rcdat->val(PY_TOTAL);
    }
    dtoa(RC_total,str,12,2);
    SetDlgItemText(IDC_PAY2,putnum0(str));
}

void CArinv::ShowPaymentGrids(int nCmdShow )
{
    m_pgrid.ShowWindow(nCmdShow);
    m_agrid.ShowWindow(nCmdShow);
    GetDlgItem(IDC_PAY)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_ADJ)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_LPAY)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_LADJ)->ShowWindow(nCmdShow);
}

void CArinv::ShowCashbillBox(int nCmdShow )
{
    GetDlgItem(IDC_BANK)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT17)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_LABEL17)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_CHEQUE)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT18)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_CQ_DATE)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT19)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_BANKIN)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT20)->ShowWindow(nCmdShow);
}

void CArinv::ShowDetail()
{
    memset(fm.acc,0,sizeof(fm.acc));
    CParent::ShowDetail();

    if(!strcmp(df->dat[IV_TYPE],"RF")) {
        ShowPaymentGrids(SW_HIDE);
        ShowCashbillBox(SW_HIDE);  //HIDE
        bal_total = 0;
        if(!isblank(df->dat[IV_CHEQUE])) {
            if(isblank(df->dat[IV_BANKIN])) {
                bal_total = df->val(IV_TOTAL);
            }
        }
        dtoa(bal_total,str,12,2);
        SetDlgItemText(IDC_BAL1,putnum0(str));
    }
    else {
        ShowPaymentGrids(SW_SHOW);
        ShowCashbillBox(SW_HIDE);
        ShowPaymentDetail();
        ShowAdjustmentDetail();
        ShowRCDetail();

        bal_total = df->val(IV_TOTAL) - pay_total - adj_total + RC_total;
        dtoa(bal_total,str,15,2);
        SetDlgItemText(IDC_BAL1,putnum0(str));
        if(bal_total>0) {
            m_balance.SetBackColor(RGB(255,0,255));
        }
        else {
            m_balance.SetBackColor(RGB(190,255,255));
        }
    }
}

void CArinv::OnDetail()
{
	CParent::OnDetail();
    if(bTaxInclude == FALSE) {
//         ChildControl(IDD_OINV,df->dat[1],sub,IdxItem);
    }
    else {
//         ChildControl(IDD_TAXOINV,df->dat[1],sub,IdxItem);
    }
    ShowInformation();
}

void CArinv::ShowInformation()
{
    char str[80];

    CStaff::ShowInformation();
    ShowInitFrame(&m_grid,sub);
    m_grid.SetRedraw(FALSE);
    ShowDetail();
    m_grid.SetRedraw(TRUE);

    dtoa(fm.acc[Oiv_AMT] + fm.acc[Oiv_VAT],str,12,2);
    SetDlgItemText(IDC_SUB1,putnum0(str));
	if(profile.dat[PF_MULTI_CURR][0]=='F') {
		SetDlgItemText(IDC_CURR,profile.dat[PF_CURRENCY]);		
	}
	else{
		SetDlgItemText(IDC_CURR,df->dat[IV_CURRENCY]);
	}

    if(SaveFlag == CANNOTSAVE) return;
    if(inv->dat[IV_HOLD_PAY][0]=='T' || inv->dat[IV_HOLD_PAY][0]=='Y'){
        m_ed[IV_REASON].SetReadOnly(FALSE);
    }
    else {
        m_ed[IV_REASON].SetReadOnly(TRUE);
    }
}

void CArinv::OnSetFocus(int fld)
{
    if(fld==IV_HOLD_PAY) {
        UpdateData();
        CString str;
        GetDlgItemText(IDC_EDIT1+fld-1,str);
    }
}

void CArinv::OnKillFocus(int fld)
{
    if(fld==IV_HOLD_PAY) {
        UpdateData();
        CString str;
        GetDlgItemText(IDC_EDIT1+fld-1,str);
    }
}

BOOL CArinv::OnInitDialog()
{
    CParent::OnInitDialog();
    AfxOleLockControl(m_agrid.GetClsid());
    AfxOleLockControl(m_pgrid.GetClsid());
    if(OpenFlag==2) {
        SetDlgItemText(IDC_INV_GLCODE,"Purchase Description");
    }
    else {
        SetDlgItemText(IDC_INV_GLCODE,"Sales Description");
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CArinv::IsChildDataCompleted(DB *sub)
{
    long xrec = qd(sub->dat[Oiv_GLCODE],&glcode);
    double price = sub->val(Oiv_AMT);
    if(price>0 && xrec>0) {
        return TRUE;
    }
    return FALSE;
}

int CArinv::FindChildTotalFields()
{
    if(bTaxInclude == FALSE) {
        return Oiv_AMT;
    }
    return sub->sumfield;
}

void CArinv::SetOneTimeScreen()
{
    m_grid.SetRedraw(FALSE);
    ShowInitFrame(&m_grid,sub);
    m_grid.SetRedraw(TRUE);
    if(STCR == St_CR) {
        if(profile.dat[PF_CUST_PO][0]=='F') {
             GetDlgItem(IDC_INVNO)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_EDIT1+IV_INVOICE-1)->ShowWindow(SW_HIDE);
        }
        GetDlgItem(IDC_EDIT14)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_REASON)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT15)->ShowWindow(SW_HIDE);
    }
    else {
        if(profile.dat[PF_SUPP_INV][0]=='F') {
             GetDlgItem(IDC_INVNO)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_EDIT1+IV_INVOICE-1)->ShowWindow(SW_HIDE);
        }
    }
    if(profile.dat[PF_ETA_DATE][0]=='F') {
         GetDlgItem(IDC_STATIC1+IV_ETA-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IV_ETA-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_PCCODE][0]=='F' || pccode.t->sumrecord==0) {
         GetDlgItem(IDC_STATIC1+IV_PCCODE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IV_PCCODE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+IV_PCCODE-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_TT][0]=='F') {
         GetDlgItem(IDC_STATIC1+IV_TT-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IV_TT-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+IV_TT-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_PREFIX][0]=='F') {
         GetDlgItem(IDC_EDIT1+IV_PREFIX-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_MULTI_CURR][0]=='F') {
         GetDlgItem(IDC_STATIC1+IV_CURRENCY-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IV_CURRENCY-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IV_RATE-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_SALESMAN][0]=='F' || staff.t->sumrecord==0) {
         GetDlgItem(IDC_STAFF)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IV_STAFF-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+IV_STAFF-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_HOLD_PAYMENT][0]=='F') {
         GetDlgItem(IDC_EDIT1+IV_HOLD_PAY-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IV_REASON-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_REASON)->ShowWindow(SW_HIDE);
    }
    

}

BEGIN_EVENTSINK_MAP(CArinv, CParent)
    //{{AFX_EVENTSINK_MAP(CArinv)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

int CArinv::IsSaveConditionOk(DB *df)
{
    int flag = CStaff::IsSaveConditionOk(df);
    if(flag==0) return 0;

    if(ProgramMenuNo == 1) {
        if(!strcmp(df->dat[IV_TYPE],"CB")) {
            if(bal_total) {
                if(MessageBox("Auto general official receipt","Cash Bill",MB_YESNO | MB_ICONQUESTION)==IDYES) {
                    AutoPayment(bal_total);
                }
            }
        }
    }
    return 1;
}
