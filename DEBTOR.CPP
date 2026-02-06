// Debtor.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Debtor.h"
#include "mgrid.h"
#include "db_ado.h"
#include "openrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum state {Cst_DATE=1,Cst_REFNO,Cst_DEBIT,Cst_CREDIT,Cst_BALANCE};

DB cstate(IDD_INVOICE,5);
/////////////////////////////////////////////////////////////////////////////
// CDebtor dialog


CDebtor::CDebtor(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CDebtor)
	m_date = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CDebtor::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebtor)
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebtor, CStaff)
	//{{AFX_MSG_MAP(CDebtor)
	ON_BN_CLICKED(IDC_BUTTON1, OnCystomerType)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnStatement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebtor message handlers

void CDebtor::SetOneTimeScreen()
{
    if(profile.dat[PF_MULTI_CURR][0]=='F') {
         GetDlgItem(IDC_STATIC1+CU_CURRENCY-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+CU_CURRENCY-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_AREA][0]=='F') {
         GetDlgItem(IDC_STATIC1+CU_AREA-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+CU_AREA-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+CU_AREA-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_SCHEME][0]=='F') {
         GetDlgItem(IDC_STATIC1+CU_SCHEME-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+CU_SCHEME-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+CU_SCHEME-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_PREFIX][0]=='F') {
         GetDlgItem(IDC_EDIT1+CU_PREFIX-1)->ShowWindow(SW_HIDE);
    }
    if(profile.dat[PF_RET_CHQ_RECORDS][0]=='F'){
         GetDlgItem(IDC_MSFLEXGRID1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_DCR_LABEL)->ShowWindow(SW_HIDE);
    }
}

void ReplaceBUYERGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,NULL,CU_CODE,CU_NAME,CU_TEL1,CU_CONTACT,-1);
}

void SetBUYERGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,2);
    g.SetTitle(df,0,CU_CODE,-1);
    g.SetTitle("Name",4900);
    g.SetTitle("Tel. No",1100);
    g.SetTitle("Contract",2000);
}

int tell_debtor(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case 3:
        GetShortName(df,fld);
        break;
    }
    return 2;
}

int start_debtor(DB *df)
{
    dbasedate(df->dat[CU_ODATE]);
    return 1;
}

int Debtor_AdminDb(DB *df,int flag)
{
	AdminAccodeDb(df);
    return 1;
}

int filter_on(DB *df,int flag)
{
    return 1;
}

int debtor_filter(DB *df,int flag)
{
    if(isblank(df->dat[CU_GLCODE])) return 0;
    return 1;
}

int creditor_filter(DB *df,int flag)
{
    if(isblank(df->dat[CU_GLCODE2])) return 0;
    return 1;
}

int save_debtor(DB *df,int flag)
{
    long irec = qq(df->dat[CU_GLCODE],&glcode);
    if(irec==0) {
        AfxMessageBox("Please key in Debtor G/L Code");
        return 0;
    }
    return 1;
}

void init_debtor(DB *df)
{
    df->e->AdminDb          = Debtor_AdminDb;
    df->e->start            = start_debtor;
    df->e->tell             = tell_debtor;
    df->e->rep              = ReplaceBUYERGrid;
    df->e->GridTitle        = SetBUYERGridTitle;
    df->BROWSE_ID           = IDD_BROWSE_DEBTOR;
    df->e->BrowseFilter     = db_AdminDb;
    df->e->IsBrowseFilterOn = db_AdminDb;
    df->e->save             = save_debtor;
}

int save_creditor(DB *df,int flag)
{
    long irec = qq(df->dat[CU_GLCODE2],&glcode);
    if(irec==0) {
        AfxMessageBox("Please key in Creditor G/L Code");
        return 0;
    }
    return 1;
}

