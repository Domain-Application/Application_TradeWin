// Prefix.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

int start_terms(DB *df,int *mi)
{
    lset(df->dat[3],"1");
    return 1;
}

void init_prefix(DB *df)
{
    df->e->initial  = start_terms;
}
