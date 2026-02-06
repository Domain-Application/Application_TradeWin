// Daily.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Daily.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaily dialog


CDaily::CDaily(UINT IDD,DB *pdf,int Idx,void(*p)(),CWnd* pParent /*=NULL*/)
    : CTio(IDD,pdf,Idx,p,pParent)
{
	//{{AFX_DATA_INIT(CDaily)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDaily::DoDataExchange(CDataExchange* pDX)
{
    CTio::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDaily)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


void CDaily::show_tio()
{
	long xrec=1;
    char tmp[80];

    TmpTio[0] = 0;
    if(df->dat[Da_ACCODE][0]!='*') {
        qd(df->dat[Da_ACCODE],&accode);
        sprintf(tmp,"\nAccode No   : %s %s",df->dat[Da_ACCODE],accode.dat[2]);
        strcat(TmpTio,tmp);
    }
    if(df->dat[Da_VEHICLE][0]!='*') {
        sprintf(tmp,"\nVehicle No  : %s",df->dat[Da_VEHICLE]);
        strcat(TmpTio,tmp);
    }
    if(df->dat[Da_IC][0]!='*') {
        qd(df->dat[Da_IC],&driver);
        sprintf(tmp,"\nDriver IC   : %s %s",df->dat[Da_IC],driver.dat[2]);
        strcat(TmpTio,tmp);
    }
    if(df->dat[Da_PT][0]!='*') {
        qd(df->dat[Da_PT],&prodtype);
        sprintf(tmp,"\nProduct     : %s %s",df->dat[Da_PT],prodtype.dat[2]);
        strcat(TmpTio,tmp);
    }
    if(df->dat[Da_TRANSP][0]!='*') {
        qd(df->dat[Da_TRANSP],&transp);
        sprintf(tmp,"\nTransporter : %s %s",df->dat[Da_TRANSP],transp.dat[2]);
        strcat(TmpTio,tmp);
    }
    if(df->dat[Da_CONTRACT][0]!='*') {
        sprintf(tmp,"\nContract No : %s",df->dat[Da_CONTRACT]);
        strcat(TmpTio,tmp);
    }
    if(df->dat[Da_PTYPE][0]!='*') {
        qd(df->dat[Da_PTYPE],&ptype);
        sprintf(tmp,"\nPay Type    : %s %s",df->dat[Da_PTYPE],ptype.dat[2]);
        strcat(TmpTio,tmp);
    }
	if(df->dat[Da_COUNTRY][0]!='*') {
        qd(df->dat[Da_COUNTRY],&country);
        sprintf(tmp,"\nCountry     : %s %s",df->dat[Da_COUNTRY],country.dat[2]);
        strcat(TmpTio,tmp);
    }
    _PL[1]  = TmpTio;
    app->SetStdCompanyLogo();
    lset(logo.dat[Logo_DATE1],df->dat[1]);
    lset(logo.dat[Logo_DATE2],df->dat[2]);
    lset(logo.dat[Logo_SELECT1],TmpTio);
    lset(logo.dat[Logo_RTITLE],SubTitle);
    lset(logo.dat[Logo_USER],user.dat[1]);
    lset(logo.dat[Logo_USERNAME],user.dat[2]);
    writerec(1,&logo);
}

BEGIN_MESSAGE_MAP(CDaily, CTio)
	//{{AFX_MSG_MAP(CDaily)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaily message handlers
