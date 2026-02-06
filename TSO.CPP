// Tso.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Tso.h"
#include "mgrid.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum aa1 {re_REFNO=1,re_DEBTOR,re_DATE,re_AMT,re_PAY,re_CN,re_RC, Re_INVNO};
/////////////////////////////////////////////////////////////////////////////
// CTso dialog

CTso::CTso(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
    : CTio(IDD, pdf, Idx, p, pParent)
{
	//{{AFX_DATA_INIT(CTso)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    bDone = FALSE;
}

CTso::~CTso()
{
//    m_p1 = m_p2 = m_p3 = NULL;
}

void CTso::SetOneTimeScreen()
{
}

void CTso::DoDataExchange(CDataExchange* pDX)
{
    CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTso)
	DDX_Control(pDX, IDC_PROGRESS3, m_progress3);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress2);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_grid);
	//}}AFX_DATA_MAP
}

void CTso::ShowBeginScreen()
{
    if(bSetWild==TRUE) {
        CTio::ShowBeginScreen();
    }
    else {
        ShowInformation();
        ShowFieldNo(1);
        m_ToolBar.HideButton(ID_PRINT,FALSE);
    }

    MGRID g(&m_grid,&report,0,10);
    g.SetTitle("No",600,1);
    g.SetTitle("Code No",750,1);
    g.SetTitle("Name",3000,1);
    g.SetTitle("Tel. No",1050,1);
    g.SetTitle("Current",1000,7);
    g.SetTitle("1 Month",1030,7);
    g.SetTitle("2 Months",1020,7);
    g.SetTitle("3 Months", 950,7);
    g.SetTitle("4+ Months",1010,7);
    g.SetTitle("Balance",1050,7);
}

void CTso::OnSetWild()
{
    setwild(df);
    dbasedate(df->dat[1]);
    firstday(df->dat[1]);
    dbasedate(df->dat[2]);
    lastday(df->dat[2]);
    ridx(1,customer,1);
    lset(df->dat[Ts_CUSTOMER1], customer->dat[1]);
    long last = read_sumindex(1,customer);
    ridx(last, customer, 1);
    lset(df->dat[Ts_CUSTOMER2], customer->dat[1]);
    lset(df->dat[Ts_TYPE], "1");
}

void CTso::ShowInformation()
{
    CTio::ShowInformation();
    CString str = "";
    qd(profile.dat[PF_CURRENCY],&currency);
    str.Format("Convert Currency to %s",currency.dat[2]);
    SetDlgItemText(IDC_CONVERT_TO_HOME_CURRENCY,str);
}

void CTso::ReplaceGridRecord()
{
    long trec = report.t->sumrecord;
    m_grid.SetRows(trec + 1);
    m_grid.SetRow(trec);

    MGRID g(&m_grid,&report,trec);
    g.SetText(&report,0,1,SaT_NAME,SaT_TEL,SaT_M0,SaT_M1,SaT_M2,SaT_M3,SaT_M4,SaT_BALANCE,-1);
}

void CTso::SaveGridRecord()
{
    lset(report.dat[1],customer->dat[1]);
    lset(report.dat[SaT_NAME],customer->dat[3]);
    lset(report.dat[SaT_TEL],customer->dat[TEL0]);
    ComputeOverFourMonthAgeing();
    for(int i=0;i<4;i++) {
        dtodb(Over_Balance[i],&report,SaT_M0+i);
    }
    dtodb(Over_Balance[4],&report,SaT_M4);
    dtodb(Over_Balance[STATEMENT_BALANCE],&report,SaT_BALANCE);
    memwrt(&report);
    ReplaceGridRecord();
}

void CTso::OnCancel()
{
    if(bDone == TRUE) {
        unlink(file);
    }
    CStp = NULL;
    CDialog::OnCancel();
}

