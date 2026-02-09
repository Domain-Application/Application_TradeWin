// Spitem.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Spitem.h"
#include "PrintDlg.h"
#include "mgrid.h"
#include "db_ado.h"
#include "einv.h"

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
	df->m_CannotAdd = TRUE;
}

BEGIN_MESSAGE_MAP(CSpitem, CParent)
	//{{AFX_MSG_MAP(CSpitem)
	ON_BN_CLICKED(IDC_BUTTON1, OnEinv)
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

int tell_einv_view(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    switch(fld) {
    case 10:
        if(sq)
        {
            qd(df->dat[GetDbFieldNo(df, "POSTCODE")], &postcode);
            lset(df->dat[GetDbFieldNo(df, "MALAYSIA_STATE")], postcode.dat[GetDbFieldNo(&postcode, "MALAYSIA_STATE")]);
            lset(df->dat[GetDbFieldNo(df, "MALAYSIA_CITY")], postcode.dat[GetDbFieldNo(&postcode, "CITY_NAME")]);
        }
        break;
	default:
		break;
    }
    return 2;
}

void init_einv_view(DB *df)
{
	df->e->tell = tell_einv_view;
}

void CSpitem::OnEinv() 
{
    if(SaveFlag == NEWFILE){
        long nrec = getdelrec(df, FALSE);
        save_entry(NONEXIST,IdxItem,nrec,df);
		SaveFlag = OLDFILE;
		ShowInformation();
    }
	if( SaveFlag == CANNOTSAVE) return;
    CString strSQL,dbName;
	dbName.Format("%s_einv", df->name);
    DB adoTmp;
    strSQL.Format("select * from %s where %s = '%s'", dbName, df->fieldName[1], df->dat[1]); //Note: create a view where contains code, field from TIN UNTIL EINV_EMAIL
    ado(&adoTmp, dbName, init_einv_view, strSQL);
    long rec = read_sumindex(0, &adoTmp);
    if(rec <= 0)
    {
		dclose(&adoTmp);
        return;
    }
    readrec(1, &adoTmp);
    CEinv p;
    p.df = &adoTmp;
    p.isFFBProgram = TRUE;
    UINT ID = p.DoModal();
    if(ID == IDOK)
    {
        fetch(df, &adoTmp);
    }
    dclose(&adoTmp);
	ShowInformation();
}