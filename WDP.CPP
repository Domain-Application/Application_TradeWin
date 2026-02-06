// Wdp.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Wdp.h"
#include "PartyGrading.h"
#include "SETPAYDATE.h"
#include "SETPAYDATEU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWdp dialog

CWdp::CWdp(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CWeighing(IDD,pdf,Idx,pParent)

{
	//{{AFX_DATA_INIT(CWdp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWdp::DoDataExchange(CDataExchange* pDX)
{
    CWeighing::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWdp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWdp, CWeighing)
	//{{AFX_MSG_MAP(CWdp)
	ON_BN_CLICKED(IDC_PARTY_GRADING, OnPartyGrading)
	ON_BN_CLICKED(IDC_SETPAYDATE, OnSetpaydate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWdp message handlers
void CWdp::W1Start(long weight)
{
	if(!stricmp(df->name,"wrt")){
		CString str;	
		str.Format("select * from [dp] where DATEDIFF(D, [DATE], GETDATE())<= 10 AND VEHICLE='%s' order by ticket",df->dat[DP_VEHICLE]);
		dp.m_strCmdText = str;
		dp.ReOpen();
		long Srec = viewidx(&dp);
		if (Srec>0){
			lset(df->dat[DP_DPTICKET],dp.dat[1]);
			lset(df->dat[DP_PO],dp.dat[DP_PO]);
			lset(df->dat[DP_UNIT],dp.dat[DP_UNIT]);
			lset(df->dat[DP_ACCODE],dp.dat[DP_ACCODE]);
			lset(df->dat[DP_PTYPE],dp.dat[DP_PTYPE]);
			lset(df->dat[DP_SUBCON],dp.dat[DP_SUBCON]);
			lset(df->dat[DP_TRANSP],dp.dat[DP_TRANSP]);
			lset(df->dat[DP_DO],dp.dat[DP_DO]);
			lset(df->dat[DP_SCRAP],dp.dat[DP_SCRAP]);
			lset(df->dat[DP_CONTRACT],dp.dat[DP_CONTRACT]);
			lset(df->dat[DP_IC],dp.dat[DP_IC]);
			lset(df->dat[DP_DRIVER],dp.dat[DP_DRIVER]);
			lset(df->dat[DP_PT1],dp.dat[DP_PT1]);
			rset(df->dat[DP_PCENT1],dp.dat[DP_PCENT1]);
			rset(df->dat[DP_WT1],dp.dat[DP_WT1]);
			lset(df->dat[DP_PT2],dp.dat[DP_PT2]);
			rset(df->dat[DP_PCENT2],dp.dat[DP_PCENT2]);
			rset(df->dat[DP_WT2],dp.dat[DP_WT2]);
			lset(df->dat[DP_PT3],dp.dat[DP_PT3]);
			rset(df->dat[DP_PCENT3],dp.dat[DP_PCENT3]);
			rset(df->dat[DP_WT3],dp.dat[DP_WT3]);
			lset(df->dat[DP_PT4],dp.dat[DP_PT4]);
			rset(df->dat[DP_PCENT4],dp.dat[DP_PCENT4]);
			rset(df->dat[DP_WT4],dp.dat[DP_WT4]);
			lset(df->dat[DP_PT5],dp.dat[DP_PT5]);
			rset(df->dat[DP_PCENT5],dp.dat[DP_PCENT5]);
			rset(df->dat[DP_WT5],dp.dat[DP_WT5]);
			lset(df->dat[DP_PT6],dp.dat[DP_PT6]);
			rset(df->dat[DP_PCENT6],dp.dat[DP_PCENT6]);
			rset(df->dat[DP_WT6],dp.dat[DP_WT6]);
			lset(df->dat[DP_PT7],dp.dat[DP_PT7]);
			rset(df->dat[DP_PCENT7],dp.dat[DP_PCENT7]);
			rset(df->dat[DP_WT7],dp.dat[DP_WT7]);
			lset(df->dat[DP_PT8],dp.dat[DP_PT8]);
			rset(df->dat[DP_PCENT8],dp.dat[DP_PCENT8]);
			rset(df->dat[DP_WT8],dp.dat[DP_WT8]);
			lset(df->dat[DP_SRNNO],dp.dat[DP_SRNNO]);
			lset(df->dat[DP_SRNDATE],dp.dat[DP_SRNDATE]);
			lset(df->dat[DP_BASIS],dp.dat[DP_BASIS]);
			rset(df->dat[DP_AMT1],dp.dat[DP_AMT1]);
			rset(df->dat[DP_AMT2],dp.dat[DP_AMT2]);
			rset(df->dat[DP_AMT3],dp.dat[DP_AMT3]);
			rset(df->dat[DP_AMT4],dp.dat[DP_AMT4]);
			rset(df->dat[DP_AMT5],dp.dat[DP_AMT5]);
			rset(df->dat[DP_AMT6],dp.dat[DP_AMT6]);
			rset(df->dat[DP_AMT7],dp.dat[DP_AMT7]);
			rset(df->dat[DP_AMT8],dp.dat[DP_AMT8]);

		}
		else{
			AfxMessageBox("Despatch Ticket Not Found");
			return;
		}
	}
    dbasedate(df->dat[DP_DATE]);
    _strtime(df->dat[DP_TIMEIN]);
    ltodb(weight,df,DP_SGROSS);
    ltodb(ActualWeight,df,DP_W1);
    ltodb(weight,df,DP_Z1);
	lset(df->dat[DP_USER1],user.dat[1]);
	
}

void CWdp::W2Start(long weight)
{
	if(!stricmp(df->name,"wrt")){
		CString str;	
		str.Format("select * from [dp] where DATEDIFF(D, [DATE], GETDATE())<= 10 AND VEHICLE='%s' order by ticket",df->dat[DP_VEHICLE]);
		dp.m_strCmdText = str;
		dp.ReOpen();
	}
    ltodb(weight,df,DP_STARE);
    dbasedate(df->dat[DP_DATE]);
    _strtime(df->dat[DP_TIMEOUT]);
	lset(df->dat[DP_USER2],user.dat[1]);
    ltodb(ActualWeight,df,DP_W2);
    ltodb(weight,df,DP_Z2);
}

void CWdp::ShowInformation()
{
    CStaff::ShowInformation();
    getmet(m,df);
   
	double total,qty,wt,tpcent,tqty,upc;
//	long   net;
    int    i,x,upori;
    char   text[20];
    char str[20];
    ltot(m[3],6,str);
	SetDlgItemText(IDC_GROSSDIFF,putnum(str));
    ltot(m[6],6,str);
	SetDlgItemText(IDC_TAREDIFF,putnum(str));
    ltot(m[12],6,str);
	SetDlgItemText(IDC_VAR,putnum(str));
    ltot(m[10],6,str);
    SetDlgItemText(IDC_PARTYNETT,putnum(str));
    ltot(m[11],6,str);
    SetDlgItemText(IDC_NETT,putnum(str));
/*
    net = m[11];
	dtoa(labs(net),text,9,0);
    SetDlgItemText(IDC_NETT,putnum(text));
    dtoa(double(m[12]),text,6,0);
    SetDlgItemText(IDC_VAR,putnum(text));
*/
	total = qty = tqty = tpcent = 0;
    for(i=0;i<8;i++){
        x = i*4+DP_PT1;
        tpcent += atof(df->dat[x+1]);
		tqty += atof(df->dat[x+2]);
		wt = atof(df->dat[x+2]) * atof(df->dat[x+3]);
//        total += wt;
//        dtoa(wt,text,9,2);
//        SetDlgItemText(IDC_TW1+i,putnum(text));
		dtodb(wt,df,DP_AMT1+i);
//        SetDlgItemText(IDC_TW1+i,df->dat[DP_AMT1+i]);
		dtoa(atof(df->dat[DP_AMT1+i]),text,9,2);
		SetDlgItemText(IDC_TW1+i,putnum(text));
		total += df->val(DP_AMT1+i);

		upori = i+DP_ORI_WT1;
		upc = fabs(df->val(x+3) - df->val(upori));
		if (upc>0){
			SetDlgItemText(IDC_UPC1+i, "*");
		}
		else{
			SetDlgItemText(IDC_UPC1+i, "");
		}
    }
	 dtoa(tpcent,text,5,0);
     SetDlgItemText(IDC_TPCENT,putnum(text));
     dtoa(tqty,text,5,0);
     SetDlgItemText(IDC_TQTY,putnum(text));
     dtoa(total,text,9,2);
     SetDlgItemText(IDC_TOTAL,putnum(text));

	 SetDlgItemText(IDC_MANUAL," ");	

}

int  FirstArr[]  = { 0,DP_SERIAL,DP_PO,DP_UNIT,DP_ACCODE,DP_PTYPE,DP_SUBCON,DP_TRANSP,DP_DO,DP_SCRAP,DP_CONTRACT,DP_IC,DP_DRIVER,DP_VEHICLE,DP_VTYPE,0};
int  Arr[]       = { 0,DP_WASTAGE,DP_PT1,DP_PCENT1,DP_QTY1,DP_WT1,DP_PT2,DP_PCENT2,DP_QTY2,DP_WT2,DP_PT3,DP_PCENT3,DP_QTY3,DP_WT3,
                       DP_PT4,DP_PCENT4,DP_QTY4,DP_WT4,DP_PT5,DP_PCENT5,DP_QTY5,DP_WT5,DP_PT6,DP_PCENT6,DP_QTY6,DP_WT6,DP_PT7,DP_PCENT7,DP_QTY7,DP_WT7,DP_PT8,DP_PCENT8,DP_QTY8,DP_WT8,
                       DP_GRADE,DP_OTHQTY,DP_REMARKS,DP_SRNNO,DP_SRNDATE,DP_BASIS,DP_ACCODE,DP_PTYPE,DP_SUBCON,DP_TRANSP,DP_DO,DP_SCRAP,DP_CONTRACT,DP_IC,DP_DRIVER,DP_VTYPE,0};

void Weighing_Despatch(DB *main,DB *dp)
{
    CWdp p1(IDD_WDP,dp,0);
	//CWdp p1(dp->ID,dp,0);

	if(!stricmp(dp->name,"wds") || !stricmp(dp->name,"ds")){
		tado(&paymentdate,"paymentdate");
	}
	else{
		tado(&paymentdate,"paymentdated");
	}

    p1.df          = dp;
    p1.m_mfile     = main;
    p1.vehicle_fld = DP_VEHICLE;
    p1.w1_array    = FirstArr;
    p1.w2_array    = Arr;
    p1.LastField   = DP_OTHQTY;
	p1.m_WinPrint  = FALSE;
    p1.DoModal();

	CString strSql,strWhere;
	strSql.Format("SELECT * FROM PT ");
	prodtype.m_strCmdText = strSql;
	prodtype.ReOpen();

	dclose(&paymentdate);
}

void CWdp::OnPartyGrading() 
{
	if (IsSaveConditionOk(df)==0) return;
	
	CPartyGrading t1(IDD_PARTY_GRADING);
	
    t1.FirstField = DP_PT11;
    t1.LastField  = DP_WT18;
	t1.df   = df;
    t1.DoModal();	
}


void CWdp::W2Saved()
{
	int nID = MessageBox("Set Payment Date ","Set Payment Date",MB_YESNO | MB_ICONQUESTION);
    if(nID==IDYES){
		OnSetpaydate();
	}
}

void CWdp::OnSetpaydate()
{
	if(df->val(DP_SGROSS)>0 && df->val(DP_STARE)>0){
		CSETPAYDATEU d;
		d.b_ticket = m_mfile->dat[1];
		d.main = m_mfile;
		d.updatepay = &paymentdate;
		int flag = d.DoModal();
		if(flag==IDCANCEL) {
			return;
		}
		DoAutoInvoice(df,&arinv_wb,&aroinv_wb);
	}
	else{
		AfxMessageBox("Failed! SGROSS AND STARE<=0.");
	}
	ShowInformation();
}