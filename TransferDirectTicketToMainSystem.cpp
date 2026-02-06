// TransferDirectTicketToMainSystem.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "TransferDirectTicketToMainSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferDirectTicketToMainSystem dialog


CTransferDirectTicketToMainSystem::CTransferDirectTicketToMainSystem(CWnd* pParent /*=NULL*/)
	: CDialog(CTransferDirectTicketToMainSystem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferDirectTicketToMainSystem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTransferDirectTicketToMainSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferDirectTicketToMainSystem)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferDirectTicketToMainSystem, CDialog)
	//{{AFX_MSG_MAP(CTransferDirectTicketToMainSystem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferDirectTicketToMainSystem message handlers

void CTransferDirectTicketToMainSystem::OnOK() 
{
	/*
	CString strMsg;
    long rec = ds.Search(df->dat[1], 1);
    if(rec>0) {
		strMsg.Format("Ticket %s already Existed at Receive Data Entry",df->dat[1]);
		AfxMessageBox(strMsg);
		return;
	}
	
    long irec = dp.Search(df->dat[1],1);
    if(irec>0){
		strMsg.Format("Ticket %s already Existed at Despatch Data Entry",df->dat[1]);
		AfxMessageBox(strMsg);
		return;
	}
	*/
	int No;
	long wt;
	long dsrec = ds.Search(df->dat[1], 1);
    if(dsrec<=0) {
		long rec,irec;
		lsetblank(&ds);
		fetch(&ds,df);
		No = 0;
		wt = 0;
		long Srec = interval(&irec, df->dat[1], sub, 1);
		m_progress.SetRange32(1,Srec);
		for(rec=0; rec<Srec; rec++) {
			m_progress.SetPos(rec+1);
			ridx(rec+irec, sub, 1);
			long xrec = prodtype.Search(sub->dat[Dti_ITEM],1);
			if(xrec>0) {
				lset(ds.dat[DP_PT1 + No*4], sub->dat[Dti_ITEM]);
				rset(ds.dat[DP_QTY1 + No*4], sub->dat[Dti_QTY1]);
				rset(ds.dat[DP_WT1 + No*4], sub->dat[Dti_PRICE1]);
				rset(ds.dat[DP_AMT1 + No], sub->dat[Dti_AMOUNT1]);			
				wt += (long) sub->val(Dti_QTY1);
				No++;
			}
		}
		ltodb(wt, &ds, DP_SGROSS);
		lset(ds.dat[1] , df->dat[1]);
		lset(ds.dat[DP_PO] , df->dat[Dt_PO]);
		lset(ds.dat[DP_ACCODE] , df->dat[Dt_SUPPLIER]);
		lset(ds.dat[DP_UNIT] , "KG");
		_strtime(ds.dat[DP_TIMEIN]);
		_strtime(ds.dat[DP_TIMEOUT]);
		//FindProductCategorySerialNo(&ds);
		if (No>0){
			ds.AddNew();
		}
	}
    long dprec = dp.Search(df->dat[1], 1);
    if(dprec<=0) {
		long rec,irec;
		lsetblank(&dp);
		fetch(&dp,df);
		No = 0;
		wt = 0;
		long Srec = interval(&irec, df->dat[1], sub, 1);
		m_progress.SetRange32(1,Srec);
		for(rec=0; rec<Srec; rec++) {
			m_progress.SetPos(rec+1);
			ridx(rec+irec, sub, 1);
			long xrec = prodtype.Search(sub->dat[Dti_ITEM2],1);
			if(xrec>0) {
				lset(dp.dat[DP_PT1 + No*4], sub->dat[Dti_ITEM2]);
				rset(dp.dat[DP_QTY1 + No*4], sub->dat[Dti_QTY2]);
				rset(dp.dat[DP_WT1 + No*4], sub->dat[Dti_PRICE2]);
				rset(dp.dat[DP_AMT1 + No], sub->dat[Dti_AMOUNT2]);
				wt += (long) sub->val(Dti_QTY2);
				No++;
			}
		}
		lset(dp.dat[1] , df->dat[1]);
		lset(dp.dat[DP_PO] , df->dat[Dt_SO]);
		lset(dp.dat[DP_ACCODE] , df->dat[Dt_BUYER]);
		lset(dp.dat[DP_UNIT] , "KG");
		ltodb(wt, &dp, DP_SGROSS);
		_strtime(dp.dat[DP_TIMEIN]);
		_strtime(dp.dat[DP_TIMEOUT]);
		//FindProductCategorySerialNo(&dp);
		if (No>0){
			dp.AddNew();
		}
	}
	AfxMessageBox("Transfer Completed");
	CDialog::OnOK();
}
