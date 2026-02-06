// Bankr.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "EnqBank.h"
#include "mgrid.h"
#include "db_ado.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

DB   blist(IDD_DEBTOR,7);
CString QuarryFieldName(DB *df,int fld)
{
	char tmp[1024];
    strcpy(tmp,DbFieldName(df,fld));
    if(fld==Bl_DATE) {
        strcpy(tmp,"Date");
    }
    else if(fld==Bl_REFNO) {
        strcpy(tmp,"Ref. No");
    }
    else if(fld==Bl_CHEQUE) {
        strcpy(tmp,"Cheque No");
    }
    else if(fld==Bl_PARTICULAR) {
        strcpy(tmp,"Particular");
    }
    else if(fld==Bl_DR) {
        strcpy(tmp,"Debit");
    }
    else if(fld==Bl_CR) {
        strcpy(tmp,"Credit");
    }
    else if(fld==Bl_BANKIN) {
        strcpy(tmp,"Cheque Clear");
    }
//    return DateStringFormat;
	return CString(tmp);
}

void ReplaceBlistGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,rec);
    g.SetText(df,Bl_CHEQUE,Bl_REFNO,Bl_DATE,Bl_PARTICULAR,Bl_DR,Bl_CR,Bl_BANKIN,-1);
}

void SetBlistGridTitle(CMSFlexGrid *grid,DB *df,int IdxItem)
{
    MGRID g(grid,df,0,7);
    g.SetTitle(df,Bl_CHEQUE,Bl_REFNO,Bl_DATE,Bl_PARTICULAR,Bl_DR,Bl_CR,Bl_BANKIN,-1);
}

void init_blist(DB *df)
{
    df->e->LongFieldName = QuarryFieldName;
    df->e->rep           = ReplaceBlistGrid;
    df->e->GridTitle     = SetBlistGridTitle;
    df->BROWSE_ID        = IDD_BLIST;
}

void blist_save(DB* rep, DB *df,int mode)
{
     fetch_rec(rep,df);
     double total = df->val(GL_AMOUNT);
     if(total>=0){
         dtodb(total,rep,Bl_DR);
     }
     else {
         total *= -1;
         dtodb(total,rep,Bl_CR);
    }
    memwrt(rep);
}

void get_blist_ob(DB* rep, DB *df)
{
    long xrec = qd(df->dat[1],&bcwac);
    if(xrec==0) return;

    lset(rep->dat[Bl_DATE],bcwac.dat[I_DATE]);
    lset(rep->dat[Bl_REFNO],bcwac.dat[1]);
    lset(rep->dat[Bl_CHEQUE],bcwac.dat[I_CHEQUE]);
    lset(rep->dat[Bl_PARTICULAR],bcwac.dat[I_PARTICULAR]);
    lset(rep->dat[Bl_PATH],"GL");
    double total = df->val(Wi_AMT);
    if(df->dat[Wi_DC][0]=='D'){
        dtodb(total,rep,Bl_DR);
        dtodb(0,rep,Bl_CR);
    }
    else {
        dtodb(0,rep,Bl_DR);
        dtodb(total,rep,Bl_CR);
    }
    lset(rep->dat[Bl_OB],"Y");
    memwrt(rep);
}

void blist_ob(DB* rep, DB *df,int mode)
{
    get_blist_ob(&blist,df);
}

void CEnqBank::put_blist(DB *df,int mode)
{
    CString strSql;
    if(mode==0) {
        strSql.Format("select * from [pgcode_collect] where [date] <= '%s' and [glcode]= %i and [bankin] is null ",
            DateStr(two.dat[2]), atoi(glcode.dat[C_PRIMARY])) ;
    }
    else {
        strSql.Format("select * from [pgcode_collect] where [date] <= '%s' and [glcode]= %i and [bankin] is not null ",
            DateStr(two.dat[2]), atoi(glcode.dat[C_PRIMARY])) ;
    }

    DB adoTmp;
    ado(&adoTmp,"",NULL,strSql);

    long rec  = 1;
    long last = read_sumindex(1,&adoTmp);
    fetch_set(&blist,&adoTmp);
    memstart(&blist);
    m_progress.SetRange32(rec,last);
    for (;rec<= last && rec>0; rec++) {
		adoFastRead(rec, &adoTmp);
        m_progress.SetPos(rec);
        blist_save(&blist,&adoTmp,mode);
    }
    dclose(&adoTmp);

    rec = 1;
    last = read_sumindex(1,&bcitem);
    m_progress.SetRange32(rec,last);
    fetch_set(&blist,&bcitem);
    for (;rec<= last && rec>0; rec++) {
        ridx(rec,&bcitem,1);
        m_progress.SetPos(rec);
        if(!strcmp(bcitem.dat[Wi_GLCODE],glcode.dat[1])){
            blist_ob(&blist,&bcitem,mode);
        }
    }
    memend(&blist);
}

void CEnqBank::OnUpdate() 
{
    OnClickMsflexgrid1();
    long xrec = qd(glcode.dat[1],&glcode);
    if(xrec>0) {
        put_blist(&pgcode,0);
        ViewEditBankReconciliation();
    }
    else {
        MessageBox("bank code not found");
    }
}

void CEnqBank::OnEditDate()
{
    OnClickMsflexgrid1();
    long xrec = qd(glcode.dat[1],&glcode);
    if(xrec>0) {
        put_blist(&pgcode,1);
        ViewEditBankReconciliation();
    }
    else {
        MessageBox("Bank code not found");
    }
}
