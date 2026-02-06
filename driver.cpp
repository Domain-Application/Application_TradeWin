
// Pdriver.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"

void PrintDriver(long irec,FILE *ptr,DB *df)
{
    int  field[] = {0};

    dopen(&one,"one.scr");
    dopen(&report,"pdriver.dbf");
    long last = df->t->sumrecord;
    for(long rec=1 ;rec<=last;rec++ ){
        fetch_rec(&report,df);
    }
//    CrystalReport(&one,&report,field,5);
//    StdPrintTableFormat(&one, df, &report, 1);
    dclose(&one);
    dclose(&report);
}

void init_driver(DB *df)
{
//    df->e->prec  = PrintDriver;
}
