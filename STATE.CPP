#include "stdafx.h"
#include "menu.h"
#include "Tso.h"

char Ob[RCA+1][16];
long custidx;
char TmpTco[500];
char p2date[130];
double Over_Balance[RCA+1];
void stateTitle()
{
    int tdays = atoi(customer->dat[CU_TERMS]);
    _PL[40] = customer->dat[CU_STAFF];
    _PL[1] = customer->dat[1];
    _PL[0] = customer->dat[CU_NAME];
    _PL[2] = customer->dat[CU_CONTACT];
    _PL[3] = customer->dat[CU_ADD1];
    _PL[4] = customer->dat[CU_ADD2];
    _PL[5] = customer->dat[CU_ADD3];
    _PL[15] = customer->dat[CU_TEL1];
    _PL[16] = customer->dat[CU_FAX];
    qd(customer->dat[CU_CURRENCY],&currency);
    strcpy(p2date,myear(two.dat[2]));

    if(tso.sumfield) {
        if(tso.dat[Ts_CONVERT][0]=='Y') {
            lset(currency.dat[1],profile.dat[PF_CURRENCY]);
            qd(currency.dat[1],&currency);
        }
        strcpy(p2date,myear(tso.dat[Ts_D2]));
    }
    _PL[6] = currency.dat[CY_PERFIX];
    if(tdays>0) {
        sprintf(TmpTco,"%3i Days",tdays);
    }
    else {
        sprintf(TmpTco,"%s"," C.O.D. ");
    }
    _PL[7]  = TmpTco;
    _PL[31] = p2date;
}

int  NoOfDaysOutstanding(char *date)
{
    int days = diffmon(date,tso.dat[2]);
    int j = days;
    if(j>11) j = 11;
    return j;
}

void FindInvoiceOutstandingBalance(char *date,double offset)
{
    int j = NoOfDaysOutstanding(date);
    Over_Balance[j] += offset;
}

void SetStatementInvoiceData(DB *df,DB *rep)
{
    lsetblank(rep);
    lset(rep->dat[St_INVNO],df->dat[IV_BILLNO]);
    lset(rep->dat[St_BILLNO],df->dat[IV_BILLNO]);
    if(STCR != St_CR) {         //ap module
        if(!isblank(df->dat[2])) {                     //supplier invoice no
            lset(rep->dat[St_INVNO],df->dat[IV_BILLNO]);
            lset(rep->dat[St_BILLNO],df->dat[IV_INVOICE]);
        }
    }
    lset(rep->dat[St_DATE],df->dat[IV_DATE]);
    lset(rep->dat[St_IDATE],df->dat[IV_DATE]);
    lset(rep->dat[St_NAME],df->dat[IV_REM]);
    lset(rep->dat[St_TYPE],"2");                               //Purchase
}

int GetAnalysisBalance(void(*gst)(void))
{
    memset(Over_Balance,0,sizeof(Over_Balance));
    (*gst)();
    return FilterBfSTate();
}

void PrintStatement(FILE *ptr)
{
    FM fm;
    DB st2;

    if(STCR == St_CR) {
        ropen(&st2,"st2.rep");
    }
    else {
        ropen(&st2,"state_ap.rep");
    }
    dupcpy(&st2,&st1);
    layout_report(ptr,&fm,&tso,&st2,solve);
    dclose(&st2);
}

void auditor_statement()
{
    FLASH = 0;

    if(STCR == St_CR) {
       rbopen(&tso,"tso.scr",init_tso);
    }
    else {
       rbopen(&tso,"tso2.scr",init_tso);
    }
    rset(tso.dat[Ts_PAGE],"29");
    ropen(&st,"st.rep");
    ropen(&st1,"st1.rep");
    ropen(&report,"satb.rep");

    CTso t1(IDD_TSO,&tso);
	t1.bRangerSelect = FALSE;
    t1.gst = GetAuditorStateData;
    t1.DoModal();

    dclose(&tso);
    dclose(&st);
    dclose(&st1);
    dclose(&report);

    FLASH = 1;
}
