// Cr.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Cr.h"
#include "AdjOffset.h"
#include "mgrid.h"
#include "DuplicateCR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern double gl,rate,cgl;
extern void GldatChildControl(UINT IDD,DB *df,DB *psub,int IdxItem);
/////////////////////////////////////////////////////////////////////////////
// CCr dialog


CCr::CCr(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CCr)
	m_customer = _T("");
	m_salesman = _T("");
	//}}AFX_DATA_INIT
    bAlwaysRefreshFrasm = FALSE;
    m_PrintOneFormOnly = TRUE;
    m_WinPrint = TRUE;
}

void CCr::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCr)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
    DDX_Text(pDX, IDC_STATIC2, m_customer);
	DDX_Control(pDX, IDC_GLGRID, m_glgrid);
	DDX_Text(pDX, IDC_STAFF, m_salesman);
	DDX_Control(pDX, IDC_BGRID, m_bgrid);
	DDX_Control(pDX, IDC_CHECKBOX1, m_bal_inv);
	//}}AFX_DATA_MAP
}

void GetCrdatGainLoss(DB *df,DB *sub)
{
    char tmp[80];
    double tpay=0,cgl,cgl1,cgl2, cgl_total = 0, cgl1_total = 0, cgl2_total = 0;
	
	DB *pf = (STCR == St_CR) ? &arinv : &apinv;
    qd(sub->dat[CN_INVOICE],pf);
	
    cgl1 = atof(sub->dat[CN_TOTAL]) * atof(df->dat[Cr_RATE]);
    dtoa(cgl1,tmp,15,2);
    cgl1 = atof(tmp);
	
    cgl2 = atof(sub->dat[CN_TOTAL]) * atof(pf->dat[IV_RATE]);
    dtoa(cgl2,tmp,15,2);
    cgl2 = atof(tmp);
	
    cgl  = cgl1 - cgl2;
    dtoa(cgl,tmp,15,2);
    cgl = atof(tmp);
	
    dtodb(cgl,sub,CN_GL);
}

void CCr::ShowInitFrame()
{
	MGRID g(&m_grid,sub,0,2);
	g.ratio = float(1.80);
	g.SetTitle("No",350);
	g.SetTitle("Inv. No",1250);
	g.SetTitle("Inv. Date",1350,4);
	g.SetTitle("Offset Date",1750,4);
	g.SetTitle("Adj. Amt",1750,7);
	/*
    int rows = (profile.dat[PF_MULTI_CURR][0]=='F') ? 5: 8;
    MGRID g(&m_grid,sub,0,rows);
    if(rows==8) {
         g.ratio = float(1.12);
         g.SetTitle("No",350);
         g.SetTitle("Inv. No",1250);
         g.SetTitle("Inv. Date",1250,4);
         g.SetTitle("Offset Date",2000,4);
         g.SetTitle("Adj. Amt",1750,7);
         g.SetTitle("Ex. Rate",1200,7);

         CString tmp;
         tmp.Format("Total Adj. (%s)",profile.dat[PF_CURRENCY]);
         g.SetTitle(tmp,1500,7);
         g.SetTitle("Gain/Loss",1500,7);
         GetDlgItem(IDC_STATIC1+Cr_CGL-1)->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_EDIT1+Cr_CGL-1)->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_STATIC1+Cr_RATE-1)->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_EDIT1+Cr_RATE-1)->ShowWindow(SW_SHOW);
    }
    else {
         g.ratio = float(1.80);
         g.SetTitle("No",350);
         g.SetTitle("Inv. No",1250);
         g.SetTitle("Inv. Date",1350,4);
         g.SetTitle("Offset Date",1750,4);
         g.SetTitle("Adj. Amt",1750,7);
         GetDlgItem(IDC_STATIC1+Cr_CGL-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+Cr_CGL-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_STATIC1+Cr_RATE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+Cr_RATE-1)->ShowWindow(SW_HIDE);
    }
	*/
    m_bgrid.ShowWindow(SW_HIDE);
}

void CCr::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    char tmp[20],tmp2[20];

    MGRID g(m_grid,sub,rec);
	//int rows = (profile.dat[PF_MULTI_CURR][0]=='F') ? 5: 8;
    
    
    g.SetText(0);
    g.SetText(CN_INVOICE);

    DB *pf = (STCR == St_CR) ? &arinv : &apinv;
    irec = pf->Search(sub->dat[CN_INVOICE],1,TRUE);
    if(irec) {
        strcpy(tmp, pf->dat[IV_DATE]);
        strcpy(tmp2, pf->dat[IV_RATE]);
    }
    else {
        space_str(tmp,8);
        space_str(tmp2,8);
    }
    g.SetText(mdate(tmp));
    g.SetText(sub,CN_DATE,CN_TOTAL,-1);	
	cgl += sub->val(CN_GL);
    ItemAmt += sub->val(CN_TOTAL);
} 

