// GwEdit.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "GwEdit.h"
#include "GetWt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGwEdit

CGwEdit::CGwEdit()
{
    flag = lastkey = 0;
}

CGwEdit::~CGwEdit()
{
}


BEGIN_MESSAGE_MAP(CGwEdit, CEdit)
	//{{AFX_MSG_MAP(CGwEdit)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGwEdit message handlers

UINT CGwEdit::OnGetDlgCode() 
{
	return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void CGwEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int ShiftKeyStatus = GetKeyState(VK_SCROLL);
	if(ShiftKeyStatus<=0) {
		GwstWeight = 0;	
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

}

void CGwEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int ShiftKeyStatus = GetKeyState(VK_SCROLL);
	if(ShiftKeyStatus>0) {
		if(nChar>=1 && nChar<=15) {
			if(nChar==8 && nFlags==14){  // BACKSPACE
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
			else if(nChar==13 && (nFlags==284 || nFlags==28)){  // ENTER
				CEdit::OnChar(nChar, nRepCnt, nFlags);
			}
			else{
				if(FacileFlag == 1) GwstWeight = atol(gst.dat[nChar+1]);//Jaxn on 2008/01/22, enum 1 is primary key, need to start from 2		
					if(!(nChar==8 && nFlags==35)){
						CEdit::OnChar(nChar, nRepCnt, nFlags);
					}
			}
		}
		else if(nChar==32) {
			if(lastkey==32) GwstWeight = 0;	
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else CEdit::OnChar(nChar, nRepCnt, nFlags);
    lastkey = nChar;
}

void CGwEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    CGetWt *p;

    p = (CGetWt *) GetParent();
    if(nChar==VK_RETURN && flag==1) {
         p->OnOK();
	}
    else if(nChar==VK_ESCAPE) {
        p->OnCancel();
	}
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
    flag = 1;
}
