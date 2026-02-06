// PartyGrading.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "PartyGrading.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPartyGrading dialog


CPartyGrading::CPartyGrading(UINT IDD,CWnd* pParent /*=NULL*/)
      :CFEdit(CPartyGrading::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPartyGrading)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPartyGrading::DoDataExchange(CDataExchange* pDX)
{
    CFEdit::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPartyGrading)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPartyGrading, CFEdit)
	//{{AFX_MSG_MAP(CPartyGrading)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartyGrading message handlers

int CPartyGrading::GetNextField(int fld)
{
    if(fld==LastField) fld = FirstField - 1;
    fld++;
    return fld;
}

void CPartyGrading::ShowInformation()
{
	double total,wt,qty,tqty,tpcent,upc;
	long	net;
    int    i,x,upori;
    char   text[20];
    char str[20];
	
  	CTabPage::ShowInformation();
	total = qty = tqty = tpcent = 0;
	getmet(m,df);
	net = m[11];
	if (m[10]>0) net = m[10];
	
    for(i=0;i<8;i++){
        x = i*4+DP_PT11;   
        qty  = df->val(x+1) * net;
        tpcent += df->val(x+1);
        tqty += df->val(x+2) ;
        wt = df->val(x+2) * df->val(x+3);
		//total += wt;
        dtoa(wt,text,9,2);
		//SetDlgItemText(IDC_TW11+i,putnum(text));
		dtodb(wt,df,DP_AMT11+i);
		SetDlgItemText(IDC_TW11+i,df->dat[DP_AMT11+i]);
		total += df->val(DP_AMT11+i);
		
		upori = i+DP_ORI_WT11;
		upc = fabs(df->val(x+3) - df->val(upori));
		if (upc>0){
			SetDlgItemText(IDC_UPC1+i, "*");
		}
		else{
			SetDlgItemText(IDC_UPC1+i, "");
		}
    }
	dtoa(tpcent,text,5,0);
	SetDlgItemText(IDC_TPCENT1,putnum(text));
	dtoa(tqty,text,5,0);
	SetDlgItemText(IDC_TQTY1,putnum(text));
	dtoa(total,text,9,2);
	SetDlgItemText(IDC_TOTAL1,putnum(text));
}
