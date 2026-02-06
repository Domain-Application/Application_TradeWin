// db.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "menu.h"
#include <stdio.h>
#include <memory.h>
#include <process.h>
#include <dos.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>

DB   *Tdb[50];
char DbCloseFlag;
int  DbOpenCount;
long _MAINREC;
void fill_field_blank(int i,DB *df)
{
    int    k,d;
    struct field *f;

    if(i==0) {
        space_str(df->dat[0], 1);
        return;
    }
    f = &df->f[i];
    k = f->len;
    space_str(df->dat[i], k);
    if(f->mode=='N'){
        d = f->dec;
        if(d>0) {
            df->dat[i][k-d-1] = '.';
        }
    }
}

void fillblank(int i,DB *df)
{
    if(i<0 || i>(int) df->sumfield) {
        waitmsg("Out of range");
        return;
    }
    fill_field_blank(i,df);
}

void lsetblank(DB *df)
{
    uint i,sumfield=df->sumfield;

    for (i=0; i<=sumfield; i++){
        fill_field_blank(i,df);
    }
}

void flush(int handle)
{
    if(FLASH) _commit(handle);
}

void FlushDB(DB *df)
{
    uint i;

    for(i=1;i<=df->sumfield;i++) {
        if(df->indexno[i]>0) {
            flush(df->indexno[i]);
        }
    }
    flush(df->handle);
}

void endmark(long rec ,DB *df)
{
    df->t->sumrecord = rec;
    write_title(df);
}

void reset(int handle)
{
    qmove( handle, 0l);
}

void DbFileOpenError()
{
    printmsg("File Open Error");
    error_exit();
}

void CheckDbStatus(DB *df)
{
    int handle = df->handle;
    int flag = 0;

    if(handle<=0 || handle>256) flag = 1;
    if(df->sumfield<1 || df->sumfield>128) flag = 1;
    if(flag>0) {
        DbFileOpenError();
    }
}

void blank_file(int handle)
{
    long tmp[12];

    memset(tmp,0,sizeof(tmp));
    reset(handle);
    qwrt(handle, (char *)tmp, sizeof(tmp));
    chsize(handle,sizeof(tmp));
}

int _create_new_handle(char *filename)
{
    int  handle;

    handle = bopen(filename);
    blank_file(handle);
    return handle;
}

int new_open(char *file)
{
    int  handle;

    if ( access(file,00 ) == -1 ) {
        handle = _create_new_handle(file);
    }
    else {
        handle = bopen(file);
    }
    return handle;
}

void _MallocField(DB *df)
{
    uint i,len;
    int  handle;
    char target[5];
    char indexfile[80];

    for (i=0; i<= df->sumfield; i++) {
        len = df->f[i].len+2;
        if(df->f[i].mode == 'D') len += 2;              //Reserve DD/MM/YYYY
        df->dat[i] = (char *) calloc(len,sizeof(char));
        df->bak[i] = (char *) calloc(len,sizeof(char));
        if(!df->bak[i] || !df->dat[i]){
            notenought(df);
        }
        df->indexno[i] = 0;
        if(i>0){
            handle = 0;
            if ( df->f[i].idx & 1) {       //Index Mode
                ltos( (long) i,3,&target[1]);
                target[0] = '.';
                sprintf(indexfile,"%s%s%s",FileDir,df->name,target);     //Directory Name 940428
                handle = new_open(indexfile);
            }
            df->indexno[i] = handle;
        }
    }
}

/******************************************************************************
*                                                                             *
* extend field                                                                *
* bit 2 control save function (see table sf.h)                                *
******************************************************************************/
int  db_save(DB *df,MI *mi,int flag)
{
    uint i;
    char tmp[40];

    for(i=2; i<=df->sumfield; i++) {
        if(df->f[i].ext & 0x04) {
            if(isblank(df->dat[i])) {
                sprintf(tmp,"Pls keyin '%s'",df->f[i].name);
                waitmsg(tmp);
                return 0;
            }
        }
    }
    return 1;
}

int checkfield(DB *df)
{
    uint i,j;
    char ch;
    struct field *f;

    for (i=1; i<= df->sumfield; i++) {
        f = &df->f[i];
        if(f->len>128) return 1;
        if(f->dec>20 || f->dec > f->len) return 1;
        if(strchr("NCLMD",f->mode)==NULL) return 1;
        for(j=0;j<10;j++){
            ch = f->name[j];
            if(ch==0) break;
            if(ch>0 && ch<32) return 1;
        }
    }
    return 0;
}

/*****************************************************************************
*      Date : 29-9-1995      *   bit0 = 1  --  *.rep                         *
*      df->e->ReportFlag     *   bit1 = 1  --  Ask printing range            *
*                            *                                               *
*****************************************************************************/
void init_db(DB *df)
{
    df->e->SumIdxItem   = 0;
    df->e->ReportFlag   = 0;
    df->e->mi           = NULL;                 //set null string

    df->e->Buffer     = NULL;
    df->e->chain      = NULL;
    df->e->htbl       = NULL;
    df->e->hi         = NULL;
}

