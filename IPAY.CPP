// Ipay.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Math.h"
#include "Ipay.h"
#include "mgrid.h"
#include "db_ado.h"
#include "PayAllocate.h"
#include "adjoffset.h"
#include "ChequeReturnRegister.h"
#include "AutoOffsetInvoice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIpay dialog


char FrmCurrency[10];
CIpay::CIpay(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
     : CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CIpay)
    m_customer = _T("");
    m_return = 0;
    m_salesman = _T("");
    //}}AFX_DATA_INIT
    bAlwaysRefreshFrasm = FALSE;
    m_PrintOneFormOnly = TRUE;
    m_WinPrint = TRUE;
    m_LoopPrint = TRUE;
}

void CIpay::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIpay)
    DDX_Control(pDX, IDC_PROGRESS1, m_progress);
    DDX_Text(pDX, IDC_STATIC2, m_customer);
    DDX_DateTimeCtrl(pDX, IDC_EDIT13, m_return);
    DDX_Text(pDX, IDC_STAFF, m_salesman);
    DDX_Control(pDX, IDC_BGRID, m_bgrid);
    DDX_Control(pDX, IDC_TOTAL, m_total);
    DDX_Control(pDX, IDC_CHECKBOX1, m_bal_inv);
    //}}AFX_DATA_MAP
}

void CIpay::SetAttribute(int flag,int fld)
{
     CParent::SetAttribute(flag,fld);
     m_bal_inv.SetEnabled(!flag);
     if(flag==TRUE) {
         m_bal_inv.SetCaption("Display current oustanding invoice");
         COleVariant propVal;
         propVal.boolVal = TRUE;
         propVal.vt = VT_BOOL;
         m_bal_inv.SetValue(propVal);
     }
     else {
         m_bal_inv.SetCaption("Direct offset current oustanding invoice");
     }
}

void CIpay::ShowInitFrame()
{
    MGRID g(&m_grid,sub,0,2);
    g.ratio = float(1.60);
    g.SetTitle("No",350,1);
    g.SetTitle("Invoice",950,1);
    g.SetTitle("Invoice Date",1200,4);
    g.SetTitle("Offset Date",1200,4);
    g.SetTitle("Paid Amount",1300,7);
    /*
	GetDlgItem(IDC_STATIC1+IP_CGL-1)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_EDIT1+IP_CGL-1)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_STATIC1+IP_RATE-1)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_EDIT1+IP_RATE-1)->ShowWindow(SW_HIDE);
	*/
    m_bgrid.ShowWindow(SW_HIDE);
}

double gl,rate,cgl;
void CIpay::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    char tmp[20],tmp2[20];

    MGRID g(m_grid,sub,rec);
    g.SetText(0);
    g.SetText(PY_INVOICE);
    DB *pf = (STCR == St_CR) ? &arinv : &apinv;

    irec = pf->Search(sub->dat[PY_INVOICE],1,TRUE);
    if(irec) {
        strcpy(tmp, pf->dat[IV_DATE]);
        strcpy(tmp2, pf->dat[IV_RATE]);
    }
    else {
        space_str(tmp,8);
        space_str(tmp2,8);
    }
    g.SetText(mdate(tmp));
    g.SetText(PY_DATE);
    g.SetText(PY_TOTAL);
	//ooi
	cgl += sub->val(PY_GL);
	
    ItemAmt += sub->val(PY_TOTAL);
	
} 

void CIpay::ShowDetail()
{
    memset(fm.acc,0,sizeof(fm.acc));
    ItemAmt = cgl = 0;
    CParent::ShowDetail();

    char tmp[80];
    ItemAmt = fm.acc[PY_TOTAL];
    dtoa(ItemAmt,tmp,12,2);
    ItemAmt = atof(tmp);
    SetDlgItemText(IDC_TOTAL,putnum0(tmp));
    dtodb(cgl,df,IP_CGL);
	//dtoa(ItemAmt,tmp,12,2);
    double bal_total = df->val(IP_TOTAL) - ItemAmt;
    if(bal_total>0) {
        m_total.SetBackColor(RGB(255,0,255));
    }
    else {
        m_total.SetBackColor(RGB(190,255,255));
    }
}

