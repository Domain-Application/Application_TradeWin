// Weighing.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Weighing.h"
#include "GetWt.h"
#include "comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeighing dialog


CWeighing::CWeighing(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CWeighing)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    vehicle_fld = 0;
    m_mfile     = NULL;
    df          = NULL;
    w1_array = w2_array = NULL;
}

CWeighing::~CWeighing()
{
    CloseCommThread();
}


void CWeighing::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeighing)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWeighing, CStaff)
	//{{AFX_MSG_MAP(CWeighing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeighing message handlers
int FldIndex(int *array,int fld)
{
    int i,f;

    for(i=1; f=array[i]; i++) {
        if(f==fld) return i;
    }
    return 0;
}

int CWeighing::GetPreviousField(int fld)
{
    int idx = FldIndex(m_array,fld);
    fld = m_array[idx-1];
    if(fld>0) return fld;
    return m_array[1];
}

BOOL CWeighing::IsLastField(int fld)
{
    int idx = FldIndex(m_array,fld);
    fld = m_array[idx+1];
    if(fld==0) return TRUE;
    return FALSE;
}

void CWeighing::W1Start(long weight)
{
}

void CWeighing::W2Start(long weight)
{
}

void CWeighing::OnAddedit()
{
    CStaff::OnAddedit();
    lsetblank(df);
    df->e->start(df);
    ShowInformation();

    CGetWt p1;
    p1.df = df;
    p1.m_vehicle = df->dat[vehicle_fld];
    int flag = p1.DoModal();

	if(flag==IDCANCEL) {
         SaveFlag = CANNOTSAVE;
         OnCancel();
         return;
	}
    ZeroWeight1 = FALSE;
    lset(df->dat[vehicle_fld],p1.m_vehicle);
    if(isblank(df->dat[vehicle_fld])) {
        if(df->rec<=0 || df->rec> df->t->sumrecord) df->rec = 1;
        df->rec       = ShowField1Table();
		if(df->rec ==0) {
			SaveFlag = CANNOTSAVE;
			OnCancel();
			return;
		}
        ExistRecordNo = ridx(df->rec,df,IdxItem);
	}
    CheckVehicleNo(df->dat[vehicle_fld]);               //vehicle no
    if(isblank(df->dat[vehicle_fld])) return;
    //ExistRecordNo =  find(df->dat[vehicle_fld], 1l, vehicle_fld, df);
	ExistRecordNo =  df->Search(df->dat[vehicle_fld],vehicle_fld, TRUE);
    backup(df);
    if(ExistRecordNo==0) {
		lset(df->dat[vehicle_fld],p1.m_vehicle);
		CheckVehicleNo(df->dat[vehicle_fld]); 
		if (strcmp(df->m_strTable,"wpr_new")==0){
			qd(df->dat[PR_VEHICLE],&lorry);			
			lset(df->dat[PR_ACCODE],lorry.dat[LO_DRIVER2]);
			qd(lorry.dat[LO_DRIVER1],&driver);
            lset(df->dat[PR_DRIVER],driver.dat[2]);
		}
		else{
			qd(df->dat[DP_VEHICLE],&lorry);			
			lset(df->dat[DP_IC],lorry.dat[LO_DRIVER1]);
			lset(df->dat[DP_ACCODE],lorry.dat[LO_DRIVER2]);
			lset(df->dat[DP_TRANSP],lorry.dat[LO_TRANSP]);
			qd(df->dat[DP_IC],&driver);
			lset(df->dat[DP_DRIVER],driver.dat[2]);
		}
        NewFlag  = NEWFILE;
        SaveFlag = NEWFILE;
        m_array  = w1_array;
        W1Start(p1.weight);
    }
    else {
        SaveFlag = OLDFILE;
        NewFlag  = OLDFILE;
		m_FirstTicket = df->dat[1]; // ccwong 20101005
//        ReadTicketNo(m_mfile,1);                       //get ticket no
		FindAddInfoTicketNo(m_mfile);				//2006-05-31
        lset(df->dat[1],m_mfile->dat[1]);
        m_array  = w2_array;
		W2Start(p1.weight);
		if (strcmp(df->m_strTable,"wdp")==0){   // Find last D/O , only for WDP 20090610 CCWong
			CString str,founddo;
			str.Format("SELECT TOP 1 DO FROM dbo.DP WHERE (ISNUMERIC(DO) = 1) ORDER BY DO DESC");
			founddo = SqlText(str);
			long dono = atof(founddo);
			dono +=1;
			char xdono[40];
			dtoa(dono,xdono,12,0);
			ltrim(xdono);
			lset(df->dat[DP_DO],xdono);		
		}
    }
    if(m_array[1]==0) {
        ShowInformation();
        OnCancel();
        return;
    }

    SetFieldEnable(m_array);
    GotoDlgCtrl(GetDlgItem(IDC_EDIT1+m_array[1]-1));
    ShowInformation();
    ShowSaveButton();
}