void CTso::OnPrintText()
{
    ReplaceRecord();
    if(bDone == TRUE) {
        if(istwin(df)) {
            junction(file,1);
            return;
        }
    }

    int iRet = MessageBox("Do you want to print now ?\n\nTo printer --> click 'Yes'\nTo preview --> click 'No'","Direct to printer",MB_ICONQUESTION | MB_YESNOCANCEL);
    if(iRet==IDCANCEL) return;

    long rec  = dseek(tso.dat[Ts_CUSTOMER1],customer,1,SF);
    long last = dseek(tso.dat[Ts_CUSTOMER2],customer,1,SL);
    pDlg = this;
    m_p1 = &m_progress2;
    m_p2 = &m_progress3;
    m_progress.SetRange32(rec,last);
    memstart(&report);
    pathfile(file,BinTmp,"State");
    FILE *ptr;

    if(iRet==IDYES) {
        ptr = fopen("lpt1","w");
    }
    else {
        ptr = topen(file);
    }
    fprintf(ptr,"2CB");
    CStp = NULL;
    m_grid.SetRedraw(FALSE);
    for (; rec<=last && rec>0; rec++) {
        ridx(rec,customer,1);
        m_progress.SetPos(rec);
//        if(customer->dat[STATEMENT][0]=='F' || customer->dat[STATEMENT][0]=='N') continue;
        if(!wildcmp(customer->dat[STAFF],tso.dat[Ts_STAFF])) {
            if(!wildcmp(customer->dat[AREA],tso.dat[Ts_AREA])) {
                char zeroPrint = customer->dat[STATEMENT][0];
                if(zeroPrint=='T') zeroPrint = 'Y';
                int flag = GetAnalysisBalance(gst);
                if(st1.t->sumrecord==0) continue;                 //2-7-2003
                if(flag>0 || zeroPrint == 'Y'){
                    PrintStatement(ptr);
                    fprintf(ptr,"");
                    SaveGridRecord();
                }
            }
        }
    }
    fclose(ptr);
    memend(&report);
	m_grid.SetRedraw(TRUE);
    m_p1 = m_p2 = NULL;
    CStp  = &m_StatusBar;
    bDone = TRUE;
    backup(df);
    if(iRet==IDNO) {
        junction(file,1);
        CDialog::OnCancel();
        return;
    }
    CDialog::OnOK();
}

int tell_tso(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case Ts_CUSTOMER1:
        if(sq) {
            lset(df->dat[Ts_CUSTOMER2],df->dat[Ts_CUSTOMER1]);
        }
        break;
    }
    return 2;
}

void init_tso(DB *df)
{
    df->e->tell = tell_tso;
}

void CTso::show_tio()
{
    TmpTio[0] = 0;
    long rec = qd(tso.dat[Ts_STAFF],&staff);
    if(rec>0){
        strcpy(TmpTio,"\nSalesman : ");
        strcat(TmpTio,tso.dat[Ts_STAFF]);
        strcat(TmpTio,staff.dat[2]);
    }
    _PL[0]  = TmpTio;
    app->SetStdCompanyLogo();
    lset(logo.dat[Logo_DATE1],df->dat[1]);
    lset(logo.dat[Logo_DATE2],df->dat[2]);
    lset(logo.dat[Logo_SELECT1],TmpTio);
    lset(logo.dat[Logo_RTITLE],SubTitle);
    lset(logo.dat[Logo_USER],user.dat[1]);
    lset(logo.dat[Logo_USERNAME],user.dat[2]);
    writerec(1,&logo);
}

BEGIN_MESSAGE_MAP(CTso, CTio)
	//{{AFX_MSG_MAP(CTso)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTso message handlers
void CTso::FindBF()
{
    CString strTable = (STCR == St_CR) ? "cstate" : "pstate";
    strSql.Format("select sum ([debit] - [credit]) as Balance from [%s] where [customer] = %s and  [date] < '%s' ",
        strTable,DbText(customer,1), tso.dat[1]);

    bal = Double(SqlValue(strSql));
    if(bal==0) return;

    lsetblank(&ciq);
    lset(ciq.dat[CIQ_DATE], tso.dat[1]);
    lset(ciq.dat[CIQ_NAME],"Balance B/F");
    if(bal>0) {
        dtodb(bal, &ciq, CIQ_DEBIT);
    }
    else {
        dtodb(bal * -1, &ciq, CIQ_CREDIT);
    }
    Totalbal += bal;
    dtodb(Totalbal, &ciq, CIQ_BALANCE);
    lset(ciq.dat[CIQ_PERIOD1],tso.dat[1]);
    lset(ciq.dat[CIQ_PERIOD2],tso.dat[2]);
    lset(ciq.dat[CIQ_ACCODE],customer->dat[1]);
    ciq.SqlUpdate(TRUE);
}