void CCr::ShowGldataInitFrame()
{
    if(profile.dat[PF_CN_CATEGORY][0]=='F') {
        MGRID g(&m_glgrid,gldat,0,3);
        g.ratio = float(1.20);
        g.SetTitle("G/L Code",1100);
        g.SetTitle("Description",4200);
        g.SetTitle("Total Amount",1500,7);
    }
    else {
        MGRID g(&m_glgrid,gldat,0,6);
        g.ratio = float(0.75);
        g.SetTitle("Your Ref.",1100);
        g.SetTitle("G/L Code",1100);
        g.SetTitle("Description",4200);
        g.SetTitle("CN",600);
        g.SetTitle("Category Name",2200);
        g.SetTitle("Amount",1300,7);
    }
}

void CCr::ReplaceGldataSubGrid(long rec)
{
    MGRID g(&m_glgrid,gldat,rec);
    if(profile.dat[PF_CN_CATEGORY][0]=='F') {
        g.SetText(gldat,CG_GLCODE,CG_DESCRIPT,CG_TOTAL, -1);
    }
    else {
        g.SetText(gldat,CG_YOUR_REF,CG_GLCODE,CG_DESCRIPT,CG_ADJ_CAT,CG_ADJ_CAT | 0x100,CG_TOTAL, -1);
    }
} 

void CCr::ShowDetail()
{
    memset(fm.acc,0,sizeof(fm.acc));
    long hrec;
    ItemAmt = cgl = 0;
    CParent::ShowDetail();

    char tmp[80];
    sprintf(tmp,"%12.2f",ItemAmt);
//    SetDlgItemText(IDC_TOTAL,putnum0(tmp));
    dtodb(cgl,df,Cr_CGL);

    m_glgrid.SetRedraw(FALSE);
    ShowGldataInitFrame();
    long Jrec = interval(&hrec,df->dat[1],gldat,1);
    m_glgrid.SetRows(Jrec+1);
    TotalGlAmount = 0;
    for(long krec=0; krec<Jrec; krec++){
        m_glgrid.SetRow(krec+1);
        ridx(krec+hrec, gldat, 1);
        TotalGlAmount += gldat->val(CG_TOTAL);
        ReplaceGldataSubGrid(krec);
    }
    dtodb(TotalGlAmount,df,Cr_TOTAL);
    cr_amt = TotalGlAmount;
    m_glgrid.SetRedraw(TRUE);
}