void CWeighing::SetFieldEnable(int *fld)
{
    int i,f;

    SetAttribute(TRUE,1);
    for(i=1; f=fld[i]; i++) {
		m_ed[f].EnableWindow(TRUE);
        m_ed[f].SetReadOnly(FALSE);
    }
}

int CWeighing::GetNextField(int fld)
{
    int idx = FldIndex(m_array,fld);
    fld = m_array[idx+1];
    if(fld>0) return fld;
    int flag = MessageBox("Do you want to save your data !","Save",MB_YESNOCANCEL | MB_ICONQUESTION);
    if(flag == IDYES) {
        OnDirectSave();
    }
    else if(flag == IDNO) {
        OnAddedit();
    }
    return m_array[1];
}

void CWeighing::SaveSecondWeightTicket(DB *fdes,DB *mfile)
{
	int  IdxItem = mfile->f[1].idx & 1;
	
	lsetblank(mfile);
    fetch(mfile,fdes);
    FindAddInfoTicketNo(mfile);
	FindProductCategorySerialNo(mfile);
    mfile->SqlUpdate(TRUE);
    CountTicketNo(mfile,1);
/*    fetch(mfile,fdes);
    long hrec = mfile->Search(fdes->dat[1],IdxItem,FALSE);      // 6.6.2000
    if(hrec>0) {
        FindAddInfoTicketNo(mfile);
    }
    FindProductCategorySerialNo(mfile);
    mfile->SqlUpdate(TRUE);
    if(hrec>0) {
        CountTicketNo(mfile,1);
    }*/
}

void CWeighing::OnPrintTicket()
{
    FILE *ptr;

    ptr = fopen("lpt1","w");
    if(ptr) {
        setvbuf(ptr,NULL,_IONBF,0);
        (*m_mfile->e->prec)(m_mfile->rec,ptr,m_mfile);
    }
    else {
        MessageBox("Error Opening LPT1.");
    }
    fclose(ptr);
 //   (*m_mfile->e->prec)(m_mfile->rec,NULL,m_mfile);
}

void CWeighing::W1Saved()
{
}

void CWeighing::W2Saved()
{
}

void CWeighing::OnDeleteFirstWeightRecord()
{
    /*
    long frec = find(df->dat[vehicle_fld], 1l, vehicle_fld, df);
    if(frec>0){
        deleterec(frec,df);
    }
    else {
        char tmp[80];

        sprintf(tmp,"%s Vehicle not found",df->dat[vehicle_fld]);
        MessageBox(tmp);
    }
    */
	char tmp[80];
	
    long frec =  df->Search(df->dat[vehicle_fld],vehicle_fld);
    if(frec>0){
         waitflock(df);
         CString str;
         str.Format("DELETE FROM [%s] WHERE [%s]= '%s' ",
             df->m_strTable, df->f[vehicle_fld].name, df->dat[vehicle_fld]);
         RunSql(str);
         unflock(df);
         df->ReOpen();
     }
     else {
         sprintf(tmp,"%s Vehicle not found",df->dat[vehicle_fld]);
         MessageBox(tmp);
     }
}
void SaveWeightMaxDataWB(DB *df,long confirmWeight,long weight2)
{
    if(!app->m_RecordWtStatus) return;
    if(cnn == NULL) return;
	
    if(weight2==0) weight2 = confirmWeight;
	
    FILE *ptr = fopen("wt.log","a");
    if(ptr) {
        char szTime[20];
        _strtime(szTime);
        fprintf(ptr,"Date/Time: %s %s %s wt: %ld\n",
            mdate(today), szTime,
            app->m_strComputerName, confirmWeight);
        fclose(ptr);
    }
	
    int fldLorry = GetDbFieldNo(df,"LORRY");
    if(fldLorry==0) {
        fldLorry = GetDbFieldNo(df,"VEHICLE");
        if(fldLorry==0) {
            fldLorry = GetDbFieldNo(df,"TRX_LORRY");
        }
    }
    CString str;
    str.Format("insert into [weight_max_data] (NEWUSER,WBNO,[ticket],[lorry],[weight],[FIRSTTICKET],[TABLE]) values ('%s','A','%s','%s',%ld,'%s','%s') ",
        user.dat[1],df->dat[1], df->dat[fldLorry], confirmWeight,m_FirstTicket,df->m_strTable);
    runsql(str,FALSE,FALSE,cnn__2);   // ccwong 20110208
	//RunSql(str);
    TicketVehicleNo = df->dat[fldLorry];
    TicketWt = weight2;
    WbTicketSaved = 1;
	m_FirstTicket="";
}

