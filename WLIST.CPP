// Wlist.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Tscr.h"
#include "multisort.h"

enum wlist {Tl_TRANSP=1,Tl_DATE,Tl_LOCATE,Tl_LNAME,Tl_TICKET,
            Tl_PRODTYPE,Tl_LORRY,Tl_ATIME,Tl_LTIME,Tl_DTIME,
            Tl_NET,Tl_TFEE,Tl_AMT,Tl_HOUR,Tl_TOTAL};

void save_wlist(DB *df,DB *rep)
{
    double amt,net,wfee;
    char time[10];

    if(TscrFilter(df)) {
        fetch_rec(&report,df);
        net = atof(df->dat[NET]) / 1000;
        amt = net * atof(df->dat[TFEE]);
        dtodb(net,&report,Tl_NET);
        dtodb(amt,&report,Tl_AMT);
        wfee = atof(report.dat[Tl_HOUR]);
        dtodb(amt+wfee,&report,Tl_TOTAL);
        qd(df->dat[LOCATE],&Locate);
        lset(report.dat[Tl_LNAME],Locate.dat[2]);
        dtime(time,df->dat[ATIME],df->dat[LTIME]);
        lset(report.dat[Tl_DTIME],time);
        ctom(report.dat[Tl_DTIME]);
        memwrt(&report);
    }
}

void solve_wlist(FILE *ptr,int line,FM *fm,char *text)
{
    qd(report.dat[1],&transp);
    replace(text,"%s",transp.dat[2]);
    image(fm,ptr,text);
}

void print_wlist()
{
    FM   fm;

    _PlanField = 1;
    fm.LineOfPage = 55;
    plan(&fm,&tscr,&report,solve_wlist);
}

void put_wlist()
{
    int  field[] = { Tl_TRANSP,Tl_TICKET,0};

    mloop(&tscr,&quarry,&report,Q_DATE,save_wlist);
    if(report.t->sumrecord) {
        sort(&report,field);
        print_wlist();
    }
}

void WaitingFeeListing()
{
    dopen(&tscr,"tscr.scr");
    dopen(&report,"wlist.rep");

    CTscr t1(IDD_TSCR,&tscr,0,put_wlist);
    t1.DoModal();

    dclose(&report);
    dclose(&tscr);
}
