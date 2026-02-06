// Profile.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "profile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProfile dialog


CProfile::CProfile(CWnd* pParent /*=NULL*/)
	: CDialog(CProfile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProfile)
	m_e1 = _T("");
	m_e2 = _T("");
	m_e3 = _T("");
	m_e4 = _T("");
	m_e5 = _T("");
	m_e6 = _T("");
	//}}AFX_DATA_INIT
}


void CProfile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProfile)
	DDX_Text(pDX, IDC_EDIT1, m_e1);
	DDX_Text(pDX, IDC_EDIT2, m_e2);
	DDX_Text(pDX, IDC_EDIT3, m_e3);
	DDX_Text(pDX, IDC_EDIT4, m_e4);
	DDX_Text(pDX, IDC_EDIT5, m_e5);
	DDX_Text(pDX, IDC_EDIT6, m_e6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProfile, CDialog)
	//{{AFX_MSG_MAP(CProfile)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfile message handlers

void CProfile::OnButton1() 
{
    CDialog::OnOK();
}

void CProfile::OnButton2() 
{
	CDialog::OnCancel();	
}
