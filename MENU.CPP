// menu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"

#include "MainFrm.h"
#include "menuDoc.h"
#include "menuView.h"
#include "password.h"
#include "start.h"
#include <locale.h>
#include "initial.h"
#include "ExportFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DB  malaysia_state;
DB  malaysia_city;
DB  postcode;
DB  classification;

// Version SST
char txtSST_StartDate[20];
////
int company_no = 0;
int curweightZeroCount=0;
CString G_company_add1 = "Lot 2929,Jalan Kempas Baru,81200 Johor Bahru,Johor.";
CString G_company_add2 = "TEL:012-7022222,016-7106222 FAX:07-2344959";
CString G_company_add3 = "HQ:PTD 43230 Mukim Senai,Jalan Seelong,81400 Senai,Johor.";
CString CardID="";
CString m_FirstTicket="";
BOOL IsStableWt;
BOOL IsWBZeroWt=FALSE;

DB   scale_qrcode;
DB   accode(IDD_ACCODE),prodtype(IDD_PT),driver(IDD_DRIVER),lorry(IDD_LORRY),vehicle(IDD_LORRY);;
DB   wdp(IDD_DP),wds(IDD_DS),ds(IDD_DS),dp(IDD_DP),category(IDD_CATEGORY);
//DB   two,one,login,user(IDD_USER),profile(IDD_PROFILE);
DB   taxcode(IDD_TAXCODE);
DB	 gst(IDD_GWST),daily;
DB   transp(IDD_TRANSP),ptype(IDD_PTYPE),unit(IDD_UNIT),actype(IDD_ACTYPE);
DB   country(IDD_COUNTRY),grade(IDD_GRADE),spitem(IDD_SPITEM);
DB   po(IDD_PO,4),poitem(IDD_POITEM);
DB   so(IDD_PO,4),soitem(IDD_POITEM);
DB   dt(IDD_DT,4),dtitem(IDD_DTITEM);
DB   arinv_wb(IDD_ARINV_WB,4),aroinv_wb(IDD_AROINV_WB);
DB   apinv_wb(IDD_ARINV_WB,4),apoinv_wb(IDD_APOINV_WB);
DB   ptgroup(IDD_PTGROUP),status(IDD_STATUS);
DB   adj(IDD_ADJ),adjitem(IDD_ADJITEM),reason(IDD_REASON);
DB   prodgrade(IDD_PRODGRADE);
DB   logerr;
//long GwstWeight,ActualWeight;
int FacileFlag = 1;
long dpKey;
//Account System

