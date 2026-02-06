#include "stdafx.h"
#include "menu.h"
#include "Tso.h"

double arrage_st(DB *df)
{
    int  field[] = {St_IDATE,St_INVNO,St_TYPE,St_DATE, 0};
    sort(df,field);

    double bf = 0;
    for(long rec=1; rec<=df->t->sumrecord; rec++){
        readrec(rec,df);
        double offset = df->val(St_DR) - df->val(St_CR);
        bf += offset;
        dtodb(bf,df,St_BALANCE);
        fastwrt(rec,df);
    }
    return bf;
}

void ComputeStateBalance()
{
    int    i,j,k;
    double bf,t1,t2;
    char   tmp[120];

    stateTitle();
    bf = arrage_st(&st);
    dtoa(bf,tmp,10,2);
    Over_Balance[STATEMENT_BALANCE] = bf;
    strcpy(Ob[STATEMENT_BALANCE],putnum(tmp));
    _PL[23] = Ob[STATEMENT_BALANCE];
    t2 = 0;
    for(i=11; i>=0; i--) {
        t1 = Over_Balance[i];
        if(t1<0 && i>0) {
             Over_Balance[i] = 0;
             Over_Balance[i-1] += Over_Balance[i];
        }
        t2 += Over_Balance[i];
    }
    for(i=0; i<12; i++) {
        dtoa(0,tmp,10,2);
        strcpy(Ob[i],putnum(tmp));
        _PL[11+i] = Ob[i];
    }

    bf = 0;
    j = month_no(tso.dat[2]);
    for(i=k=0; i<=11; i++) {
        dtoa(Over_Balance[i],tmp,10,2);
        strcpy(Ob[i],putnum(tmp));
        _PL[10+j] = Ob[i];
        if(k==1) bf += Over_Balance[i];
        j--;
        if(j<1){ j = 12; k = 1;}
    }

    dtoa(bf,tmp,12,2);
    strcpy(Ob[12],putnum(tmp));
    _PL[24] = Ob[12];
    t2 -= Over_Balance[RCA];
    dtoa(t2,tmp,10,2);
    strcpy(Ob[RCA],putnum(tmp));
}

void SetStatementOpeningBalance(double bf,long rec)
{
    lsetblank(&st);
    lset(st.dat[1],tso.dat[Ts_D1]);
    lset(st.dat[St_NAME],"B/F");
    dtodb(bf,&st,St_BALANCE);
    writerec(rec,&st);
}

int SetOpenItemStatementType(double bf)
{
    long rec,last,mrec;

    if(bf) {
        if(st1.t->sumrecord==0) {
            mrec = append(&st1);
            lset(st1.dat[St_DATE],tso.dat[1]);
            lset(st1.dat[St_NAME],"B/F");
            dtodb(bf,&st1,St_BALANCE);
            fastwrt(mrec,&st1);
        }
    }
    last = st.t->sumrecord;
    for(rec=1; rec<=last; rec++){
        readrec(rec,&st);
        mrec = append(&st1);
        fetch(&st1,&st);
        fastwrt(mrec,&st1);
        if(rec==last) endmark(mrec,&st1);
    }
    return int(st1.t->sumrecord);
}

int SetBFStatementType(double bf)
{
    memstart(&st1);

    if(bf) {
        append(&st1);
        lset(st1.dat[St_DATE],tso.dat[1]);
        lset(st1.dat[St_NAME],"B/F");
        dtodb(bf,&st1,St_BALANCE);
        memwrt(&st1);
    }
    long last = st.t->sumrecord;
    fetch_set(&st1,&st);
    for(long rec=1; rec<=last; rec++){
        readrec(rec,&st);
        fetch_rec(&st1,&st);
        memwrt(&st1);
    }
    memend(&st1);
    return int(st1.t->sumrecord);
}

int SetStatementType(double bf)
{
    int flag;

    if(tso.dat[Ts_TYPE][0]=='1') {
        flag = SetOpenItemStatementType(bf);
    }
    else if(tso.dat[Ts_TYPE][0]=='2') {
        flag = SetBFStatementType(bf);
    }
    return flag;
}

int FilterBfSTate()
{
    int    flag=0;
    double bf=0;

    long last = st.t->sumrecord;
    if(last<=0) return 0;
    for(long rec=1; rec<=last; rec++){
        readrec(rec,&st);
        if(strcmp(st.dat[St_IDATE],tso.dat[Ts_D1])<0) {
            bf = st.val(St_BALANCE);
            setmark(rec,&st);
            flag = 1;
        }
        else {
            break;
        }
    }
    if(flag==1) {
        pack(&st);
    }
    flag = SetStatementType(bf);
    return flag;
}
