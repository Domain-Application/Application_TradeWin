// EditToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "staff.h"
#include "EditToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditToolBar

CEditToolBar::CEditToolBar()
{
}

CEditToolBar::~CEditToolBar()
{
}


BEGIN_MESSAGE_MAP(CEditToolBar, CToolBarCtrl)
	//{{AFX_MSG_MAP(CEditToolBar)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditToolBar message handlers

void CEditToolBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CToolBarCtrl::OnMouseMove(nFlags, point);
}

void CEditToolBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CToolBarCtrl::OnPaint() for painting messages
}
