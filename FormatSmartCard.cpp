// FormatSmartCard.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "FormatSmartCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CFormatSmartCard dialog


CFormatSmartCard::CFormatSmartCard(CWnd* pParent /*=NULL*/)
	: CDialog(CFormatSmartCard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormatSmartCard)
	m_msg = _T("");
	m_cardno = _T("");
	//}}AFX_DATA_INIT
    m_tuid = "";
	lset(tng.dat[1],"");
    busy = FALSE;
}


void CFormatSmartCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormatSmartCard)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_CLOCKFRAME, m_Clock);
	DDX_Text(pDX, IDC_MSG, m_msg);
	DDX_Text(pDX, IDC_EDIT1, m_cardno);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormatSmartCard, CDialog)
	//{{AFX_MSG_MAP(CFormatSmartCard)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatSmartCard message handlers

void CFormatSmartCard::OnDestroy() 
{
    int port = atoi( profile.dat[PF_PORT]);
    if(port>0) {
        KillTimer(1);
        CloseMifare();
    }
	CDialog::OnDestroy();
}

BOOL CFormatSmartCard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    CheckRadioButton( IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);

	//m_Clock.Start(IDB_CLOCKST_PANE, IDB_CLOCKST_BIG, IDB_CLOCKST_SMALL);
    this->ShowWindow(SW_SHOWMAXIMIZED);

    GetDlgItem(IDOK)->EnableWindow(FALSE);
    int port = atoi( profile.dat[PF_PORT]);
    if(port>0) {
        OpenMifare(port);
        SetTimer(1,2000,NULL);
        OnRadio1();
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormatSmartCard::OnTimer(UINT nIDEvent) 
{
    if(busy) return;
    busy = TRUE;
	CDialog::OnTimer(nIDEvent);

    if( !OnScanCardID(m_tuid)){
        GetDlgItem(IDOK)->EnableWindow(FALSE);
        SetDlgItemText(IDC_EDIT1,"");
        SetDlgItemText(IDC_HEXDATA, "");
        SetDlgItemText(IDC_MSG, "Ready.");
        busy = FALSE;
        return;
    }
    if( strcmp(tng.dat[1], m_tuid)==0) {
        UpdateData();
        m_cardno.TrimRight();
        if(m_cardno == m_tuid) {
            busy = FALSE;
            return;
        }
    }

    lset(tng.dat[1], m_tuid);
    long xrec = tng.Search(tng.dat[1],1);
    SetDlgItemText(IDC_EDIT1,tng.dat[1]);

    keyMifare d1;

    d1.code = atol(tng.dat[1]);
    unsigned char Snr[20];
    Snr[0] = d1.ch[0];
    Snr[1] = d1.ch[1];
    Snr[2] = d1.ch[2];
    Snr[3] = d1.ch[3];
 //   CString s = ExchangeHexToString(Snr,4);
 //   SetDlgItemText(IDC_HEXDATA, s);

    if(xrec) {
        GetDlgItem(IDOK)->EnableWindow(FALSE);
        str.Format("Smart card data existed in database.\r\n\r\nCan not proceed.");
        SetDlgItemText(IDC_MSG, str);
    }
    else {
        SetDlgItemText(IDC_MSG, "");
        GetDlgItem(IDOK)->EnableWindow(TRUE);
    }
    busy = FALSE;
}

void CFormatSmartCard::OnCancel() 
{
	CDialog::OnCancel();
}

void CFormatSmartCard::OnOK() 
{
    KillTimer(1);

    key = GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO3) - IDC_RADIO1 + 1;
    if(key<=2) {
        int state = rf_M1_authentication2(0, 0x60,0, k[ 0 ]);
        if(!state){
            str.Format("This card already formatted");
            MessageBox( str );
            return;
        }
    }
    else {
        key = 0;
    }
    if( !OnScanCardID(m_tuid)) return;
	
    m_msg = "WARNING: Formatting will erasy ALL data on this card.\nTo format this card, click yes\nTo Quit, click no";
    UINT ID = MessageBox(m_msg, "Format card",MB_YESNO | MB_ICONQUESTION);
    if(ID==IDNO) return;

    UpdateData(FALSE);
    DoFormat();
    m_msg = "Successful....(Please remove you card.)";
    UpdateData(FALSE);

    SetTimer(1,3000,NULL);
}

void FormatSmartCard()
{
    CFormatSmartCard p;

    p.DoModal();
}

BOOL CFormatSmartCard::DoSector(int sector)
{
	WORD icdev = 0x0000;
	unsigned char mode = 0x60;
	unsigned char cLen;
    unsigned char secnr = (sector*4) ;
	
    int state = rf_M2_authentication2(icdev,mode,secnr, key);
	if(state){
        str.Format("Authenticate key error\n\nSector %i error", sector);
        ErrorBox( str );
        return FALSE;
	}

    unsigned char z[16];

    for(int i=0; i<4; i++) {
        if(i==0 && sector==0) continue;

        secnr = (sector*4) + i;
        state = rf_M1_read(icdev,secnr,z,&cLen);
        if(state || cLen != 16){
            str.Format("Read sector %i blk %i error", sector, i);
            ErrorBox( str );
            return FALSE;
        }
        if(i==3) {
            if(key==0) continue;

            z[0] = z[10] = 0x37;
            z[1] = z[11] = 0x89;
            z[2] = z[12] = 0x00;
            z[3] = z[13] = 0x72;
            z[4] = z[14] = 0x88;
            z[5] = z[15] = 0x00;
        }
        else {
            memset(z, 0, sizeof(z));
        }

        state = rf_M1_write(icdev,secnr,z);
        if(state){
            str.Format("Write sector %i blk %i error", sector, i);
            ErrorBox( str );
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CFormatSmartCard::DoFormat()
{
    m_progress.SetRange32(1,16);
    for(int sector=0; sector<16; sector++) {
         m_msg.Format("Formating (%i/16)....",sector+1);
         UpdateData(FALSE);

         m_progress.SetPos(sector+1);
         BOOL flag = DoSector(sector);
         if(flag==0) return FALSE;
    }
    return TRUE;
}

void CFormatSmartCard::OnRadio1() 
{
	SetDlgItemText(IDOK,"Format old card");
}

void CFormatSmartCard::OnRadio2() 
{
	SetDlgItemText(IDOK,"Format new card");
}

void CFormatSmartCard::OnRadio3() 
{
	SetDlgItemText(IDOK,"Clean card data");
}