int STCR,STDR;
double dr_amt, cr_amt,ItemAmt;
DB period(IDD_PERIOD,4,IDD_BROWSE_NORMAL),ido(IDD_IDO),locate(IDD_LOCATE),pt(IDD_PT);
DB period_acc(IDD_PERIOD,4,IDD_BROWSE_NORMAL),acccat;
DB currency(IDD_CURRENCY),receipt(IDD_RECEIPT);
DB *ttt,scheme(IDD_SCHEME);
DB pccode(IDD_PCCODE,2,IDD_BROWSE_NORMAL);
DB debtor(IDD_DEBTOR),creditor(IDD_DEBTOR),supplier(IDD_DEBTOR);//,accode(IDD_SPDEBTOR);
DB pc,sg(IDD_AREA);
DB paytype(IDD_PAYTYPE);
DB staff(IDD_STAFF),wac(IDD_WAC),wacitem(IDD_WACITEM);
DB bcwac(IDD_WAC),bcitem(IDD_WACITEM);
DB area(IDD_AREA);
DB arinv(IDD_ARINV),apinv(IDD_ARINV);
DB itemrec(IDD_STAFF),tmo;
DB aripay(IDD_IPAY),arpaydat(IDD_PAYDAT);
DB apipay(IDD_IPAY),appaydat(IDD_PAYDAT);
DB arrc(IDD_IPAY),arrcdat(IDD_PAYDAT);
DB aprc(IDD_IPAY),aprcdat(IDD_PAYDAT);
DB arcr(IDD_CR),arcrdat(IDD_CRDAT),*customer,*ipay,*paydat,*cr,*crdat,*rc,*rcdat;
DB apcr(IDD_CR),apcrdat(IDD_CRDAT);
DB glcode(IDD_GLCODE,4),glgroup(IDD_GLGROUP,4,IDD_BROWSE_TINY);
DB ctrl_ac(IDD_GLCODE,4);
DB tac(IDD_GLCODE,4);
DB ap_gldat(IDD_GLDAT),ar_gldat(IDD_GLDAT),*gldat,ar_adj_cat(IDD_ADJ_CAT),ap_adj_cat(IDD_ADJ_CAT);
DB ap_oinv(IDD_OINV),ar_oinv(IDD_OINV),*inv,*oinv;
DB prefix(IDD_PREFIX,2,IDD_BROWSE_NORMAL),dc(IDD_AREA,2,IDD_BROWSE_TINY);
DB Gst_type(IDD_T,3),emode(IDD_EMODE);
DB bank(IDD_BANK,2,IDD_BROWSE_LINE);
DB tt(IDD_AREA);
DB acr(IDD_ACR),acrdat(IDD_ACRDAT);
DB cnq(IDD_CNQ),cnqdat(IDD_CNQDAT);
DB tso(IDD_TSO),st,st1,st2,tww,pgcode;
DB gl_init(IDD_GL_INIT,3),glitem;
DB salesman(IDD_SALESMAN);
DB vtype(IDD_VTYPE);//,tng(IDD_TNG);
DB ciq;
DB paymentdate,arriveddate;
double price_unit=1;//1000
HINSTANCE m_hInstMaster;
BOOL ScanApproveFlag=FALSE;
/////////////////////////////////////////////////////////////////////////////
// CMenuApp initialization
DB  scalesub(IDD_SCALESUB),scaledwsub(IDD_SCALESUB);
DB  scale(IDD_SCALE),scaledw(IDD_SCALE);
DB  ptsub(IDD_PTSUB);
DB  pr_new(IDD_PR_NEW),wpr_new(IDD_PR_NEW),dp_rt;
DB  wrt(IDD_WRT);
DB  rt(IDD_RT);
DB  incentive(IDD_INCENTIVE);
DB  dp_do(IDD_DP_DO);
DB  accodehq(IDD_ACCODEHQ);
DB  stkpf(IDD_STKPF);

char *company = "TRADEWIN METALS SDN BHD";
char *tcompany[20] = {
		"A9C9DAAA9A795A0AEC1A9AA9DA2AB9ECB9AA0AECCA6AAA",
};
CMenuApp theApp;
CMenuApp::CMenuApp()
{
	
    IsSqlFile = TRUE;
    GetSystemProviderString();
    GetInternetFolderString();
    ProgramVersionNo = 2;
	m_RecordWtStatus = TRUE;
	bComOutput = TRUE;
}

BOOL CMenuApp::RptResetFiles(const CString m_CrystalReportName,const CString repname,CString &newname)
{
    if(STCR == St_CR) return FALSE;
    if(!repname.CompareNoCase("aripay")) {
        newname = "apipay";
        return TRUE;
    }
    else if(!repname.CompareNoCase("debtor")) {
        newname = "supplier";
        return TRUE;
    }
    else if(!repname.CompareNoCase("arpaydat")) {
        newname = "appaydat";
        return TRUE;
    }
    else if(!repname.CompareNoCase("arpayvar")) {
        newname = "appayvar";
        return TRUE;
    }
    else if(!repname.CompareNoCase("aroinv")) {
        newname = "apoinv";
        return TRUE;
    }
    else if(!repname.CompareNoCase("arinv")) {
        newname = "apinv";
        return TRUE;
    }
    else if(!repname.CompareNoCase("arrc")) {
        newname = "aprc";
        return TRUE;
    }
    else if(!repname.CompareNoCase("arrcdat")) {
        newname = "aprcdat";
        return TRUE;
    }
    else if(!repname.CompareNoCase("arcr")) {
        newname = "apcr";
        return TRUE;
    }
    else if(!repname.CompareNoCase("arcrdat")) {
        newname = "apcrdat";
        return TRUE;
    }
    else if(!repname.CompareNoCase("argldat")) {
        newname = "apgldat";
        return TRUE;
    }
    return FALSE;
}