void CTso::FindOutstanding()
{
    adoTmp.DefaultOrder = FALSE;
    CString strTable = "arinv_bal";
    if(STCR == St_DR) strTable = "apinv_bal";
    lset(one.dat[1],tso.dat[1]);
    plusday(one.dat[1],-1);

    BOOL flag = adoTmp.OpenStoreProcedure(strTable,
        ADOCG::adVarChar,"debtor", customer->dat[1],
        ADOCG::adDate,"date",one.dat[1],
        0);

	int fld = GetDbFieldNo(&adoTmp,"invno");
	
    while( adoTmp.GetSpData() ) {
        // bal = adoTmp.val(re_AMT) - adoTmp.val(re_PAY) - adoTmp.val(re_CN);
		// OOI
		bal = adoTmp.val(re_AMT) - adoTmp.val(re_PAY) -
			adoTmp.val(re_CN) + adoTmp.val(re_RC);
		//
        bal = Double(bal);
        if(bal) {
            lsetblank(&ciq);
            lset(ciq.dat[CIQ_PERIOD1],tso.dat[1]);
            lset(ciq.dat[CIQ_PERIOD2],tso.dat[2]);
            lset(ciq.dat[CIQ_ACCODE],adoTmp.dat[re_DEBTOR]);
            lset(ciq.dat[CIQ_IDATE],adoTmp.dat[re_DATE]);
            lset(ciq.dat[CIQ_DATE],adoTmp.dat[re_DATE]);
            lset(ciq.dat[CIQ_NAME],"balance");
            lset(ciq.dat[CIQ_REFNO],adoTmp.dat[re_REFNO]);
			if(fld) lset(ciq.dat[CIQ_INVNO],adoTmp.dat[Re_INVNO]);
            if(STCR == St_DR) bal *= -1;
            if(bal>0) {
                dtodb(bal, &ciq, CIQ_DEBIT);
            }
            else {
                dtodb(bal * -1, &ciq, CIQ_CREDIT);
            }
            Totalbal += bal;
            dtodb(Totalbal, &ciq, CIQ_BALANCE);
            ciq.SqlUpdate(TRUE);
        }
    }
    dclose(&adoTmp);
}

void CTso::FindPeriodData()
{
    CString strTable = (STCR == St_CR) ? "cstate" : "pstate";
    strSql.Format("select [date],[refno],[debit],[credit],[particular],[invno] from [%s] where [debtor] = %s and [date] >= '%s' and [date] <= '%s' order by [date],type,refno",
        strTable,DbText(customer,1), tso.dat[1], tso.dat[2]);

    adoTmp.DefaultOrder = FALSE;
    ado(&adoTmp,"",NULL, strSql);
    long last = read_sumindex(1,&adoTmp);
    for(long rec=1; rec<=last; rec++) {
        adoTmp.read(rec);
        lsetblank(&ciq);
        lset(ciq.dat[CIQ_PERIOD1],tso.dat[1]);
        lset(ciq.dat[CIQ_PERIOD2],tso.dat[2]);
        lset(ciq.dat[CIQ_ACCODE],customer->dat[1]);
        lset(ciq.dat[CIQ_IDATE],adoTmp.dat[1]);
        lset(ciq.dat[CIQ_DATE],adoTmp.dat[1]);
        lset(ciq.dat[CIQ_NAME],adoTmp.dat[5]);
        lset(ciq.dat[CIQ_INVNO],adoTmp.dat[6]);
        lset(ciq.dat[CIQ_REFNO],adoTmp.dat[2]);
        lset(inv->dat[1],adoTmp.dat[2]);

        long xrec = inv->Search(inv->dat[1],1,TRUE);
        if(xrec>0) {
            bal = InvoiceRealBalance(inv,ciq.dat[CIQ_REFNO],tso.dat[2]);
            if(bal==0) {
				lset(ciq.dat[CIQ_FLAG],"F");
			}
            else if(bal < inv->val(IV_TOTAL)) {
				lset(ciq.dat[CIQ_FLAG],"P");
			}
		}
        bal = adoTmp.val(3) - adoTmp.val(4);
        if(bal>0) {
            dtodb(bal, &ciq, CIQ_DEBIT);
        }
        else {
            dtodb(bal * -1, &ciq, CIQ_CREDIT);
        }
        Totalbal += bal;
        dtodb(Totalbal, &ciq, CIQ_BALANCE);
        ciq.SqlUpdate(TRUE);
    }

    dclose(&adoTmp);
}

