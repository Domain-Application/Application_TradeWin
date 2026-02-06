// EmodeAmt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "EmodeAmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmodeAmt dialog


CEmodeAmt::CEmodeAmt(CWnd* pParent /*=NULL*/)
	: CDialog(CEmodeAmt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEmodeAmt)
	m_label = _T("");
	m_wac = _T("");
	m_amt = _T("");
	//}}AFX_DATA_INIT
}


void CEmodeAmt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmodeAmt)
	DDX_Text(pDX, IDC_EMODEAMT, m_label);
	DDX_Text(pDX, IDC_INVNO, m_wac);
	DDX_Text(pDX, IDC_EDIT1, m_amt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmodeAmt, CDialog)
	//{{AFX_MSG_MAP(CEmodeAmt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmodeAmt message handlers

BOOL CEmodeAmt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEmodeAmt::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