void init_creditor(DB *df)
{
    df->e->AdminDb          = Debtor_AdminDb;
    df->e->start            = start_debtor;
    df->e->tell             = tell_debtor;
    df->e->rep              = ReplaceBUYERGrid;
    df->e->GridTitle        = SetBUYERGridTitle;
    df->BROWSE_ID           = IDD_BROWSE_DEBTOR;
    df->e->BrowseFilter     = db_AdminDb;
    df->e->IsBrowseFilterOn = db_AdminDb;
    df->e->save             = save_creditor;
}

void AdminAccodeDb(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CDebtor p1(df->ID,df,df->f[1].idx & 0x01);
    p1.m_OpenLpt = FALSE;
    p1.LastField = CU_CUST_BAL;
    p1.DoModal();
}

void CDebtor::OnCystomerType() 
{
    AdminDb(&staff);
}

void CDebtor::OnButton2() 
{
    AdminDb(&area);
}

void CDebtor::OnButton3() 
{
    AdminDb(&currency);
}

void CDebtor::OnButton4() 
{
	UpdateData();
	lset(one.dat[1],m_date);
    CString strSql,strTable = (STCR == St_CR) ? "cstate" : "pstate";
    strSql.Format("select [date],[refno],[debit],[credit] from [%s] where [debtor] = %s and [date] <= '%s' order by [date],type,refno",
        strTable,DbText(df,1), DateStr(one.dat[1]));

    cstate.DefaultOrder = FALSE;
    dopen(&cstate,"state.rep");

    OpenRs  p1(strSql);
    p1.ToDBF(&cstate);
    AutoDebitCreditBalance(&cstate,3);

    long xrec = viewidx(&cstate);
    dclose(&cstate);
}

enum aa1 {re_RENO=1,re_DEBTOR,re_DATE,re_AMT,re_PAY,re_CN,re_RC};
void CollectReceipt21(DB *data,char *cust, char *cur,char *paydate)
{
    memset(Over_Balance,0,sizeof(Over_Balance));
    char szTmp[40];

    DB adoTmp;
    CString strTable = "arinv_bal";
    if(STCR == St_DR) strTable = "apinv_bal";

    BOOL flag = adoTmp.OpenStoreProcedure(strTable,
        ADOCG::adVarChar,"debtor", cust,
        ADOCG::adDate,"date",one.dat[1],
        0);

    memstart(data);
    lsetblank(data);

//    int nID = AfxMessageBox("Show zero balance transaction",MB_ICONQUESTION | MB_YESNO);
    double Totalbal = 0;
    while( adoTmp.GetSpData() ) {
		
        double bal = adoTmp.val(re_AMT) - adoTmp.val(re_PAY) -
                     adoTmp.val(re_CN) + adoTmp.val(re_RC);
        dtoa(bal, szTmp, 15, 2);
        bal = atof(szTmp);
//        if(nID ==IDNO && bal==0) continue;
        if(bal==0) continue;
        lset(data->dat[RE_INVOICE],adoTmp.dat[1]);
        lset(data->dat[RE_DATE],adoTmp.dat[Ibal_DATE]);
        double iamt = adoTmp.val(re_AMT);
        dtodb(iamt,data,RE_TOTAL);
        dtodb(bal,data,RE_IBAL);
        dtodb(bal,data,RE_BAL);
        int Noofmons = diffmon(adoTmp.dat[Ibal_DATE],paydate);
        ltodb(Noofmons,data,RE_MONTH);

        int NoofDays = diffday(adoTmp.dat[Ibal_DATE],paydate);
        ltodb(NoofDays,data,RE_DAY);

        if(NoofDays > atoi(adoTmp.dat[Ibal_TERMS])) {
            lset(data->dat[RE_OVERDUE],"*");
        }
        else {
            lset(data->dat[RE_OVERDUE],"");
        }
        Totalbal += data->val(RE_BAL);
        dtodb(Totalbal, data ,RE_ACC);
        memwrt(data);
    }
    dclose(&adoTmp);

    memend(data);
}

