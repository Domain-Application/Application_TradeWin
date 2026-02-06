// Incentive.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Incentive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIncentive dialog


CIncentive::CIncentive(CWnd* pParent /*=NULL*/)
	: CDialog(CIncentive::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIncentive)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIncentive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIncentive)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIncentive, CDialog)
	//{{AFX_MSG_MAP(CIncentive)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIncentive message handlers