void CTso::FindAgeing()
{
    adoTmp.DefaultOrder = FALSE;
    CString strTable = "arinv_bal";
    if(STCR == St_DR) strTable = "apinv_bal";
    lset(one.dat[1],tso.dat[2]);

    BOOL flag = adoTmp.OpenStoreProcedure(strTable,
        ADOCG::adVarChar,"debtor", customer->dat[1],
        ADOCG::adDate,"date",one.dat[1],
        0);


    double total[20];
    memset(total, 0, sizeof(total));

    int m1 = month_no(tso.dat[2]);

    char szLastYear[20];
    strcpy(szLastYear,tso.dat[2]);
    plusyear(szLastYear,-1);
    strcpy(&szLastYear[4],"1231");

    while( adoTmp.GetSpData() ) {
        //double bal = adoTmp.val(re_AMT) - adoTmp.val(re_PAY) - adoTmp.val(re_CN);
		// OOI
		double bal = adoTmp.val(re_AMT) - adoTmp.val(re_PAY) -
			adoTmp.val(re_CN) + adoTmp.val(re_RC);
        bal = Double(bal);
        if(bal==0) continue;
        if( strcmp(adoTmp.dat[re_DATE], szLastYear) <=0) {
            total[0] += bal;
            continue;
        }
        int m2 = month_no(adoTmp.dat[re_DATE]);
        if(m2 > m1) continue;
        total[m2] += bal;
    }
    dclose(&adoTmp);

    tado(&adoTmp,"ageing");
    lsetblank(&adoTmp);
    lset(adoTmp.dat[1], customer->dat[1]);
    lset(adoTmp.dat[2],tso.dat[1]);
    lset(adoTmp.dat[3],tso.dat[2]);

    for(int i=0; i<=12; i++) {
        dtodb(total[i], &adoTmp, 4+i);
    }
    adoTmp.AddNew();
    dclose(&adoTmp);
}


void CTso::DoStatement()
{

    tado(&ciq, "state");
    memset(Over_Balance,0,sizeof(Over_Balance));

    Totalbal = 0;
    m_progress2.SetRange(1,3);
    if(df->dat[Ts_TYPE][0]=='1') {
       FindOutstanding();
    }
    else {
       FindBF();
    }
    m_progress2.SetPos(1);
    FindPeriodData();
    m_progress2.SetPos(2);
    FindAgeing();
    m_progress2.SetPos(3);

    dclose(&ciq);
}

void CTso::OnPrint()
{
    int iRet = MessageBox("Do you want to print text mode ?","Mode",MB_ICONQUESTION | MB_YESNOCANCEL);
    if(iRet==IDYES){
        OnPrintText();
        return;
    }

    strSql = "truncate table [state]";
    RunSql(strSql);
    strSql = "truncate table [ageing]";
    RunSql(strSql);

    ReplaceRecord();

    long rec  = dseek(tso.dat[Ts_CUSTOMER1],customer,1,SF);
    long last = dseek(tso.dat[Ts_CUSTOMER2],customer,1,SL);
    pDlg = this;
    m_progress.SetRange32(rec,last);
    for (; rec<=last && rec>0; rec++) {
        ridx(rec,customer,1);
        m_progress.SetPos(rec);
        SetDlgItemText(IDC_TYPE,"NAME");
        SetDlgItemText(IDC_MSG, customer->dat[2]);
        strSql.Format("%ld/%ld",rec,last);
        SetDlgItemText(IDC_INVDATE, strSql);
        DoStatement();
    }
    if((STCR == St_CR)) {
        PrintRPT("state_ar.rpt",TRUE);
    }
    else {
        PrintRPT("state_ap.rpt",TRUE);
    }

    CDialog::OnOK();
}

