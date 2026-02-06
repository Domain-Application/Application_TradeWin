// Gldat.cpp : implementation file
//

#include "stdafx.h"
#include "Gldat.h"
#include "save_dir.h"
#include "mgrid.h"
#include "Parent.h"
#include "menu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGldat dialog


CGldat::CGldat(UINT IDD,DB *psub,CWnd* pParent /*=NULL*/)
    : CChild(IDD, psub, pParent)
{
	//{{AFX_DATA_INIT(CGldat)
	//}}AFX_DATA_INIT
}


void CGldat::DoDataExchange(CDataExchange* pDX)
{
    CChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGldat)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGldat, CChild)
	//{{AFX_MSG_MAP(CGldat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGldat message handlers

void CGldat::ShowInitFrame()
{
    if(profile.dat[PF_CN_CATEGORY][0]=='F') {
        MGRID g(&m_grid,sub,0,4);
        g.SetTitle("No",500);
        g.SetTitle("G/L Code",1500);
        g.SetTitle("Description",6200);
        g.SetTitle("Total Amount",1500,7);
    }
    else {
        MGRID g(&m_grid,sub,0,6);
        g.SetTitle("No",300);
        g.SetTitle("Your Ref No",1200);
        g.SetTitle("G/L Code",1000);
        g.SetTitle("CN Category",1000);
        g.SetTitle("Description",4600);
        g.SetTitle("Total Amount",1400,7);
    }
}

void CGldat::ReplaceChildGrid(long rec)
{
    if(profile.dat[PF_CN_CATEGORY][0]=='F') {
        MGRID g(&m_grid,sub,rec);
        g.SetText(sub,NULL,CG_GLCODE,CG_DESCRIPT,CG_TOTAL, -1);
    }
    else {
        MGRID g(&m_grid,sub,rec);
        g.SetText(sub,NULL,CG_YOUR_REF,CG_GLCODE,CG_ADJ_CAT,CG_DESCRIPT,CG_TOTAL, -1);
    }
}

void CGldat::SetOneTimeScreen()
{
    if(profile.dat[PF_CN_CATEGORY][0]=='F') {
         GetDlgItem(IDC_EDIT1+CG_ADJ_CAT-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_LABEL1+CG_ADJ_CAT-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_STATIC1+CG_ADJ_CAT-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_EDIT1+CG_YOUR_REF-1)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_STATIC1+CG_YOUR_REF-1)->ShowWindow(SW_HIDE);
    }
}

void CGldat::ShowChildTotal()
{
    char tmp[100];
    cr_amt = fm.acc[CG_TOTAL];
    CParent *p  = (CParent *) GetParent();
    p->ShowGrandTotal(fm.acc);
    dtoa(cr_amt,tmp,15,2);
    SetDlgItemText(IDC_TOTAL7,putnum(tmp));
}

void GldatChildControl(UINT IDD,DB *df,DB *psub,int IdxItem)
{
    int tmpFlash = FLASH;
    FLASH = 0;

    DB sub;
    Save_Dir s1;
    char filename[256],MainFile[256];
    strcpy(MainFile,psub->name);
    ReportPathFile(filename,"",MainFile);
    copyst(psub,filename,TRUE);
    dopen(&sub,filename);
    s1.restore();

    psub->e->init(&sub);
    long Srec,irec,rec;

    memstart(&sub);
    Srec = interval(&irec,df->dat[1],psub,IdxItem);
    for(rec=0; rec<Srec; rec++) {
        ridx(rec+irec,psub,IdxItem);
        memwrt(&sub,psub);
    }
    memend(&sub);

    CGldat p1(IDD,&sub);
    if(profile.dat[PF_CN_CATEGORY][0]=='F') {
        p1.FirstField = CG_GLCODE;
    }
    int flag = p1.DoModal();
    if(flag==IDOK) {
        waitflock(psub);
        psub->Delete(df->dat[1]);
//        Srec = interval(&irec,df->dat[1],psub,IdxItem);
//        delsame(irec,Srec,psub,IdxItem);
        double no=1;
        fetch_set(psub,&sub);
        for(rec=1; rec<= sub.t->sumrecord; rec++) {
            readrec(rec,&sub);

            long mrec = getdel(psub);
            fetch_rec(psub,&sub);
            lset(psub->dat[1],df->dat[1]);
            if(!strcmp(psub->f[2].name,"SQNO")) {
                dtodb(no++,psub,2);
            }
            AppendIdx(mrec,psub,IdxItem);
        }
        unflock(psub);
    }

    dclose(&sub);
    FLASH = tmpFlash;
}

int save_gldat(DB *df,int flag)
{
    CString str;
    long xrec = qd(df->dat[CG_GLCODE],&glcode);
    if(xrec==0) {
        str.Format("Error message\nPlease key in glcode");
        AfxMessageBox(str);
        return 0;
    }
    double price = df->val(CG_TOTAL);
    if(price==0) {
        str.Format("Warning\n%s\nAmount is zero",glcode.dat[2]);
        AfxMessageBox(str);
        return 0;
    }
    return 1;
}

int tell_gldat(UINT nChar,int &flag,int &fld,DB *df,int sq)
{
    switch(fld) {
    case CG_GLCODE:
        if(sq) {
            qq(df->dat[fld],&glcode);
            lset(df->dat[CG_DESCRIPT],glcode.dat[2]);
        }
        break;
    }
    return 2;
}

int start_gldat(DB *df)
{
    if(ItemAmt > cr_amt) {
        dtodb(ItemAmt - cr_amt, df, CG_TOTAL);
    }
    return 1;
}

void init_gldat(DB *df)
{
//    df->e->save = save_gldat;                     // part out
    df->e->tell = tell_gldat;
    df->e->start= start_gldat;
}
