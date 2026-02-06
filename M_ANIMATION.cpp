// M_ANIMATION.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "M_ANIMATION.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// M_ANIMATION dialog


M_ANIMATION::M_ANIMATION(CWnd* pParent /*=NULL*/)
	: CDialog(M_ANIMATION::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(M_ANIMATION)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void M_ANIMATION::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void M_ANIMATION::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(M_ANIMATION)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(M_ANIMATION, CDialog)
	//{{AFX_MSG_MAP(M_ANIMATION)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(M_ANIMATION, CDialog)
	//{{AFX_DISPATCH_MAP(M_ANIMATION)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IM_ANIMATION to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {ECC82F48-0E93-11D3-8243-DF2125695E5B}
static const IID IID_IM_ANIMATION =
{ 0xecc82f48, 0xe93, 0x11d3, { 0x82, 0x43, 0xdf, 0x21, 0x25, 0x69, 0x5e, 0x5b } };

BEGIN_INTERFACE_MAP(M_ANIMATION, CDialog)
	INTERFACE_PART(M_ANIMATION, IID_IM_ANIMATION, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// M_ANIMATION message handlers