void FileDestroyMessage(char *MainFile)
{
    char tmp[80];

    sprintf(tmp,"%s Destroy",MainFile);
    disp_error_box(tmp);
}

void rep_vd(long irec,DB *df,int field)
{
    fillblank(field,df);
    if(df->f[field].ext & 0xf0) {
        fillblank(field+1,df);
    }
}

int CheckTotalField(MI *mi,DB *df)
{
    int  sumitem;

    sumitem = (!mi->last) ? df->sumfield : mi->last;
    if(sumitem> (int) df->sumfield) {
    }
    return sumitem;
}

void trim_field(char *q,char *text)
{
    int i,len;

    len = strlen(text);
    strcpy(q,text);
    for(i=len-1;i>0;i--) {
        if(q[i]>='0' && q[i]<='9') {
            q[i] = 0;
        }
    }
}

DB *GetMatchTable(int i,DB *df)
{
    int  j;
    char *p,q[10];

    if((df->f[i].ext & 0xf0)) {
        if(DbFileHandle>0){
            for(j=0; j<DbFileHandle; j++){
                p = trim_file(strupr(Tdb[j]->name));
                trim_field(q,df->f[i].name);
                if(!strcmp(q, p)){
                    return Tdb[j];
                }
            }
        }
    }
    return NULL;
}

char FileDir[20];
void dopen(DB *df,char *DbFileName)
{
    int  flen,len,item,handle,ReportFileFlag;
    char DbtFile[64],markfile[64],filename[64],MainFile[64];
    struct title tmp;

    DbOpenCount++;
    len       = strlen(DbFileName) + 1;
    df->name  = (char *) malloc(len);
    if(!df->name) memory_alloc_err();
    strcpy(df->name,DbFileName);
    item  = instr(1,df->name,".");
    if(item>0){
        df->name[item-1] = 0;
    }

    sprintf(filename,"%s%s",FileDir,DbFileName);      //Directory Name 940428

    ReportFileFlag = instr(1,filename,".rep");
    item  = instr(1,filename,".");
    if(item==0){
        strcat(filename,".dat");
        item = (int) instr(1,filename,".");
    }
    strcpy(MainFile,filename);
    strcpy(DbtFile,filename);
    inset(DbtFile, item, ".dbt");
    strcpy(markfile,filename);
    inset(markfile, item, ".mrk");
    mid(filename,1,item-1,filename);

    df->e = (class db_ext *) malloc (sizeof(class db_ext));
    if(!df->e) memory_alloc_err();

    if ( access(MainFile,00 ) == -1 ) {
        openerror(MainFile);
    }
    handle = df->handle = bopen(MainFile);
    flen  = qread( handle,(char *) &tmp ,sizeof(struct title));
    item  = (tmp.startadd) / (sizeof(struct title));
    df->f = (struct field *) calloc (sizeof(struct field) * item,1);
    if(!df->f) memory_alloc_err();
    df->indexno = (int *)   malloc (item * sizeof(int));
    df->dat     = (char **) malloc (item * sizeof(char *));
    df->bak     = (char **) malloc (item * sizeof(char *));
    reset(handle);
    qread(handle,(char *) df->f,item * sizeof(struct field));
    item  = df->sumfield = item - 1;                      /* Sum Field */
    df->t = (struct title *) df->f;
    if(checkfield(df) || (tmp.reclen >= sizeof(RW_Buffer)) || item<1 || item>128 || flen!=0x20) {
        FileDestroyMessage(MainFile);
    }

    _MallocField(df);

    df->markno = df->e->cache    = 0;
    df->rec    = df->e->SumIndex = 0;
    if(df->f[1].idx & 0x01){
        if(!(df->t->pass & 0x01)) {                  //23/04/95
            df->markno = new_open(markfile);
        }
    }
    df->e->memo = 0;
    if(df->t->mode==0x83){                          //memo file
        df->e->memo = bopen(DbtFile);
    }
    lsetblank(df);
    df->e->status = 0;                             //Default Setting
    init_db(df);                                   //Pointer Setting
    DbCloseFlag = 1;
    if(ReportFileFlag>0) {                               //18-06-95
        ReportPathFile(MainFile,"",df->name);
        sprintf(filename,"%s%s",FileDir,MainFile);      //950705
        copyst(df,filename);
        dclose(df);
        dopen(df,MainFile);
        len = strlen(filename) + 1;                      //951124
        df->e->htbl = (char *) malloc(len);
        strcpy((char *)df->e->htbl,filename);
        df->e->ReportFlag = 1;
        df->e->vd         = rep_vd;
    }
    if(df->t->pass & 0x02){
        df->e->ReportFlag = 2;
    }
    if(df->ID>0) {
        Tdb[DbFileHandle] = df;
        DbFileHandle++;
        if(DbFileHandle>=50) {
            printmsg("Buf. full");
            error_exit();
        }
    }
}

