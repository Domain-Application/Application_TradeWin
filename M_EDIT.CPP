// m_edit.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "m_edit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// m_edit dialog


m_edit::m_edit(CWnd* pParent /*=NULL*/)
	: CDialog(m_edit::IDD, pParent)
{
	//{{AFX_DATA_INIT(m_edit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void m_edit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(m_edit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(m_edit, CDialog)
	//{{AFX_MSG_MAP(m_edit)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// m_edit message handlers
