// slip2.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

void pslip2(long irec,FILE *ptr,DB *df)
{
//   ptr = topen("test.txt");
	/* 20/4/2015
	if(isblank(df->dat[DP_SCRAP])) {
		AfxMessageBox("Scrap G/Note is blank. Not allow to print.");
		return;
	}
	*/
	// CCWONG 20110331, slip2.rep and slip2_gr.rep is same structure.
	if(!strcmp(df->m_strTable,"rt")){
		PrintTicketSlip("slip2_gr.rep",ptr,df);
	}
	else PrintTicketSlip("slip2.rep",ptr,df);
//   fclose(ptr);
//   junction("test.txt",0);
}
