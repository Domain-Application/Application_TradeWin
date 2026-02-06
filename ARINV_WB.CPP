// arinv_wb.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "arinv_wb.h"
#include "db_ado.h"
#include "openrs.h"
#include "ARINV_UNLOCK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DB invwb(IDD_ACCODE,6),allTicket;
/////////////////////////////////////////////////////////////////////////////
// CArinvWb dialog

CArinvWb::CArinvWb(UINT IDD,DB *pdf, int Idx,CWnd* pParent /*=NULL*/)
	: CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CArinvWb)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    ExtraButton1 = IDC_BUTTON1;
}


void CArinvWb::DoDataExchange(CDataExchange* pDX)
{
	CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CArinvWb)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArinvWb, CParent)
    //{{AFX_MSG_MAP(CArinvWb)
	ON_BN_CLICKED(IDC_BUTTON1, OnCreateNewInvoice)
	ON_BN_CLICKED(IDC_Unlock, OnUnlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CArinvWb::IsCannotEdit()
{
    char invno[20];
    strcpy(invno,arinv.dat[1]);
    lset(invno,df->dat[1]);
	long xrec =0;
	if(!stricmp(df->name,"arinv_wb")) {
		xrec = arinv.Search(invno,1); //acc\ar\invoice
	}
	else
	{
		xrec = apinv.Search(invno,1); //acc\ap\invoice
	}
    
	if(xrec>0) {
		waitmsg("This invoice already post to ACC\nModification Not Allow");
		return TRUE;
    }
	/*
	if(df->dat[IV_STATUS][0]=='1'){
        waitmsg("Already posted to AR! can not be modified");
        if(atoi(user.dat[UR_LEVEL])<9) {
            return TRUE;
        }
        else{
            return FALSE;
        }
    }*/
    return FALSE;
}

void CArinvWb::ShowInformation()
{   
    ReFreshSubFile();
	
	CString str;
	char txt[100];
	if(!stricmp(sub->name,"apoinv_wb")) {
	//str.Format("SELECT (SUM(AMOUNT)+sum(SCALE_AMT))AS NET FROM [%s] WHERE INVOICE='%s' GROUP BY INVOICE",sub->name,df->dat[IVWB_INVOICE]);
		str.Format("SELECT (SUM(AMOUNT)) AS NET FROM [%s] WHERE INVOICE='%s' GROUP BY INVOICE",sub->name,df->dat[IVWB_INVOICE]);
	}
    else{
		str.Format("SELECT (SUM(AMOUNT)) AS NET FROM [%s] WHERE INVOICE='%s' GROUP BY INVOICE",sub->name,df->dat[IVWB_INVOICE]);
	}
	double net = SqlValue(str);
	dtoa(net,txt,9,2);
	dtodb(net, df, IVWB_AMOUNT);
	//SetDlgItemText(IDC_NETTOTAL,txt);
    CParent::ShowInformation();    
}

void CArinvWb::ShowInitFrame()
{
    fg.Initial(&m_grid,sub,2);
	fg.SetTitle("No",500,1);
    fg.SetTitle("Ticket No",1200,1);
    fg.SetTitle("Date",1000,1);
    fg.SetTitle("Vehicle No",1000,1);
//    fg.SetTitle("P/O No",1300,7);
    fg.SetTitle("D/O No",1400,1);
    fg.SetTitle("Qty(Kg)",1000,7);
    fg.SetTitle("Amount",1200,7);

/*
	if(!stricmp(sub->name,"apoinv_wb")) {
	fg.SetTitle("No",500,1);
    fg.SetTitle("Ticket No",900,1);
    fg.SetTitle("Date",1000,1);
    fg.SetTitle("Vehicle No",1000,1);
    fg.SetTitle("P/O No",1600,7);
    fg.SetTitle("D/O No",1400,7);
    fg.SetTitle("Qty(Kg)",1000,7);
    fg.SetTitle("Amount",1200,7);
	fg.SetTitle("Scale No",1000,7);
	fg.SetTitle("Scale Amt",1200,7);
	}
    else{
	fg.SetTitle("No",500,1);
    fg.SetTitle("Ticket No",900,1);
    fg.SetTitle("Date",1000,1);
    fg.SetTitle("Vehicle No",1000,1);
    fg.SetTitle("P/O No",1600,7);
    fg.SetTitle("D/O No",1400,7);
    fg.SetTitle("Qty(Kg)",1000,7);
    fg.SetTitle("Amount",1200,7);
	}
	*/
}

void CArinvWb::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
	fg.show(rec);

    long Srec = qd(sub->dat[VIWB_DO],&allTicket);
    fg.SetText(sub,0,VIWB_DO,-1);
   // fg.SetText(&allTicket,2,3,4,5,-1);
	fg.SetText(&allTicket,2,3,5,-1);
    fg.SetText(sub,VIWB_QTY);
    fg.SetText(sub,VIWB_AMOUNT);		
}

