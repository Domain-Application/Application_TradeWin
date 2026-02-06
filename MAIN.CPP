#include "stdafx.h"
#include "menu.h"
#include "Staff.h"
#include "ds.h"
#include <wininet.h>
#include "profile.h"
#include "AskPassword.h"
#include "AdminDb.h"
#include "state.h"
#include "DpDo.h"
#include "Pr.h"


enum menu {M_WEIGHING=1,M_VIEW,M_INVOICE,M_CTRL,M_REPORT,M_OTHER,M_STOCK,M_BACKUP};
enum kkk {M_C1 = M_CTRL *10+1,M_C2, M_C3,M_C4};
//enum kkk2 {AR = M_ACC *10+1,AP};

long m[15];

void PublicTicketData(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
	
    CPr p1(df->ID,df,df->f[1].idx & 0x01);
	//p1.m_WinPrint  = FALSE;
    p1.LastField = PR_USER2;
	
	
    p1.DoModal();
}	
void TicketData(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
	if(!stricmp(df->name,"wds") || !stricmp(df->name,"ds")){
		tado(&paymentdate,"paymentdate");
	}else{
		tado(&paymentdate,"paymentdated");
	}
	

    CDs p1(df->ID,df,df->f[1].idx & 0x01);
	//p1.m_WinPrint  = FALSE;
    p1.LastField = DP_OTHQTY;

	
    p1.DoModal();
	
	CString strSql,strWhere;
	strSql.Format("SELECT * FROM PT ");
	prodtype.m_strCmdText = strSql;
	prodtype.ReOpen();	
	dclose(&paymentdate);
}

void ViewData(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }

	if(!stricmp(df->name,"wds") || !stricmp(df->name,"ds")){
		tado(&paymentdate,"paymentdate");
	}else{
		tado(&paymentdate,"paymentdated");
	}

    CDs p1(IDD_WDS,df,df->f[1].idx & 0x01);
	//p1.m_WinPrint  = FALSE;
    p1.LastField = DP_OTHQTY;
	
	
    p1.DoModal();
	
	CString strSql,strWhere;
	strSql.Format("SELECT * FROM PT ");
	prodtype.m_strCmdText = strSql;
	prodtype.ReOpen();	
	dclose(&paymentdate);
}

void ProfileData(DB *df)
{
    CProfile p1;

	p1.m_e1 = profile.dat[1];
	p1.m_e2 = profile.dat[2];
	p1.m_e3 = profile.dat[3];
    p1.m_e4 = profile.dat[4];
    p1.m_e5 = profile.dat[5];
    p1.m_e6 = profile.dat[6];
	    int flag = p1.DoModal();
	if(flag==IDOK) {
		lset(profile.dat[1],p1.m_e1);
		lset(profile.dat[2],p1.m_e2);
		lset(profile.dat[3],p1.m_e3);
        lset(profile.dat[4],p1.m_e4);
        lset(profile.dat[5],p1.m_e5);
        lset(profile.dat[6],p1.m_e6);
		fastwrt(1,&profile);
	}
}

void AdminProfileDb(DB *df)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    CAskPassword p2;
	
    if(p2.DoModal() != IDOK) return;
	
	
    CStaff p1(df->ID,df,df->f[1].idx & 0x01);
    p1.OneRecordOnly = TRUE;
    p1.DoModal();

}

void AdminOneRecord(DB *df)
{
	if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }

	CStaff p1(df->ID,df,df->f[1].idx & 0x01);
    p1.OneRecordOnly = TRUE;
    p1.DoModal();
}

void Despatch_DO(DB *df)
{
	if(df->ID==0) {
        AfxMessageBox("Database not open");
        return;
    }
    	
    CDpDo p1(df->ID,df,df->f[1].idx & 0x01);    
    p1.DoModal();
}

