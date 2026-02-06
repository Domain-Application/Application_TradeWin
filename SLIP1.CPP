// slip1.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"
#include "AskPassword.h"
//#include "Scan.h"

enum slip1 {S2_TICKET=1,S2_DATE,S2_VEHICLE,S2_DO,S2_CONTRACT,
           S2_NAME,S2_ADD1,S2_ADD2,S2_ADD3,
           S2_PT1,S2_PNAME1,S2_QTY1,S2_WT1,S2_EXT1,
           S2_PT2,S2_PNAME2,S2_QTY2,S2_WT2,S2_EXT2,
           S2_PT3,S2_PNAME3,S2_QTY3,S2_WT3,S2_EXT3,
           S2_PT4,S2_PNAME4,S2_QTY4,S2_WT4,S2_EXT4,
           S2_PT5,S2_PNAME5,S2_QTY5,S2_WT5,S2_EXT5,
           S2_PT6,S2_PNAME6,S2_QTY6,S2_WT6,S2_EXT6,
           S2_PT7,S2_PNAME7,S2_QTY7,S2_WT7,S2_EXT7,
           S2_PT8,S2_PNAME8,S2_QTY8,S2_WT8,S2_EXT8,
           S2_TW,S2_EXT0,S2_REMARKS,S2_TIMEIN,S2_TIMEOUT,
           S2_SGROSS,S2_STARE,S2_SBAG,S2_SNET,
           S2_IC,S2_DRIVER,S2_ICNO,S2_TNAME,S2_SUBCON,S2_PTYPE,S2_GRNAME,
		   S2_PADD1,S2_PADD2,S2_PADD3,S2_PAGE,S2_SCALE,S2_MANUAL,S2_VTYPE,S2_VTNAME,
		   S2_PUSER,S2_TNG,S2_RT,S2_TNGMRK,S2_PYDATE};

int CheckVarianceWeight(DB *df)
{
    long net;

    net = VarianceWeight(df);
    if(net) {
        waitmsg("Net Weight <> total weight");
        CAskPassword p1;

        if(p1.DoModal()==IDOK) {
            return 1;
        }
        return 0;
    }
    return 1;
}

void GetProductName(DB *df,int fld,DB *rep,int repfld)
{
    DB adoTmp;
    CString str;
    str.Format("select [ACCPTNAME] from [spitem] where [pt]='%s' and [accode]='%s'",
        df->dat[fld],df->dat[DP_ACCODE]);
    ado(&adoTmp,"adoTmp",NULL,str);
	long Srec = read_sumindex(0,&adoTmp);
	if (Srec) {
		readrec(1,&adoTmp);
        lset(rep->dat[repfld],adoTmp.dat[1]);
	}
    else{
        qd(df->dat[fld],&prodtype);
        lset(rep->dat[repfld],prodtype.dat[2]);
    }
	dclose(&adoTmp);
}

