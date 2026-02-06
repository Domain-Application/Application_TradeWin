// DbScr.cpp : implementation fil
//

#include "stdafx.h"
#include "menu.h"
#include "DbScr.h"
#include "Browse.h"
#include "recno.h"
#include "EditDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbScr dialog

CDbScr::CDbScr(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CDialog(IDD, pParent)
{	
    //{{AFX_DATA_INIT(CDbScr)
	//}}AFX_DATA_INIT
    pDlg = this;
    df   = pdf;
    IdxItem = Idx;
    FirstField = 2;
    LastField  = int(df->sumfield);
}


void CDbScr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDbScr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDbScr, CDialog)
    //{{AFX_MSG_MAP(CDbScr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbScr message handlers

long CDbScr::ShowField1Table()
{
    CBrowse b1;
    b1.df = df;
    b1.IdxItem = IdxItem;
    b1.DoModal();
    df->rec = b1.no;
	return df->rec;
}

int CDbScr::OnTellMe(UINT nChar,int &id,int &fld)
{
	CString str;

    ReplaceFieldContent(fld);
    pf = GetMatchTable(fld,df);
    if(pf) {
        GetItem(fld,df);
	}
    ShowFieldContent(fld);
	ShowRecordState();
	return 1;
}

long CDbScr::GetItemTable(int fld)
{
	int     IdxItem;
	CString s;

    CBrowse b1;
    b1.df = pf;
    IdxItem = pf->f[1].idx & 1;
    b1.IdxItem = IdxItem;
    b1.DoModal();
    pf->rec = b1.no;
	if(pf->rec) {
		ridx(pf->rec,pf,IdxItem);
		SetDlgItemText(IDC_EDIT1+fld-1,pf->dat[1]);
		SetDlgItemText(IDC_LABEL1+fld-1,pf->dat[2]);
	}
	return pf->rec;
}

void CDbScr::GetItem(int fld,DB *df)
{
    long xrec,mrec;

    xrec = qd(df->dat[fld],pf);
    if(xrec==0) {
		pf->rec = GetItemTable(fld);
        if(pf->rec > 0) {
           mrec = ridx(pf->rec,pf,IdxItem);
           lset(df->dat[fld],pf->dat[1]);
        }
    }
}

void CDbScr::GetControlTable(int fld,DB *df,DB *tab)
{
    pf = tab;
	GetItem(fld,df);
}

void CDbScr::SetButton(int button_no,int button_sq,int IdNo,CString string)
{
	m_Buttons[button_no].iBitmap = button_sq;
	m_Buttons[button_no].idCommand = IdNo;
	m_Buttons[button_no].fsState = TBSTATE_ENABLED;
	m_Buttons[button_no].fsStyle = TBSTYLE_BUTTON;
	m_Buttons[button_no].dwData = 0;

	int nStringLength = string.GetLength() + 1;
	TCHAR * pString = string.GetBufferSetLength(nStringLength);
	pString[nStringLength] = 0;

	VERIFY((m_Buttons[button_no].iString = m_ToolBar.AddStrings(pString)) != -1);

	string.ReleaseBuffer();

}

void CDbScr::ShowFieldNo(int fld)
{
    CString s;

    s.Format("%i/%i",fld,df->sumfield);
    m_StatusBar.SetText(s,5,0);
    m_StatusBar.SetText(df->f[fld].name,6,0);
}

void CDbScr::CreateToolBarAndStatusBar()
{
	RECT rect;
	rect.top = rect.left = 0;
	rect.right = 100;
	rect.bottom = 20;
    m_StatusBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS | SBT_TOOLTIPS, rect, this, 0);

    int strPartDim[]= { 25,150, 200, 290, 340, 390, 460,520,-1};
    m_StatusBar.SetParts(9,strPartDim);
	m_StatusBar.SetText( df->name,1,0); 
    m_StatusBar.SetText( "File",0,0);
    m_StatusBar.SetText( "Records",2,0);
    m_StatusBar.SetText( "Field No",4,0);
    m_StatusBar.SetText( "Esc: Exit",8,0);
	m_StatusBar.FlashWindow(TRUE);

	VERIFY(m_ToolBar.Create(WS_VSCROLL | WS_CHILD | WS_VISIBLE | CCS_TOP | TTS_ALWAYSTIP , rect, this, 0));
	
	SetButton(0,0,IDOK,"&Save");
	SetButton(1,1,IDCANCEL,"&Cancel");
    SetButton(2,2,ID_PRINT,"Print");

	m_ToolBar.SetRows(3,TRUE,&rect);

	m_ToolBar.AddButtons(9,m_Buttons);
	m_ToolBar.AddBitmap(9,IDR_TOOLBAR_EDIT);
	CSize sizeButton(16,19);
	m_ToolBar.SetBitmapSize(sizeButton);
	CSize sizeBitmap(16,15);
	m_ToolBar.SetBitmapSize(sizeBitmap);
	m_ToolBar.AutoSize();
	m_ToolTip.Create(this);

    CString s;
    s.Format("%s Data Maintenance",df->name);
    m_ToolTip.AddTool(this,s);
    m_ToolBar.SetToolTips(&m_ToolTip);
	m_ToolTip.Activate(TRUE);
}	

