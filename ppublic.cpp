// slip1.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"
#include "AskPassword.h"

enum ppublic {S2_TICKET=1,S2_DATE,S2_VEHICLE,S2_DO,S2_CONTRACT,
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
           S2_IC,S2_DRIVER,S2_ICNO,S2_TNAME,S2_SUBCON,S2_PTYPE,S2_GRNAME};

extern  int CheckVarianceWeight(DB *df);
extern void GetProductName(DB *df,int fld,DB *rep,int repfld);

void PrintBlankCompanyName(FILE *ptr, char *title)
{
    fprintf(ptr,"2C!M");
    fprintf(ptr,"");
    fprintf(ptr,"M");
    for(int i=1; i<=3; i++){
        fprintf(ptr,"\n");
    }
    fprintf(ptr,"\n");

}

void PrintPublicTicket(long irec,FILE *ptr,DB *df)
{
    int    i,j,flag;
    double total,amt,wt,Tw;
    DB     slip1;

    dopen(&slip1,"ppublic.rep");
    setvbuf(ptr,NULL,_IONBF,0);
    fprintf(ptr,"2C!M");
    lsetblank(&slip1);
    fetch(&slip1,df);
    qd(df->dat[PR_ACCODE],&accode);
    lset(slip1.dat[S2_NAME],accode.dat[2]);
    lset(slip1.dat[S2_ADD1],accode.dat[ACCODE_ADD1]);
    lset(slip1.dat[S2_ADD2],accode.dat[ACCODE_ADD2]);
    lset(slip1.dat[S2_ADD3],accode.dat[ACCODE_ADD3]);
    
    getprmet(m,df);
    ltodb(m[4],&slip1,S2_SNET);
    if ( AfxMessageBox("Print Company Name",MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES ){
		PrintTicket(ptr,title);
	}
	else{
		PrintBlankCompanyName(ptr,title);
	}
    
    mprint(ptr,&slip1);
    fprintf(ptr,"");

    dclose(&slip1);
}

