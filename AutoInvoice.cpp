// AutoInvoice.cpp : implementation file
//
#include "stdafx.h"
#include "menu.h"
#include "db_ado.h"

extern DB invwb,tmp_inv,tmp_oinv;
double paymentAmt = 0;

void DoAutoInvoice(DB *df,DB *invmain,DB *invsub)
{
	CString str,strSql;
	str.Format("Select INVOICE from [%s] where [DO]='%s'",invsub->name,df->dat[1]);
	CString strInv = SqlText(str);
	if (strInv.GetLength()>0){
		str.Format("Ticket No : %s already existed in Invoice : %s.\r\nPlease rectify",df->dat[1],strInv);
		AfxMessageBox(str);
	/*
	if(AfxMessageBox("Do you want to reprint Ticket?", MB_ICONQUESTION | MB_YESNO)==IDYES){
	//df->PrintOneFormOnly();
	df->m_PrintOneFormOnly = TRUE;
	if(!stricmp(df->name,"ds")){
	PrintOneForm(df,"ds_gst.rpt");
	}
	else{
	PrintOneForm(df,"dp_gst.rpt");
	}
			}
	*/
		return;
	}

	if(AfxMessageBox("Auto create Invoice?", MB_ICONQUESTION | MB_YESNO)==IDNO) return;

	CString strTable = (Purchase_Flag) ? "DSINV_NOPO" : "DPINV_NOSO";
    
	strSql.Format("select * from [%s] where [TICKET]='%s' and [DO] IS NULL",
		strTable,df->dat[1]);

	ado(&invwb,"",NULL,strSql);
	long last = read_sumindex(1,&invwb);
	if (last>0){
		readrec(1,&invwb);
		int fldAccode = GetDbFieldNo(df,"ACCODE");
		qd(df->dat[fldAccode],&accode);
		lsetblank(invmain);
		int fldDate = GetDbFieldNo(df,"Date");
		lset(invmain->dat[IVWB_DATE], df->dat[fldDate]); 
		
		FindAddInfoTicketNo(invmain);	
		lset(invmain->dat[IVWB_ACCODE], accode.dat[1]);    
		FindTaxCode(invmain,Purchase_Flag);
		int fldPtype = GetDbFieldNo(df,"Ptype");

		if(strcmp(df->dat[fldPtype],"01   ")==0 || strcmp(df->dat[fldPtype],"CASH ")==0 ){  // cash 
			lset(invmain->dat[IVWB_TERMS],"CASH");
		}
		//lset(invmain->dat[IVWB_STATUS],"P");
		lset(invmain->dat[IVWB_CURRENCY],"RM");
		dtodb(1,invmain,IVWB_RATE);
		rset(invmain->dat[IVWB_AMOUNT], invwb.dat[5]);
	 
		SetSubTotal(invmain);
		invmain->AddNew();
		if(invmain->m_SaveFlag==FALSE) {
			cnn->RollbackTrans();
			dclose(&invwb);
			return;
		
		}
		lsetblank(invsub);
		lset(invsub->dat[1], invmain->dat[1]);
		invsub->Find_SequenceNo();
		lset(invsub->dat[VIWB_DO], invwb.dat[1]);
		rset(invsub->dat[VIWB_QTY], invwb.dat[4]);
		rset(invsub->dat[VIWB_AMOUNT], invwb.dat[5]);
		lset(invsub->dat[GetDbFieldNo(invsub, "CLASSIFICATION")], "022");

		invsub->AddNew();
		if(invsub->m_SaveFlag==FALSE) {
			cnn->RollbackTrans();
			dclose(&invwb);
			return;
			
		}
		paymentAmt = invwb.val(5);

		//if(AfxMessageBox("Do you want to print Invoice?", MB_ICONQUESTION | MB_YESNO)==IDNO) return;
		//invmain->PrintOneFormOnly();
		//df->PrintOneFormOnly();
		/*
		df->m_PrintOneFormOnly = TRUE;
		if(!stricmp(df->name,"ds")){
			PrintOneForm(df,"ds_gst.rpt");
		}
		else{
			PrintOneForm(df,"dp_gst.rpt");
		}*/		
	}
	dclose(&invwb);
}

