// Wds.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Wds.h"
#include "PartyGrading.h"
#include "SETPAYDATE.h"
#include "SETPAYDATEU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWds dialog


CWds::CWds(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CWeighing(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CWds)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWds::DoDataExchange(CDataExchange* pDX)
{
    CWeighing::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWds)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWds, CWeighing)
	//{{AFX_MSG_MAP(CWds)
	ON_BN_CLICKED(IDC_PARTY_GRADING, OnPartyGrading)
	ON_BN_CLICKED(IDC_SETPAYDATE, OnSetpaydate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWds message handlers
void CWds::W1Start(long weight)
{
    dbasedate(df->dat[DP_DATE]);
    _strtime(df->dat[DP_TIMEIN]);
    ltodb(weight,df,DP_SGROSS);
    ltodb(ActualWeight,df,DP_W1);
    ltodb(weight,df,DP_Z1);
	lset(df->dat[DP_USER1],user.dat[1]);
}

void CWds::W2Start(long weight)
{
    ltodb(weight,df,DP_STARE);
    dbasedate(df->dat[DP_DATE]);
    _strtime(df->dat[DP_TIMEOUT]);	
	ltodb(ActualWeight,df,DP_W2);
	ltodb(weight,df,DP_Z2);
	lset(df->dat[DP_USER2],user.dat[1]);
}

void CWds::ShowInformation()
{
    CStaff::ShowInformation();
    getmet(m,df);
   
	double total,qty,wt,tpcent,tqty,upc;
	long   net=0;
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
    net = m[4];
	dtoa(labs(net),text,9,0);
    SetDlgItemText(IDC_NETT,putnum(text));
    dtoa(double(m[7]),text,6,0);
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
//		dtodb(wt,df,DP_AMT1+i);
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

}

void CWds::W2Saved()
{
	/*   // CCWong 20100408 To mark due to user don't want this feature, by Dingsc
    int flag = MessageBox("Transfer To Despatch ","Transfer To Despatch",MB_YESNO | MB_ICONQUESTION);
    if(flag==IDYES) {
            long xrec = viewidx(&accode);
            if(xrec>0) {
                waitflock(&dp);
                long mrec = append(&dp);
                fetch(&dp,&wds);
                lset(dp.dat[DP_ACCODE],accode.dat[1]);
                CountTicketNo(&dp,1);
                FindProductCategorySerialNo(&dp);
                AppendIdx(mrec,&dp,1);
                unflock(&dp);
			}	      
    }
	*/
	int nID = MessageBox("Set Payment Date ","Set Payment Date",MB_YESNO | MB_ICONQUESTION);
    if(nID==IDYES){
		OnSetpaydate();
	}
}
//int  FirstArray[]  = { 0,DP_SERIAL,DP_PO,DP_UNIT,DP_ACCODE,DP_PTYPE,DP_SUBCON,DP_TRANSP,DP_DO,DP_SCRAP,DP_CONTRACT,DP_IC,DP_DRIVER,DP_CGROSS,DP_CTARE,DP_CWASTAGE,DP_VEHICLE,DP_SGROSS,DP_SALESMAN,DP_VTYPE};
int  FirstArray[]  = { 0,DP_SERIAL,DP_PO,DP_UNIT,DP_ACCODE,DP_PTYPE,DP_SUBCON,DP_TRANSP,DP_DO,DP_SCRAP,DP_CONTRACT,DP_IC,DP_DRIVER,DP_CGROSS,DP_CTARE,DP_CWASTAGE,DP_VEHICLE,DP_SALESMAN,DP_VTYPE};
int  Array[]       = { 0,DP_WASTAGE,DP_PT1,DP_PCENT1,DP_QTY1,DP_WT1,DP_PT2,DP_PCENT2,DP_QTY2,DP_WT2,DP_PT3,DP_PCENT3,DP_QTY3,DP_WT3,
                       DP_PT4,DP_PCENT4,DP_QTY4,DP_WT4,DP_PT5,DP_PCENT5,DP_QTY5,DP_WT5,DP_PT6,DP_PCENT6,DP_QTY6,DP_WT6,DP_PT7,DP_PCENT7,DP_QTY7,DP_WT7,DP_PT8,DP_PCENT8,DP_QTY8,DP_WT8,
					   DP_GRADE,DP_SCALE,DP_SALESMAN,DP_VTYPE,DP_OTHQTY,DP_REMARKS,DP_SRNNO,DP_SRNDATE,DP_BASIS,DP_ACCODE,DP_PTYPE,DP_SUBCON,DP_TRANSP,DP_DO,DP_SCRAP,DP_CONTRACT,DP_IC,DP_DRIVER,0};
					//DP_GRADE,DP_SCALE,DP_SALESMAN,DP_VTYPE,DP_OTHQTY,DP_REMARKS,DP_SRNNO,DP_SRNDATE,DP_BASIS,DP_ACCODE,DP_PTYPE,DP_SUBCON,DP_TRANSP,DP_DO,DP_SCRAP,DP_CONTRACT,DP_IC,DP_DRIVER,DP_SGROSS,DP_STARE,0};
					
int  DSWeighingArray[] = { 0,DP_WASTAGE,DP_PT1,DP_PCENT1,DP_QTY1,DP_WT1,DP_PT2,DP_PCENT2,DP_QTY2,DP_WT2,DP_PT3,DP_PCENT3,DP_QTY3,DP_WT3,
						DP_PT4,DP_PCENT4,DP_QTY4,DP_WT4,DP_PT5,DP_PCENT5,DP_QTY5,DP_WT5,DP_PT6,DP_PCENT6,DP_QTY6,DP_WT6,DP_PT7,DP_PCENT7,DP_QTY7,DP_WT7,DP_PT8,DP_PCENT8,DP_QTY8,DP_WT8,
						DP_GRADE,DP_SCALE,DP_SALESMAN,DP_VTYPE,DP_OTHQTY,DP_REMARKS,DP_SRNNO,DP_SRNDATE,DP_BASIS,DP_ACCODE,DP_PTYPE,DP_SUBCON,DP_TRANSP,DP_DO,DP_SCRAP,DP_CONTRACT,DP_IC,DP_DRIVER,DP_SGROSS,DP_STARE,0};

void WeighingProcess(DB *main,DB *ds)
{
    CWds p1(IDD_WDS,ds,0);
	
	if(!stricmp(ds->name,"wds") || !stricmp(ds->name,"ds")){
		tado(&paymentdate,"paymentdate");
	}else{
		tado(&paymentdate,"paymentdated");
	}
    p1.df          = ds;
    p1.m_mfile     = main;
    p1.vehicle_fld = DP_VEHICLE;
    p1.w1_array    = FirstArray;
	if(!stricmp(ds->name,"wds")) p1.w2_array    = DSWeighingArray;
    else p1.w2_array    = Array;
    p1.LastField   = DP_OTHQTY;
	p1.m_WinPrint  = FALSE;	
    p1.DoModal();

	CString strSql,strWhere;
	strSql.Format("SELECT * FROM PT ");
	prodtype.m_strCmdText = strSql;
	prodtype.ReOpen();

	dclose(&paymentdate);
}

void CWds::OnPartyGrading() 
{
	isCheck = FALSE;
	if (IsSaveConditionOk(df)==0) return;

	CPartyGrading t1(IDD_PARTY_GRADING);
	
    t1.FirstField = DP_PT11;
    t1.LastField  = DP_WT18;
	t1.df   = df;
    t1.DoModal();	
	isCheck = TRUE;
}

void CWds::OnSetpaydate()
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
		DoAutoInvoice(df,&apinv_wb,&apoinv_wb);
	}
	else{
		AfxMessageBox("Failed! SGROSS AND STARE<=0.");
	}
	ShowInformation();
}