// wdes.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "wdes.h"
#include "GetWt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cwdes dialog


extern int FldIndex(int *array,int fld);
Cwdes::Cwdes(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CStaff(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(Cwdes)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Cwdes::DoDataExchange(CDataExchange* pDX)
{
    CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cwdes)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void Cwdes::SetFieldEnable(int *fld)
{
    int i,f;

    SetAttribute(TRUE,1);
    for(i=1; f=fld[i]; i++) {
        if(df->f[f].mode=='D') {
            m_date[f].EnableWindow(TRUE);
        }
        else {
			m_ed[f].EnableWindow(TRUE);
            m_ed[f].SetReadOnly(FALSE);
        }
    }
}

void Cwdes::OnAddedit()
{
	int flag;

    CStaff::OnAddedit();
    CGetWt p1;
    lsetblank(df);
    dbasedate(df->dat[PM_DATE]);
    _strtime(df->dat[PM_TIMEIN]);
    ShowInformation();
    flag = p1.DoModal();
	if(flag==IDCANCEL) {
         SaveFlag = CANNOTSAVE;
         OnCancel();
         return;
	}
    lset(df->dat[PM_VEHICLE],p1.m_vehicle);
	if(isblank(df->dat[PM_VEHICLE])) {
		df->rec = ShowField1Table();
        if(df->rec == 0) {
             SaveFlag = CANNOTSAVE;
             OnCancel();
             return;
        }
        ExistRecordNo = ridx(df->rec,df,IdxItem);
	}

    ExistRecordNo =  find(df->dat[PM_VEHICLE], 1l, PM_VEHICLE, df);
    if(ExistRecordNo==0) {
        ltodb(p1.weight,df,PM_PGROSS);
        NewFlag  = NEWFILE;
        SaveFlag = NEWFILE;
        m_array  = FirstArray;
    }
    else {
        SaveFlag = OLDFILE;
        NewFlag  = OLDFILE;
        m_array  = Array;
        ltodb(p1.weight,df,PM_PTARE);
        dbasedate(df->dat[PM_DATE]);
        _strtime(df->dat[PM_TIMEOUT]);
    }
    SetFieldEnable(m_array);
    GotoDlgCtrl(GetDlgItem(IDC_EDIT1+m_array[1]-1));
    ShowInformation();
    ShowSaveButton();
}

void Cwdes::ShowInformation()
{
    char str[20];

    CStaff::ShowInformation();
    getmet(m,df);
    ltot(m[3],6,str);
	SetDlgItemText(IDC_GROSSDIFF,putnum(str));
    ltot(m[6],6,str);
	SetDlgItemText(IDC_TAREDIFF,putnum(str));
    ltot(m[12],6,str);
	SetDlgItemText(IDC_VAR,putnum(str));
    ltot(m[10],6,str);
	SetDlgItemText(IDC_SNET,putnum(str));
    ltot(m[11],6,str);
	SetDlgItemText(IDC_PNET,putnum(str));

}


void SaveDespatch(long frec,DB *fdes,DB *df)
{
    long mrec = appnet(&despatch);

	CountTicketNo(&despatch,1);
    fetch(&despatch,&first);
    AppendIdx(mrec,&despatch,1);

    deleterec(frec,&first);
}

void Cwdes::OnSave()
{
	long nrec;

	if(SaveFlag == CANNOTSAVE) {
		MessageBox("Can not save");
	}
	else if(SaveFlag == NEWFILE){
        nrec = getdel(df);
        ReplaceRecord();
        CountTicketNo(df,1);
        save_entry(NONEXIST,IdxItem,nrec,df);
        OnAddedit();
    }
	else if(SaveFlag == OLDFILE){
        ReplaceRecord();
		save_entry(EXIST,IdxItem,ExistRecordNo,df);
        SaveDespatch(ExistRecordNo,df,m_mfile);
        if(MessageBox("Do you want to print","option",MB_YESNO | MB_ICONQUESTION)==IDYES) {
            df->e->prec    = prec;
            OnPrint();
        }
        OnAddedit();
	}
}

int Cwdes::GetPreviousField(int fld)
{
    int idx = FldIndex(m_array,fld);
    fld = m_array[idx-1];
    if(fld>0) return fld;
    return m_array[1];
}

int Cwdes::GetNextField(int fld)
{
    int idx = FldIndex(m_array,fld);
    fld = m_array[idx+1];
    if(fld>0) return fld;
    return m_array[1];
}


BEGIN_MESSAGE_MAP(Cwdes, CStaff)
	//{{AFX_MSG_MAP(Cwdes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cwdes message handlers
int FirstArray[] = { 0,PM_SEAL01,PM_SEAL02,PM_SEAL03,PM_SEAL04,PM_SEAL05,PM_SEAL06,
                        PM_MILL,PM_CONTRACT,PM_BUYER,PM_PT,PM_TANK,
                        PM_TRANSP,PM_DRIVER,
                        PM_VEHICLE,
                        0};
int  Array[] = { 0,PM_REMARKS,PM_PL3,PM_TRIP,PM_FFA,PM_MOIST,PM_IMP,0};
void weighting_despatch()
{
    Cwdes p1(first.ID,&first,0);

    p1.df         = &first;
//  p1.FirstField = 2;
//  p1.LastField  = 34;   //33
	p1.m_WinPrint  = FALSE;
    p1.DoModal();
}

void ReplaceWdesGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
	CString s;
	char    text[20];

	space_str(text,12);
    grid->SetCol(0);
    s.Format("%ld",rec);
    grid->SetText(s);
    grid->SetCol(1);
    grid->SetText(df->dat[1]);
    grid->SetCol(2);
    grid->SetText(mdate(df->dat[PM_DATE]));
    grid->SetCol(3);
    grid->SetText(df->dat[PM_VEHICLE]);
    grid->SetCol(4);
    grid->SetText(df->dat[PM_BUYER]);
    grid->SetCol(5);
    grid->SetText(df->dat[PM_TRANSP]);
    grid->SetCol(6);
    grid->SetText(df->dat[PM_TANK]);
    grid->SetCol(7);
    grid->SetText(df->dat[PM_PT]);
    grid->SetCol(8);
    grid->SetText(df->dat[PM_PGROSS]);
}

void SetWdesGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    int width,len=0;

    grid->SetCols(9);
    ReplaceDBGridNo(grid,df,IdxItem);
    len += ReplaceDBGridFieldNo(grid,df,PM_TICKET,1);
    len += ReplaceDBGridFieldNo(grid,df,PM_DATE,2);
    len += ReplaceDBGridFieldNo(grid,df,PM_VEHICLE,3);
    len += ReplaceDBGridFieldNo(grid,df,PM_BUYER,4);
    len += ReplaceDBGridFieldNo(grid,df,PM_TRANSP,5);
    len += ReplaceDBGridFieldNo(grid,df,PM_TANK,6);
    width = ReplaceDBGridFieldNo(grid,df,PM_PT,7);

    grid->SetCol(8);
    grid->SetColWidth(8,900);
    grid->SetText("Weight (kgs)");
}

extern int Despatch_initial(DB *df,int *fld);
void init_first(DB *df)
{
    df->e->rep       = ReplaceWdesGrid;
    df->e->GridTitle = SetWdesGridTitle;
    df->e->tell      = tell_net;
    df->e->prec      = prec;
    df->e->initial   = Despatch_initial;
    df->e->prec      = unprint;
}


BOOL Cwdes::OnInitDialog() 
{
	CStaff::OnInitDialog();
    return TRUE;
}
