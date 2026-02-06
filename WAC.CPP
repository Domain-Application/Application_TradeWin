// Wac.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Wac.h"
#include "Math.h"
#include "mgrid.h"
#include "save_dir.h"
#include "EmodeAmt.h"
#include "openrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWac dialog


CWac::CWac(UINT IDD,DB *pdf,int Idx,CWnd* pParent /*=NULL*/)
    : CKohFrm(IDD,pdf,Idx,pParent)
{
	//{{AFX_DATA_INIT(CWac)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    if(profile.dat[PF_GLWAC][0]!='0') {
        m_WinPrint = FALSE;
    }
}


void CWac::DoDataExchange(CDataExchange* pDX)
{
    CKohFrm::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWac)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWac, CKohFrm)
	//{{AFX_MSG_MAP(CWac)
	ON_COMMAND(ID_GOTO_NEXT, OnGotoNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWac message handlers

void CWac::ShowInitFrame(CMSFlexGrid *grid,DB *sub)
{
    MGRID g(grid,sub,0,5);
    if(profile.dat[PF_GLTAX][0]=='Y' || profile.dat[PF_GLTAX][0]=='T'){
        g.SetCols(7);
    }
    if(profile.dat[PF_GLTAX][0]=='Y' || profile.dat[PF_GLTAX][0]=='T'){
        g.SetRatio( grid_ratio );
        g.SetTitle("No",500);
        g.SetTitle("G/L Code",1200);
        g.SetTitle("Description",4700);
        g.SetTitle("T",400,7);
        g.SetTitle("Vat",900,7);
        g.SetTitle("Debit Amount",1700,7);
        g.SetTitle("Credit Amount",1700,7);
    }
    else {
        g.SetRatio( grid_ratio );
        g.SetTitle("No",500);
        g.SetTitle("G/L Code",1800);
        g.SetTitle("Description",5000);
        g.SetTitle("Debit Amount",1900,7);
        g.SetTitle("Credit Amount",1900,7);
    }
}

void CWac::ReplaceSubGrid(long rec,long irec,CMSFlexGrid *m_grid,DB *sub,int IdxItem)
{
    MGRID g(m_grid,sub,rec);
    if(profile.dat[PF_GLTAX][0]=='Y' || profile.dat[PF_GLTAX][0]=='T'){
        g.SetText(sub,0,Wi_GLCODE,Wi_NAME,Wi_T,Wi_GST,-1);
    }
    else {
        g.SetText(sub,0,Wi_GLCODE,Wi_NAME,-1);
    }
    double sub_total;
    if(sub->dat[Wi_DC][0]=='D') {
        sub_total = sub->val(Wi_AMT) + sub->val(Wi_GST);
        dr_amt += sub_total;
        g.SetText(sub_total,sub->f[Wi_AMT].len,sub->f[Wi_AMT].dec);
        g.SetText("");
    }
    else {
        sub_total = sub->val(Wi_AMT) + sub->val(Wi_GST);
        cr_amt += sub_total;
        g.SetText("");
        g.SetText(sub_total,sub->f[Wi_AMT].len,sub->f[Wi_AMT].dec);
    }
} 

void CWac::ShowInformation()
{
    dr_amt = cr_amt = 0;
    CKohFrm::ShowInformation();

    if(dr_amt != cr_amt) {
        CString str;
        str.Format("select sum(amt) from [wacitem] where [dc]= 'D' and [refno]='%s'",df->dat[1]);
        OpenRs p1(str);
        if(p1.rows) {
            dr_amt = p1.dblVal(1);
        }
        str.Format("select sum(amt) from [wacitem] where [dc]= 'C' and [refno]='%s'",df->dat[1]);
        OpenRs p2(str);
        if(p2.rows) {
            cr_amt = p2.dblVal(1);
        }

    }
    char str[20];
    dtoa(dr_amt, str, 12, 2);
    SetDlgItemText(IDC_TOTAL1,putnum0(str));

    dtoa(cr_amt, str, 12, 2);;
    SetDlgItemText(IDC_TOTAL2,putnum0(str));
}

void CWac::AutoAddInFastMode(DB *wsub)
{
    if(wsub->t->sumrecord>0) return;

    long xrec = qd(df->dat[I_EMODE],&emode);
    if(xrec==0) return;

/*    if(SaveFlag == NEWFILE){
        if(SaveNewFileOnly()==FALSE) return;
    }*///16/03/2002
    double total = emode.val(Em_AMT);
    CEmodeAmt h1;
    if(emode.dat[Em_TYPE][0]=='1') {
        h1.m_label = "Pay Amount";
    }
    else {
        h1.m_label = "Receipt Amt";
    }
    h1.m_wac = df->dat[1];
    char tmp[20];
    dtoa(total,tmp,12,2);
    h1.m_amt = tmp;
    if(h1.DoModal()==IDCANCEL) return;

    SaveMainInformation();
    total = atof(h1.m_amt);
    lset(df->dat[I_PARTICULAR],emode.dat[Em_PARTICULAR]);
    lset(df->dat[I_REMARKS],emode.dat[Em_REMARKS]);

    long xrec1 = qd(emode.dat[Em_GLCODE1],&glcode);

    if(xrec1>0) {
        long mrec = append(wsub);
        lset(wsub->dat[Wi_GLCODE],emode.dat[Em_GLCODE1]);
        lset(wsub->dat[Wi_NAME],glcode.dat[2]);
        lset(wsub->dat[Wi_DC],"D");
        dtodb(total,wsub,Wi_AMT);
        writerec(mrec,wsub);
    }

    long xrec2 = qd(emode.dat[Em_GLCODE2],&glcode);
    if(xrec2>0) {
        long mrec = append(wsub);
        lset(wsub->dat[Wi_GLCODE],emode.dat[Em_GLCODE2]);
        lset(wsub->dat[Wi_NAME],glcode.dat[2]);
        lset(wsub->dat[Wi_DC],"C");
        dtodb(total,wsub,Wi_AMT);
        writerec(mrec,wsub);
    }
}

BOOL CWac::IsChildDataCompleted(DB *sub)
{
    long xrec = qd(sub->dat[Wi_GLCODE],&glcode);
    double price = sub->val(Wi_AMT);
    if(price>0 && xrec>0) {
        return TRUE;
    }
    return FALSE;
}

void CWac::ChildControl(UINT IDD_SUB,char *key,DB *psub,int IdxItem)
{
    int tmpFlash = FLASH;
    FLASH = 0;

    if(NewFlag == NEW_MODE) {
        CWacitem p1(IDD_SUB,psub);
        p1.ratio = child_grid_ratio;
        AutoAddInFastMode(psub);
        if(p1.DoModal()==IDOK) {
            double no=1;
            for(long rec=1; rec<= psub->t->sumrecord; rec++) {
                readrec(rec, psub);
                lset(psub->dat[1],key);
                if(!strcmp(psub->f[2].name,"SQNO")) {
                    dtodb(no++,psub,2);
                }
                fastwrt(rec,psub);
            }
        }
    }
    else {
        DB tmpDB;
        Save_Dir s1;
        char filename[256],MainFile[256];
        strcpy(MainFile,psub->name);
        ReportPathFile(filename,"",MainFile);
        copyst(psub,filename,TRUE);
        dopen(&tmpDB,filename);
        s1.restore();

	tmpDB.l->ParentFile = psub->l->ParentFile;
	psub->e->init(&tmpDB);

        dupcpy(&tmpDB, key,psub,IdxItem);
        AutoAddInFastMode(&tmpDB);

        CWacitem p1(IDD_SUB,&tmpDB);
        p1.ratio = child_grid_ratio;
        if(p1.DoModal()==IDOK) {
            BatchSave(&tmpDB,key,psub,IdxItem);
        }

	dclose(&tmpDB);
    }
    ReFreshSubFile();
    ShowInformation();

    FLASH = tmpFlash;
}

void CWac::OnDetail()
{
    SaveMainInformation();
    if(profile.dat[PF_GLTAX][0]=='Y' || profile.dat[PF_GLTAX][0]=='T'){
         ChildControl(IDD_WACITEM_TAX,df->dat[1],sub,IdxItem);
    }
    else {
         ChildControl(IDD_WACITEM,df->dat[1],sub,IdxItem);
    }
    ShowInformation();
}

int CWac::FindChildTotalFields()
{
    if(profile.dat[PF_GLTAX][0]=='Y' || profile.dat[PF_GLTAX][0]=='T'){
        return sub->sumfield;
    }
    return Wi_AMT;
}

void CWac::SetOneTimeScreen()
{
    if( dBfldLogic(&profile,PF_EMODE) == 0  || emode.t->sumrecord==0) {
         GetDlgItem(IDC_STATIC1+I_EMODE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+I_EMODE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+I_EMODE-1)->ShowWindow(SW_HIDE);
    }
    if( dBfldLogic(&profile,PF_PCCODE) == 0 || pccode.t->sumrecord==0) {
         GetDlgItem(IDC_STATIC1+I_PCCODE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+I_PCCODE-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+I_PCCODE-1)->ShowWindow(SW_HIDE);
    }
    SetDlgItemText(IDC_CURR,profile.dat[PF_CURRENCY]);
}

void CWac::OnGotoNext() 
{
    GotoDlgCtrl(GetDlgItem(CtrlId(I_CHEQUE)));
}
