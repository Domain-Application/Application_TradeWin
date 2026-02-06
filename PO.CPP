// PO.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "PO.h"
#include "db_ado.h"
#include "openrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL Purchase_Flag;
/////////////////////////////////////////////////////////////////////////////
// CPO dialog

CPO::CPO(UINT IDD,DB *pdf, int Idx,CWnd* pParent /*=NULL*/)
	: CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CPO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPO::DoDataExchange(CDataExchange* pDX)
{
	CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPO)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPO, CParent)
    //{{AFX_MSG_MAP(CPO)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPO::ShowInitFrame()
{
    fg.Initial(&m_grid,sub,2);

	fg.SetTitle("No",500,1);
    fg.SetTitle("Scrap Code",2000,1);
    fg.SetTitle("Particular",4500,1);
    fg.SetTitle("Category",1000,1);
    fg.SetTitle("Qty",900,7);
    fg.SetTitle("Price",900,7);
    fg.SetTitle("Amount",1200,7);
}

void CPO::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
	fg.show(rec);
    fg.SetText(sub,0,POD_ITEM,POD_NAME,
                     POD_CATEGORY,POD_QTY,POD_PRICE,POD_AMOUNT,-1);
}

void CPO::OnDetail()
{
	long Srec = qd(df->dat[PO_SUPPLIER],&accode);
	if (!Srec){
		AfxMessageBox("Please key in Registered Account Code.");
		return;
	}
	CParent::OnDetail();
}

extern void PurchaseOrderData();
int AdminDb_po(DB *df,int flag)
{
    PurchaseOrderData();
    return 1;
}

void FindSpitem(DB *df)
{
    CString str;
    str.Format("select [price] from [spitem] where [pt]='%s' and [accode]='%s'",
        df->dat[POD_ITEM],df->e->parent->dat[PO_SUPPLIER]);

    double price = SqlValue(str);
    if(price==0) {
        price = (Purchase_Flag) ? prodtype.val(PT_COST) : prodtype.val(PT_SALES);
    }
    dtodb(price, df, POD_PRICE);

}

int tell_poitem(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    switch(fld) {
    case POD_ITEM:
        if(sq) {
            lset(df->dat[POD_NAME],prodtype.dat[2]);
			qd(prodtype.dat[PT_PTGROUP],&ptgroup);
            lset(df->dat[POD_CATEGORY],ptgroup.dat[PTGROUP_CATEGORY]);
         //   FindSpitem(df);
		//	dtodb(ChangePrice(df->e->parent->dat[PO_SUPPLIER],df->dat[fld]),df,POD_PRICE);
        }
        break;
    case POD_CATEGORY:
        id = DO;
        break;
    }

    double total = atof(df->dat[POD_QTY]) * atof(df->dat[POD_PRICE]);
    dtodb(total,df,POD_AMOUNT);

    return 2;
}

int start_po(DB *df)
{
    df->FldDefault(2);
    return 1;
}

void init_po(DB *df)
{
    df->e->AdminDb = AdminDb_po;
    df->e->start   = start_po;
}

void init_poitem(DB *df)
{
    df->e->tell = tell_poitem;
}

void PurchaseOrderData()
{

    Purchase_Flag = TRUE;
    CPO p1(IDD_PO,&po,1);
    p1.sub       = &poitem;
    p1.IDD_SUB   = IDD_POITEM;
	p1.NonDupArray[0] = POD_ITEM;
    p1.DoModal();

}

void SalesOrderData()
{
    Purchase_Flag = FALSE;
    CPO p1(IDD_PO,&so,1);
    p1.sub       = &soitem;
    p1.IDD_SUB   = IDD_POITEM;
	p1.NonDupArray[0] = POD_ITEM;
    p1.DoModal();
}


