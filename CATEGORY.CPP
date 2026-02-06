// caegory.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"

void PrintCat(long irec,FILE *ptr,DB *df)
{
    dopen(&one,"one.scr");
    dopen(&report,"pcat.rep");
    StdPrintTableFormat(&one, df, &report, 0);
    dclose(&one);
    dclose(&report);
}

void init_category(DB *df)
{
//    df->e->prec  = PrintCat;
}