CString CMenuApp::ConfirmCrystalTableFile(char *FieldName,CString m_CrystalReportName)
{
    if(STCR == St_CR) return FieldName;
    CString repname = FieldName;
    if(!repname.CompareNoCase("aripay")) {
        return "payment";
    }
    else if(!repname.CompareNoCase("debtor")) {
        return "supplier";
    }
    else if(!repname.CompareNoCase("arpaydat")) {
        return "appaydat";
    }
    else if(!repname.CompareNoCase("arpayvar")) {
        return "appayvar";
    }
    else if(!repname.CompareNoCase("aroinv")) {
        return "apoinv";
    }
    else if(!repname.CompareNoCase("arinv")) {
        return "purchase";
    }
    else if(!repname.CompareNoCase("arrc")) {
        return "aprc";
    }
    else if(!repname.CompareNoCase("arrcdat")) {
        return "aprcdat";
    }
    else if(!repname.CompareNoCase("arcr")) {
        return "suppliercr";
    }
    else if(!repname.CompareNoCase("arcrdat")) {
        return "apcrdat";
    }
    else if(!repname.CompareNoCase("argldat")) {
        return "apgldat";
    }
    return repname;
}

void CMenuApp::Profile()
{
    HANDLE handle = new_open("menu.ini");
    char CompanyNo;
    reset(handle);
    qread(handle,(char*)&CompanyNo,sizeof(char));
    if(CompanyNo==0) {
        qread(handle,(char*)&CompanyNo,sizeof(char));
    }
    CloseHandle(handle);
	
    company_no = CompanyNo;
    if(company_no<0 || company_no>15) company_no = 0;
	
    //strcpy(title,tcompany[company_no]);
	//encrypt(title);
	char *company = "TRADEWIN METALS SDN BHD";
	strcpy(title, company);
    SetRegistryKey(_T(title));
    LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	
}