void ViewOutstandingInvoice(char *cust,char *date)
{
    dbopen(&receipt,"receipt.rep",init_receipt);

    CollectReceipt21(&receipt, cust ,"", date);
    if(receipt.t->sumrecord==0) {
        AfxMessageBox("No more outstanding");
    }
	else {
		viewinv(&receipt);
	}
    dclose(&receipt);
}

void CDebtor::OnButton5() 
{
	UpdateData();
	lset(one.dat[1],m_date);
    ViewOutstandingInvoice(df->dat[1],one.dat[1]);
}

void CDebtor::to_st2(DB *rep, DB *df)
{
    lsetblank(rep);
    rset(rep->dat[St_DR], df->dat[RE_TOTAL]);
    lset(rep->dat[St_BILLNO], df->dat[RE_INVOICE]);
    lset(rep->dat[St_NAME],"Sales");
    lset(rep->dat[St_INVNO], df->dat[RE_INVOICE]);
    lset(rep->dat[St_IDATE], df->dat[RE_DATE]);
    lset(rep->dat[St_DATE], df->dat[RE_DATE]);
    BalAmt += rep->val(St_DR)  - rep->val(St_CR);
    dtodb(BalAmt,rep,St_BALANCE);
    memwrt(rep);

    long irec,rec,Srec;
    Srec = interval(&irec, df->dat[RE_INVOICE], &arpaydat, PY_INVOICE);
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec, &arpaydat, PY_INVOICE);
        qd(arpaydat.dat[1], &aripay);
        double total = arpaydat.val(PY_TOTAL);
        lsetblank(rep);
        dtodb(total, rep, St_CR);
        lset(rep->dat[St_NAME],"Payment Received");
        lset(rep->dat[St_BILLNO], aripay.dat[1]);
        lset(rep->dat[St_INVNO], aripay.dat[1]);
        lset(rep->dat[St_IDATE], df->dat[RE_DATE]);
        lset(rep->dat[St_DATE], aripay.dat[IP_DATE]);
        BalAmt += rep->val(St_DR)  - rep->val(St_CR);
        dtodb(BalAmt,rep,St_BALANCE);
        memwrt(rep);
    }
    Srec = interval(&irec, df->dat[RE_INVOICE], &arcrdat, PY_INVOICE);
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec, &arcrdat, PY_INVOICE);
        qd(arcrdat.dat[1], &arcr);
        double total = arcrdat.val(PY_TOTAL);
        lsetblank(rep);
        dtodb(total, rep, St_CR);
        lset(rep->dat[St_NAME],"Credit Note");
        lset(rep->dat[St_BILLNO], arcr.dat[1]);
        lset(rep->dat[St_INVNO], arcr.dat[1]);
        lset(rep->dat[St_IDATE], df->dat[RE_DATE]);
        lset(rep->dat[St_DATE], arcr.dat[IP_DATE]);
        BalAmt += rep->val(St_DR)  - rep->val(St_CR);
        dtodb(BalAmt,rep,St_BALANCE);
        memwrt(rep);
    }
    Srec = interval(&irec, df->dat[RE_INVOICE], &arrcdat, PY_INVOICE);
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec, &arrcdat, PY_INVOICE);
        qd(arrcdat.dat[1], &arrc);
        double total = arrcdat.val(PY_TOTAL);
        lsetblank(rep);
        dtodb(total, rep, St_DR);
        lset(rep->dat[St_NAME],"Cheque Return");
        lset(rep->dat[St_BILLNO], arrc.dat[1]);
        lset(rep->dat[St_INVNO], arrc.dat[1]);
        lset(rep->dat[St_IDATE], df->dat[RE_DATE]);
        lset(rep->dat[St_DATE], arrc.dat[IP_DATE]);
        BalAmt += rep->val(St_DR)  - rep->val(St_CR);
        dtodb(BalAmt,rep,St_BALANCE);
        memwrt(rep);
    }
}