void OnDoAutoPayment(DB *df)
{
	if (isblank(profile.dat[PF_PC])){
		AfxMessageBox("Please set Petty Cash GLCODE at Account profile in ACC System.\r\nPlease re-do payment at ACC System");
		return;
	}
	//double paymentAmt = PurchaseInvoiceAmt - TaxInvoiceAmt;
	
	DB apipay,appaydat;
	tado(&apipay,"apipay");
	tado(&appaydat,"appaydat");
	
	int fldDate = GetDbFieldNo(df,"DATE");
	int fldAccode = GetDbFieldNo(df,"ACCODE");

	lsetblank(&apipay);
	FindAddInfoTicketNo(&apipay);	
	lset(apipay.dat[IP_DATE],df->dat[fldDate]);
	lset(apipay.dat[IP_CUSTOMER],df->dat[fldAccode]);
	lset(apipay.dat[IP_CURR],profile.dat[PF_CURRENCY]);
    lset(apipay.dat[IP_PAYTYPE],"C");
	lset(apipay.dat[IP_CHEQUE],"CASH");
	lset(apipay.dat[IP_CQ_DATE],df->dat[fldDate]);
	lset(apipay.dat[IP_REMARKS],"Auto Cash Payment");
	dtodb(1,&apipay,IP_RATE);
	dtodb(paymentAmt,&apipay,IP_TOTAL);
	//dtodb(paymentAmt,&apipay,IP_RM);
	lset(apipay.dat[IP_GLCODE],profile.dat[PF_PC]);
	apipay.AddNew();
	if(apipay.m_SaveFlag==FALSE) {
        cnn->RollbackTrans();
		dclose(&apipay);
		dclose(&appaydat);
        return;
    }	
	lsetblank(&appaydat);
	lset(appaydat.dat[1],apipay.dat[1]);
	lset(appaydat.dat[PY_INVOICE],apinv_wb.dat[1]);
	lset(appaydat.dat[PY_DATE],df->dat[fldDate]);
	dtodb(paymentAmt,&appaydat,PY_TOTAL);
	appaydat.AddNew();
	if(appaydat.m_SaveFlag==FALSE) {
        cnn->RollbackTrans();
		dclose(&apipay);
		dclose(&appaydat);
        return;
    }
	if(AfxMessageBox("Do you want to print Payment Voucher?", MB_ICONQUESTION | MB_YESNO)==IDYES){
		apipay.PrintOneFormOnly();
	}

	dclose(&apipay);
	dclose(&appaydat);
}

extern BOOL TRansferToAP(char *key,DB *df);
extern BOOL TRansferToAR(char *key,DB *df);
void DoInvoiceExportAndPayment(DB *df)
{
	double paymentAmt = 0;
	char key[100];
	CString strInvoice;	
	DB adoTmp,payTmp;
	long xrec = 0;
	
	if(!stricmp(df->name,"ds") || !stricmp(df->name,"scale")) {
		if (isblank(profile.dat[PF_PURCHASE])){
			waitmsg("PURCHASE GLCODE NOT SET.PLEASE CHECK ACC PROFILE");		
			return;
		}
		tado(&tmp_inv,"apinv");
		tado(&tmp_oinv,"apoinv");

		DoAutoInvoice(df,&apinv_wb,&apoinv_wb);

		xrec = apoinv_wb.Search(df->dat[1],VIWB_DO,TRUE);
		if (xrec ==0)return;	//create failed

		strcpy(key,apoinv_wb.dat[1]);
		strInvoice.Format("SELECT * FROM [APINV_WB] where [INVOICE]='%s'",key);
		ado(&adoTmp,"adoTmp",NULL,strInvoice);
		long Srec = read_sumindex(0,&adoTmp);
		if (Srec){
			strcpy(key,adoTmp.dat[1]);
			for(long rec=1;rec<=Srec; rec++){
				readrec(rec,&adoTmp);
				lset(key,adoTmp.dat[1]);
				xrec =  apinv_wb.Search(key,1,TRUE);
				if (xrec){
					if (TRansferToAP(key,&adoTmp)){
						lset(apinv_wb.dat[IVWB_STATUS],"1");
						apinv_wb.Update();
					}
				}
			}		
		}
		OnDoAutoPayment(df);
		
		tado(&payTmp,"paymentdate");
		int fldDate = GetDbFieldNo(df,"DATE");

		xrec = payTmp.Search(df->dat[1],1,TRUE);
		if (xrec){
			lset(payTmp.dat[2],df->dat[fldDate]);
			payTmp.Update();
		}
		else{
			lsetblank(&payTmp);
			lset(payTmp.dat[1],df->dat[1]);		
			lset(payTmp.dat[2],df->dat[fldDate]);
			payTmp.AddNew();
		}
	}/*
	else{
		if (isblank(profile.dat[PF_SALES])){
			waitmsg("SALES GLCODE NOT SET.PLEASE CHECK ACC PROFILE");		
			return;
		}
		tado(&tmp_inv,"arinv");
		tado(&tmp_oinv,"aroinv");

		xrec = aroinv_wb.Search(df->dat[1],VIWB_DO,TRUE);
		strcpy(key,aroinv_wb.dat[1]);
		strInvoice.Format("SELECT * FROM [ARINV_WB] where [INVOICE]='%s'",key);
		ado(&adoTmp,"adoTmp",NULL,strInvoice);
		long Srec = read_sumindex(0,&adoTmp);
		if (Srec){
			strcpy(key,adoTmp.dat[1]);
			for(long rec=1;rec<=Srec; rec++){
				readrec(rec,&adoTmp);
				lset(key,adoTmp.dat[1]);
				xrec =  arinv_wb.Search(key,1,TRUE);
				if (xrec){
					if (TRansferToAR(key,&adoTmp)){
						lset(arinv_wb.dat[IVWB_STATUS],"1");
						arinv_wb.Update();
					}
				}
			}		
		}		
	}*/	
	dclose(&payTmp);
	dclose(&adoTmp);
	dclose(&tmp_inv);
    dclose(&tmp_oinv);
}