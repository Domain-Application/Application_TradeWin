// country.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"

void PrintCountry(long irec,FILE *ptr,DB *df)
{
    dopen(&one,"one.scr");
    dopen(&report,"pcountry.rep");
    StdPrintTableFormat(&one, df, &report, 0);
    dclose(&one);
    dclose(&report);
}

void init_country(DB *df)
{
//    df->e->prec  = PrintCountry;
}
