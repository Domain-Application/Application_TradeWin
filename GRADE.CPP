// grade.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"

void PrintGrade(long irec,FILE *ptr,DB *df)
{
    dopen(&one,"one.scr");
    dopen(&report,"pgrade.rep");
    StdPrintTableFormat(&one, df, &report, 0);
    dclose(&one);
    dclose(&report);
}

void init_grade(DB *df)
{
//    df->e->prec  = PrintGrade;
}