BOOL CMenuApp::Domain()
{

    CStart p1;
//    p1.InitialFlag = 0x0004;
    p1.InitialFlag = 0x0002 | INIT_ADO_USER | INIT_ADO_PROGRAM_PASSWORD;
    p1.DefaultUser = m_defaultUser;
    p1.DoModal();

    if(p1.rec==0) return FALSE;
    UserLogin(p1.UserID);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
        IDR_MAINMENU,
		RUNTIME_CLASS(CMenuDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CMenuView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	HideStaffMenu = 99990;
    m_useDropDownBtnInsteadSrollbar = TRUE;

	return TRUE;
}

void init_itye(DB *df)
{
    df->BROWSE_ID = IDD_BROWSE_LINE;
}

void init_class(DB *df)
{
    df->BROWSE_ID = IDD_BROWSE_CLASS;
}

void CMenuApp::ToDisplay(HANDLE hComm,long weight, long GwstWeight)
{
	// win.ini setting , MinWt = -100 (weight received - vaue)
    static long LastWeight;
    CString str;
	
	if (weight<0){
		IsWBZeroWt = FALSE;
		curweightZeroCount=0;
	}
	else if( weight == 0 ) {
		curweightZeroCount++;
		if(curweightZeroCount==2) {
			IsWBZeroWt = TRUE;
			curweightZeroCount=0;
		}
	}
	else{
		curweightZeroCount=0;
	}
	//if( weight == 0 ) IsWBZeroWt = TRUE;
	
    if( weight <= 0 ) {
        zeroCount++;
        if(zeroCount==10) {
            zeroCount = 0;
          //  runsql("truncate table timefrm");
        }
    }
	
    long var = 30;
    if(labs(LastWeight - weight) < var ) {
        IsStableWt = TRUE;
    }
    else {
        IsStableWt = FALSE;
    }
    LastWeight = weight;
	
    if( weight > 1000 ) {
        if(GwstWeight) {
            weight -= GwstWeight;
        }
        else if(GwstPercent) {
            weight *= double (100-GwstPercent) / 100;
        }
        weight = radix( weight );
    }
    if( weight > 200 ) {
        if( LastWeight != rememberWt) {
            str.Format("INSERT INTO [timefrm] (WT,AW) VALUES (%i,%i) ", weight, LastWeight);
			runsql(str,FALSE,FALSE,cnn__2);   // ccwong 20110208
			//runSql(str);
            rememberWt = LastWeight;
        }
    }
	
    if(weight>=0) {
        str.Format("%c+ %6.6ld kg Gross%c%c%c",2,weight,13,10,3);
    }
    else {
        str.Format("%c- %6.6ld kg Gross%c%c%c",2,weight,13,10,3);
    }
    return;
    DWORD len = 21;
    DWORD NumberOfBytesWritten;
    BOOL bResult = WriteFile(hComm,str,len,&NumberOfBytesWritten,NULL);
    if(!bResult) {
    }
}

//DB logo;
void CMenuApp::SetStdCompanyLogo()
{
    lset(logo.dat[Logo_NAME],title);
	lset(logo.dat[Logo_ADD1],stkpf.dat[PRO_ADD1]);
    lset(logo.dat[Logo_ADD2],stkpf.dat[PRO_ADD2]);
    lset(logo.dat[Logo_ADD3],stkpf.dat[PRO_ADD3]);
/*    lset(logo.dat[Logo_CODE],profile.dat[PF_CODE]);
    lset(logo.dat[Logo_ADD1],profile.dat[PF_ADD1]);
    lset(logo.dat[Logo_ADD2],profile.dat[PF_ADD2]);
    lset(logo.dat[Logo_ADD3],profile.dat[PF_ADD3]);
	lset(logo.dat[Logo_ADD4],profile.dat[PF_ADD4]);*/ 
	lset(logo.dat[Logo_PROGRAM_ID],Pref);
//	lset(logo.dat[Logo_BRANCH],profile.dat[PF_BRANCH]);
	//GST
	lset(&logo,"GST_NO",&stkpf,"GST_NO");
	// Version SST
	int fld = GetDbFieldNo(&profile,"RMCD");
	if (fld>0){
        lset(&logo,"SST_NO",profile.dat[fld]);
	}
	space_str(txtSST_StartDate,8);
	lset(txtSST_StartDate,"20180901");
}

extern void init_gldat(DB *df);
void CMenuApp::OpenAllDataFile()
{  
	if (ado(&profile,"profile")){	//acc profile
#ifdef _DEBUG
		profile.ID=IDD_ACCPF;
#else
		profile.ID = IDD_PROFILE2;
#endif
		readrec(1,&profile);      
		w_zeroWt = profile.lval(PF_MINIMUM);
        _add1 = profile.dat[PF_ADD1];
        _add2 = profile.dat[PF_ADD2];
        _add3 = profile.dat[PF_ADD3];
		//		ado(&logo,"logo");
		//		readrec(1,&logo);
		//		app->SetStdCompanyLogo();
		//		logo.Update();
		//		logo.ReOpen();
		//		readrec(1,&logo);
		//logo.ReOpen();
	}   
	
	if (ado(&stkpf,"stkpf")){	//wb profile
		stkpf.ID = IDD_STKPF;
		if (read_sumindex(0,&stkpf)<=0){
			lset(stkpf.dat[PRO_CODE],"1");
			stkpf.Update(TRUE);
			stkpf.ReOpen();
		}
		readrec(1,&stkpf);

		if (ado(&logo,"logo")){
			if (read_sumindex(0,&logo)<=0){
				lset(logo.dat[1],stkpf.dat[PRO_CODE]);
				logo.Update(TRUE);
				logo.ReOpen();
			}
			readrec(1,&logo);
            fetch(&logo, &stkpf);
			SetStdCompanyLogo();
			logo.Update();
		}
	}

    ado(&malaysia_state,"malaysia_state");
    ado(&malaysia_city,"malaysia_city");
    ado(&postcode,"postcode");
    ado(&classification, "classification");
    
    //tado(&scale_qrcode, "scale_qrcode");
	tado(&scaledwsub,"scaledwsub",init_scalesub);
    tado(&scaledw,"scaledw",init_scale);
    tado(&scalesub,"scalesub",init_scalesub);
    tado(&scale,"scale",init_scale);
    tado(&ptsub,"ptsub");
	tado(&dp_rt,"dp_rt");
    tado(&wpr_new,"wpr_new",init_wpr);
	tado(&pr_new,"pr_new",init_pr);
    tado(&wrt,"wrt",init_wdp);
	tado(&rt,"rt",init_dp);    
    tado(&incentive,"incentive");
    tado(&dp_do,"dp_do",init_dp_do);
    ado(&accodehq,"accodehq");
	ado(&glgroup,"glgroup",init_glgroup);
	ado(&glcode,"glcode",init_glcode);
	tado(&logerr,"logerr");
	
    tado(&dp,"dp",init_dp);
    tado(&ds,"ds",init_ds);
//	ado(&tng,"tng",init_Tng);
	ado(&salesman,"salesman");
	ado(&vtype,"vtype");
	ado(&prodgrade,"prodgrade");
    ado(&wdp,"wdp",init_wdp);
    ado(&wds,"wds",init_wds);
    ado(&accode,"accode",init_accodes);
    ado(&prodtype,"pt",init_ptentry);
    ado(&ptgroup,"ptgroup");
    ado(&driver,"ic");
    ado(&lorry,"lorry",init_lorry);
	ado(&vehicle,"vehicle",init_lorry);
    ado(&transp,"transp",init_transp);
    ado(&category,"category",init_category);    
    ado(&actype,"actype");
    if (ado(&gst,"gst")){
		readrec(1,&gst);
	}    
    ado(&country,"country",init_country);
    ado(&grade,"grade",init_grade);
    ado(&ptype,"ptype",init_ptype);

    tado(&spitem,"spitem");
	ado(&status,"status");
	 
    ado(&po,"po",init_po);
    tado(&poitem,"poitem",init_poitem);
    ado(&so,"so",init_po);
    tado(&soitem,"soitem",init_poitem);
    tado(&dt,"dt",init_dt);
    tado(&dtitem,"dtitem",init_dtitem);

    tado(&arinv_wb,"arinv_wb",init_arinv_wb);
    tado(&aroinv_wb,"aroinv_wb",init_aroinv_wb);
    tado(&apinv_wb,"apinv_wb",init_arinv_wb);
    tado(&apoinv_wb,"apoinv_wb",init_aroinv_wb);
	//stock module
	tado(&adj,"adj",init_adj);
    tado(&adjitem,"adjitem",init_adjitem);
    ado(&reason,"reason");

	//acc module
	BOOL NewToday = FALSE;
    long mrec = login.mrec;
    if(mrec>1) {
        backup(&login);
        readrec(mrec-1,&login);
        if(strcmp(login.dat[1],login.bak[1])<0) {
            NewToday = TRUE;
        }
        restore(&login);
        login.mrec = mrec;
    }
	tado(&arriveddate,"ARRIVEDDATE");
    ado(&paytype,"paytype",init_itye);
    ado(&staff,"staff");
    ado(&currency,"currency");
    ado(&area,"area");
    ado(&pccode,"pccode");
    ado(&pc,"pccode");	
    tado(&arinv,"arinv",init_arinv);	
    tado(&apinv,"purchase",init_purchase);	
    ado(&period,"period");
    ado(&period_acc,"period_acc");
    tado(&aripay,"aripay",init_ipay);
    tado(&arpaydat,"arpaydat");
    tado(&arrc,"arrc",init_ipay);
    tado(&arrcdat,"arrcdat");	
    tado(&arcr,"arcr",init_cr);
    tado(&arcrdat,"arcrdat");	
    tado(&apipay,"payment",init_payment);
    tado(&appaydat,"appaydat");
    tado(&aprc,"aprc",init_ipay);
    tado(&aprcdat,"aprcdat");
    tado(&apcr,"suppliercr",init_apcr);
    tado(&apcrdat,"apcrdat");
    tado(&tmpinv,"arinv",init_arinv);
    tado(&scheme,"scheme");
    tado(&ar_gldat,"argldat",init_gldat);
    tado(&ap_gldat,"apgldat",init_gldat);
    ado(&ctrl_ac,"ctrl_ac",init_glcode);
    ado(&tac,"tac",init_glcode);
    tado(&ar_oinv,"aroinv",init_oinv);
    tado(&ap_oinv,"apoinv",init_oinv);	
    ado(&tt,"tt");
    ado(&sg,"sg");
    ado(&dc,"dc");	
    ado(&ar_adj_cat,"adj_cat");
    ado(&bank,"bank",init_bank);	
    ado(&prefix,"prefix",init_prefix);
    tado(&wac,"wac",init_wac);
    tado(&wacitem,"wacitem",init_wacitem);
    ado(&emode,"emode",init_emode);
    tado(&pgcode,"pgcode");
    ado(&unit,"unit");	
    OpenARData();
    ado(&creditor,"creditor",init_creditor);
    ado(&supplier,"supplier",init_creditor);
    ado(&debtor,"debtor",init_debtor);
    ado(&Gst_type,"t",init_Gst_type);
	ado(&taxcode,"taxcode");
	return;
	
    tado(&ido,"ido");
    ado(&pt,"pt",init_pt);
    ado(&locate,"locate");
    ado(&acccat,"acccat");
}

void CMenuApp::SetDBRelations()
{
	prodtype.sub = &ptsub;
	ds.fieldAlias[DP_LORRY] = wds.fieldAlias[DP_LORRY] = 
	dp.fieldAlias[DP_LORRY] = wdp.fieldAlias[DP_LORRY] = 
	pr_new.fieldAlias[DP_LORRY] = scale.fieldAlias[DP_LORRY] = scaledw.fieldAlias[DP_LORRY] = "LORRY";
	lorry.fieldAlias[LO_DRIVER2] = "ACCODE";

	ds.m_PrintOneFormOnly = wds.m_PrintOneFormOnly = TRUE;
    pr_new.m_PrintOneFormOnly = wpr_new.m_PrintOneFormOnly = TRUE;
    dp.m_PrintOneFormOnly = wdp.m_PrintOneFormOnly = TRUE;
	scale.m_PrintOneFormOnly = scaledw.m_PrintOneFormOnly = TRUE;
	arinv_wb.m_PrintOneFormOnly = apinv_wb.m_PrintOneFormOnly = TRUE;

#ifdef _DEBUG
    ds.m_PreviewCrystalReport = TRUE;
    pr_new.m_PreviewCrystalReport = TRUE;
    dp.m_PreviewCrystalReport = TRUE;
	scale.m_PreviewCrystalReport = TRUE;
	scaledw.m_PreviewCrystalReport = TRUE;
#else
    ds.m_PreviewCrystalReport = FALSE;
    pr_new.m_PreviewCrystalReport = FALSE;
    dp.m_PreviewCrystalReport = FALSE;
	//20230703
	// scale.m_PreviewCrystalReport = FALSE;
	// scaledw.m_PreviewCrystalReport = FALSE;
	scale.m_PreviewCrystalReport = TRUE;
	scaledw.m_PreviewCrystalReport = TRUE;
	//END
#endif //_DEBUG
}

void CMenuApp::SetDBNodeNo()
{
	//if program no is even number reportflag must pass 1 for not allow create new record, node number can pass program no for user control
	prodtype.NodeNo = 411;
	prodtype.e->ReportFlag  = 1;
	lorry.NodeNo = 412;
	unit.NodeNo = 414;
	ptype.NodeNo = 416;
	ptype.e->ReportFlag = 1;
	accode.NodeNo = 421;
	accode.e->ReportFlag = 1;
	driver.NodeNo = 422;
	transp.NodeNo = 423;
	//transp.e->ReportFlag = 423;
	
}

void CMenuApp::OnLogAlram4DriveThrough(long cMaxWt1, const char *cMaxDate1, const char *cMaxTime1, const char *WeighbridgeNo)
{
    CString s,str;
	
    s.Format("Drive through without weighing\r\n\r\nA vehicle about %ld kg at %s %s on WB %s",
        cMaxWt1, cMaxDate1, cMaxTime1, WeighbridgeNo);
    str.Format("insert into [logAlarm] ([violation],[computer],[user],[vname1]) values (%i,'%s','%s','%s') ",
        0, m_strComputerName,
        user.dat[1], s);
	runsql(str,FALSE,FALSE,cnn__2);   // ccwong 20110208
	//runSql(str);
	
    int key = SqlInt("select max(pkkey) from logAlarm");
    str.Format("Insert into [logAlarmData] ([id],[date],wt,aw) select %i,[date],wt,aw from timefrm", key);
	
	RunSql(str,FALSE,FALSE,cnn__2);   // ccwong 20110208
	//runSql(str);
    m_pMainWnd->SendMessage(WM_COMMAND, ID00099, (LPARAM)0);
	runsql("truncate table timefrm",FALSE,FALSE,cnn__2);   // ccwong 20110208
	//runsql("truncate table timefrm");
}

void CMenuApp::OnLogAlram4OverMaxWt(long cMaxWt1,long TicketWt,const char *cMaxDate1, const char *cMaxTime1, const char *WeighbridgeNo)
{
    long var = cMaxWt1 - TicketWt;
    if( var >= w_maxVar) {
		CString s,str;
		
		s.Format("Over max weight variance\r\n\r\nA vehicle(%s) about %ld kg at %s %s on WB %s but ticket wt only: %ld kg",
			TicketVehicleNo,cMaxWt1, cMaxDate1, cMaxTime1, WeighbridgeNo, TicketWt);
		str.Format("insert into [logAlarm] ([violation],[computer],[user],[vname1],[vehicle],[ticketwt]) values (%i,'%s','%s','%s','%s',%i) ",
			1, app->m_strComputerName,
			user.dat[1], s,TicketVehicleNo, TicketWt);
		runsql(str,FALSE,FALSE,cnn__2);   // ccwong 20110208
		//runSql(str);
		int key = SqlInt("select max(pkkey) from logAlarm");
		str.Format("Insert into [logAlarmData] ([id],[date],wt,aw) select %i,[date],wt,aw from timefrm",
			key);
		RunSql(str,FALSE,FALSE,cnn__2);   // ccwong 20110208
		//runSql(str);
		m_pMainWnd->SendMessage(WM_COMMAND, ID00099, (LPARAM)0);
    }
	runsql("truncate table timefrm",FALSE,FALSE,cnn__2);   // ccwong 20110208
	//   runsql("truncate table timefrm");
}