void CArinvWb::SetAttribute(int flag,int fld)
{
    if(flag) {
		if(atoi(user.dat[UR_LEVEL])>=99){
	        GetDlgItem(IDC_Unlock)->ShowWindow(SW_SHOW);
		}
		else{
			GetDlgItem(IDC_Unlock)->ShowWindow(SW_HIDE);
		}
    }
    else {
        GetDlgItem(IDC_Unlock)->ShowWindow(SW_HIDE);
    }
    CParent::SetAttribute(flag,fld);
}

int AdminDb_arinv_wb(DB *df,int flag)
{
    SalesInvoiceWb();
    return 1;
}

int tell_aroinv_wb(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    switch(fld) {
    case VIWB_DO:
        id = DO;
        break;	
	default:
		break;
    }
    return 2;	
}

int start_arinv_wb(DB *df)
{
    df->FldDefault(2);
	//lset(df->dat[IVWB_STATUS],"U");
	lset(df->dat[IVWB_STATUS],"0");
	lset(df->dat[IVWB_CURRENCY],"RM");
    return 1;
}

int arinv_wb_initial(DB *df,int *fld)                //initial setting
{
    if(atoi(user.dat[UR_LEVEL])<9) {
        fld[0] = IVWB_DATE;
        fld[2] = 0;
    }
    return 1;
}

int tell_arinv_wb(UINT nChar,int &id,int &fld,DB *df,int sq)
{
	switch(fld){
		case IVWB_STATUS:
        if(atoi(user.dat[UR_LEVEL])<9) {
            id = DO;
        }
        break;
	case IVWB_ACCODE:
		if(sq){
			qd(df->dat[fld],&accode);
			/*
			if(!strncmp(accode.dat[ACCODE_PREFIX],"CSH",3)){
				lset(df->dat[IVWB_TERMS],"CASH");
			}else if(!strncmp(accode.dat[ACCODE_PREFIX],"CHQ",3)){
				lset(df->dat[IVWB_TERMS],"CHEQUE");
			}else{
				lset(df->dat[IVWB_TERMS],accode.dat[ACCODE_TERMS]);
			}*/

            FindTaxCode(df,Purchase_Flag);
		}
		break;
    case IVWB_TAXCODE:
		if(sq){
            FindPercen(df);
			SetSubTotal(df);
		}
		break;
		//Ver 1.5
	case IVWB_TAXAMT:
		if (isblank(df->dat[fld])){
			FindPercen(df);
			SetSubTotal(df);
		}
		if (UserLevel()<9){
			id = DO;
		}		
		break;
	}
    SetSubTotal(df);
	return 2;
}

int delete_arinv_wb(long irec,DB *df,int IdxItem)
{
    char invno[20];
    //strcpy(invno,arinv.dat[1]);
    lset(invno,df->dat[1]);
	long xrec =0;
	if(!stricmp(df->name,"apinv_wb")) {
		xrec = apinv.Search(invno,1); //acc\ap\invoice			
	}
	else{
		xrec = arinv.Search(invno,1); //acc\ar\invoice
	}    
	if(xrec>0) {
        CString str;
        str.Format("Error Message\n\nCannot delete Invoice No: %s\nbecause this invoice already Post to ACC.\n",df->dat[1]);
        AfxMessageBox(str);
		return 0;
    }
    else {
        return Delete(irec,df,IdxItem);
    }
}

void init_arinv_wb(DB *df)
{
    df->e->AdminDb = AdminDb_arinv_wb;
    df->e->start   = start_arinv_wb;
    df->e->initial = arinv_wb_initial;
	df->e->tell	   = tell_arinv_wb;
	df->e->Delete  = delete_arinv_wb;
}

void init_aroinv_wb(DB *df)
{
    df->e->tell = tell_aroinv_wb;
}

