// dt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "dt.h"
#include "db_ado.h"
#include "openrs.h"
#include "TransferDirectTicketToMainSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double check_qty1,check_qty2;
/////////////////////////////////////////////////////////////////////////////
// CDt dialog

CDt::CDt(UINT IDD,DB *pdf, int Idx,CWnd* pParent /*=NULL*/)
	: CParent(IDD,pdf,Idx,pParent)
{
    //{{AFX_DATA_INIT(CDt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDt::DoDataExchange(CDataExchange* pDX)
{
	CParent::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDt)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDt, CParent)
    //{{AFX_MSG_MAP(CDt)
	ON_BN_CLICKED(IDC_BUTTON1, OnTransferToMain)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnUpdatePrice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int dtitem_initial(DB *df,int *fld)                //initial setting
{
	long dsrec = ds.Search(df->dat[1], 1);
	long dprec = dp.Search(df->dat[1], 1);
	if(dsrec>0 && dprec>0){
		fld[0] = Dti_ITEM;
		fld[1] = Dti_QTY1;
		fld[2] = Dti_PRICE1;
		fld[3] = Dti_ITEM2;
		fld[4] = Dti_QTY2;
		fld[5] = Dti_PRICE2;
		fld[6] = 0;		
	}
    else if(dsrec>0) {
		fld[0] = Dti_ITEM;
		fld[1] = Dti_QTY1;
		fld[2] = Dti_PRICE1;
		fld[3] = 0;
	}
    else if(dprec>0 ) {
		fld[0] = Dti_ITEM2;
		fld[1] = Dti_QTY2;
		fld[2] = Dti_PRICE2;	
		fld[3] = 0;
	}
	else{

	}
    return 1;
}

void CDt::SetAttribute(int flag,int fld)
{
    if(flag) {
		CString strMsg;
		long rec = ds.Search(df->dat[1], 1);
		long dprec = dp.Search(df->dat[1],1);
		if(rec>0 && dprec>0) {
			GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);        
		}
		else{
			GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
		}		
    }
    else {
        GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);        
    }
    CStaff::SetAttribute(flag,fld);
}

BOOL CDt::IsCannotEdit()
{
	long rec = ds.Search(df->dat[1], 1);
	long dprec = dp.Search(df->dat[1],1);
	if(rec>0 && dprec>0) {
        waitmsg("Already Transfer to Main System! can not be modified");
        return TRUE;
    }	
    return FALSE;
}

void getdirect(long *m,DB *df)
{
    long weight1,weight2;
	
    weight1 = atol(df->dat[Dt_PW1]);
    weight2 = atol(df->dat[Dt_PW2]);
    if(weight1>weight2) {
        m[2] = weight1; m[5] = weight2;
    }
    else {
        m[5] = weight2; m[2] = weight1;
    }
    weight1 = atol(df->dat[Dt_SW1]);
    weight2 = atol(df->dat[Dt_SW2]);
    if(weight1>weight2) {
        m[1] = weight1; m[4] = weight2;
    }
    else {
        m[4] = weight2; m[1] = weight1;
    }
	
    m[7]  = atol(df->dat[Dt_SW3]);
    m[8]  = atol(df->dat[Dt_PW3]);
	
    m[10] = labs(m[4]-m[1]) - m[7];
    m[11] = labs(m[5]-m[2]) - m[8];
	
    m[3]  = m[2]-m[1];
    m[6]  = m[5]-m[4];
    m[9]  = m[8]-m[7];
    m[12] = m[11]-m[10];
}

void CDt::ShowInformation()
{
	char str[20];

	CParent::ShowInformation();

	getdirect(m,df);
    ltot(m[3],6,str);
	SetDlgItemText(IDC_1STDIFF,putnum(str));
    ltot(m[6],6,str);
	SetDlgItemText(IDC_2NDDIFF,putnum(str));
    ltot(m[12],6,str);
	SetDlgItemText(IDC_NETT3,putnum(str));
    ltot(m[10],6,str);
    SetDlgItemText(IDC_NETT2,putnum(str));
    ltot(m[11],6,str);
    SetDlgItemText(IDC_NETT1,putnum(str));	
}

