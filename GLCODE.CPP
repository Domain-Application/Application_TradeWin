#include "stdafx.h"
#include "menu.h"
#include "BROWSE.h"
#include "multisort.h"

void UpdateGlgroupType()
{
    long last = read_sumindex(1,&glcode);
    for(long rec=1; rec<= last; rec++) {
        long mrec = ridx(rec, &glcode, 1);
        qd(glcode.dat[C_GLGROUP],&glgroup);
        lset(glcode.dat[C_TYPE],glgroup.dat[Gp_TYPE]);
        fastwrt(mrec, &glcode);
    }
}


BOOL CheckAndUpdateGlcode(char *glkey)
{
    long irec = qd(glkey,&glcode);
/*    if(irec==0) {
        waitmsg("Please assign 'CONTROL A/C'");
        return FALSE;
    }*/
    if(isblank(glcode.dat[C_TYPE])) {
        int IdxItem = glcode.f[1].idx & 1;
        long mrec = ridx(irec,&glcode,IdxItem);
        qd(glcode.dat[C_GLGROUP],&glgroup);
        lset(glcode.dat[C_TYPE],glgroup.dat[Gp_TYPE]);
        fastwrt(mrec,&glcode);
    }
    return TRUE;
}

int glcode_save(DB *df,int flag)
{
    long irec = dseek(df->dat[C_GLGROUP],&glgroup,0,MF);
    if(irec==0) {
        waitmsg("Please assign 'GL GROUP'");
        return 0;
    }
    lset(df->dat[C_TYPE],glgroup.dat[Gp_TYPE]);
    return 1;
}

int glcode_initial(DB *df,int *fld)                //initial setting
{
    fld[0] = CBAL;
    fld[1] = C_TYPE;
    fld[2] = 0;
	return 1;
}

long GlcodeBrowse(DB *df,int fld,DB *pf,CRect &rt,int Idx,BOOL MoveWindowFlag,int EditFlag)
{
    CBrowse b1(pf->BROWSE_ID);
    b1.rt = &rt;
    b1.df = pf;
    b1.IdxItem = Idx;
    b1.MoveWindowFlag = MoveWindowFlag;
    b1.EditFlag    = EditFlag;
    if(ProduresID == 1) {
        b1.m_LockKey   = "SALES";
        b1.m_LockField = Oiv_GLCODE;
    }
    else if(ProduresID == 2) {
        b1.m_LockKey   = "COGS";
        b1.m_LockField = Oiv_GLCODE;
    }
    else if(ProduresID == 3) {
        b1.m_LockKey   = "CA";
        b1.m_LockField = Oiv_GLCODE;
    }
    else if(ProduresID == 4) {
        b1.m_LockKey   = "CL";
        b1.m_LockField = Oiv_GLCODE;
    }
    b1.DoModal();
    pf->rec = b1.no;
    return pf->rec;
}

void init_glcode(DB *df)
{
 //   df->e->prec    = PGLCode;
    df->e->save    = glcode_save;
    df->e->initial = glcode_initial;
    df->BROWSE_ID  = IDD_BROWSE_GLCODE;
    df->e->Browse  = GlcodeBrowse;
}

int glgroup_save(DB *df,int flag)
{
    if(!(df->dat[Gp_TYPE][0]=='B' || df->dat[Gp_TYPE][0]=='I')) {
        waitmsg("Please assign 'Income or Balance sheet'");
        return 0;
    }
    return 1;
}

void init_glgroup(DB *df)
{
    df->e->save   = glgroup_save;
}
