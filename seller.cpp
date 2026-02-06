
// pseller.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"

void PrintSeller(long irec,FILE *ptr,DB *df)
{
    dopen(&one,"one.scr");
    dopen(&report,"pcode.rep");
    StdPrintTableFormat(&one, df, &report, 0);
    dclose(&one);
    dclose(&report);
}

void init_accode(DB *df)
{
//    df->e->prec  = PrintSeller;
}