void CDt::ShowInitFrame()
{
    fg.Initial(&m_grid,sub,2);

	fg.SetTitle("No",500,1);
    fg.SetTitle("Code",1000,1);
    fg.SetTitle("Particular",2250,1);
    fg.SetTitle("Qty(Purchase)",1100,7);
    fg.SetTitle("Price",700,7);
    fg.SetTitle("Amount",900,7);

	fg.SetTitle("",90,7);
    fg.SetTitle("Code",1000,1);
    fg.SetTitle("Particular",2250,1);
    fg.SetTitle("Qty(Sales)",800,7);
    fg.SetTitle("Price",700,7);
    fg.SetTitle("Amount",900,7);
}

void CDt::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
	fg.show(rec);
    fg.SetText(sub,0,Dti_ITEM,Dti_ITEM | 0x200,
                     Dti_QTY1,Dti_PRICE1,Dti_AMOUNT1,-1);
	fg.SetText("");
    fg.SetText(sub, Dti_ITEM2,Dti_ITEM2 | 0x200,
                    Dti_QTY2,Dti_PRICE2,Dti_AMOUNT2,-1);
}


int AdminDb_dt(DB *df,int flag)
{
    DirectDeliveryTicket();
    return 1;
}

void FindSellerPrice(DB *df)
{
    CString str;
    str.Format("select [price] from [spitem] where [pt]='%s' and [accode]='%s'",
        df->dat[Dti_ITEM],df->e->parent->dat[Dt_SUPPLIER]);

    double price = SqlValue(str);

    dtodb(price, df, Dti_PRICE1);

    str.Format("select [price] from [spitem] where [pt]='%s' and [accode]='%s'",
        df->dat[Dti_ITEM],df->e->parent->dat[Dt_BUYER]);

    price = SqlValue(str);
    dtodb(price, df, Dti_PRICE2);
}

int tell_dtitem(UINT nChar,int &id,int &fld,DB *df,int sq)
{	
	double totalqty;

    switch(fld) {
    case Dti_ITEM:
        if(sq) {
         //   FindSellerPrice(df);
			Purchase_Flag = TRUE;
		//	dtodb(ChangePrice(dt.dat[Dt_SUPPLIER],df->dat[fld]),df,Dti_PRICE1);
        }
        break;
	case Dti_ITEM2:
		if(sq) {
			Purchase_Flag = FALSE;
		//	dtodb(ChangePrice(dt.dat[Dt_BUYER],df->dat[fld]),df,Dti_PRICE2);
        }
        break;
	case Dti_AMOUNT1:
	case Dti_AMOUNT2:
		id = DO;
		break;
    }

    double total = atof(df->dat[Dti_QTY1]) * atof(df->dat[Dti_PRICE1]);
    dtodb(total,df,Dti_AMOUNT1);
     total = atof(df->dat[Dti_QTY2]) * atof(df->dat[Dti_PRICE2]);
    dtodb(total,df,Dti_AMOUNT2);
	
    return 2;
}

int tell_dt(UINT nChar,int &id,int &fld,DB *df,int sq)
{
    switch(fld) {
    case Dt_IC:
        if(sq) {
            qd(df->dat[Dt_IC],&driver);
            lset(df->dat[Dt_DRIVER],driver.dat[2]);
        }
        break;
    }
	check_qty1 = fabs(df->val(Dt_PW1)-df->val(Dt_PW2))-df->val(Dt_PW3);
	check_qty2 = fabs(df->val(Dt_SW1)-df->val(Dt_SW2))-df->val(Dt_SW3);
    return 2;
}