void dclose(DB *df)
{
    uint i;
    int  handle,flag;
    char tmp[100];

    if(df->sumfield<= 0) {
        sprintf(tmp,"%s Close Err",df->name);
        waitmsg(tmp);
        return;
    }
    if(df->ID>0) {
        Tdb[--DbFileHandle] = NULL;
    }
    DbOpenCount--;
    unflock(df);                                //950706
    flag = df->e->ReportFlag & 0x01;
    if(flag == 1) {
        strcpy(tmp,(char *)df->e->htbl);
        free(df->e->htbl);
    }
    for (i=0; i<= df->sumfield; i++) {
        free(df->dat[i]);
        free(df->bak[i]);
        handle = df->indexno[i];
        if(handle>0 && i>0) close(handle);
    }
    close(df->handle);
    if(df->markno>0) close(df->markno);
    if(df->e->memo>0) close(df->e->memo);            //Cloase memo file
    free(df->indexno);
    free(df->dat);
    free(df->bak);
    free(df->name);
    free(df->f);                            //free db field
    free(df->e);                            //free db ext
    df->sumfield = df->e->ReportFlag = 0;
    if(DbCloseFlag == 1 && flag == 1) {
        unlink(tmp);
    }
}

void save_entry(int flag,int IdxItem,long mrec,DB *df)
{
	int  item = (IdxItem) ? IdxItem : 1;
    long SaveRec,irec;

    waitflock(df);
	if (flag == EXIST) {
        vary(mrec,df,IdxItem,0);
        irec = df->rec;
	}
	else {
         if(IdxItem>0){
              _MAINREC = mrec  = getdelrec(df);
              if (flag!=ADD_DATA) {
				  SaveRec = dseek(df->dat[IdxItem], df, IdxItem, BT );
                  if(SaveRec==-1L){
                      if(!(df->f[1].msi & 0x01)) {
//                        waitmsg(ExistMsg);
                          mrec = 0;
                      }
                  }
			  }
              if(mrec>0) {
                  irec = AppendIdx(mrec,df,item);
              }
		  }
		  else {
			  mrec = dseek(df->dat[item], df ,IdxItem,SL);
			  saverec(mrec,df);
              irec = mrec+1;
		  }
	}
    df->rec = irec;
    if(df->rec<1l) df->rec = 1;
    unflock(df);
}

void DB::ReplaceGrid(long rec,CMSFlexGrid *grid,DB *df,int IdxItem)
{
    uint i;
	CString s;

    grid->SetCol(0);
	if(rec>0) {
		s.Format("%4ld",rec);
	}
	else {
		s = "    ";
	}
    grid->SetText(s);
	for(i=1; i<=this->rep_field; i++) {
        grid->SetCol(i);
        if(df->f[i].mode=='D' && rec>0)
            grid->SetText(mdate(df->dat[i]));
        else
            grid->SetText(df->dat[i]);
    }
}

void DB::SetGridTitle(CMSFlexGrid *grid,int IdxItem,int fld)
{
    int width,title_width,len=0;

	if(this->rep_field>0) {
		grid->SetCols(this->rep_field+1);
	}
	CString s;
	s.Format("%ld",this->t->sumrecord);
	width = strlen(s) * 110;
	if(width<440) width = 440;
    grid->SetRows(11);

    grid->SetRow(0);
    grid->SetCol(0);
    grid->SetColWidth(0,width);
    grid->SetText("No");

	for(uint i=1; i<=this->rep_field; i++) {
		width = this->f[i].len * 110;
		if(width<500) width = 500;
		if(this->f[i].mode=='D') width += 200;
		title_width = strlen(this->f[i].name) * 120;
		if(width<title_width) width = title_width;
		grid->SetCol(i);
        if(i==this->rep_field) {
            if(len+width<6050) {
				width = 6050 - len;
            }
        }
		grid->SetColWidth(i,width);
		grid->SetText(this->f[i].name);
        len += width;
	}
}

void DB::SetGridSize(CDialog *pDlg,int fld)
{
    return;
    RECT rt;
    pDlg->GetWindowRect(&rt);
    rt.top -= 0;
    rt.bottom -= 0;
    rt.left -= 20;
    rt.right -= 20;
    pDlg->MoveWindow(&rt);
}

void DB::SetGridScroll(CScrollBar *pSB,int fld)
{
    return;
    RECT rt;
    pSB->GetWindowRect(&rt);
    rt.top -= 55;
    rt.bottom -= 55;
    rt.left -= 80;
    rt.right -= 80;
    pSB->MoveWindow(&rt);
}

int DB::OnTellMe(UINT nChar,int id,int fld)
{
    return 1;
}
