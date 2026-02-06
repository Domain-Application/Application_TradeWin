
// Lorry.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "multisort.h"
#include "PrintJob.h"
#include <io.h>

enum plorry {Pl_VEHICLE=1,Pl_DRIVER1,Pl_DNAME1,Pl_DRIVER2,Pl_DNAME2,Pl_TARE};
void print_driver_sptf(DB * df,DB * rep)
{
    icon(&one,rep,Pl_DRIVER1,Pl_DNAME1,1,&driver,2);
    icon(&one,rep,Pl_DRIVER2,Pl_DNAME2,1,&driver,2);
}

void PrintLorryCode(long irec,FILE *ptr,DB *df)
{
    char filename[80];

    sprintf(filename,"%s.dbf",df->name);
    if ( access(filename,00 ) == -1 ) {
        AfxMessageBox("File not found");
        return;
    }

    dopen(&report,filename);
      
    int IdxItem = df->f[1].idx & 0x01;
    DoStdPrintTableFormat(&one, df, &report, IdxItem);
    print_driver_sptf(df,&report);
	CPrintJob p1;
    p1.scr = &one;
    p1.rep = &report;
    sprintf(filename,"%s.rpt",df->name);
    p1.m_CrystalReportName = filename;
//    p1.m_textPrintEnable = FALSE;
    p1.m_PreviewEnable = TRUE;
    p1.m_DirectPrintEnable=FALSE;
    p1.DoModal();

    dclose(&report);
}


void init_lorry(DB *df)
{
//    df->e->prec  = PrintLorryCode;
}