void PrintTicketSlip(char *rep,FILE *ptr,DB *df)
{
    int    i,j,flag;
    double total,amt,wt,Tw;
    DB     slip1;

    dopen(&slip1,rep);
    setvbuf(ptr,NULL,_IONBF,0);
    fprintf(ptr,"2C!P");
    lsetblank(&slip1);
	if (isblank(df->dat[DP_PUSER])){
		lset(df->dat[DP_PUSER],user.dat[1]);
		df->Update();
	}
    fetch(&slip1,df);
    qd(df->dat[DP_ACCODE],&accode);
    lset(slip1.dat[S2_NAME],accode.dat[2]);
    lset(slip1.dat[S2_ADD1],accode.dat[ACCODE_ADD1]);
    lset(slip1.dat[S2_ADD2],accode.dat[ACCODE_ADD2]);
    lset(slip1.dat[S2_ADD3],accode.dat[ACCODE_ADD3]);
    qd(df->dat[DP_PTYPE],&ptype);
    lset(slip1.dat[S2_PTYPE],ptype.dat[2]);
    qd(df->dat[DP_GRADE],&grade);
    lset(slip1.dat[S2_GRNAME],grade.dat[2]);

	if(!strcmp(df->m_strTable,"rt")){
		lset(slip1.dat[S2_RT],"(GOODS REJECT)");
	}

	if (IsManual(df)){
		lset(slip1.dat[S2_MANUAL],"*");
	}

	qd(df->dat[DP_VTYPE],&vtype);
    lset(slip1.dat[S2_VTNAME],vtype.dat[2]);

    total = Tw = 0;
    for(i=0;i<8;i++){
        j = i*4+DP_PT1;
        GetProductName(df,j,&slip1,S2_PNAME1+i*5);
        wt  = atof(df->dat[j+2]);
        Tw  += wt;
        amt = wt * atof(df->dat[j+3]);
        total += amt;
        dtodb(amt,&slip1,S2_EXT1+i*5);
    }
    dtodb(Tw,&slip1,S2_TW);
    dtodb(total,&slip1,S2_EXT0);

    getmet(m,df);
    ltodb(m[11],&slip1,S2_SNET);
    qd(df->dat[DP_IC],&driver);
    lset(slip1.dat[S2_ICNO],driver.dat[3]);
    qd(df->dat[DP_TRANSP],&transp);
    lset(slip1.dat[S2_TNAME],transp.dat[3]);
    flag = CheckVarianceWeight(df);
    if(flag) {
        PrintTicket(ptr,title);
        mprint(ptr,&slip1);
        fprintf(ptr,"");
    }
    dclose(&slip1);
}

void pslip1(long irec,FILE *ptr,DB *df)
{
//   ptr = topen("test.txt");
	/* 20/4/2015
	if(isblank(df->dat[DP_SCRAP])) {
		AfxMessageBox("Scrap G/Note is blank. Not allow to print.");
		return;
	}
	*/
   PrintTicketSlip("slip1.rep",ptr,df);
//   fclose(ptr);
//   junction("test.txt",0);
}

void PrintLDoTitle(DB *scr,DB *df)
{
//    char tmp[20];
    fetch(scr,df);
	rset(scr->dat[S2_PAGE],"8");
	
	lset(scr->dat[S2_PADD1],stkpf.dat[PRO_ADD1]);
    lset(scr->dat[S2_PADD2],stkpf.dat[PRO_ADD2]);
    lset(scr->dat[S2_PADD3],stkpf.dat[PRO_ADD3]);
	/*
	
	lset(scr->dat[S2_PADD1],G_company_add1);
    lset(scr->dat[S2_PADD2],G_company_add2);
	*/
	
}

