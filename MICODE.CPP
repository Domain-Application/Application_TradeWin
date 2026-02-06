// mifare.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString CardKey = "Domain Software";
CString CardAC,CardPt,CardVehicle,MCardKey,CardTransporter;
BOOL UpdateACInfo(long xrec)
{
 /*   BOOL flag = ReadOneField(8,3,accode.dat[2]);
    if(!flag) return FALSE;
    flag = ReadOneField(12,1,accode.dat[CU_TEL]);
    if(!flag) return FALSE;
    flag = ReadOneField(13,1,accode.dat[CU_FAX]);
    if(!flag) return FALSE;
    flag = ReadOneField(16,3,accode.dat[CU_ADD1]);
    if(!flag) return FALSE;
    flag = ReadOneField(20,3,accode.dat[CU_ADD2]);
    if(!flag) return FALSE;
    flag = ReadOneField(24,3,accode.dat[CU_ADD3]);
    if(!flag) return FALSE;
    flag = ReadOneField(28,3,accode.dat[CU_EMAIL]);
    if(!flag) return FALSE;
    flag = ReadOneField(32,3,accode.dat[CU_CONTACT]);
    if(!flag) return FALSE;
    if(xrec==0) {
        accode.AddNew();
    }
    else {
        accode.Update();
    } 
	*/
    return TRUE;
}

BOOL UpdateProductInfo(long xrec)
{
   /* BOOL flag = ReadOneField(40,3,prodtype.dat[2]);
    if(!flag) return FALSE;
    flag = ReadOneField(44,3,prodtype.dat[PT_REMARKS]);
    if(!flag) return FALSE;
    flag = ReadOneField(48,1,prodtype.dat[PT_PRICE]);
    if(!flag) return FALSE;
    flag = ReadOneField(49,1,prodtype.dat[PT_UNIT]);
    if(!flag) return FALSE;
    if(xrec==0) {
        prodtype.AddNew();
    }
    else {
        prodtype.Update();
    }
	*/
    return TRUE;
}

BOOL UpdateTransporterInfo(long xrec)
{
    BOOL flag = ReadOneField(52,3,transp.dat[2]);
    if(!flag) return FALSE;
    if(xrec==0) {
        transp.AddNew();
    }
    else {
        transp.Update();
    }
    return TRUE;
}
/*
BOOL ReadCardKey()
{
    char szTmp[20];

    space_str(szTmp, 15);
    BOOL flag = ReadOneField( CardKeyBlk,1, szTmp);  //New Smart Card
    if(!flag) return FALSE;

    MCardKey = szTmp;
	return TRUE;
}

BOOL ReadCardInfo(DB *df,int mode)
{
    BOOL flag = ReadCardKey();
    if(!flag) return FALSE;
    if(strcmp(MCardKey,CardKey)) {
        return TRUE;				//IS new card no need to read card info
    }

    flag = ReadOneField( VehicleBlk,1, df->dat[Tng_VEHICLE]);
    if(!flag) return FALSE;
    CardVehicle = df->dat[Tng_VEHICLE];

    flag = ReadOneField( AccodeBlk,1, df->dat[Tng_ACCODE]);
    if(!flag) return FALSE;

    CardAC = df->dat[Tng_ACCODE];
    if(!isblank(df->dat[Tng_ACCODE])) {
        long xrec = qd(df->dat[Tng_ACCODE],&accode);
        lset(accode.dat[1],df->dat[Tng_ACCODE]);
        if(mode) {
            UpdateACInfo(xrec);
        }
    }

    flag = ReadOneField(ProductBlk,1, df->dat[Tng_PT]);
    if(!flag) return FALSE;

    CardPt = df->dat[Tng_PT];
    if(!isblank(df->dat[Tng_PT])) {
        long xrec = qd(df->dat[Tng_PT],&prodtype);
        lset(prodtype.dat[1],df->dat[Tng_PT]);
        if(mode) {
            UpdateProductInfo(xrec);
        }
    }

    flag = ReadOneField(TransporterBlk,1, df->dat[Tng_TRANSP]);
    if(!flag) return FALSE;

    CardTransporter = df->dat[Tng_TRANSP];
    if(!isblank(df->dat[Tng_TRANSP])) {
        long xrec = qd(df->dat[Tng_TRANSP],&transp);
        lset(transp.dat[1],df->dat[Tng_TRANSP]);
        if(mode) {
            UpdateTransporterInfo(xrec);
        }
    }

    flag = ReadOneField( 56, 3, df->dat[Tng_REMARKS]);
    if(!flag) return FALSE;

    flag = ReadOneField( TareWtBlk, 1, df->dat[Tng_TAREWT]);
    if(!flag) return FALSE;

    return TRUE;
}

BOOL IsSmartCardCorrect(DB *df)
{
	CString m_tuid;

    if( !OnScanCardID(m_tuid)){
        ErrorBox("Smart card not found\n\nPlease place the card on the scan machine.");
        return FALSE;
    }

    if(strcmp(m_tuid,df->dat[1])) {
		CString s;

		s.Format("Invalid smart card\n\nDiff. card no");
        ErrorBox(s);
        return FALSE;
	}
    return TRUE;
}
*/