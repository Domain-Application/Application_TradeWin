// Spitem.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Spitem.h"
#include "PrintDlg.h"
#include "mgrid.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpitem dialog

CSpitem::CSpitem(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CParent(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CSpitem)
	//}}AFX_DATA_INIT
}

void CSpitem::DoDataExchange(CDataExchange* pDX)
{
    CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpitem)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpitem, CParent)
	//{{AFX_MSG_MAP(CSpitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSpitem::OnInitDialog()
{
    CParent::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CSpitem message handlers

void CSpitem::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    fg.Initial(grid,sub,2);
    fg.SetTitle("No",400);
    fg.SetTitle("Item Code",1200);
    fg.SetTitle("Product Grade",1500,1);
    fg.SetTitle("Buyer Product Name",2500);
}

void CSpitem::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    fg.show(rec);
    fg.SetText(sub,NULL,Sp_ITEM,Sp_PRODGRADE,Sp_BUYERPTNAME,-1);
}

BEGIN_EVENTSINK_MAP(CSpitem, CParent)
    //{{AFX_EVENTSINK_MAP(CSpitem)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void AccodeMaintenance()
{
    CSpitem p1(IDD_ACCODE,&accode,1);
    p1.sub       = &spitem;
    p1.IDD_SUB   = IDD_SPITEM;
	p1.NonDupArray[0] = Sp_ITEM;
    p1.m_WinPrint = TRUE;
    p1.DoModal();
}

int accodes_save(DB *df,int flag)
{
//	if(isblank(df->dat[ACCODE_ACCODEHQ])){
//		AfxMessageBox("HQ. ACC. CODE. cannot be blank.");
//		return 0;
//	}
	/*
	if(!isblank(df->dat[ACCODE_BASIS])){
		if (strncmp(df->dat[ACCODE_BASIS],"O",1))				
			if (strncmp(df->dat[ACCODE_BASIS],"P",1)){
				waitmsg("Basis - O(Own) or P(Party)");
				return 0;
			}
	}

	if(isblank(df->dat[ACCODE_TAXCODE]) || df->dat[ACCODE_TAXCODE]==" " ){
		AfxMessageBox("Please assign Taxcode.");
		return 0;
	}
	*/
	return 1;
}

int tell_accode(UINT nChar,int &id,int &fld,DB *df,int sq)
{  
/*
	switch(fld) {
		
	case ACCODE_BASIS:		
        if(sq){
            if (strncmp(df->dat[fld],"O",1))				
				if (strncmp(df->dat[fld],"P",1)){
					waitmsg("Basis - O(Own) or P(Party)");
					lset(df->dat[fld],"O");
					return 2;
				}
        }
        break; 
	
	}*/
	return 1;
}

void init_accodes(DB *df)
{
	df->e->save	= accodes_save;
	df->e->tell = tell_accode;
}