void PrintSlipDs(long irec,FILE *ptr,DB *df)
{
    long   Srec,rec,sq;
//    char   tmp[120];
    double amt;
    DB     scr;
    enum slip_ds {SLDS_ITEM=1,SLDS_PT,SLDS_NAME,SLDS_QTY,SLDS_UPRICE,SLDS_AMT,SLDS_TICKET};
	
	CString str;
	str.Format("Select isnull([print counter],0) from %s where ticket='%s'",df->name,df->dat[1]);
	long pcount = SqlValue(str);
	//if (df->val(DP_PRINT_COUNTER)>=1){  // void CStaff::OnPrint() ==>popdf(df,p); will pull back old value
/*	if (pcount>=1){  // ccwong temp mark 110620, wait henghup to buy new LPT print
		CScan p1;
		p1.df = &tng;
		int flag = p1.DoModal();
		if(flag==IDCANCEL) {
			df->autoaddprintcounter = FALSE;
			return;
		}			
	} */
	df->autoaddprintcounter = TRUE;
	char filename[280];	
	if(!stricmp(df->name,"dp")){														//book 2015/05/19
		if(AfxMessageBox("Print Ticket With Price?",MB_ICONQUESTION | MB_YESNO)==IDNO){
			sprintf(filename,"slip_np_%s.rep",df->name);
		}
		else
		sprintf(filename,"slip_%s.rep",df->name);
	}
	else
		sprintf(filename,"slip_%s.rep",df->name);
	//dopen(&report,"slip_ds.rep");
	dopen(&report,filename);
    dopen(&scr,"slip_ds.scr");
	//dopen(&scr,filename);
	
	if (isblank(df->dat[DP_PUSER])){
		lset(df->dat[DP_PUSER],user.dat[1]);
		df->Update();
	}
    PrintLDoTitle(&scr,df);
	
	qd(df->dat[DP_ACCODE],&accode);
	lset(scr.dat[S2_NAME],accode.dat[ACCODE_CUSTOMER]);
	lset(scr.dat[S2_ADD1],accode.dat[ACCODE_ADD1]);
    lset(scr.dat[S2_ADD2],accode.dat[ACCODE_ADD2]);
    lset(scr.dat[S2_ADD3],accode.dat[ACCODE_ADD3]);

	
	qd(df->dat[DP_TRANSP],&transp);
	int fldtr_name = GetDbFieldNo(&transp,"TR_SNAME");
	lset(scr.dat[S2_TNAME],transp.dat[fldtr_name]);

	qd(df->dat[DP_GRADE],&grade);
	int fld_grnname = GetDbFieldNo(&grade,"NAME");
	lset(scr.dat[S2_GRNAME],grade.dat[fld_grnname]);

	qd(df->dat[DP_IC],&driver);
	int fld_icno = GetDbFieldNo(&driver,"NERIC");
    lset(scr.dat[S2_ICNO],driver.dat[fld_icno]);

	qd(df->dat[DP_PTYPE],&ptype);
    lset(scr.dat[S2_PTYPE],ptype.dat[2]);

    lset(scr.dat[S2_SCALE],df->dat[DP_SCALE]);

	if (IsManual(df)){
		lset(scr.dat[S2_MANUAL],"*");
	}
/*	if (!isblank(df->dat[DP_TNG])){
		lset(scr.dat[S2_TNGMRK],"*");
	}
*/	getmet(m,df);
    ltodb(m[11],&scr,S2_SNET);

	qd(df->dat[DP_VTYPE],&vtype);
    lset(scr.dat[S2_VTNAME],vtype.dat[2]);

	if(!stricmp(df->name,"wds") || !stricmp(df->name,"ds")){
		
		str.Format("Select [PAYDATE] FROM paymentdate where [TICKET]='%s'",df->dat[1]);
	}
	else{
		
		str.Format("Select [PAYDATE] FROM paymentdated where [TICKET]='%s'",df->dat[1]);
	}

//	str.Format("Select [PAYDATE] FROM paymentdate where [TICKET]='%s'",df->dat[1]);
	CString pydate = SqlText(str);
	lset(scr.dat[S2_PYDATE],pydate);


	memstart(&report);
	
	amt = 0;
	sq = 1;
	double total,wt,Tw,up;
	long j =0;
	char   text[20];
	total = Tw = 0;
    for(long i=0;i<8;i++){	// Select all product from ds
        j = i*4+DP_PT1;
		if (isblank(df->dat[j])) continue;
		GetProductName(df,j,&report,SLDS_NAME);			
        wt  = atof(df->dat[j+2]);
        Tw  += wt;
		up  = atof(df->dat[j+3]);
        amt = wt * atof(df->dat[j+3]);
        total += amt;
        dtodb(amt,&scr,S2_EXT1+i*5);
		lset(report.dat[SLDS_PT],scr.dat[S2_PT1+i*5]);
		dtodb(wt,&report,SLDS_QTY);
		dtodb(up,&report,SLDS_UPRICE);
		dtodb(amt,&report,SLDS_AMT);
		dtoa(sq,text,3,0);
		lset(report.dat[SLDS_ITEM],text);
		sq ++;
		memwrt(&report);
    }	
    Srec = interval(&irec,df->dat[DP_SCALE],&scalesub,1);
  //  memstart(&report);
    for (rec=0; rec<Srec; rec++) {
        ridx(irec+rec,&scalesub,1);
        lsetblank(&report);
        fetch(&report,&scalesub);
		GetProductName(&scalesub,SCASB_PT,&report,SLDS_NAME);		
        ltodb(sq++,&report,1);
        memwrt(&report);
       // backup(&report);
    }
    memend(&report);    
//	stdlayout(ptr,&scr,&report);  // Without preview
//    fprintf(ptr,"");
    stdplan(&scr,&report);  //WITH PREVIEW
    dclose(&report);	
    dclose(&scr);		
}

