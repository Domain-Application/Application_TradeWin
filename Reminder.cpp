// Reminder.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "wac.h"
#include "Reminder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReminder dialog


CReminder::CReminder(CWnd* pParent /*=NULL*/)
	: CDialog(CReminder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReminder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReminder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReminder)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReminder, CDialog)
	//{{AFX_MSG_MAP(CReminder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReminder message handlers

BOOL CReminder::OnInitDialog() 
{
	CDialog::OnInitDialog();

    for(uint i=1; i<= df->sumfield; i++) {
        if(df->f[i].mode=='D')
            SetDlgItemText(CtrlId(i),mdate(df->dat[i]));
        else if(df->f[i].mode=='N')
            SetDlgItemText(CtrlId(i),putnum0(df->dat[i]));
        else
            SetDlgItemText(CtrlId(i),df->dat[i]);
    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

enum reminder {Rem_NEXTNO=1,Rem_DATE,Rem_CODE,Rem_PARTICULAR,Rem_REMARKS,
    Rem_TYPE,Rem_GLCODE1,Rem_GLCODE2,Rem_AMT};
void CReminder::SaveFastMode()
{
    waitflock(&wac);
    waitflock(&wacitem);

    long mrec = wac_mrec = appnet(&wac);
    lset(wac.dat[1],df->dat[1]);
    lset(wac.dat[I_DATE],df->dat[Rem_DATE]);
    lset(wac.dat[I_EMODE],df->dat[Rem_CODE]);
    lset(wac.dat[I_PARTICULAR],df->dat[Rem_PARTICULAR]);
    lset(wac.dat[I_REMARKS],df->dat[Rem_REMARKS]);
    AppendIdx(mrec,&wac,1);

	double total = df->val(Rem_AMT);
    mrec = append(&wacitem);
    lset(wacitem.dat[1],df->dat[1]);
    dtodb(1.00,&wacitem,2);
    lset(wacitem.dat[Wi_GLCODE],emode.dat[Em_GLCODE1]);
    qd(wacitem.dat[Wi_GLCODE],&glcode);
    lset(wacitem.dat[Wi_NAME],glcode.dat[2]);
    lset(wacitem.dat[Wi_DC],"D");
    dtodb(total,&wacitem,Wi_AMT);
    AppendIdx(mrec,&wacitem);

    mrec = append(&wacitem);
    lset(wacitem.dat[1],df->dat[1]);
    dtodb(2.00,&wacitem,2);
    lset(wacitem.dat[Wi_GLCODE],emode.dat[Em_GLCODE2]);
    qd(wacitem.dat[Wi_GLCODE],&glcode);
    lset(wacitem.dat[Wi_NAME],glcode.dat[2]);
    lset(wacitem.dat[Wi_DC],"C");
    dtodb(total,&wacitem,Wi_AMT);
    AppendIdx(mrec,&wacitem);

    unflock(&wac);
    unflock(&wacitem);
}
 
void CReminder::UpdateFastMode()
{
    long xrec = qd(df->dat[Rem_CODE],&emode);
    if(xrec>0) {
        char date[20],ch=emode.dat[Em_PERIOD][0];
        strcpy(date,df->dat[Rem_DATE]);
        if(ch=='1') {
            plusday(date,1);
        }
        else if(ch=='2') {
            plusday(date,7);
        }
        else if(ch=='3') {
            struct tm pst;
            sever_date(&pst,date);
            pst.tm_mon++;
            if(pst.tm_mon>12) {
                pst.tm_mon=1;
                pst.tm_year++;
            }
            CString str;
            str.Format("%4.4i%2.2i%2.2i",pst.tm_year,pst.tm_mon,pst.tm_mday);
            lset(date,str);
        }
        else if(ch=='4') {
            struct tm pst;
            sever_date(&pst,date);
            pst.tm_mon+=6;
            if(pst.tm_mon>12) {
                pst.tm_mon=1;
                pst.tm_year++;
            }
            CString str;
            str.Format("%4.4i%2.2i%2.2i",pst.tm_year,pst.tm_mon,pst.tm_mday);
            lset(date,str);
        }
        else if(ch=='5') {
            struct tm pst;
            sever_date(&pst,date);
            pst.tm_year++;
            CString str;
            str.Format("%4.4i%2.2i%2.2i",pst.tm_year,pst.tm_mon,pst.tm_mday);
            lset(date,str);
        }
        lset(emode.dat[Em_DATE],date);
        incno(emode.dat[Em_NEXTNO]);
        fastwrt(xrec,&emode);
    }
}

int CReminder::WacModule(DB *df,int id)
{
    if(df->ID==0) {
        AfxMessageBox("Database not open");
        return 0;
    }
    CWac p1(id,df,df->f[1].idx & 0x01);
    p1.sub     = &wacitem;
    p1.IDD_SUB = IDD_WACITEM;
    p1.OneRecordOnly = TRUE;
    p1.grid_ratio = float(1.00);
    p1.child_grid_ratio = float(0.67);
    return p1.DoModal();
}

void CReminder::UpdateWac()
{
    long irec = dseek(wac.dat[1],&wac,1,MF);
    long mrec = ridx(irec,&wac,1);
    wac.mrec  = mrec;
    wac.irec  = irec;
    wac.rec   = irec;
    int flag  = WacModule(&wac,IDD_WAC);
    if ( flag == IDSAVE ){
        fastwrt(mrec,&wac);
    }
}

void CReminder::OnOK() 
{
    CString str;
    GetDlgItemText( CtrlId(1), str);
    lset(df->dat[1],str);
    long xrec = dseek(df->dat[1],&wac,1,MF);
    if(xrec>0) {
        str.Format("Voucher No:%s already existed in records :%ld",df->dat[1],xrec);
        MessageBox(str,"Error Message",MB_ICONSTOP);
        return;
    }
    SaveFastMode();
    UpdateFastMode();
    UpdateWac();
	
	CDialog::OnOK();
}
