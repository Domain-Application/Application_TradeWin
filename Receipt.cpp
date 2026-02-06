// receipt.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"
#include <math.h>
#include "mgrid.h"

void ReplaceReceiptGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,0,RE_INVOICE,RE_DATE,RE_DAY,
         RE_TOTAL,RE_BAL,RE_ACC,-1);
}

void SetReceiptGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,2);

    g.ratio = float(1.0);
    g.SetTitle(df,0,RE_INVOICE,RE_DATE,RE_DAY,RE_TOTAL,RE_BAL,RE_ACC,-1);
}

void Set17Rows(CMSFlexGrid *grid,DB *df)
{
    grid->SetRows(17);
}

void init_receipt(DB *df)
{
    df->e->SetGridRows = Set17Rows;
    df->e->rep       = ReplaceReceiptGrid;
    df->e->GridTitle = SetReceiptGridTitle;
    df->BROWSE_ID    = IDD_BROWSE_RECEIPT;
}