void SalesInvoiceWb()
{
	tado(&allTicket,"all_dp");
    Purchase_Flag = FALSE;

    CArinvWb p1(IDD_ARINV_WB,&arinv_wb,1);
    p1.sub       = &aroinv_wb;
    p1.IDD_SUB   = IDD_AROINV_WB;
//    p1.Status = 0x00ff;	//Not Allow to delete
	p1.m_WinPrint = TRUE;
    p1.DoModal();
	dclose(&allTicket);

}

void PurchaseInvoiceWb()
{
	tado(&allTicket,"all_ds");
    Purchase_Flag = TRUE;

    CArinvWb p1(IDD_APINV_WB,&apinv_wb,1);
    p1.sub       = &apoinv_wb;
    p1.IDD_SUB   = IDD_APOINV_WB;
//    p1.Status = 0x00ff; //Not Allow to delete
	p1.m_WinPrint = TRUE;
    p1.DoModal();
//	SetAttribute(TRUE,TRUE);
	dclose(&allTicket);
}

void CArinvWb::OnCreateNewInvoice() 
{
    CString strTable = (Purchase_Flag) ? "DSINV_NOPO" : "DPINV_NOSO";

    if(!strcmp(df->dat[IVWB_ACCODE],stkpf.dat[PRO_ACCODE])){
		strSql.Format("select * from [%s] where ([ptype]='01' OR [ptype]='CASH' ) and [date] <= %s and [DO] IS NULL",
			strTable, DbText(df,IVWB_DATE,df->dat[IVWB_DATE]));
	}else{
		strSql.Format("select * from [%s] where [accode]='%s' and [date] <= %s and [DO] IS NULL",
			strTable,df->dat[IVWB_ACCODE], DbText(df,IVWB_DATE,df->dat[IVWB_DATE]));
	}
	
	for(;;) {
        ado(&invwb,"",NULL,strSql);
        long irec = viewidx(&invwb);
        if(irec==0){
            dclose(&invwb);
            break;
        }
        if(SaveMainInformation()==FALSE){
            dclose(&invwb);
            break;
        }
        lset(sub->dat[1], df->dat[1]);
        sub->Find_SequenceNo();
        lset(sub->dat[VIWB_DO], invwb.dat[1]);
        rset(sub->dat[VIWB_QTY], invwb.dat[4]);
        rset(sub->dat[VIWB_AMOUNT], invwb.dat[5]);
/* Scale ticket will consider a ticket
		if(Purchase_Flag) {
			qd(sub->dat[VIWB_DO],&ds);
			lset(sub->dat[VIWB_SCALE], ds.dat[DP_SCALE]);
			CString s;
			s.Format("select sum(amt) from SCALESUB where ticket = '%s'",  ds.dat[DP_SCALE]);
			double amt = SqlValue(s);
			dtodb(amt, sub, VIWB_SCALE_AMT);
		}
		*/
        sub->AddNew();

/*   // temporary mark due to unused by customer (function for calling qty and amt from scale weighing) ccwong091111
		if (strcmp(strTable,"dsinv")==0){
			qd(invwb.dat[1],&ds);
			if (!isblank(ds.dat[DP_SCALE])){
				CString str;
				double sqty,samt;
				sqty=samt=0;	
				str.Format("Select sum(qty),sum(amt) from SCALESUB Where ticket='%s' group by ticket",ds.dat[DP_SCALE]);
				sqty=SqlValue(str,1);
				samt=SqlValue(str,2);
				lset(sub->dat[1], df->dat[1]);
				sub->Find_SequenceNo();
				lset(sub->dat[VIWB_DO], ds.dat[DP_SCALE]);
				dtodb(sqty,sub,VIWB_QTY);
				dtodb(samt,sub,VIWB_AMOUNT);
				sub->AddNew();
			}
		}
		*/
        ReFreshSubFile();
        ShowInformation();
		//GST
		SetSubTotal(df);
		ShowInformation();
        dclose(&invwb);
    }
}

void CArinvWb::OnUnlock()
{	
	CARINV_UNLOCK a;
	a.DoModal();
	ShowInformation();
	CDialog::OnCancel();
}

BEGIN_EVENTSINK_MAP(CArinvWb, CParent)
    //{{AFX_EVENTSINK_MAP(CArinvWb)
	ON_EVENT(CArinvWb, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CArinvWb::OnDblClickMsflexgrid1() 
{
	OnDblClickMsflexgrid(&m_grid,sub);
	//GST
	SetSubTotal(df);
	ShowInformation();	
}
