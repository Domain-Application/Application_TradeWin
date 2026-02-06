#include "stdafx.h"
#include "menu.h"
#include "arinv.h"
#include "ipay.h"
#include "cr.h"
#include "Wac.h"

int OpenFlag,ProduresID;
double OinvAmt;
BOOL bTaxInclude;
char *AcStr[] = {"Debtor","Creditor"};
void ShowInvoiceInformation()
{
    inv->rec = qd(inv->dat[1],inv);
    if(inv->rec==0) return;

    CArinv p1(IDD_ARINV,inv,1);
    p1.m_customer    = AcStr[ProgramMenuNo-1];
    p1.m_salesman    = "Salesman";
    p1.m_EnquiryOnly = TRUE;
    p1.IDD_SUB       = IDD_OINV;
    p1.sub           = oinv;
    p1.DoModal();
}

void ShowIpayInformation()
{
    ipay->rec = qd(ipay->dat[1],ipay);
    if(ipay->rec==0) return;

    CIpay p1(IDD_IPAY,ipay,1);
    p1.sub     = paydat;
    p1.IDD_SUB = IDD_PAYDAT;
    p1.m_customer = AcStr[ProgramMenuNo-1];
    p1.m_salesman    = "Salesman";
    p1.m_EnquiryOnly = TRUE;
    p1.DoModal();
}

void ShowRCInformation()
{
    rc->rec = qd(rc->dat[1],rc);
    if(rc->rec==0) return;

    CIpay p1(IDD_IPAY,rc,1);
    p1.sub     = rcdat;
    p1.IDD_SUB = IDD_PAYDAT;
    p1.m_customer = AcStr[ProgramMenuNo-1];
    p1.m_salesman    = "Salesman";
    p1.m_EnquiryOnly = TRUE;
    p1.DoModal();
}

void ShowCRInformation()
{
    cr->rec = qd(cr->dat[1],cr);
    if(cr->rec==0) return;

    CCr p1(IDD_CR,cr,1);
    p1.sub     = crdat;
    p1.IDD_SUB = IDD_CRDAT;
    p1.m_customer = AcStr[ProgramMenuNo-1];
    p1.m_salesman = "Salesman";
    p1.m_EnquiryOnly = TRUE;
    p1.DoModal();
}

void ShowWACInformation()
{
    wac.rec = qd(wac.dat[1],&wac);
    if(wac.rec==0) return;

    CWac p1(IDD_WAC,&wac,1);
    p1.m_EnquiryOnly = TRUE;
    p1.sub     = &wacitem;
    p1.IDD_SUB = IDD_WACITEM;
    p1.DoModal();
}

HANDLE VatNo;
void CloseARData()
{
    CloseHandle(VatNo);
    ProgramMenuNo = 0;
}

void CloseAPData()
{
    ProgramMenuNo = 0;
}

void CloseGLData()
{
    ProgramMenuNo = 0;
}

void OpenARData()
{
    ProgramMenuNo = 1;
    customer = &debtor;
    ipay = &aripay;
    paydat = &arpaydat;
    rc = &arrc;
    rcdat = &arrcdat;
    gldat = &ar_gldat;
    cr = &arcr;
    crdat = &arcrdat;
    oinv  = &ar_oinv;
    inv  = &arinv;
    STCR = St_CR;
    STDR = St_DR;
}

void OpenAPData()
{
    ProgramMenuNo = 2;
    customer = &creditor;
    ipay = &apipay;
    paydat = &appaydat;
    rc = &aprc;
    rcdat = &aprcdat;
    gldat = &ap_gldat;
    cr = &apcr;
    crdat = &apcrdat;
    oinv  = &ap_oinv;
    inv  = &apinv;
    STCR = St_DR;
    STDR = St_CR;
}

void OpenGLData()
{
    ProgramMenuNo = 3;
    STCR = St_CR;
    STDR = St_DR;

    if(OpenFlag == 3){
        return;
    }
    else if(OpenFlag == 2){
        CloseAPData();
    }
    if(OpenFlag == 1){
        CloseARData();
    }
    OpenFlag = 3;
    ProgramMenuNo = 3;
}

void CheckCompanyProfileSetting()
{
    for(uint i=1; i<= profile.sumfield; i++) {
        if(profile.f[i].mode!='L') continue;
        char ch = profile.dat[i][0];
        if(ch=='T') {
            lset(profile.dat[i],"Y");
        }
        else if(ch=='F' || ch==' ') {
            lset(profile.dat[i],"N");
        }
    }
    writerec(1,&profile);
}

void SetPaperSize(FILE *ptr)
{
    int lp[] = { 66, 44, 33, 22, 0};
    int offset = 0;

    if(OpenFlag == 3){
        if(profile.dat[PF_GLWAC][0]=='2') offset = 2;
        if(profile.dat[PF_GLWAC][0]=='3') offset = 3;
        fprintf(ptr,"2C%c",char(lp[offset]));
    }
    else if(OpenFlag == 1){
        if(profile.dat[PF_ARPAY][0]=='2') offset = 2;
        if(profile.dat[PF_ARPAY][0]=='3') offset = 3;
        fprintf(ptr,"2C%c",char(lp[offset]));
    }
    else if(OpenFlag == 2){
        if(profile.dat[PF_ARPAY][0]=='3') offset = 1;       //  2/3 PAGE
        fprintf(ptr,"2C%c",char(lp[offset]));
    }
}

void NotePad(char *file)
{
    char cmd[90];
    sprintf(cmd,"notepad %s",file);
    WinExec(cmd,SW_SHOW);
}