void CCr::SetAttribute(int flag,int fld)
{
     CParent::SetAttribute(flag,fld);
     if(flag==FALSE) {
        GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
     }
     else {
        GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
     }
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

BEGIN_MESSAGE_MAP(CCr, CParent)
	//{{AFX_MSG_MAP(CCr)
	ON_BN_CLICKED(IDC_BUTTON1, OnShow)
	ON_BN_CLICKED(IDC_BUTTON2, OnDuplicate)
	ON_COMMAND(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCr message handlers

BOOL CCr::OnInitDialog()
{
    CParent::OnInitDialog();

    if(STCR == St_DR) {
        CMenu *p = GetMenu();
        p->EnableMenuItem(IDC_BUTTON2,MF_GRAYED);
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CCr::SaveGiData(double *amt,DB *sub)
{
    char text[20];
    DB *pf = (STCR == St_CR) ? &arinv : &apinv;
    double balance = InvoiceBalance(pf,receipt.dat[OFI_INVOICE]);
    CAdjOffset p1;

    p1.m_invoiceno = receipt.dat[OFI_INVOICE];
    p1.m_balamt    = receipt.dat[OFI_BAL];
    double balamt = atof(receipt.dat[OFI_BAL]);
    double cnamt = atof(sub->dat[CN_TOTAL]);
    if(balamt<cnamt) {
        p1.m_offsetamt = receipt.dat[OFI_BAL];
    }
    else {
        p1.m_offsetamt = sub->dat[CN_TOTAL];
    }
    if(balance<balamt && balance>0){
        dtoa(balance,text,12,2);
        p1.m_offsetamt = text;
    }
    int flag = p1.DoModal();

    if(flag==IDOK) {
        double cal = ItemAmt + atof(p1.m_offsetamt);
		char tmp[20];
                dtoa(cal,tmp,16,2);
		cal = atof(tmp);
        if (cal > cr->val(Cr_TOTAL) ){
            waitmsg("The Adj.Amt must equal or less than Total Amt.");
            return 0;
        }
        else {
            long mrec = getdelrec(sub);
            double offsetamt = atof(p1.m_offsetamt);
            dtodb(offsetamt,sub,CN_TOTAL);
            dbasedate(sub->dat[CN_DATE]);
            //dtodb(gl,sub,CN_GL);
			GetCrdatGainLoss(cr,sub);
            sub->Find_SequenceNo();
            AppendIdx(mrec,sub,IdxItem);
            *amt -= sub->val(CN_TOTAL);
        }
    }
    return flag;
}

void CCr::get_invoice_balance(DB *df,DB *sub,int total_field)
{
    double amt  = df->val(total_field) - ItemAmt;
    if(amt<=0) return;
    if(receipt.t->sumrecord<=0) return;
    while(TRUE) {
        if(amt<=0) break;
        //rec = viewinv(&receipt);
        long rec = viewidx(&receipt);
        if(rec==0){
            break;
        }
        double offset = receipt.val(OFI_TOTAL);
        if(offset==0) continue;
        lsetblank(sub);
        lset(sub->dat[1],df->dat[1]);
        lset(sub->dat[CN_INVOICE],receipt.dat[OFI_INVOICE]);
        rset(sub->dat[CN_TOTAL],receipt.dat[OFI_TOTAL]);
        if(amt>=offset) {
            dtodb(offset,sub,CN_TOTAL);
        }
        else {
            dtodb(amt,sub,CN_TOTAL);
        }
        int key = SaveGiData(&amt,sub);
        ShowInformation();
        if(key==IDOK) {
            deleterec(rec,&receipt);
        }
        if(receipt.t->sumrecord<=0) break;
        if(amt<=0) break;
        ResetReceiptBalance();
    }
    ShowInformation();
}


void CCr::OnShow() 
{
    double balance = df->val(Cr_TOTAL) - fm.acc[CN_TOTAL];
    char tnp[20];
    dtoa(balance,tnp,15,2);
    if(atof(tnp)==0) return;
    get_invoice_balance(df,sub,Cr_TOTAL);
    if(balance>0) {
        cbTotal[5].SetBackColor(RGB(255,0,255));
    }
    else {
        cbTotal[5].SetBackColor(RGB(190,255,255));
    }
    ReFreshSubFile();
    ShowInformation();
}

BOOL CCr::IsChildDataCompleted(DB *sub)
{
    long xrec = qd(sub->dat[CG_GLCODE],&glcode);
    double price = sub->val(CG_TOTAL);
    if(price>0 && xrec>0) {
        return TRUE;
    }
    return FALSE;
}

void CCr::SetOneTimeScreen()
{
    if(profile.dat[PF_MULTI_CURR][0]=='F') {
         GetDlgItem(IDC_EDIT1+Cr_CURRENCY-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_STATIC1+Cr_RATE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+Cr_RATE-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_SALESMAN][0]=='F' || staff.t->sumrecord==0) {
         GetDlgItem(IDC_STAFF)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+Cr_STAFF-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+Cr_STAFF-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_PCCODE][0]=='F' || pccode.t->sumrecord==0) {
         GetDlgItem(IDC_STATIC1+Cr_PCCODE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+Cr_PCCODE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+Cr_PCCODE-1)->ShowWindow(SW_HIDE);
    }
    if(STCR == St_CR) {
        if(profile.dat[PF_COLLECT_BY][0]=='F') {
             GetDlgItem(IDC_STAFF)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_EDIT1+Cr_STAFF-1)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_LABEL1+Cr_STAFF-1)->ShowWindow(SW_HIDE);
        }
    }
    else {
        if(profile.dat[PF_PAID_BY][0]=='F') {
             GetDlgItem(IDC_STAFF)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_EDIT1+Cr_STAFF-1)->ShowWindow(SW_HIDE);
             GetDlgItem(IDC_LABEL1+Cr_STAFF-1)->ShowWindow(SW_HIDE);
        }
    }
    //SetDlgItemText(IDC_CURR,profile.dat[PF_CURRENCY]);
    m_bal_inv.SetCaption("");
}

void CCr::ReplaceBalanceGrid(long rec)
{
    MGRID g2(&m_bgrid,&receipt,rec);
    g2.SetText(&receipt,0,OFI_INVOICE,OFI_IBAL,OFI_ACC,OFI_OVERDUE,-1);
}

void CCr::SetBalanceGridInitFrame(long rec)
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

void CCr::RefreshBalanceInvoiceGrid()
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

