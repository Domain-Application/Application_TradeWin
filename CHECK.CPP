// check.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "wdes.h"


int ShowCheckError(char *tl,char *dept,char *s1,char *s2)
{
    char tmp[80],ptitle[80];

    sprintf(ptitle,"Diffrence %s Data",tl);
    sprintf(tmp,"Weighbride Dept: %s",s1);
    sprintf(tmp,"%15.15s: %s",dept,s2);
    pDlg->MessageBox(tmp,ptitle);
    return 0;
}

int CheckDespatchPlanning(DB *df, char *dept,int check_no,int skip_flag)
{
    long rec,last,xrec,mrec;
    char tmp[80];
    int  flag,no;

    rec  = dseek(first.dat[PM_DATE],df,Pld_DATE,SF);
    last = dseek(first.dat[PM_DATE],df,Pld_DATE,SL);

    flag = 0;
    for(;rec<=last && rec>0; rec++) {
        mrec = ridx(rec,df,Pld_DATE);
        if(!strcmp(first.dat[PM_VEHICLE],df->dat[Pld_VEHICLE])) {
            flag = 1;
            xrec = rec;
            if(!strcmp(first.dat[PM_PT],df->dat[Pld_PT])) {
                flag = 2;
                xrec = rec;
                if(!strcmp(first.dat[PM_TANK],df->dat[Pld_TANK])) {
                    flag = 3;
                    xrec = rec;
                    if(!strcmp(first.dat[PM_FJB],df->dat[Pld_FJB])) {
                        if(skip_flag==0) return 1;
                        if(check_no == Pld_CHECK){
                            no = atoi(df->dat[Pld_USE]);
                            if(no<=atoi(df->dat[Pld_CHECK])) {
                                return 1;
                            }
                        }
                        else if(check_no == Pld_USE) {
                            no = atoi(df->dat[Pld_USE]);
                            if(no<=atoi(df->dat[Pld_CHECK])) {
                                ltodb((long)no+1,df,Pld_USE);
                                lset(df->dat[Pld_WT1+no],despatch.dat[1]);
                                fastwrt(mrec,df);
                                return 1;
                            }
                        }
                        flag = 4;
                    }
                }
            }
        }
    }
    if(flag>=1 && flag<=3) {
        readrec(xrec,df);
        if(flag==3) {
           return ShowCheckError("FJB Tank",dept,first.dat[PM_FJB],df->dat[Pld_FJB]);
        }
        else if(flag==2) {
            return ShowCheckError("FALMAJU Tank",dept,first.dat[PM_TANK],df->dat[Pld_TANK]);
        }
        else if(flag==1) {
            return ShowCheckError("PRODUCT",dept,first.dat[PM_PT],df->dat[Pld_PT]);
        }
    }
    sprintf(tmp,"Vehicle Data Not Found (%s)",dept);
    waitmsg(tmp);
    return 0;
}

int CheckLabDespatchPlanning()
{
    return CheckDespatchPlanning(&pl_des,"LAB Dept.",Pld_CHECK,0);
}

int CheckMarketingDespatchPlanning()
{
    return CheckDespatchPlanning(&mk_des,"MARKETING Dept.",Pld_CHECK,1);
}
