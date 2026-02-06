// Tng.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Tng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTng dialog


CTng::CTng(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CTng)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    busy = FALSE;
}


void CTng::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTng)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTng, CStaff)
    //{{AFX_MSG_MAP(CTng)
    ON_WM_DESTROY()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*
/////////////////////////////////////////////////////////////////////////////
// CTng message handlers
void CTng::ShowInformation()
{
    CStaff::ShowInformation();
}


BOOL CTng::OnInitDialog()
{
    CStaff::OnInitDialog();

	int port = atoi( profile.dat[PF_PORT]);
    if(port>0) {
        OpenMifare(port);
        SetTimer(1,6000,NULL);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTng::OnDestroy()
{
    int port = atoi( profile.dat[PF_PORT]);
    if(port>0) {
        KillTimer(1);
        CloseMifare();
    }
    CStaff::OnDestroy();
}

void CTng::OnTimer(UINT nIDEvent)
{
    CStaff::OnTimer(nIDEvent);
	
    if(busy) return;
    busy = TRUE;
    if(SaveFlag != CANNOTSAVE){
        busy = FALSE;
        return;
    }
	
    CardAC = CardPt = CardVehicle = MCardKey = CardTransporter = "";
    if( !OnScanCardID(m_tuid)){
        busy = FALSE;
        return;
    }
	
    if(!strcmp(m_tuid,df->dat[1])) {
        busy = FALSE;
		//        ChangeToEditMode(2);
		//        GotoEdit(2);
		return;
	}
    lset(df->dat[1], m_tuid);
	
    long xrec = df->Search(df->dat[1], 1, TRUE);
    if(xrec>0) {
		ShowInformation();
    }
	else {
		CString str;
		str.Format("New Card No:%s found.\n\nDo you want register new card ?", m_tuid);
		UINT ID = MessageBox(str , "Register New Card", MB_ICONQUESTION | MB_YESNO);
        if(ID==IDNO) {
			MessageBox("Pls remove your card now....");
			busy = FALSE;
			return;
		}
        int state = rf_M1_authentication2(0, 0x60,0, k[ 0 ]);
        if(state){
            str.Format("This card unformatted.\n\n\nPlease remove your card and goto format process.");
            MessageBox( str );
            busy = FALSE;
            return;
        }
        if( !OnScanCardID(m_tuid)){
            busy = FALSE;
            return;
        }
		
        BOOL flag = ReadCardKey();
        if( !flag ){
            busy = FALSE;
            return;
        }
        if(!strcmp(MCardKey,CardKey)) {
			if(MessageBox("Import data from card ?","Question",MB_ICONQUESTION | MB_YESNO)==IDYES) {
				lsetblank(df);
				lset(df->dat[1], m_tuid);
				ReadCardInfo(df,1);
				df->AddNew();
				ShowInformation();
				MessageBox("Import Data Completed\n\nPlease remove your card now..");
				busy = FALSE;
				return;
			}
        }
		else {
			lsetblank(df);
			lset(df->dat[1], m_tuid);
			ShowInformation();
			
            long value = 0;
            state = rf_M1_initval(0, 1, value);
			df->AddNew();
			
			MessageBox("New Card Found..");
			ShowInformation();
            ChangeToEditMode(2);
            CurrentField = GetFirstAvailableField();
            GotoEdit( CurrentField);
		}
    }
    busy = FALSE;
}


void AdminDbTng(DB *df)
{
    if(df->ID==0) {                           
        AfxMessageBox("Database not open");   
        return;                               
    }                                         
    CTng p1(df->ID,df,df->f[1].idx & 0x01);
    p1.DoModal();                             
}

int Tng_AdminDb(DB *df,int flag)
{
    AdminDbTng(df);
    return 1;
}

void init_Tng(DB *df)
{
    df->e->AdminDb = Tng_AdminDb;
}
*/