// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D6EA9608_A1F2_11D0_8F37_945AC87D3839__INCLUDED_)
#define AFX_STDAFX_H__D6EA9608_A1F2_11D0_8F37_945AC87D3839__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxctl.h>
#include <afxhtml.h>
#include <afxsock.h>        // MFC socket extensions
#include <peplus.h>
#include <crpe.h>
#include <math.h>
#include <sf32.h>
#include <tan.h>

#endif // _AFX_NO_AFXCMN_SUPPORT

#include "HexEdit.h"
extern HINSTANCE m_hInstMaster;
extern int (WINAPI* lib_ver)(unsigned int *nVer);
extern int (WINAPI* des_encrypt)(unsigned char *szOut,unsigned char *szIn , unsigned int inlen,unsigned char *key,unsigned int keylen);
extern int (WINAPI* des_decrypt)(unsigned char *szOut,unsigned char *szIn , unsigned int inlen,unsigned char *key,unsigned int keylen);
extern int (WINAPI* rf_init_com)(int port,long baud);
extern int (WINAPI* rf_init_device_number)(unsigned short icdev);
extern int (WINAPI* rf_get_device_number)(unsigned short *Icdev);
extern int (WINAPI* rf_get_model)(unsigned short icdev,unsigned short *Version);
extern int (WINAPI* rf_get_snr)(unsigned short icdev,unsigned char *Snr);
extern int (WINAPI* rf_beep)(unsigned short icdev,unsigned char msec);
extern int (WINAPI* rf_init_sam)(unsigned short icdev,unsigned char bound);
extern int (WINAPI* rf_sam_rst)(unsigned short icdev, unsigned char *pData,unsigned char *pMsgLg);
extern int (WINAPI* rf_sam_cos)(unsigned short icdev, unsigned char *command,unsigned char cmdLen ,unsigned char *pData,unsigned char* Length);
extern int (WINAPI* rf_init_type)(unsigned short icdev,unsigned char type);
extern int (WINAPI* rf_antenna_sta)(unsigned short icdev, unsigned char model);
extern int (WINAPI* rf_GetErrorMessage)();
extern int (WINAPI* rf_ClosePort)();
extern int (WINAPI* rf_light)(unsigned short icdev,unsigned char color);
extern int (WINAPI* rf_at020_deselect)(unsigned short icdev);
extern int (WINAPI* rf_at020_count)(unsigned short icdev,unsigned char *data);
extern int (WINAPI* rf_at020_lock)(unsigned short icdev,unsigned char *data);
extern int (WINAPI* rf_at020_write)(unsigned short icdev, unsigned char page, unsigned char *data);
extern int (WINAPI* rf_at020_read)(unsigned short icdev, unsigned char page, unsigned char *pData,unsigned char* pMsgLen);
extern int (WINAPI* rf_at020_check)(unsigned short icdev, unsigned char *key);
extern int (WINAPI* rf_hltb)(unsigned short icdev,unsigned long PUPI);
extern int (WINAPI* rf_typeb_cos)(WORD icdev, unsigned char CID,unsigned char *command, unsigned char cmdLen,unsigned char *pData,unsigned char *pMsgLg);
extern int (WINAPI* rf_attrib)(WORD icdev, unsigned long PUPI, unsigned char CID);
extern int (WINAPI* rf_atqb)(unsigned short icdev,unsigned char model,unsigned char *pData,unsigned char *pMsgLg);
extern int (WINAPI* rf_cos_command)(unsigned short icdev,unsigned char *command,unsigned char cmdLen,unsigned char *pData,unsigned char* pMsgLg);
extern int (WINAPI* rf_typea_rst)(unsigned short icdev,unsigned char model,unsigned char *pData,unsigned char *pMsgLg);
extern int (WINAPI* rf_M1_transfer)(unsigned short icdev, unsigned char block);
extern int (WINAPI* rf_M1_restore)(unsigned short icdev, unsigned char block);
extern int (WINAPI* rf_M1_increment)(unsigned short icdev, unsigned char block,long value);
extern int (WINAPI* rf_M1_decrement)(unsigned short icdev, unsigned char block,long value);
extern int (WINAPI* rf_M1_readval)(unsigned short icdev, unsigned char block,long* pValue);
extern int (WINAPI* rf_M1_initval)(unsigned short icdev, unsigned char block, long value);
extern int (WINAPI* rf_M1_write)(unsigned short icdev, unsigned char block, unsigned char *data);
extern int (WINAPI* rf_M1_read)(unsigned short icdev, unsigned char block, unsigned char *pData,unsigned char *pLen);
extern int (WINAPI* rf_M1_authentication2)(unsigned short icdev,unsigned char model,unsigned char block,unsigned char *key);
extern int (WINAPI* rf_M1_authentication1)(WORD icdev, unsigned char mode, unsigned char secnr);
extern int (WINAPI* rf_download_key)(WORD icdev, unsigned char mode, unsigned char *key);
extern int (WINAPI* rf_halt)(unsigned short icdev);
extern int (WINAPI* rf_select)(unsigned short icdev,unsigned char *pSnr,unsigned char srcLen,unsigned char *Size);
extern int (WINAPI* rf_anticoll)(unsigned short icdev, unsigned char bcnt, unsigned char *pSnr,unsigned char* pRLength);
extern int (WINAPI* rf_request)(unsigned short icdev, unsigned char model, unsigned short *TagType);

#define LIB_SUCCESS      0
#define LIB_FAILED       1
#define LastTopupBlk  36
#define CardKeyBlk  37
#define TransporterBlk  2
#define ProductBlk  6
#define VehicleBlk  4
#define AccodeBlk  5
#define TareWtBlk  50

#define keyMifare   union WCOL22
union WCOL22 {
    unsigned int code;
    char ch[4];
};
extern unsigned char k[4][7];

#define MAX_RF_BUFFER     1024
extern BOOL OnScanCardID(CString &m_tuid);
extern void MifareInitial();
extern int OpenMifare(int port);
extern void CloseMifare();
extern BOOL CardBalance(double &amt);
extern BOOL UpdateACInfo(long xrec);
extern BOOL WriteOneField(unsigned char blk,int lno,char *szText);
extern BOOL ReadOneField(unsigned char blk,int lno,char *szText);
extern BOOL OnWrtBlkData(unsigned char secnr, unsigned char *pData);
extern BOOL OnReadBlkData(unsigned char secnr, unsigned char *pData);
extern int rf_M2_authentication2(WORD icdev,unsigned char mode,unsigned char secnr,int a=0);
extern CString ExchangeStringToHex(BYTE* pBuff,int nTextLength);
extern CString ExchangeHexToString(BYTE* pBuff,int nTextLength);
extern BOOL ReadCardInfo(DB *df,int mode=0);
extern BOOL ReadCardKey();
extern BOOL IsSmartCardCorrect(DB *df);
extern CString CardKey,CardAC,CardPt,CardVehicle,MCardKey,CardTransporter;
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D6EA9608_A1F2_11D0_8F37_945AC87D3839__INCLUDED_)