void PrintSlipScale(long irec,FILE *ptr,DB *df)
{
    long   Srec,rec,sq;
//    char   tmp[120];
    double amt;
    DB     scr;
	DB *sub;
    enum slip_ds {SLDS_ITEM=1,SLDS_PT,SLDS_NAME,SLDS_QTY,SLDS_UPRICE,SLDS_AMT,SLDS_TICKET};
	
	if (Purchase_Flag){
		if (df->dat[1][0] == 'R'){
			dopen(&report,"slip_ds2.rep");
			dopen(&scr,"slip_ds2.scr");
		}
		else{
			dopen(&report,"slip_ds.rep");
			dopen(&scr,"slip_ds.scr");
		}
		sub =&scalesub;
	}
	else{
		if(AfxMessageBox("Print Ticket With Price?",MB_ICONQUESTION | MB_YESNO)==IDYES){
			dopen(&report,"slip_dw.rep");
			dopen(&scr,"slip_dw.scr");
			sub =&scaledwsub;
		}
		else{
			dopen(&report,"slip_dw1.rep");
			dopen(&scr,"slip_dw1.scr");
			sub =&scaledwsub;
		}
	}	
	
	//dopen(&report,"slip_ds.rep");
    //dopen(&scr,"slip_ds.scr");
    PrintLDoTitle(&scr,df);
	qd(df->dat[SCA_ACCODE],&accode);
	lset(scr.dat[S2_NAME],accode.dat[ACCODE_CUSTOMER]);
	lset(scr.dat[S2_ADD1],accode.dat[ACCODE_ADD1]);
    lset(scr.dat[S2_ADD2],accode.dat[ACCODE_ADD2]);
    lset(scr.dat[S2_ADD3],accode.dat[ACCODE_ADD3]);
	
//	SCA_TICKET=1,SCA_SERIAL,SCA_DATE,SCA_VEHICLE,SCA_UNIT,SCA_ACCODE,SCA_PTYPE,SCA_TRANSP,SCA_DO,SCA_SCRAP,SCA_IC,SCA_DRIVER,SCA_BASIS};
	qd(df->dat[SCA_TRANSP],&transp);
	int fldtr_name = GetDbFieldNo(&transp,"TR_SNAME");
	lset(scr.dat[S2_TNAME],transp.dat[fldtr_name]);
	/*
	qd(df->dat[DP_GRADE],&grade);
	int fld_grnname = GetDbFieldNo(&grade,"NAME");
	lset(scr.dat[S2_GRNAME],grade.dat[fld_grnname]);
	*/
	qd(df->dat[SCA_IC],&driver);
	int fld_icno = GetDbFieldNo(&driver,"NERIC");
    lset(scr.dat[S2_ICNO],driver.dat[fld_icno]);

	CString str;
	
	str.Format("Select [PAYDATE] FROM paymentdate where [TICKET]='%s'",df->dat[1]);
	CString pydate = SqlText(str);
	lset(scr.dat[S2_PYDATE],pydate);
	
	memstart(&report);
	
	amt = 0;
	sq = 1;
	double total,Tw;
	long j =0;
//	char   text[20];
	total = Tw = 0;

    Srec = interval(&irec,df->dat[1],sub,1);
	//  memstart(&report);
    for (rec=0; rec<Srec; rec++) {
        ridx(irec+rec,sub,1);
        lsetblank(&report);
        fetch(&report,sub);
		GetProductName(sub,SCASB_PT,&report,SLDS_NAME);		
        ltodb(sq++,&report,1);
        memwrt(&report);
		// backup(&report);
    }

	qd(df->dat[SCA_PTYPE],&ptype);
    lset(scr.dat[S2_PTYPE],ptype.dat[2]);
    memend(&report);
    stdplan(&scr,&report);
    dclose(&report);	
    dclose(&scr);		
}
