// exp_ap.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Logerr.h"
#include "Two.h"

extern DB   exp_inv,tmp_inv,tmp_oinv;

enum exp_inv {Epi_GLCODE=1,Epi_AMOUNT};
/*
void ExportPurchaseInvoiceItem(DB *df)
{
    long Srec,irec,rec;
    int  i;
    double total=0;

    fetch(&tmp_inv,df);
    Srec = interval(&irec,df->dat[1],&apoinv_wb,1);
    for(rec=0; rec<Srec; rec++) {
        ridx(irec+rec,&apoinv_wb,1);
        total += atof(apoinv_wb.dat[VIWB_AMOUNT]);
    }
    
    //append data to tmp_oinv
    lsetblank(&tmp_oinv);
    lset(tmp_oinv.dat[1],df->dat[1]);
    ltodb(irec,&tmp_oinv,Oiv_SQNO);
    lset(tmp_oinv.dat[Oiv_GLCODE],profile.dat[PF_SALES]);
    qd(tmp_oinv.dat[Oiv_GLCODE],&glcode);
    lset(tmp_oinv.dat[Oiv_NAME],glcode.dat[2]);
    dtodb(total,&tmp_oinv,Oiv_AMOUNT);
    tmp_oinv.AddNew();

    dtodb(total,&tmp_inv,IV_TOTAL);
	tmp_inv.Update();
}

void TRansferToAP(char *key,DB *df)
{
    long xrec,mrec;

    lset(key,df->dat[1]);
    xrec = tmp_inv.Search(key,1);
    if(xrec==0) {
        lsetblank(&tmp_inv);
		fetch(&tmp_inv,df);
        lset(tmp_inv.dat[1],key);
        lset(tmp_inv.dat[IV_CUSTOMER],df->dat[IVWB_ACCODE]);
		qd(df->dat[IVWB_ACCODE],&debtor);
        lset(tmp_inv.dat[IV_STAFF],debtor.dat[CU_STAFF]);
        lset(tmp_inv.dat[IV_TERMS],df->dat[IVWB_TERMS]);
		lset(tmp_inv.dat[IV_PREFIX],"CRE");
        lset(tmp_inv.dat[IV_REM],"PURCHASE");
        lset(tmp_inv.dat[IV_TYPE],"IV");
        lset(tmp_inv.dat[IV_CURRENCY],df->dat[IVWB_CURRENCY]);
		rset(tmp_inv.dat[IV_RATE],df->dat[IVWB_RATE]);
        dtodb(1,&tmp_inv,IV_RATE);
        tmp_inv.AddNew();         
        ExportPurchaseInvoiceItem(df);
    }
}

void DoExportPurchaseInvoice(char *main, char *data)
{

    long last,rec;
    char key[20];

    strcpy(key,tmp_inv.dat[1]);
    
	CString strSql,strCmd;
	
    strSql.Format("SELECT * FROM [%s] WHERE ",main);
    strCmd.Format("[date]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("and [date]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
    strCmd.Format(" order by [%s]",apinv_wb.fieldName[1]);
    strSql+=strCmd;
	DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,strSql);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			fetch(&tmp_inv,&adoTmp);
			TRansferToAP(key,&adoTmp);
		}	
	}
	dclose(&adoTmp);
}
*/
BOOL ExportPurchaseInvoiceItem(DB *df)
{
    long Srec,irec,rec;
    int  i;
    double total=0;
	
    fetch(&tmp_inv,df);
    Srec = interval(&irec,df->dat[1],&apoinv_wb,1);
    for(rec=0; rec<Srec; rec++) {
        ridx(irec+rec,&apoinv_wb,1);
        total += atof(apoinv_wb.dat[VIWB_AMOUNT]);
		//append data to tmp_oinv
		lsetblank(&tmp_oinv);
		lset(tmp_oinv.dat[1],df->dat[1]);
		ltodb(irec,&tmp_oinv,Oiv_SQNO);
		lset(tmp_oinv.dat[Oiv_GLCODE],profile.dat[PF_PURCHASE]);
		qd(tmp_oinv.dat[Oiv_GLCODE],&glcode);
		lset(tmp_oinv.dat[Oiv_NAME],glcode.dat[2]);
		dtodb(total,&tmp_oinv,Oiv_AMOUNT);
		tmp_oinv.AddNew();

		if(tmp_oinv.m_SaveFlag == FALSE){
			CString strMsg,strSql;
			strSql.Format("Delete [apinv] where [refno]='%s'",df->dat[1]);
			RunSql(strSql);
			strMsg.Format("Invoice :%s export to ACC fail. Please re-do",df->dat[1]);
			waitmsg(strMsg);
			return 0;
		}
    }	
    dtodb(total,&tmp_inv,IV_TOTAL);
	tmp_inv.Update();
	return 1;
}