int dt_save(DB *df,int flag)
{
	CString strqty1,strqty2;
	double sum_value1,sum_value2;	
	
	strqty1.Format("SELECT SUM(QTY1) FROM DTITEM WHERE TICKET ='%s'",df->dat[Dti_REFNO]);
	strqty2.Format("SELECT SUM(QTY2) FROM DTITEM WHERE TICKET ='%s'",df->dat[Dti_REFNO]);
	
	sum_value1 = SqlValue(strqty1);
	sum_value2 = SqlValue(strqty2);

	if(!isblank(df->dat[Dt_PW1]) || !isblank(df->dat[Dt_PW2])){
		if(sum_value1 > check_qty1)
		{
			AfxMessageBox("PURCHASE OVERLOAD");
			return 0;
		}
	}

	if(!isblank(df->dat[Dt_SW1]) || !isblank(df->dat[Dt_SW2])){
		if(sum_value2 > check_qty2)
		{
			AfxMessageBox("SALES OVERLOAD");
			return 0;
		}
	}
	return 1;
}

int dtitem_save(DB *df,int flag)
{
	CString strqty1,strqty2;
	double sum_value1,sum_value2,total_qty1,total_qty2;	

	strqty1.Format("SELECT SUM(QTY1) FROM DTITEM WHERE TICKET ='%s'",df->dat[Dti_REFNO]);
	strqty2.Format("SELECT SUM(QTY2) FROM DTITEM WHERE TICKET ='%s'",df->dat[Dti_REFNO]);

	sum_value1 = SqlValue(strqty1);
	sum_value2 = SqlValue(strqty2);

	total_qty1 = fabs(atof(df->bak[Dti_QTY1]) - atof(df->dat[Dti_QTY1]));
	total_qty1 = total_qty1+sum_value1;

	total_qty2 = fabs(atof(df->bak[Dti_QTY2]) - atof(df->dat[Dti_QTY2]));
	total_qty2 = total_qty2+sum_value2;

	if(!isblank(dt.dat[Dt_PW1]) || !isblank(dt.dat[Dt_PW2])){
		if(total_qty1 > check_qty1){
			AfxMessageBox("PURCHASE QTY OVERLOADED");
			return 0;
		}
	}

	if(!isblank(dt.dat[Dt_SW1]) || !isblank(dt.dat[Dt_SW2])){
		if(total_qty2 > check_qty2){
			AfxMessageBox("SALES QTY OVERLOADED");
			return 0;
		}
	}
	return 1;
}

int start_dt(DB *df)
{
    df->FldDefault(2);
	_strtime(df->dat[Dt_TIMEIN]);
	_strtime(df->dat[Dt_TIMEOUT]);
    return 1;
}

void init_dt(DB *df)
{
    df->e->AdminDb = AdminDb_dt;
    df->e->start   = start_dt;
    df->e->tell = tell_dt;
	df->e->save = dt_save;
}

void init_dtitem(DB *df)
{
	df->e->initial =dtitem_initial;
    df->e->tell = tell_dtitem;
	df->e->save = dtitem_save;
}

void DirectDeliveryTicket()
{

    CDt p1(IDD_DT,&dt,1);
    p1.sub       = &dtitem;
    p1.IDD_SUB   = IDD_DTITEM;
	p1.m_WinPrint = TRUE;
    p1.DoModal();

}


void CDt::OnTransferToMain() 
{
	if (isblank(df->dat[Dt_VEHICLE])){
		AfxMessageBox("Please key in vehicle no.");
		return;
	}
	CString strMsg;
    long rec = ds.Search(df->dat[1], 1);
    if(rec>0) {
		strMsg.Format("Ticket %s already Existed at Receive Data Entry",df->dat[1]);
		AfxMessageBox(strMsg);
	//	return;
	}
	
    long irec = dp.Search(df->dat[1],1);
    if(irec>0){
		strMsg.Format("Ticket %s already Existed at Despatch Data Entry",df->dat[1]);
		AfxMessageBox(strMsg);
	//	return;
	}
	
	if(rec>0 && irec>0){
		return;
	}
	else{
		CTransferDirectTicketToMainSystem h1;
		h1.df  = df;
		h1.sub = sub;
		h1.DoModal();
		SetAttribute(1,1);
		ShowInformation();
	}
}

void CDt::OnButton2() 
{
	
}

void CDt::OnUpdatePrice() 
{
	DirectDeliveryPricingUpdate();
	ShowInformation();
}