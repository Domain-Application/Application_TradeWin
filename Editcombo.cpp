// EditCombo.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "EditCombo.h"
#include "staff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCombo

CEditCombo::CEditCombo()
{
}

CEditCombo::~CEditCombo()
{
}


BEGIN_MESSAGE_MAP(CEditCombo, CComboBox)
	//{{AFX_MSG_MAP(CEditCombo)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCombo message handlers

void CEditCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CStaff *p;
	int fld,Id;

	MessageBox("ok");
	p = (CStaff *) GetParent();
	Id  = GetDlgCtrlID();
    fld = (Id-IDC_EDIT1+1);
    if(nChar==VK_UP) {
        if(fld==1) {
            nChar = VK_RETURN;
        }
        else {
            p->ReplaceFieldContent(fld);
            if(fld==p->FirstField) {
                p->GotoDlgCtrl(p->GetDlgItem(IDC_EDIT1+p->LastField-1));
            }
            else {
                p->PrevDlgCtrl();
            }
        }
	}
    else if(nChar==VK_DOWN || nChar==VK_TAB) {
        if(fld==1) {
            nChar = VK_RETURN;
        }
		else {
            p->ReplaceFieldContent(fld);
            if(fld==p->LastField) {
                p->GotoDlgCtrl(p->GetDlgItem(IDC_EDIT2));
            }
            else {
                p->NextDlgCtrl();
            }
        }
	}
    else if(nChar==VK_ESCAPE) {
        p->OnCancel();
	}
    if(fld==1) {
        if(nChar == VK_PRIOR || nChar == VK_NEXT) {
            nChar = VK_RETURN;
        }
    }
    if(nChar == VK_RETURN) {
        Id = p->OnTellMe(nChar,Id,fld);
        if(Id) {
            if(fld==p->LastField) {
                p->GotoDlgCtrl(p->GetDlgItem(IDC_EDIT2));
            }
            else {
                p->NextDlgCtrl();
            }
        }
	}
	else {
        CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}

void CEditCombo::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CStaff *p;
	int fld,Id;

	p = (CStaff *) GetParent();
	
	Id  = GetDlgCtrlID();
    fld = (Id-IDC_EDIT1+1);
    p->ShowFieldNo(fld);
    if(fld!=1) {
        if(nChar == VK_F12) {
            p->OnSave();
        }
    }
	CComboBox::OnKeyUp(nChar, nRepCnt, nFlags);
}

UINT CEditCombo::OnGetDlgCode() 
{
	// TODO: Add your message handler code here and/or call default
	
    return CComboBox::OnGetDlgCode() | DLGC_WANTALLKEYS;
}