void DoUploadProcess()
{
	char szPath[1024],szFile[1024];
	GetCurrentDirectory(1000,szPath);
	strcat(szPath,"\\WINCUT");
	strcpy(szFile,szPath);
	strcat(szFile,"\\wincut.exe");
	ShellExecute(NULL,NULL,szFile,NULL,szPath,SW_SHOW);
	PeriodCrystalRpt("Upload.rpt","Upload Report");
	return;
}

int ProgramMenuNo,level;
int CMenuApp::SubProgram(int key)
{
	int ShiftKeyStatus = GetKeyState(VK_SHIFT);
    int CtrlKeyStatus  = GetKeyState(VK_CONTROL);
    int AltKeyStatus   = GetKeyState(VK_MENU);
    bTaxInclude = FALSE;
    if(profile.dat[PF_TAX][0]=='Y' || profile.dat[PF_TAX][0]=='T'){
        bTaxInclude = TRUE;
    }

    ProgramMenuNo = 0;
    switch(key) {
    case M_WEIGHING*10+1:
        Purchase_Flag = TRUE;
        WeighingProcess(&ds,&wds);
        break;
    case M_WEIGHING*10+2:
        Purchase_Flag = FALSE;
        Weighing_Despatch(&dp,&wdp);
        break;
    case M_WEIGHING*10+3:
        Purchase_Flag = TRUE;
        //ViewData(&wds);
		TicketData(&wds);
        break;
    case M_WEIGHING*10+4:
        Purchase_Flag = FALSE;
        TicketData(&wdp);
        break;
	case M_WEIGHING*10+5:		
        PublicWeighingProcess(&pr_new,&wpr_new);		
        break;
	case M_WEIGHING*10+6:
        Purchase_Flag = TRUE;
        AdminDbScale(&scale,&scalesub);
        break;
	case M_WEIGHING*10+7:
        Purchase_Flag = FALSE;
        AdminDbScale(&scaledw,&scaledwsub);
        break;
	case M_WEIGHING*10+8:
        Purchase_Flag = FALSE;
        Weighing_Despatch(&rt,&wrt);
        break;
	case M_WEIGHING*10+9:
        Purchase_Flag = FALSE;
        TicketData(&wrt);
        break;	
    case M_VIEW*10+1:
		Purchase_Flag = TRUE;
		TicketData(&ds);
        break;
    case M_VIEW*10+2:
        Purchase_Flag = FALSE;
        TicketData(&dp);
        break;
    case M_VIEW*10+3:
        DirectDeliveryTicket();
        break;
	case M_VIEW*10+4:
        DirectDeliveryPricingUpdate();
        break;
	case M_VIEW*10+5:
		dpKey = 0;
        Despatch_DO(&dp_do);
		dpKey = 0;
        break;
	case M_VIEW*10+6:
        PublicTicketData(&pr_new);
        break;
	case M_VIEW*10+7:
        Purchase_Flag = FALSE;
        TicketData(&rt);
        break;
    case M_INVOICE*10+1:
        PurchaseInvoiceWb();
        break;
    case M_INVOICE*10+2:
        SalesInvoiceWb();
        break;
	case M_INVOICE*10+3:
        ExportPurchaseInvoiceTransactions();
        break;
	case M_INVOICE*10+4:
        ExportInvoiceTransactions();
        break;
    case M_C1*10+1:
		AdminDbPtentry(&prodtype,&ptsub);
        break;
	case M_C1*10+2:
        AdminDb(&lorry);
        break;
    case M_C1*10+3:
        AdminDb(&category);
        break;
    case M_C1*10+4:
        AdminDb(&unit);
        break;
    case M_C1*10+5:
		prodgrade.Run();       
        break;
    case M_C1*10+6:
        //AdminDb(&ptype);
		AdminDbPTYPE(&ptype);
        break;
    case M_C1*10+7:
        AdminDb(&country);
        break;
    case M_C1*10+8:
        AdminDb(&ptgroup);
        break;
    case M_C1*10+9:
        AdminDb(&gst);
        break;
    case M_C2 * 10+1:
        AccodeMaintenance();
        break;
    case M_C2*10+2:
        AdminDb(&driver);
        break;
    case M_C2*10+3:
        AdminDb(&transp);
        break;
    case M_C2*10+4:
		taxcode.Run();	       
        break;
	case M_C2*10+5:
	//	incentive.Run();
		AdminDb(&grade);	
		break;
	case M_C2*10+6:
		AdminDb(&status);
		break;
	case M_C2*10+7:
		salesman.Run();
		break;
	case M_C2*10+8:
	//	tng.Run();
		vtype.Run();
		break;
	case M_C2*10+9:
	//	accodehq.Run();
		break;
    case M_C3*10+1:
        PurchaseOrderData();
        break;
    case M_C3*10+2:
        SalesOrderData();
        break;
	case M_C4*10+1:
        OpenARData();
        AdminAccodeDb(&debtor);
        break;
    case M_C4*10+2:
        OpenAPData();
        AdminAccodeDb(&creditor);
        break;
    case M_C4*10+3:
        AdminDb(&paytype);
        break;
    case M_C4*10+4:
        AdminDb(&prefix);
        break;
    case M_C4*10+5:
        AdminDb(&area);
        break;
    case M_C4*10+6:
        AdminDb(&tt);
        break;
    case M_C4*10+7:
        AdminDb(&bank);
        break;
	case M_C4*10+8:
        AdminDb(&glcode);
        break;
	case M_C4*10+9:
        AdminDb(&glgroup);
        break;
    case M_REPORT:
        TableContainer("rptctn");
        break;
    case M_REPORT*10+1:   //51
      FileChangeReport(&ds);
        break;
    case M_REPORT*10+2:   //52
      FileChangeReport(&dp);
        break;
    case M_REPORT*10+3:   //53
	  VarianceDetailReport(&ds);
        break;
    case M_REPORT*10+4:   //54
	  VarianceDetailReport(&dp);
        break;
    case M_OTHER*10+1:
        break;
    case M_OTHER*10+2:
        //ProfileData(&profile);
		AdminProfileDb(&stkpf);
        break;
    case M_OTHER*10+3:
		SetUser();
        break;
    case M_OTHER*10+4:
        CleanDatabase();
        break;    
    case M_OTHER*10+5:
        return OnSystemUpgrade("HENGHUP.EXE");
        break;
	case M_OTHER*10+6:
        ViewViolation();
        break;		
	case M_STOCK*10+1:
        AdjustmentEntry();
        break;		
	case M_STOCK*10+2:
        AdminDb(&reason);
        break;		
/*
	case AR*10+1:
        SalesInvoice();
        break;		
    case AR*10+2:
        Payment_Entry();
        break;
    case AR*10+3:
        CreditNote();
        break;
    case AR*10+4:
        AR_ChequeReturn();
        break;
    case AR*10+5:
        ViewAR_ChequeReturn();
        break;
    case AR*10+6:
        OpenARData();
        CustomerInquired();
        break;
    case AR*10+7:
        OpenARData();
        auditor_statement();
        break;
    case AR*10+8:
        OpenARData();
        NewTableContainer("arctn");
        break;
    case AR*10+9:
		ExportInvoiceTransactions();        
        break;
    case AP*10+1:
        PurchaseInvoice();
        break;
    case AP*10+2:
        Receipt_Entry();
        break;
    case AP*10+3:
        SupplierCreditNote();
        break;
    case AP*10+4:
        AP_ChequeReturn();
        break;
    case AP*10+5:
        ViewAP_ChequeReturn();
        break;
    case AP*10+6:
        OpenAPData();
        CustomerInquired();
        break;
    case AP*10+7:
        OpenAPData();
        auditor_statement();
        break;
    case AP*10+8:
        OpenAPData();
        NewTableContainer("apctn");
        break;		
	case AP*10+9:
		ExportPurchaseInvoiceTransactions();        
        break;
		*/
		/*
	case M_BACKUP:
        BackupProcess();
        break;
		*/
    default:
         return 0;
    }
    return 1;
}

void PrintLogo(FILE *ptr,char *tl)
{
}