BOOL TRansferToAP(char *key,DB *df)
{
    long xrec,mrec;
	
    lset(key,df->dat[1]);
    xrec = tmp_inv.Search(key,1);
    if(xrec==0) {
        lsetblank(&tmp_inv);
		fetch(&tmp_inv,df);
        lset(tmp_inv.dat[1],key);
        lset(tmp_inv.dat[IV_CUSTOMER],df->dat[IVWB_ACCODE]);
		qd(df->dat[IVWB_ACCODE],&debtor);
        lset(tmp_inv.dat[IV_STAFF],debtor.dat[CU_STAFF]);
        lset(tmp_inv.dat[IV_TERMS],df->dat[IVWB_TERMS]);
		lset(tmp_inv.dat[IV_PREFIX],"CRE");
        lset(tmp_inv.dat[IV_REM],"PURCHASE");
        lset(tmp_inv.dat[IV_TYPE],"IV");
        lset(tmp_inv.dat[IV_CURRENCY],df->dat[IVWB_CURRENCY]);
		rset(tmp_inv.dat[IV_RATE],df->dat[IVWB_RATE]);
        dtodb(1,&tmp_inv,IV_RATE);
        tmp_inv.AddNew();
		if(tmp_inv.m_SaveFlag == FALSE){
			CString strMsg;
			strMsg.Format("Invoice :%s export to ACC fail. Please re-do",df->dat[1]);
			waitmsg(strMsg);
			return 0;
		}
        return ExportPurchaseInvoiceItem(df);
    }
	return 0;
}

void DoExportPurchaseInvoice(DB *main,DB *data)
{
	char key[30];
    CString strSql,strCmd;
    DB adoTmp;
    //strcpy(key,tmp_inv.dat[1]);
	
    strSql.Format("SELECT * FROM [%s] WHERE ([status] = '0' or [status] is null)",main->m_strTable);
    strCmd.Format("and [date]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("and [date]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
	strCmd.Format(" order by [%s]",main->fieldName[1]);
    strSql+=strCmd;
    ado(&adoTmp,"adoTmp",NULL,strSql);
    long Srec = read_sumindex(0,&adoTmp);
    if (Srec){
		strcpy(key,tmp_inv.dat[1]);
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
            lset(key,adoTmp.dat[1]);
			long xrec =  main->Search(key,1,TRUE);
            if (xrec){
				if (TRansferToAP(key,&adoTmp)){
					lset(main->dat[IVWB_STATUS],"1");
					main->Update();
                }
            }
		}		
	}
    else{
        waitmsg("No Record Found");
    }
	dclose(&adoTmp);
}

void put_exp_ap()
{
    char tmp[80];

    FLASH = 0;
    //DoExportPurchaseInvoice("apinv_wb","apoinv_wb");
	DoExportPurchaseInvoice(&apinv_wb,&apoinv_wb);
    waitmsg("Export Completed");
    FLASH = 1;

}

void ExportPurchaseInvoiceTransactions()
{
	if (isblank(profile.dat[PF_PURCHASE])){
		waitmsg("PURCHASE GLCODE NOT SET.PLEASE CHECK ACC PROFILE");		
		return;
	}
    tado(&tmp_inv,"apinv");
    tado(&tmp_oinv,"apoinv");

    CLogerr p1;
    p1.level = 9;
    p1.m_purpose = "CONFIRM EXPORT INVOICE TO AP";
    p1.df = &tmp_inv;
    int wflag = p1.DoModal();
    if(wflag==IDOK) {

       CTwo t1(IDD_IMPORTSCR,&two,0,put_exp_ap);
       t1.Status = 1;
       t1.DoModal();

    }
    dclose(&tmp_inv);
    dclose(&tmp_oinv);
}
