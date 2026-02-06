// SetPrice.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "SetPrice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetPrice dialog


CSetPrice::CSetPrice(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
: CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CSetPrice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetPrice::DoDataExchange(CDataExchange* pDX)
{
	CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetPrice)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetPrice, CStaff)
	//{{AFX_MSG_MAP(CSetPrice)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetPrice message handlers
extern BOOL IsWholeWildString(char *str);
void upp_pm(DB *df)
{
    char d1[20],d2[20];
    strcpy(d1,DateStr(df->dat[ML_DATE1]));
    strcpy(d2,DateStr(df->dat[ML_DATE2]));

    CString cmd,filter,str,cmdOwndel,strFrom;
	int Flag=0;
	if (df->dat[ML_ACCODETYPE][0]=='Y'){
		cmd.Format("update [dtitem] set [PRICE1] = %f ,[AMOUNT1] = [QTY1] * %f ",
			df->val(ML_PRICE),df->val(ML_PRICE));
		strFrom.Format("FROM dbo.DT LEFT OUTER JOIN dbo.DTITEM ON dbo.DT.TICKET = dbo.DTITEM.TICKET	");
		cmd +=strFrom;
		filter.Format("where dbo.DT.[date] >= '%s' AND dbo.DT.[date] <= '%s' ", d1, d2);
		if(!IsWholeWildString(df->dat[ML_ACCODE])) {
			str.Format("and dbo.DT.[ACCODE] = '%s' ",df->dat[ML_ACCODE] );
			filter += str;
		}
		if(!IsWholeWildString(df->dat[ML_PT])) {
			str.Format("AND dbo.DTITEM.[PT] = '%s' ",df->dat[ML_PT] );
			filter += str;
		}
		str.Format("AND dbo.DTITEM.TICKET NOT IN (SELECT TICKET FROM DS)");
		filter += str;
		cmd += filter;
		Flag = RunSql(cmd);
		if (Flag ==1){
			str.Format("select dbo.DT.[TICKET],dbo.DT.[DATE],dbo.DT.[ACCODE],dbo.DTITEM.[PT],dbo.DTITEM.[PRICE1] ");
			str+=strFrom;
			str+=filter;
			DB adoTmp(1,5);
			ado(&adoTmp,"",NULL,str);
			viewidx(&adoTmp);
			dclose(&adoTmp);
		}
	}
	else{
		cmd.Format("update [dtitem] set [PRICE2] = %f ,[AMOUNT2] = [QTY2] * %f ",
			df->val(ML_PRICE),df->val(ML_PRICE));
		strFrom.Format("FROM dbo.DT LEFT OUTER JOIN dbo.DTITEM ON dbo.DT.TICKET = dbo.DTITEM.TICKET	");
		cmd +=strFrom;
		filter.Format("where dbo.DT.[date] >= '%s' AND dbo.DT.[date] <= '%s' ", d1, d2);
		if(!IsWholeWildString(df->dat[ML_ACCODE])) {
			str.Format("and dbo.DT.[ACCODE2] = '%s' ",df->dat[ML_ACCODE] );
			filter += str;
		}
		if(!IsWholeWildString(df->dat[ML_PT])) {
			str.Format("AND dbo.DTITEM.[PT2] = '%s' ",df->dat[ML_PT] );
			filter += str;
		}
		str.Format("AND dbo.DTITEM.TICKET NOT IN (SELECT TICKET FROM DP)");
		filter += str;
		cmd += filter;
		Flag = RunSql(cmd);
		if (Flag ==1){
			str.Format("select dbo.DT.[TICKET],dbo.DT.[DATE],dbo.DT.[ACCODE2],dbo.DTITEM.[PT2],dbo.DTITEM.[PRICE2] ");
			str+=strFrom;
			str+=filter;
			DB adoTmp(1,5);
			ado(&adoTmp,"",NULL,str);
			viewidx(&adoTmp);
			dclose(&adoTmp);
		}
	}
	
	if (Flag ==1){
		lset(df->dat[ML_UPDATE],"Y");
		df->Update();
		AfxMessageBox("Update Completed");	
	}
	
    
/*
    cmd.Format("update [dtitem] set [AMOUNT] = [QTY1] * [PRICE1] ",
        df->val(ML_PRICE));
    cmd += filter;
    RunSql(cmd);
*/
}

BOOL CSetPrice::IsCannotEdit()
{
    if(df->dat[ML_UPDATE][0]=='Y') {
		waitmsg("Price Updated.Modification Not Allow");
		return TRUE;
	}	
    return FALSE;
}

void CSetPrice::ShowInformation()
{
	CStaff::ShowInformation();
}

void CSetPrice::after_save(int ExistFlag)
{
    if ( choice_menu("Posting Data",FAIL) == OK ){
        upp_pm(df);

    }
}

int setprice_save(DB *df,int flagi)
{
    if(atof(df->dat[ML_PRICE])<=0){
        waitmsg("Unit Price <= 0");
        return 0;
    }
    long rec = qd(df->dat[ML_PT],&prodtype);
    if(rec==0){
        waitmsg("Please key in product");
        return 0;
    }
	rec = qd(df->dat[ML_ACCODE],&accode);
    if(rec==0){
        waitmsg("Please key in ACCODE");
        return 0;
    }
    return 1;
}

int setprice_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = ML_UPDATE;
    fld[1] = 0;    
    return 1;
}

void init_setprice(DB *df)
{
	df->e->save      = setprice_save;
	df->e->initial   = setprice_initial;
}

void DirectDeliveryPricingUpdate()
{
	DB setprice(IDD_SETPRICE);
	ado(&setprice,"setprice",init_setprice);
    
	CSetPrice p1(setprice.ID,&setprice,setprice.f[1].idx & 0x01);	    
    p1.DoModal();

    dclose(&setprice);
}