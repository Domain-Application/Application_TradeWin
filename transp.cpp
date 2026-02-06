
// transp.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"

void PrintTransp(long irec,FILE *ptr,DB *df)
{
    dopen(&one,"one.scr");
    dopen(&report,"ptransp.rep");
    StdPrintTableFormat(&one, df, &report, 0);
    dclose(&one);
    dclose(&report);
}

void init_transp(DB *df)
{
//    df->e->prec  = PrintTransp;
}