BEGIN_MESSAGE_MAP(CIpay, CParent)
	//{{AFX_MSG_MAP(CIpay)
	ON_COMMAND(ID_GOTO_NEXT, OnGotoNext)
	ON_BN_CLICKED(IDC_BUTTON1, OnChequeReturnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpay message handlers
void GetPaydatGainLoss(DB *df,DB *sub)
{
    char tmp[80];
    double tpay=0,cgl,cgl1,cgl2, cgl_total = 0, cgl1_total = 0, cgl2_total = 0;

	DB *pf = (STCR == St_CR) ? &arinv : &apinv;
    qd(sub->dat[PY_INVOICE],pf);
	
    cgl1 = atof(sub->dat[PY_TOTAL]) * atof(df->dat[IP_RATE]);
    dtoa(cgl1,tmp,15,2);
    cgl1 = atof(tmp);

    cgl2 = atof(sub->dat[PY_TOTAL]) * atof(pf->dat[IV_RATE]);
    dtoa(cgl2,tmp,15,2);
    cgl2 = atof(tmp);

    cgl  = cgl1 - cgl2;
    dtoa(cgl,tmp,15,2);
    cgl = atof(tmp);

    dtodb(cgl,sub,PY_GL);
}

int CIpay::SaveGiData(double *amt,DB *sub)
{
    char text[20];

    DB *pf = (STCR == St_CR) ? &arinv : &apinv;
    qd(sub->dat[PY_INVOICE],pf);
    double balance = InvoiceBalance(pf, pf->dat[IV_BILLNO]);
    CAdjOffset p1;

    p1.m_invoiceno = receipt.dat[OFI_INVOICE];
    dtoa(balance,text,12,2);
    p1.m_balamt    = text;
    p1.m_offsetamt = sub->dat[PY_TOTAL];
    int flag = p1.DoModal();

    if(flag==IDOK) {

        if(atof(p1.m_offsetamt) > atof(p1.m_balamt)){
            waitmsg("The Offset Amount can not more than Invoice Amount");
            return 0;
        }

        double cal = ItemAmt + atof(p1.m_offsetamt);
        char tmp[20];
        dtoa(cal,tmp,12,2);
        cal = atof(tmp);
        if (cal > ipay->val(IP_TOTAL)) {
            waitmsg("The Pay Amt must equal or less than Total Amt.");
            return 0;
        }
        else {
            long mrec = getdelrec(sub);
            double offsetamt = atof(p1.m_offsetamt);
            dtodb(offsetamt,sub,PY_TOTAL);
            qd(sub->dat[PY_INVOICE], inv);
            lset(sub->dat[PY_DATE],df->dat[IP_DATE]);
            if(strcmp(inv->dat[IV_DATE],sub->dat[PY_DATE])>0) {
                lset(sub->dat[PY_DATE],inv->dat[IV_DATE]);
            }
            GetPaydatGainLoss(ipay,sub);
            AppendIdx(mrec,sub,IdxItem);
            *amt -= sub->val(PY_TOTAL);
        }
    }
    return flag;
}

void CIpay::get_invoice_balance(DB *df,DB *sub,int total_field, int OrderBy)
{
    double amt = df->val(total_field) - ItemAmt;
    if(amt<=0) return;

    if(receipt.t->sumrecord<=0) return;

    if(OrderBy) {
        int field[] = {OFI_DATE,OFI_INVOICE,0};
        SquSortField(&receipt,field);
    }
    else {
        int field[] = {OFI_INVOICE,0};
        SquSortField(&receipt,field);
    }
    while(TRUE) {
        if(amt==0) break;
//        long rec = viewinv(&receipt);
        ResetReceiptBalance();
        long rec = viewidx(&receipt);
        if(rec==0){
            break;
        }
        double offset = receipt.val(OFI_IBAL);
        if(offset==0) continue;

        lsetblank(sub);
        lset(sub->dat[1],df->dat[1]);
        lset(sub->dat[PY_INVOICE],receipt.dat[OFI_INVOICE]);
        lset(sub->dat[PY_TOTAL],receipt.dat[OFI_TOTAL]);
        if(amt>=offset) {
            dtodb(offset,sub,PY_TOTAL);
        }
        else {
            dtodb(amt,sub,PY_TOTAL);
        }
        int key = SaveGiData(&amt,sub);
        ReFreshSubFile();
        ShowInformation();
        if(key==IDOK) {
            waitflock(&receipt);
            deleterec(rec,&receipt);
            unflock(&receipt);
        }
        if(receipt.t->sumrecord<=0) break;
        if(amt<=0) break;
    }
    ShowInformation();
}

void CIpay::AddOffsetInvoice()
{
    long last = receipt.t->sumrecord;

    waitflock(sub);
    for(long rec=1; rec<=last; rec++) {
        readrec(rec,&receipt);
        if(receipt.val(OFI_PAY)==0) continue;
        long mrec = getdel(sub);
        lset(sub->dat[1],df->dat[1]);
        lset(sub->dat[PY_INVOICE],receipt.dat[OFI_INVOICE]);
        rset(sub->dat[PY_TOTAL],receipt.dat[OFI_PAY]);
        qd(sub->dat[PY_INVOICE], inv);
        lset(sub->dat[PY_DATE],df->dat[IP_DATE]);
        if(strcmp(inv->dat[IV_DATE],sub->dat[PY_DATE])>0) {
            lset(sub->dat[PY_DATE],inv->dat[IV_DATE]);
        }
        sub->Find_SequenceNo();
        AppendIdx(mrec,sub,IdxItem);
    }
    unflock(sub);

    ShowInformation();
}

void CIpay::AutoCreateCreditNote()
{
    lset(cr->dat[1],df->dat[1]);
    long xrec = dseek(cr->dat[1],cr,1,MF);
    if(xrec>0) return;

    long last = receipt.t->sumrecord;

    waitflock(cr);
    waitflock(crdat);
    waitflock(gldat);
    double cr_amt = 0;
    for(long rec=1; rec<=last; rec++) {
        readrec(rec,&receipt);
        if(receipt.val(OFI_DISCOUNT)==0) continue;
        long nrec = getdel(crdat);
        lset(crdat->dat[1],df->dat[1]);
        lset(crdat->dat[CN_INVOICE],receipt.dat[OFI_INVOICE]);
        rset(crdat->dat[CN_TOTAL],receipt.dat[OFI_DISCOUNT]);
        lset(crdat->dat[CN_DATE],df->dat[IP_DATE]);
        crdat->Find_SequenceNo();
        AppendIdx(nrec,crdat,1);
        cr_amt += crdat->val(CN_TOTAL);
    }
    if(cr_amt) {
        long mrec = getdel(cr);
        lset(cr->dat[1],df->dat[1]);
        lset(cr->dat[Cr_DATE],df->dat[IP_DATE]);
        lset(cr->dat[Cr_CUSTOMER],df->dat[IP_CUSTOMER]);
        lset(cr->dat[Cr_CURRENCY],df->dat[IP_CURRENCY]);
        rset(cr->dat[Cr_RATE],df->dat[IP_RATE]);
        dtodb(cr_amt,cr,Cr_TOTAL);
        AppendIdx(mrec,cr,1);
        mrec = getdel(gldat);
        lset(gldat->dat[1],df->dat[1]);
        qd(customer->dat[CU_SCHEME],&scheme);
        lset(gldat->dat[CG_GLCODE],scheme.dat[Sch_GLCODE]);
        dtodb(cr_amt,gldat,CG_TOTAL);
        gldat->Find_SequenceNo();
        AppendIdx(mrec,gldat,1);
    }
    unflock(cr);
    unflock(crdat);
    unflock(gldat);
}

void CIpay::SetOneTimeScreen()
{
    if(profile.dat[PF_MULTI_CURR][0]=='F') {
         GetDlgItem(IDC_STATIC1+IP_CURRENCY-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IP_CURRENCY-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_STATIC1+IP_RATE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IP_RATE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_STATIC1+IP_CGL-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IP_CGL-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_PAYTYPE][0]=='F') {
         GetDlgItem(IDC_STATIC1+IP_PAYTYPE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IP_PAYTYPE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+IP_PAYTYPE-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_CUSTOMER_BANK][0]=='F') {
         GetDlgItem(IDC_STATIC1+IP_BANK2-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IP_BANK2-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_TAX_NO][0]=='F') {
         GetDlgItem(IDC_STATIC1+IP_VATNO-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IP_VATNO-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_TEMP_REC][0]=='F') {
         GetDlgItem(IDC_STATIC1+IP_RECEIPT-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+IP_RECEIPT-1)->ShowWindow(SW_HIDE);
    }
    if(STCR == St_CR) {
        if(profile.dat[PF_COLLECT_BY][0]=='F' || staff.t->sumrecord==0) {
             GetDlgItem(IDC_STAFF)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_EDIT1+IP_STAFF-1)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_LABEL1+IP_STAFF-1)->ShowWindow(SW_HIDE);
        }
    }
    else {
        if(profile.dat[PF_PAID_BY][0]=='F' || staff.t->sumrecord==0) {
             GetDlgItem(IDC_STAFF)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_EDIT1+IP_STAFF-1)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_LABEL1+IP_STAFF-1)->ShowWindow(SW_HIDE);
        }
    }
    m_bal_inv.SetCaption("");
    //SetDlgItemText(IDC_CURR,profile.dat[PF_CURRENCY]);
}