void CCr::ShowInformation()
{
	if(profile.dat[PF_MULTI_CURR][0]=='F') {
		SetDlgItemText(IDC_CURR,profile.dat[PF_CURRENCY]);		
	}
	else{
		SetDlgItemText(IDC_CURR,df->dat[Cr_CURRENCY]);
	}
    CParent::ShowInformation();
    if(SaveFlag == CANNOTSAVE) return;
    if(strcmp(FrmCurrency,df->dat[Cr_CURRENCY])) {
        strcpy(FrmCurrency,df->dat[Cr_CURRENCY]);
        ShowInitFrame();
    }
	
}

void CCr::OnDetail()
{
    if(SaveFlag == NEWFILE){
        SaveNewFileOnly();
    }
    COleVariant propVal;
    propVal.boolVal = TRUE;
    m_bal_inv.SetValue(propVal);
    ShowOutstandingInvoice();

    GldatChildControl(IDD_GLDAT,df,gldat,IdxItem);
    ReFreshSubFile();
    ShowInformation();
    OnShow();
}

BEGIN_EVENTSINK_MAP(CCr, CParent)
    //{{AFX_EVENTSINK_MAP(CCr)
	ON_EVENT(CCr, IDC_BGRID, -601 /* DblClick */, OnDblClickBgrid, VTS_NONE)
	ON_EVENT(CCr, IDC_CHECKBOX1, 2 /* Change */, OnChangeCheckbox1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

int CCr::SavePayData(double *amt,DB *sub)
{
    CAdjOffset p1;

    double cramt  = df->val(Cr_TOTAL);
    double netamt = cramt - ItemAmt;

    p1.m_invoiceno = receipt.dat[OFI_INVOICE];
    p1.m_offsetamt = sub->dat[CN_TOTAL];

    int flag = p1.DoModal();

    if(flag==IDOK) {

        if(atof(p1.m_offsetamt) > atof(p1.m_balamt)){
            waitmsg("The Offset Amount can not more than Invoice Amount");
            return 0;
        }

        long mrec = getdelrec(sub);
        double offsetamt = atof(p1.m_offsetamt);
        dtodb(offsetamt,sub,CN_TOTAL);
        qd(sub->dat[CN_INVOICE], inv);
        lset(sub->dat[CN_DATE],df->dat[IP_DATE]);
        if(strcmp(inv->dat[IV_DATE],sub->dat[CN_DATE])>0) {
            lset(sub->dat[CN_DATE],inv->dat[IV_DATE]);
        }
		GetCrdatGainLoss(cr,sub);
        AppendIdx(mrec,sub,IdxItem);
        double total = cr->val(Cr_TOTAL) + offsetamt;
        dtodb(total,cr,Cr_TOTAL);
    }
    return flag;
}

void CCr::OnDblClickBgrid() 
{
    BOOL bValue = m_bal_inv.GetEnabled();
    if(bValue==FALSE) return;

    COleVariant propVal;
    propVal =  m_bal_inv.GetValue();
    if(propVal.boolVal==FALSE) return;

    long last = receipt.t->sumrecord;
    if(last==0) return;

    if(SaveFlag == NEWFILE){
        SaveNewFileOnly();
    }

    long rec = m_bgrid.GetRow();
    readrec(rec,&receipt);

    lsetblank(sub);
    lset(sub->dat[1],df->dat[1]);
    lset(sub->dat[CN_INVOICE],receipt.dat[OFI_INVOICE]);
    rset(sub->dat[CN_TOTAL],receipt.dat[OFI_IBAL]);
    double amt = sub->val(CN_TOTAL);
	
    int key = SavePayData(&amt,sub);
    ReFreshSubFile();
    ShowInformation();
    if(key==IDOK) {
        deleterec(rec,&receipt);
        RefreshBalanceInvoiceGrid();
    }
}

void CCr::ShowOutstandingInvoice()
{
    m_bal_inv.SetCaption("Please wait a moment...");
    m_p1 = &m_progress;
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GetInvoiceInformation(cr);
    m_bgrid.ShowWindow(SW_SHOW);
    RefreshBalanceInvoiceGrid();
    m_bal_inv.SetCaption("Direct offset oustanding invoice");
}

void CCr::OnChangeCheckbox1() 
{
    COleVariant propVal = m_bal_inv.GetValue();

    if(propVal.boolVal==FALSE){
        m_bal_inv.SetCaption("Display current oustanding invoice");
        m_bgrid.ShowWindow(SW_HIDE);
        return;
    }
    ShowOutstandingInvoice();
}

void CCr::OnDuplicate() 
{
    CDuplicateCR h1;

    h1.m_crno = df->dat[1];
    h1.DoModal();
	
}

void CCr::OnButton2() 
{
	
}