void CDebtor::to_st3(DB *rep, DB *df)
{
    lsetblank(rep);
    rset(rep->dat[St_DR], df->dat[Cst_DEBIT]);
    rset(rep->dat[St_CR], df->dat[Cst_CREDIT]);
    lset(rep->dat[St_BILLNO], df->dat[Cst_REFNO]);
    lset(rep->dat[St_NAME],"Sales");
    lset(rep->dat[St_INVNO], df->dat[Cst_REFNO]);
    lset(rep->dat[St_IDATE], df->dat[Cst_DATE]);
    lset(rep->dat[St_DATE], df->dat[Cst_DATE]);
    BalAmt += rep->val(St_DR)  - rep->val(St_CR);
    dtodb(BalAmt,rep,St_BALANCE);
    memwrt(rep);
}

void GetAgeing(DB *df)
{
    memset(Over_Balance, 0, sizeof(Over_Balance));
    double lbal = 0;
    int nYear2 = year_no(two.dat[2]);
    for(long rec=1; rec<=receipt.t->sumrecord;rec++) {
        readrec(rec,&receipt);
        int j = diffmon(receipt.dat[RE_DATE],two.dat[2]);
        if(j>=12) j = 12;
        double total = df->val(RE_TOTAL);
        Over_Balance[j] += total;
        if(year_no(receipt.dat[RE_DATE]) != nYear2) {
            lbal += total;
        }
	}
    char tmp[20];
    double bf = 0;
    int i,k,j = month_no(two.dat[2]);
    for(i=k=0; i<=11; i++) {
        dtoa(Over_Balance[i],tmp,10,2);
        strcpy(Ob[i],putnum(tmp));
        _PL[10+j] = Ob[i];
        if(k==1) bf += Over_Balance[i];
        j--;
        if(j<1){ j = 12; k = 1;}
    }

    dtoa(bf,tmp,12,2);
    strcpy(Ob[12],putnum(tmp));
    _PL[24] = Ob[12];

    dtoa(lbal,tmp,12,2);
    strcpy(Ob[13],putnum(tmp));
    _PL[23] = Ob[13];
}

void CDebtor::OnStatement() 
{
	UpdateData();
    lset(two.dat[1],m_date);
    firstday(two.dat[1]);
    lset(two.dat[2],m_date);
    lastday(two.dat[2]);

    lset(one.dat[1],two.dat[1]);
    plusday(one.dat[1],-1);

    DB st2;
    rbopen(&st1,"st1.rep",init_st1);
    dbopen(&receipt,"receipt.rep",init_receipt);
    ropen(&st2,"st2.rep");
    cstate.DefaultOrder = FALSE;
    dopen(&cstate,"state.rep");

    CollectReceipt21(&receipt, df->dat[1] ,"", one.dat[1]);

    BalAmt = 0;
    memstart(&st2);
    for(long rec=1; rec<=receipt.t->sumrecord;rec++) {
        readrec(rec,&receipt);
        to_st2(&st2,&receipt);
	}

    CString strSql,strTable = (STCR == St_CR) ? "cstate" : "pstate";
    strSql.Format("select [date],[refno],[debit],[credit] from [%s] where [debtor] = %s and [date] <= '%s' order by [date],type,refno",
        strTable,DbText(df,1), DateStr(two.dat[2]));


    OpenRs  p1(strSql);
    p1.ToDBF(&cstate);
    for(rec=1; rec<=cstate.t->sumrecord;rec++) {
        readrec(rec,&cstate);
        if(strcmp(cstate.dat[1],two.dat[1])<0) continue;
        to_st3(&st2,&cstate);
	}

    memend(&st2);

    stateTitle();
    CollectReceipt21(&receipt, df->dat[1] ,"", two.dat[2]);
    GetAgeing(&receipt);
    stdplan(&two,&st2);

    dclose(&cstate);
    dclose(&receipt);
    dclose(&st2);
    dclose(&st1);
}