BOOL CIpay::OnInitDialog() 
{
    CParent::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIpay::SetBalanceGridInitFrame(long rec)
{
    MGRID g2(&m_bgrid,&receipt,0,5);
    g2.ratio = float(1.08);
    if(rec>7) {
        g2.ratio = 1;
    }
    g2.SetTitle("#",300,1);
    g2.SetTitle("Invoice",900,1);
    g2.SetTitle("Bal.",950,7);
    g2.SetTitle("Acc. Amt",950,7);
    g2.SetTitle("",100);
}

void CIpay::ReplaceBalanceGrid(long rec)
{
    MGRID g2(&m_bgrid,&receipt,rec);
    g2.SetText(&receipt,0,OFI_INVOICE,OFI_IBAL,OFI_ACC,OFI_OVERDUE,-1);
}

void CIpay::RefreshBalanceInvoiceGrid()
{
    long last = receipt.t->sumrecord;
    m_bgrid.SetRedraw(FALSE);
    m_bgrid.SetRows(last+1);
    SetBalanceGridInitFrame(last);
    double iacc = 0;
    for(long rec=1; rec<=last; rec++) {
        readrec(rec,&receipt);
        iacc += receipt.val(OFI_IBAL);
        dtodb(iacc,&receipt,OFI_ACC);
        m_bgrid.SetRow(rec);
        ReplaceBalanceGrid(rec);
    }
    m_bgrid.ShowWindow(SW_SHOW);
    m_bgrid.SetRedraw(TRUE);
}

void CIpay::ShowInformation()
{
    if(strcmp(FrmCurrency,df->dat[IP_CURRENCY])) {
        strcpy(FrmCurrency,df->dat[IP_CURRENCY]);
        ShowInitFrame();
    }
	if(profile.dat[PF_MULTI_CURR][0]=='F') {
		SetDlgItemText(IDC_CURR,profile.dat[PF_CURRENCY]);		
	}
	else{
		SetDlgItemText(IDC_CURR,df->dat[IP_CURRENCY]);
	}
    CParent::ShowInformation();
    ItemAmt = fm.acc[PY_TOTAL];
    if(SaveFlag == CANNOTSAVE) return;

    double balance = PaydatTotal();
    if(balance==0) return;
}

void CIpay::OnGotoNext() 
{
    GotoDlgCtrl(GetDlgItem(CtrlId(IP_CHEQUE)));
}

BEGIN_EVENTSINK_MAP(CIpay, CParent)
    //{{AFX_EVENTSINK_MAP(CIpay)
	ON_EVENT(CIpay, IDC_BGRID, -601 /* DblClick */, OnClickBalanceInvoice, VTS_NONE)
	ON_EVENT(CIpay, IDC_CHECKBOX1, 2 /* Change */, OnChangeCheckbox1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
int CIpay::SavePayData(double *amt,DB *sub)
{
    CAdjOffset p1;

    p1.m_invoiceno = receipt.dat[OFI_INVOICE];
    p1.m_offsetamt = sub->dat[PY_TOTAL];
    int flag = p1.DoModal();

    if(flag==IDOK) {

        if(atof(p1.m_offsetamt) > atof(p1.m_balamt)){
            waitmsg("The Offset Amount can not more than Invoice Amount");
            return 0;
        }

        long mrec = getdelrec(sub);
        double offsetamt = atof(p1.m_offsetamt);
        dtodb(offsetamt,sub,PY_TOTAL);
        qd(sub->dat[PY_INVOICE], inv);
        lset(sub->dat[PY_DATE],df->dat[IP_DATE]);
        if(strcmp(inv->dat[IV_DATE],sub->dat[PY_DATE])>0) {
            lset(sub->dat[PY_DATE],inv->dat[IV_DATE]);
        }
        GetPaydatGainLoss(ipay,sub);
        AppendIdx(mrec,sub,IdxItem);
        double total = ipay->val(IP_TOTAL) + offsetamt;
        dtodb(total,ipay,IP_TOTAL);
    }
    return flag;
}

void CIpay::OnClickBalanceInvoice() 
{
    BOOL bValue = m_bal_inv.GetEnabled();
    if(bValue==FALSE) return;

    COleVariant propVal;
    propVal =  m_bal_inv.GetValue();
    bValue = propVal.boolVal;
    if(bValue==FALSE) return;

    if(SaveFlag == NEWFILE){
        SaveNewFileOnly();
    }

    long last = receipt.t->sumrecord;
    if(last==0) return;

    long rec = m_bgrid.GetRow();
    readrec(rec,&receipt);

    lsetblank(sub);
    lset(sub->dat[1],df->dat[1]);
    lset(sub->dat[PY_INVOICE],receipt.dat[OFI_INVOICE]);
    rset(sub->dat[PY_TOTAL],receipt.dat[OFI_IBAL]);
    double amt = sub->val(PY_TOTAL);

    int key = SavePayData(&amt,sub);
    ComputeBankCommission(df);
    ReFreshSubFile();
    ShowInformation();
    if(key==IDOK) {
        waitflock(&receipt);
        deleterec(rec,&receipt);
        unflock(&receipt);
        RefreshBalanceInvoiceGrid();
    }
}

void CIpay::ShowOutstandingInvoice()
{
    m_bal_inv.SetCaption("Please wait a moment...");
    m_p1 = &m_progress;

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GetInvoiceInformation(ipay);
    m_bgrid.ShowWindow(SW_SHOW);
    RefreshBalanceInvoiceGrid();
    m_bal_inv.SetCaption("Direct offset oustanding invoice");
}

void CIpay::OnChangeCheckbox1() 
{
    COleVariant propVal;
    propVal =  m_bal_inv.GetValue();
    BOOL bValue = propVal.boolVal;
    if(bValue==FALSE){
        m_bal_inv.SetCaption("Display current oustanding invoice");
        m_bgrid.ShowWindow(SW_HIDE);
        return;
    }
    ShowOutstandingInvoice();
}

void CIpay::OnChequeReturnRegister() 
{
    CChequeReturnRegister h1;

    h1.df = df;
    h1.sub = sub;
    h1.DoModal();
}

double CIpay::PaydatTotal()
{
    CString str;
    str.Format("select sum(total) from [%s] where [%s]= %s",
        sub->m_strTable, sub->fieldName[1], DbText(df,1));
    double paytotal = SqlValue(str);
    double balance = Double( df->val(IP_TOTAL) - paytotal );
    return balance;
}

void CIpay::OnDetail()
{
    if(SaveMainInformation()==FALSE) return;

    COleVariant propVal;
    propVal.boolVal = TRUE;
    m_bal_inv.SetValue(propVal);
    ShowOutstandingInvoice();

    ReFreshSubFile();
    ShowInformation();

    long xrec = qd(ipay->dat[IP_CUSTOMER],customer);
    if(receipt.t->sumrecord==0) {
        double balance = PaydatTotal();
        if(balance==0) return;
        CString msg;
        msg.Format("%s\n\nNo more oustanding balance\n\nreason:-\n1)Advance payment 2)Wrong a/c code",customer->dat[2]);
        MessageBox(msg,"Remaining message",MB_ICONSTOP);
        return;
    }

    CAutoOffsetInvoice h1;
    h1.df = df;
    h1.sub = sub;
    h1.DoModal();

    ReFreshSubFile();
    ShowInformation();
    GotoDlgCtrl(GetDlgItem(CtrlId(IP_REMARKS)));
    ShowOutstandingInvoice();
}
