// Pt.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "mgrid.h"

void ReplaceProductGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,NULL,PT_PART,PT_NAME,PT_PTGROUP,-1);
    qd(df->dat[PT_PTGROUP],&ptgroup);
    g.SetText(ptgroup.dat[PTGROUP_CATEGORY]);
    g.SetText(df,PT_UNIT,PT_COST,PT_SALES,-1);
}

void SetProductGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID fg(grid,df,0,2);

    fg.SetTitle("No",480,1);
    fg.SetTitle("Code",900,1);
    fg.SetTitle("Name",2500,1);
    fg.SetTitle("Pt Group",830,1);
    fg.SetTitle("Category",830,1);
    fg.SetTitle("Unit",885,1);
    fg.SetTitle("Purchase",1200,7);
    fg.SetTitle("Sales",885,7);
}

int pt_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = PT_LASTUPDATE;
	fld[1] = PT_C_ADJ;
	fld[2] = PT_S_ADJ;
	fld[3] = 0;
    return 1;
}

void init_pt(DB *df)
{
    df->e->rep       = ReplaceProductGrid;
    df->e->GridTitle = SetProductGridTitle;
	df->e->initial   = pt_initial;
}
