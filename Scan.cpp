// Scan.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Scan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScan dialog


CScan::CScan(CWnd* pParent /*=NULL*/)
	: CDialog(CScan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	busy = FALSE;
}


void CScan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScan)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScan, CDialog)
	//{{AFX_MSG_MAP(CScan)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScan message handlers



BOOL CScan::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int port = atoi( profile.dat[PF_PORT]);
    if(port>0) {
        OpenMifare(port);
        SetTimer(1,1000,NULL);
    }	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScan::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);
	if(busy) return;
    busy = TRUE;

    CardAC = CardPt = CardVehicle = MCardKey = CardTransporter = "";
   if( !OnScanCardID(m_tuid)){
        busy = FALSE;
       return;
    }
 	
 /*   if(!strcmp(m_tuid,df->dat[1])) {
        busy = FALSE;
		return;
		}*/
    lset(df->dat[1], m_tuid);
	
    long xrec = df->Search(df->dat[1], 1, TRUE);
    if(xrec>0) {
		ScanApproveFlag=TRUE;
		CardID = m_tuid;
		OnOK();

    }
	else {
        int state = rf_M1_authentication2(0, 0x60,0, k[ 0 ]);
        if(state){
            str.Format("This card unformatted.\n\n\nPlease remove your card and goto format process.");
            MessageBox( str );
        }
		else{
			CString str;
			str.Format("New Card No:%s found.\n\nPlease register before use!", m_tuid);
			AfxMessageBox(str);
		}
		busy = FALSE;
		return;
    }
    busy = FALSE;
}

void CScan::OnDestroy() 
{
    int port = atoi( profile.dat[PF_PORT]);
    if(port>0) {
        KillTimer(1);
        CloseMifare();
    }	
	CDialog::OnDestroy();
}

void CScan::OnCancel() 
{
	ScanApproveFlag=FALSE;
	CardID = "";
	CDialog::OnCancel();
}

void CScan::OnOK() 
{
	CDialog::OnOK();
}
