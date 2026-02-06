#include "stdafx.h"
#include "menu.h"
#include "EnqBank.h"
#include "db_ado.h"

void CEnqBank::OnPrint() 
{
    OnClickMsflexgrid1();
    long xrec = glcode.Search(glcode.dat[1],1,TRUE);
    if(xrec==0) return;

    RunStoreProcedure(FALSE,"dt_bankp",
        ADOCG::adInteger,"bank", atoi(glcode.dat[C_PRIMARY]),
        ADOCG::adDate,"date2",two.dat[2],
        0);

    sprintf(SubTitle,"%s - %s", glcode.dat[1], glcode.dat[2]);
    PrintRPT_withScr(&two,"bankp.rpt",TRUE);
}