void CDbScr::ShowBeginScreen()
{
	long last = read_sumindex(IdxItem,df);
	if(last>0) {
        if(df->rec<1 || df->rec>last) df->rec = last;
		ridx(df->rec,df,IdxItem);
        CMenu *p;
        p = GetMenu();
        p->EnableMenuItem(ID_PRINT,MF_ENABLED);
	}
	else {
        df->rec = 0;
		lsetblank(df);
		m_ToolBar.EnableButton(ID_PRINT,FALSE);
	}
	ShowInformation();
	ShowRecordState();
	SetAttribute(TRUE,TRUE);
    ShowFieldNo(1);
    m_ToolBar.HideButton(IDOK,TRUE);
    m_ToolBar.HideButton(ID_PRINT,FALSE);
    m_StatusBar.SetText( "",7,0);
}

void CDbScr::EditFieldSetting(int i)
{
    int  len;

    if(df->f[i].mode=='C' && (df->f[i].idx & 0x02)==0x02) {
        m_cb[i].SubclassDlgItem(CtrlId(i),this);
        m_cb[i].LimitText(1);
    }
    else if(df->f[i].mode=='D') {
        m_date[i].SubclassDlgItem(CtrlId(i),this);
    }
    else {
        m_ed[i].SubclassDlgItem(CtrlId(i),this);
        len = int(df->f[i].len);
        m_ed[i].SetLimitText(len);
    }
}

void CDbScr::InitialEditFieldSetting()
{
    EditFieldSetting(1);
    for(int i=FirstField; i<= LastField ; i++) {
        EditFieldSetting(i);
	}
}

BOOL CDbScr::OnInitDialog()
{
    CDialog::OnInitDialog();
	CreateToolBarAndStatusBar(); 
    InitialEditFieldSetting();
    ShowBeginScreen();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDbScr::ShowFieldContent(int i)
{
    char tmp[256];
	struct tm pst;

    if(df->f[i].mode=='D') {
        strcpy(tmp,df->dat[i]);
		if(isblank(tmp)) {
			CTime t(1980,1,1,0,0,0);
			m_date[i].SetTime(&t);
		}
		else {
			sever_date(&pst,tmp);
			CTime t(pst.tm_year,pst.tm_mon,pst.tm_mday,0,0,0);
			m_date[i].SetTime(&t);
		}
    }
    else {
        strcpy(tmp,df->dat[i]);
        rtrim(tmp);
        SetDlgItemText(CtrlId(i),tmp);
    }
    pf = GetMatchTable(i,df);
    if(pf) {
        qd(df->dat[i],pf);
        SetDlgItemText(IDC_LABEL1+i-1,pf->dat[2]);
    }
}

void CDbScr::ShowInformation()
{
	int  i;

    ShowFieldContent(1);
    for(i=FirstField; i<=LastField; i++ ) {
        ShowFieldContent(i);
	}
}

void CDbScr::ShowRecordState()
{
	char tmp[40];
    long last = read_sumindex(IdxItem,df);

    sprintf(tmp,"%ld/%ld",df->rec,last);
    m_StatusBar.SetText(tmp,3,0);
}

void CDbScr::SetAttribute(int flag,int fld)
{
    int  i,kflag;

    m_ed[1].SetReadOnly(flag);
    for(i=FirstField; i<=LastField; i++ ) {
        if(df->f[i].mode=='C' && (df->f[i].idx & 0x02)==0x02) {
            kflag = flag ? FALSE : TRUE;
            m_cb[i].EnableWindow(kflag);
        }
        else if(df->f[i].mode=='D') {
            kflag = flag ? FALSE : TRUE;
            m_date[i].EnableWindow(kflag);
        }
        else {
            m_ed[i].SetReadOnly(flag);
        }
    }
    m_ed[1].SetReadOnly(fld);
}

void CDbScr::SetEnable(int flag)
{
    int i;

    GetDlgItem(IDC_EDIT1)->EnableWindow(flag);
    for(i=FirstField; i<=LastField; i++ ) {
        GetDlgItem(IDC_EDIT1+i-1)->EnableWindow(flag);
    }
}

void CDbScr::ReplaceFieldContent(int i)
{
    double  value;
    CString str;

    if(df->f[i].mode=='D') {
		CTime t;
        int   year;
		m_date[i].GetTime(t);
        year = t.GetYear();
        str.Format("%4.4i%2.2i%2.2i",
            year,t.GetMonth(),t.GetDay());
        lset(df->dat[i],str);
    }
    else if(df->f[i].mode=='N') {
        GetDlgItemText(CtrlId(i),str);
        value = atof(str);
        dtodb(value,df,i);
    }
    else {
        GetDlgItemText(CtrlId(i),str);
        lset(df->dat[i],str);
    }
}

void CDbScr::ReplaceRecord()
{
    int i;

    ReplaceFieldContent(1);
    for(i=FirstField; i<=LastField; i++ ) {
        ReplaceFieldContent(i);
	}
}

void CDbScr::OnOK()
{
}

void CDbScr::OnCancel()
{
    CDialog::OnCancel();
}

BOOL CDbScr::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(	&m_ToolTip != NULL) {
		m_ToolTip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
