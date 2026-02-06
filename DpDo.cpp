// DpDo.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "DpDo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDpDo dialog


CDpDo::CDpDo(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
: CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CDpDo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDpDo::DoDataExchange(CDataExchange* pDX)
{
	CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDpDo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDpDo, CStaff)
	//{{AFX_MSG_MAP(CDpDo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDpDo message handlers
void CDpDo::ShowInformation()
{
	CStaff::ShowInformation();
	//long Srec = qd(df->dat[DPDO_DP],&dp);
	long Srec;
	if (isblank(ticketfrdp)){
		Srec = dp.Search(df->dat[DPDO_DP],1,TRUE);
	}	
	else Srec = dp.Search(ticketfrdp,1,TRUE);
	if (Srec>0){
		SetDlgItemText(IDC_TICKET_DATE,mdate(dp.dat[DP_DATE]));
		SetDlgItemText(IDC_TICKET_TIMEIN,dp.dat[DP_TIMEIN]);
		SetDlgItemText(IDC_TICKET_TIMEOUT,dp.dat[DP_TIMEOUT]);
		SetDlgItemText(IDC_TICKET_LORRY,dp.dat[DP_VEHICLE]);
		qd(dp.dat[DP_ACCODE],&accode);
		SetDlgItemText(IDC_ACCODE,accode.dat[2]);
		SetDlgItemText(IDC_TICKET_TARE,putnum(dp.dat[DP_SGROSS]));
		SetDlgItemText(IDC_TICKET_GROSS,putnum(dp.dat[DP_STARE]));
		char str[20];
		getmet(m,&dp);
		ltot(m[11],18,str);
		SetDlgItemText(IDC_TICKET_NET,putnum(str));
	}
	else{
		SetDlgItemText(IDC_TICKET_DATE,"");
		SetDlgItemText(IDC_TICKET_TIMEIN,"");
		SetDlgItemText(IDC_TICKET_TIMEOUT,"");
		SetDlgItemText(IDC_TICKET_LORRY,"");		
		SetDlgItemText(IDC_ACCODE,"");
		SetDlgItemText(IDC_TICKET_TARE,"");
		SetDlgItemText(IDC_TICKET_GROSS,"");		
		SetDlgItemText(IDC_TICKET_NET,"");
	}
	
	qd(df->dat[DPDO_DELIVERY],&accode);
	SetDlgItemText(IDC_ADD1,accode.dat[ACCODE_ADD1]);
	SetDlgItemText(IDC_ADD2,accode.dat[ACCODE_ADD2]);
	SetDlgItemText(IDC_ADD3,accode.dat[ACCODE_ADD3]);
	
}
int tell_dp_do(UINT nChar,int &id,int &fld,DB *df,int sq)
//int tell_dp_do(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
	switch(fld) {
	case DPDO_GASNO:
		if (df->dat[DPDO_GAS][0]=='N' || df->dat[DPDO_GAS][0]=='F'){
			id = DO;
		}
		break;
	case DPDO_DP:	
		if (isblank(df->dat[fld])){
			if(dpKey==1) {
				lset(df->dat[fld], dp.dat[1]);        
			}
		}
		long Srec = qd(df->dat[DPDO_DP],&dp);
		if (Srec<=0){
			waitmsg("Despatch Ticket Not Existed");
		}	
		return TAB_REFRESH_ALL;
		break;

	}
	return 2;
}

int dp_do_save(DB *df,int flag)
{
	long Srec = qd(df->dat[DPDO_DP],&dp);
	if (Srec<=0){
		waitmsg("Despatch Ticket Not Existed");
		return 0;
	}
	CString str,strMsg;
	str.Format("Select [DP_DO] from [DP_DO] WHERE [DP]='%s' and [DP_DO]<>'%s'",df->dat[DPDO_DP],df->dat[1]);
	if (SqlText(str).GetLength()>0){
		strMsg.Format("DESPATCH TICKET : %s already existed at DO NO : %s",df->dat[DPDO_DP],SqlText(str));
		waitmsg(strMsg);
		return 0;
	}
	return 1;
}

int start_dp_do(DB *df)
{
    dbasedate(df->dat[DPDO_DATE]);
    if(dpKey==1) {
        lset(df->dat[DPDO_DP], dp.dat[1]);   
		lset(df->dat[DPDO_DO],dp.dat[DP_DO]);
    }
	
	return 1;
}

void InitialValue_dp_do(DB *df)
{
	if(dpKey==1) {
        lset(df->dat[DPDO_DP], dp.dat[1]);        
    }
	
}

int dp_do_initial(DB *df,int *fld)                //initial setting
{
        fld[0] = DPDO_DP;
        fld[1] = 0;
       return 1;
}

    
void init_dp_do(DB *df)
{
    df->e->initial = dp_do_initial;
    df->e->tell		 = tell_dp_do;
	df->e->save      = dp_do_save;
	df->e->start     = start_dp_do;
    //df->e->InitialValue      = InitialValue_dp_do;
}

BOOL CDpDo::OnInitDialog() 
{
	CStaff::OnInitDialog();
	
	if(dpKey==1) {
        PostMessage(WM_COMMAND,ID_ADDEDIT);
    }	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