void CWeighing::OnSave()
{
    int  flag;
	ReplaceRecord();
	//CheckMaxWeight();
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
		m_FirstTicket = ""; // ccwong 20101020
		SaveWeightMaxDataWB(df,df->val(DP_SGROSS));
		IsWBZeroWt = 0;
		CountTicketNo(df,1);
        OnAddedit();
    }
	else if(SaveFlag == OLDFILE){
		
        flag = (*df->e->save)(df,0);
        if(flag==0) return;
        waitflock(df);
        waitflock(m_mfile);
        ReplaceRecord();
        SaveSecondWeightTicket(df,m_mfile);
        W2Saved();
        OnDeleteFirstWeightRecord();
        unflock(df);
        unflock(m_mfile);
        if(m_mfile->e->prec != unprint) {
            if(MessageBox("Do you want to print","option",MB_YESNO | MB_ICONQUESTION)==IDYES) {
                // OnPrintTicket();
                // m_mfile->IncPrinterCounter(); // ccwong 20110531
                CString str;
	            str.Format("%s.RPT",m_mfile->name);
	            // df->m_PreviewCrystalReport = TRUE; //20230326 user requested to auto print
	            m_mfile->PrintOneFormOnly(str);
				
            }
        }
		SaveWeightMaxDataWB(df,df->val(DP_STARE));
		IsWBZeroWt = 0;
        OnAddedit();
	}
}
void CWeighing::CheckMaxWeight()
{
	CString str;
	str.Format("SELECT  TOP 1 PKKEY FROM WEIGHT_BRIDGE_DATA WHERE (WEIGHT = 0) ORDER BY PKKEY DESC");
	long key = SqlValue(str);
	str.Format("SELECT  MAX(WEIGHT) FROM WEIGHT_BRIDGE_DATA WHERE PKKEY>%i",key);
	double maxwt = SqlValue(str);
	if (isblank(df->dat[DP_STARE])){
		if (maxwt>df->val(DP_SGROSS)+profile.val(PF_VARTOLERANCE)){
			str.Format("Max weight %6.0f > Gross Weight %6.0f + Tolerance %6.0f",maxwt,df->val(DP_SGROSS),profile.val(PF_VARTOLERANCE));
			AfxMessageBox(str);
		}
	}
	else{
		if (maxwt>df->val(DP_STARE)+profile.val(PF_VARTOLERANCE)){
			str.Format("Max weight %6.0f > Gross Weight %6.0f + Tolerance %6.0f",maxwt,df->val(DP_STARE),profile.val(PF_VARTOLERANCE));
			AfxMessageBox(str);
		}
	}
	
}

BOOL CWeighing::ChangeToEditMode(int fld)
{
    if(SaveFlag == CANNOTSAVE) {
        GotoDlgCtrl(GetDlgItem(IDC_EDIT1));
        return FALSE;
    }
	else {
		if(fld==1) {
			MessageBox("Can not change code no");
		}
	}
	return FALSE;
}

int CWeighing::OnTellMe(UINT nChar,int &id,int &fld)
{
	CString str;

    ReplaceFieldContent(fld);
    int flag = (*df->e->tell)(nChar,id,fld,df,0);
	if(flag==0) {
		return 0;
	}
    if(flag) {
        if(fld==1) {
        }
        else {
            int q = strcmp(df->old[fld],df->dat[fld]);
            pf = GetList(df,fld);
            if(pf) {
                if(pf->rec==0) return 0;
                q = strcmp(df->old[fld],df->dat[fld]);
            }
            flag = (*df->e->tell)(nChar,id,fld,df,q);
        }
    }
    if(flag==2 || flag==TAB_REFRESH_ALL) {
        ShowInformation();
    }
    else {
        ShowFieldContent(fld);
    }
	ShowRecordState();
	return flag;
}

BOOL CWeighing::OnInitDialog() 
{
    if(vehicle_fld==0) {
        MessageBox("Please assign vehicle field no");
        CDialog::OnCancel();
        return TRUE;  // return TRUE unless you set the focus to a control
    }
    if(m_mfile == NULL) {
        MessageBox("Please assign ticket DB pointer");
        CDialog::OnCancel();
        return TRUE;  // return TRUE unless you set the focus to a control
    }
    if(df == NULL) {
        MessageBox("Please assign first weight DB pointer");
        CDialog::OnCancel();
        return TRUE;
    }
    if( w1_array == NULL ) {
        MessageBox("Please assign first weight array");
        CDialog::OnCancel();
        return TRUE;  // return TRUE unless you set the focus to a control
    }
    if( w2_array == NULL ) {
        MessageBox("Please assign second weight array");
        CDialog::OnCancel();
        return TRUE;  // return TRUE unless you set the focus to a control
    }
    HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_WB);
    this->SetIcon(m_hIcon, TRUE);         // Set big icon

	CStaff::OnInitDialog();

	return TRUE;	// Due to user not allow to enter weighing entry - 20211004 JF


#ifdef _DEBUG
//	return TRUE;
#endif

    HWND hWnd = GetSafeHwnd();
    BOOL bOpen = SetCommThread(hWnd);
/*	if(bOpen==FALSE) {
        OnCancel();
    }
*/	// Due to user not allow to enter weighing entry - 20211004 JF

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWeighing::OnDirectSave()
{
    OnSave();
}

void CWeighing::OnNew()
{
     OnAddedit();
}

int CWeighing::GetRecord()
{
    OnAddedit();
    return 1;
}


