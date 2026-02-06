// exp_inv.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Logerr.h"
#include "Two.h"

DB   exp_inv,tmp_inv,tmp_oinv;
int  exp_invFlag;
enum exp_inv {Epi_GLCODE=1,Epi_AMOUNT};

/*
void Exporttmp_invItem(DB *df)
{
    long Srec,irec,rec;
//    int  i;
    double total=0;

    fetch(&tmp_inv,df);
    Srec = interval(&irec,df->dat[1],&aroinv_wb,1);
    for(rec=0; rec<Srec; rec++) {
        ridx(irec+rec,&aroinv_wb,1);
        total += atof(aroinv_wb.dat[VIWB_AMOUNT]);
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

void TRansferToAR(char *key,DB *df)
{
    long xrec;//,mrec;

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
        lset(tmp_inv.dat[IV_REM],"SALES");
        lset(tmp_inv.dat[IV_TYPE],"IV");
        lset(tmp_inv.dat[IV_CURRENCY],df->dat[IVWB_CURRENCY]);
		rset(tmp_inv.dat[IV_RATE],df->dat[IVWB_RATE]);
        dtodb(1,&tmp_inv,IV_RATE);
        tmp_inv.AddNew();         
        Exporttmp_invItem(df);
    }
}

void DoExportInvoice(char *main, char *data)
{

//    long last,rec;
    char key[20];

    strcpy(key,tmp_inv.dat[1]);
    
	CString strSql,strCmd;
	
    strSql.Format("SELECT * FROM [%s] WHERE ",main);
    strCmd.Format("[date]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("and [date]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
    strCmd.Format(" order by [%s]",arinv_wb.fieldName[1]);
    strSql+=strCmd;
	DB adoTmp;
	ado(&adoTmp,"adoTmp",NULL,strSql);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec){
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
			fetch(&tmp_inv,&adoTmp);
			TRansferToAR(key,&adoTmp);
		}	
	}
	dclose(&adoTmp);
}
*/

BOOL Exporttmp_invItem(DB *df)
{
    long Srec,irec,rec;
    double total=0;
	CString strSql;
	DB adoTmp;

    fetch(&tmp_inv,df);	
    /*
	Srec = interval(&irec,df->dat[1],&aroinv_wb,1);
	for(rec=0; rec<Srec; rec++) {
        ridx(irec+rec,&aroinv_wb,1);
        total += atof(aroinv_wb.dat[VIWB_AMOUNT]);
		//append data to tmp_oinv
		lsetblank(&tmp_oinv);
		lset(tmp_oinv.dat[1],df->dat[1]);
		ltodb(irec,&tmp_oinv,Oiv_SQNO);
		lset(tmp_oinv.dat[Oiv_GLCODE],profile.dat[PF_SALES]);
		qd(tmp_oinv.dat[Oiv_GLCODE],&glcode);
		lset(tmp_oinv.dat[Oiv_NAME],glcode.dat[2]);
		dtodb(total,&tmp_oinv,Oiv_AMOUNT);
		tmp_oinv.AddNew();
		
		if(tmp_oinv.m_SaveFlag == FALSE){
			CString strMsg,strSql;
			strSql.Format("Delete [arinv] where [refno]='%s'",df->dat[1]);
			RunSql(strSql);
			strMsg.Format("Invoice :%s export to ACC fail. Please re-do",df->dat[1]);
			waitmsg(strMsg);
			return 0;
		}
	}*/
	
	strSql.Format("select * from [DPINV_EXPINV] where [INVOICE]='%s'",df->dat[1]);//invoice,glcode1,qty,amount
	ado(&adoTmp,"adoTmp",NULL,strSql);

	Srec = read_sumindex(1,&adoTmp);
    for(rec=1; rec<=Srec; rec++) {
		readrec(rec,&adoTmp);
		//fetch(&tmp_inv,&adoTmp);
        //ridx(irec+rec,&aroinv_wb,1);
        total += atof(adoTmp.dat[4]);
		lsetblank(&tmp_oinv);
		lset(tmp_oinv.dat[1],df->dat[1]);
		ltodb(irec,&tmp_oinv,Oiv_SQNO);
		if(!isblank(adoTmp.dat[2])){
			lset(tmp_oinv.dat[Oiv_GLCODE],adoTmp.dat[2]);
		}
		else{
			lset(tmp_oinv.dat[Oiv_GLCODE],profile.dat[PF_SALES]);
		}
		qd(tmp_oinv.dat[Oiv_GLCODE],&glcode);
		lset(tmp_oinv.dat[Oiv_NAME],glcode.dat[2]);
//		dtodb(adoTmp.val(3),&tmp_oinv,Oiv_QTY);
		dtodb(adoTmp.val(4),&tmp_oinv,Oiv_AMOUNT);
		tmp_oinv.AddNew();

		if(tmp_oinv.m_SaveFlag == FALSE){
			CString strMsg,strSql;
			strSql.Format("Delete [arinv] where [refno]='%s'",df->dat[1]);
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

BOOL TRansferToAR(char *key,DB *df)
{
    long xrec;//,mrec;
	
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
        lset(tmp_inv.dat[IV_REM],"SALES");
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
        return Exporttmp_invItem(df);
    }
	return 0;
}

void DoExportInvoice(DB *main,DB *data)
{
    char key[25];
    CString strSql,strCmd;
    DB adoTmp;
	
    strSql.Format("SELECT * FROM [%s] WHERE ([status] <> '1' or [status] is null) ",main->m_strTable);
    strCmd.Format("and [date]>='%s' ",DateStr(two.dat[1]));
    strSql+=strCmd;
    strCmd.Format("and [date]<='%s' ",DateStr(two.dat[2]));
    strSql+=strCmd;
    strCmd.Format(" order by INVOICE");
    strSql+=strCmd;
    ado(&adoTmp,"adoTmp",NULL,strSql);
    long Srec = read_sumindex(0,&adoTmp);
    if (Srec){
		strcpy(key,adoTmp.dat[1]);
		for(long rec=1;rec<=Srec; rec++){
			readrec(rec,&adoTmp);
            lset(key,adoTmp.dat[1]);
            long xrec =  main->Search(key,1,TRUE);
            if (xrec){
                if (TRansferToAR(key,&adoTmp)){
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

void put_exp_inv()
{
//    char tmp[80];

    FLASH = 0;
    exp_invFlag = 0;
    //DoExportInvoice("arinv_wb","aroinv_wb");
	DoExportInvoice(&arinv_wb,&aroinv_wb);
    waitmsg("Export Completed");
    FLASH = 1;

}

void ExportInvoiceTransactions()
{
	if (isblank(profile.dat[PF_SALES])){
		waitmsg("SALES GLCODE NOT SET.PLEASE CHECK ACC PROFILE");		
		return;
	}
    tado(&tmp_inv,"arinv");//cland/acc/ar/
    tado(&tmp_oinv,"aroinv");//cland/acc/ar/

    CLogerr p1;
    p1.level = 9;
    p1.m_purpose = "CONFIRM EXPORT INVOICE TO AR";
    p1.df = &tmp_inv;
    int wflag = p1.DoModal();
    if(wflag==IDOK) {

       CTwo t1(IDD_IMPORTSCR,&two,0,put_exp_inv);
       t1.Status = 1;
       t1.DoModal();

    }
    dclose(&tmp_inv);
    dclose(&tmp_oinv);
}
