// Wpr.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Wpr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWpr dialog


CWpr::CWpr(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
: CWeighing(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CWpr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWpr::DoDataExchange(CDataExchange* pDX)
{
	CWeighing::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWpr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWpr, CWeighing)
	//{{AFX_MSG_MAP(CWpr)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWpr message handlers
void getprmet(long *m,DB *df)
{

    m[1] = atol(df->dat[PR_SGROSS]);
    m[2] = atol(df->dat[PR_STARE]);    
    m[4] = labs(m[2] - m[1]);
    
}

void CWpr::SaveSecondWeightTicket(DB *fdes,DB *mfile)
{
	int  IdxItem = mfile->f[1].idx & 1;
	
    fetch(mfile,fdes);
    FindAddInfoTicketNo(mfile);
    mfile->SqlUpdate(TRUE);
    CountTicketNo(mfile,1);
}


void CWpr::OnSave()
{
    int  flag;
	
	if(SaveFlag == CANNOTSAVE) {
		MessageBox("Can not save");
	}
	else if(SaveFlag == NEWFILE){
        flag = (*df->e->save_first_wb)(df,0);
        if(flag==0) return;
        waitflock(df);
        FindAddInfoTicketNo(df);            // ooi        CountTicketNo(df,1);
        save_entry(NONEXIST,IdxItem,0,df);      //TRUE: NO NEED LOCK AGAIN
        W1Saved();
        unflock(df);
        if(df->e->prec != unprint) {
			//            PrintLoadingUnloadingForm(0,NULL,df);
        }
        OnAddedit();
    }
	else if(SaveFlag == OLDFILE){
        flag = (*df->e->save)(df,0);
        if(flag==0) return;
        ReplaceRecord();
        SaveSecondWeightTicket(df,m_mfile); 
        W2Saved();   
        if(m_mfile->e->prec != unprint) {
            if(MessageBox("Do you want to print","option",MB_YESNO | MB_ICONQUESTION)==IDYES) {
				fetch(m_mfile,df);
                OnPrintTicket();
            }
        }
        OnDeleteFirstWeightRecord();   
        OnAddedit();
	}
}

void CWpr::W1Start(long weight)
{
    dbasedate(df->dat[PR_DATE]);
    _strtime(df->dat[PR_TIMEIN]);
    ltodb(weight,df,PR_SGROSS);
    ltodb(ActualWeight,df,PR_W1);
    ltodb(weight,df,PR_Z1);
	lset(df->dat[PR_USER1],user.dat[1]);
}

void CWpr::W2Start(long weight)
{
    ltodb(weight,df,PR_STARE);
    dbasedate(df->dat[PR_DATE]);
    _strtime(df->dat[PR_TIMEOUT]);	
	ltodb(ActualWeight,df,PR_W2);
	ltodb(weight,df,PR_Z2);
	lset(df->dat[PR_USER2],user.dat[1]);
}

void CWpr::ShowInformation()
{
    CStaff::ShowInformation();
    getprmet(m,df);
   
	char str[20];
    ltot(m[4],6,str);
    SetDlgItemText(IDC_NETT,putnum(str));
	
}

void CWpr::W2Saved()
{
    
}

int  PrFirstArray[]  ={ 0,PR_SERIAL,PR_DRIVER,PR_ACCODE,PR_PRODUCT,PR_REMARKS,PR_AMOUNT,PR_VEHICLE,0};
int  PrArray[]  = { 0,PR_SERIAL,PR_DRIVER,PR_ACCODE,PR_PRODUCT,PR_REMARKS,PR_AMOUNT,0};
void PublicWeighingProcess(DB *main,DB *ds)
{
	
    CWpr p1(main->ID,ds,main->f[1].idx & 0x01);

    p1.df          = ds;
    p1.m_mfile     = main;
    p1.vehicle_fld = PR_VEHICLE;
    p1.w1_array    = PrFirstArray;
    p1.w2_array    = PrArray;
    p1.LastField   = PR_USER2;
	p1.m_WinPrint  = FALSE;
    p1.DoModal();
	
}

int pr_initial(DB *df,int *fld)                //initial setting
{
	fld[0] = PR_USER1;
	fld[1] = PR_USER2;
	if (UserLevel()<999){
		fld[3] = PR_MODIFY;
		fld[4] = 0;		
	}
	if (UserLevel()<9){
		fld[3] = PR_DATE;
		fld[4] = 0;		
	}
	if(UserLevel()<5) {
	//	fld[1] = PR_STARE;
	//	fld[2] = PR_SGROSS;
		fld[4] = PR_TIMEIN;
		fld[5] = PR_TIMEOUT;        
		fld[6] = 0;
	}
    return 1;
}

int pr_tell(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
	switch(fld) {
	case PR_VEHICLE:
		if(strcmp(df->dat[fld],df->bak[fld])) {
            qd(df->dat[PR_VEHICLE],&lorry);			
			lset(df->dat[PR_ACCODE],lorry.dat[LO_DRIVER2]);
			qd(lorry.dat[LO_DRIVER1],&driver);
            lset(df->dat[PR_DRIVER],driver.dat[2]);
			CopyBufferToControl(PR_ACCODE,df);
            CopyBufferToControl(PR_DRIVER,df);
            return TAB_REFRESH_ALL;
        }
		break;
	}
	return TAB_REFRESH_ALL;
}

int pr_save(DB *df,int flagi)
{
	if (df->val(PR_SGROSS)!=atoi(df->bak[PR_SGROSS])){
		lset(df->dat[PR_USER1],user.dat[1]);
	}
	if (df->val(PR_STARE)!=atoi(df->bak[PR_STARE])){
		lset(df->dat[PR_USER2],user.dat[1]);
	}
	if(strcmp(df->m_strTable,"pr_new")==0){
		SetModifyMark(PR_SGROSS,PR_MODIFY,df);
		SetModifyMark(PR_STARE,PR_MODIFY,df);
	}
	return 1;
}


void init_pr(DB *df)
{
//    df->e->prec		 = PrintPublicTicket;
    df->e->initial	 = pr_initial;
	df->e->tell		 = pr_tell;
	df->e->save      = pr_save;
    
}

void init_wpr(DB *df)
{
    df->e->prec		 = unprint;    
    df->e->initial	 = pr_initial;
}
