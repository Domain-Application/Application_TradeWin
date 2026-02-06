// mifare.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// A0A1A2A3A4A5
// B0B1B2B3B4B5
// 378900728800
// A0A1A2A3A4A5FF078069B0B1B2B3B4B5
// 000000000000FF078069B0B1B2B3B4B5
// 378900728800FF078069378900728800

unsigned char k[4][7];

int rf_M2_authentication2(WORD icdev,unsigned char mode,unsigned char secnr, int a)
{
    int state = 0;

    state = rf_M1_authentication2(icdev,mode,(secnr/4)*4, k[ a ]);

    if(state){
        ErrorBox("authenticate key error");
    }
    return state;
}

BOOL OnScanCardID(CString &m_tuid)
{
	WORD icdev = 0x0000;
	unsigned char mode = 0x52;
	unsigned short TagType;
	unsigned char bcnt = 0x04;//mifare card use 0x04
	unsigned char Snr[MAX_RF_BUFFER];
	unsigned char len;
	unsigned char Size;
	
	int status = rf_request(icdev,mode,&TagType);//search all card
	if(status) {//error
        return FALSE;
	}
	
    rf_light(0,1);
    rf_beep(0, 1);
	memset(Snr, 0, sizeof(Snr));
	status = rf_anticoll(icdev,bcnt,Snr,&len);//return serial number of card
	if(status || len != 4) { //error
        return FALSE;
	}
	
	status = rf_select(icdev,Snr,len,&Size);//lock ISO14443-3 TYPE_A 
	if(status) {//error
        return FALSE;
	}

    keyMifare d1;
    d1.ch[0] = Snr[3];
    d1.ch[1] = Snr[2];
    d1.ch[2] = Snr[1];
    d1.ch[3] = Snr[0];
    m_tuid.Format("%10.10u",d1.code);

    rf_light(0,2);
	
    return TRUE;
}

void MifareInitial()
{
	TCHAR szBuf[MAX_PATH];	
	strcpy(szBuf, _T("c:\\windows\\system32\\MasterRD.dll"));

	m_hInstMaster = LoadLibrary(szBuf);	 

	if(m_hInstMaster){
        (FARPROC&)lib_ver               = GetProcAddress(m_hInstMaster,_T("lib_ver"));
		(FARPROC&)des_encrypt           = GetProcAddress(m_hInstMaster,_T("des_encrypt"));
		(FARPROC&)des_decrypt           = GetProcAddress(m_hInstMaster,_T("des_decrypt"));				
		(FARPROC&)rf_init_com           = GetProcAddress(m_hInstMaster,_T("rf_init_com"));
		(FARPROC&)rf_init_device_number = GetProcAddress(m_hInstMaster,_T("rf_init_device_number"));
		(FARPROC&)rf_get_device_number  = GetProcAddress(m_hInstMaster,_T("rf_get_device_number"));
		(FARPROC&)rf_get_model          = GetProcAddress(m_hInstMaster,_T("rf_get_model"));
		(FARPROC&)rf_get_snr            = GetProcAddress(m_hInstMaster,_T("rf_get_snr"));
		(FARPROC&)rf_beep               = GetProcAddress(m_hInstMaster,_T("rf_beep"));
		(FARPROC&)rf_init_sam           = GetProcAddress(m_hInstMaster,_T("rf_init_sam"));
		(FARPROC&)rf_sam_rst            = GetProcAddress(m_hInstMaster,_T("rf_sam_rst"));
		(FARPROC&)rf_sam_cos            = GetProcAddress(m_hInstMaster,_T("rf_sam_cos"));
		(FARPROC&)rf_init_type          = GetProcAddress(m_hInstMaster,_T("rf_init_type"));
		(FARPROC&)rf_antenna_sta        = GetProcAddress(m_hInstMaster,_T("rf_antenna_sta"));
		(FARPROC&)rf_request            = GetProcAddress(m_hInstMaster,_T("rf_request"));
		(FARPROC&)rf_anticoll           = GetProcAddress(m_hInstMaster,_T("rf_anticoll"));
		(FARPROC&)rf_select             = GetProcAddress(m_hInstMaster,_T("rf_select"));
		(FARPROC&)rf_halt               = GetProcAddress(m_hInstMaster,_T("rf_halt"));
		(FARPROC&)rf_download_key       = GetProcAddress(m_hInstMaster,_T("rf_download_key"));
		(FARPROC&)rf_M1_authentication1 = GetProcAddress(m_hInstMaster,_T("rf_M1_authentication1"));
		(FARPROC&)rf_M1_authentication2 = GetProcAddress(m_hInstMaster,_T("rf_M1_authentication2"));
		(FARPROC&)rf_M1_read            = GetProcAddress(m_hInstMaster,_T("rf_M1_read"));
		(FARPROC&)rf_M1_write           = GetProcAddress(m_hInstMaster,_T("rf_M1_write"));
		(FARPROC&)rf_M1_initval         = GetProcAddress(m_hInstMaster,_T("rf_M1_initval"));
		(FARPROC&)rf_M1_readval         = GetProcAddress(m_hInstMaster,_T("rf_M1_readval"));
		(FARPROC&)rf_M1_decrement       = GetProcAddress(m_hInstMaster,_T("rf_M1_decrement"));
		(FARPROC&)rf_M1_increment       = GetProcAddress(m_hInstMaster,_T("rf_M1_increment"));
		(FARPROC&)rf_M1_restore         = GetProcAddress(m_hInstMaster,_T("rf_M1_restore"));
		(FARPROC&)rf_M1_transfer        = GetProcAddress(m_hInstMaster,_T("rf_M1_transfer"));
		(FARPROC&)rf_typea_rst          = GetProcAddress(m_hInstMaster,_T("rf_typea_rst"));
		(FARPROC&)rf_cos_command        = GetProcAddress(m_hInstMaster,_T("rf_cos_command"));
		(FARPROC&)rf_atqb               = GetProcAddress(m_hInstMaster,_T("rf_atqb"));
		(FARPROC&)rf_attrib             = GetProcAddress(m_hInstMaster,_T("rf_attrib"));
		(FARPROC&)rf_typeb_cos          = GetProcAddress(m_hInstMaster,_T("rf_typeb_cos"));
		(FARPROC&)rf_hltb               = GetProcAddress(m_hInstMaster,_T("rf_hltb"));
		(FARPROC&)rf_at020_check        = GetProcAddress(m_hInstMaster,_T("rf_at020_check"));
		(FARPROC&)rf_at020_read         = GetProcAddress(m_hInstMaster,_T("rf_at020_read"));
		(FARPROC&)rf_at020_write        = GetProcAddress(m_hInstMaster,_T("rf_at020_write"));
		(FARPROC&)rf_at020_lock         = GetProcAddress(m_hInstMaster,_T("rf_at020_lock"));
		(FARPROC&)rf_at020_count        = GetProcAddress(m_hInstMaster,_T("rf_at020_count"));
		(FARPROC&)rf_at020_deselect     = GetProcAddress(m_hInstMaster,_T("rf_at020_deselect"));
		(FARPROC&)rf_light              = GetProcAddress(m_hInstMaster,_T("rf_light"));
		(FARPROC&)rf_ClosePort          = GetProcAddress(m_hInstMaster,_T("rf_ClosePort"));
		(FARPROC&)rf_GetErrorMessage    = GetProcAddress(m_hInstMaster,_T("rf_GetErrorMessage"));
		
		
		
		if(NULL == lib_ver                ||
			NULL == des_encrypt    ||
			NULL == des_decrypt    ||			
			NULL == rf_init_com           ||
			NULL == rf_init_device_number ||
			NULL == rf_get_device_number    ||
			NULL == rf_get_model    ||
			NULL == rf_beep    ||
			NULL == rf_init_sam    ||
			NULL == rf_sam_rst    ||
			NULL == rf_sam_cos    ||
			NULL == rf_init_type    ||
			NULL == rf_antenna_sta    ||
			NULL == rf_request    ||
			NULL == rf_anticoll    ||
			NULL == rf_select    ||
			NULL == rf_halt    ||
			NULL == rf_download_key    ||
			NULL == rf_M1_authentication1    ||
			NULL == rf_M1_authentication2    ||
			NULL == rf_M1_read    ||
			NULL == rf_M1_write    ||
			NULL == rf_M1_initval    ||
			NULL == rf_M1_readval    ||
			NULL == rf_M1_decrement    ||
			NULL == rf_M1_increment    ||
			NULL == rf_M1_restore    ||
			NULL == rf_M1_transfer    ||
			NULL == rf_typea_rst    ||
			NULL == rf_cos_command    ||
			NULL == rf_atqb    ||
			NULL == rf_attrib    ||
			NULL == rf_typeb_cos    ||
			NULL == rf_hltb    ||
			NULL == rf_at020_check    ||
			NULL == rf_at020_read    ||
			NULL == rf_at020_write    ||
			NULL == rf_at020_lock    ||
			NULL == rf_at020_count    ||
			NULL == rf_at020_deselect    ||
			NULL == rf_light           ||
			NULL == rf_ClosePort ||
			NULL == rf_GetErrorMessage){			
			ErrorBox(_T("Load MasterRD.dll failed !"));
		}
	}
	else{
		ErrorBox(_T("Load MasterRD.dll failed !"));
        return;
	}
}

int OpenMifare(int port)
{
    int state = rf_init_com(port,19200);
    if(state==0) {
         rf_light(0,1);
         rf_beep(0, 50);
		 Sleep(500);
         rf_light(0,2);
    }
	if(state != LIB_SUCCESS){
		rf_ClosePort();			
		ErrorBox("Open port error!");
		return FALSE;
	}
    return TRUE;
}

void CloseMifare()
{
    rf_beep(0, 50);
	rf_light(0,0);
    rf_ClosePort();
}

BOOL CardBalance(double &amt)
{
	WORD icdev = 0x0000;
	unsigned char mode = 0x60;
	
    long pValue = 0;
	
	unsigned char secnr = 0x00;
	
    int state = rf_M2_authentication2(icdev,mode, secnr);
	if(state){
		ErrorBox(_T("authenticate key error"));
		return 0;	
	}
	
    state = rf_M1_readval( 0, 1,&pValue);
	if(state){
        ErrorBox(_T("Read balance error"));
		return 0;	
	}

    amt = double(pValue)/ 100;
    return TRUE;
}

BOOL WriteOneField(unsigned char blk,int lno, char *szText)
{
	WORD icdev = 0x0000;
	unsigned char mode = 0x60;
	unsigned char pData[MAX_RF_BUFFER];
	CString str;

    for(int i=0; i<lno; i++) {
        unsigned char secnr = blk + i;
        if(i==0) {
            int state = rf_M2_authentication2(icdev,mode,secnr);
            if(state){
                return FALSE;
            }
        }
        memcpy(pData, &szText[i*16], 16);
        int state = rf_M1_write(icdev,secnr,pData);
        if(state){
            str.Format("write card error\n\n%s",szText);
            ErrorBox(str);
            return FALSE;
        }
    }
    return TRUE;
}

BOOL ReadOneField(unsigned char blk,int lno, char *szText)
{
	WORD icdev = 0x0000;
	unsigned char mode = 0x60;
	unsigned char pData[MAX_RF_BUFFER];
	unsigned char cLen;
	CString str;

    for(int i=0; i<lno; i++) {
        unsigned char secnr = blk + i;
        if(i==0) {
            int state = rf_M2_authentication2(icdev, mode, secnr);
            if(state) return FALSE;
        }

        int state = rf_M1_read(icdev,secnr,pData,&cLen);
        if(state || cLen != 16){
            ErrorBox(_T("Read card error"));
            return FALSE;
        }
        lset(&szText[i*16], (char *) pData);
    }
    return TRUE;
}

BOOL OnWrtBlkData(unsigned char secnr, unsigned char *pData)
{
	WORD icdev = 0x0000;
	unsigned char mode = 0x60;
	CString str;
	
    int state = rf_M2_authentication2(icdev,mode,secnr);
	if(state){
		ErrorBox(_T("authenticate key error"));
        return FALSE;
	}
    state = rf_M1_write(icdev,secnr,pData);
	if(state){
        str.Format("write card error\n\nDate and time");
        ErrorBox(str);
        return FALSE;
    }
    return TRUE;
}

BOOL OnReadBlkData(unsigned char secnr, unsigned char *pData)
{
	WORD icdev = 0x0000;
	unsigned char mode = 0x60;
	unsigned char cLen;
	
    int state = rf_M2_authentication2(icdev,mode,secnr);
	if(state){
		ErrorBox(_T("authenticate key error"));
        return FALSE;
	}
    state = rf_M1_read(icdev,secnr,pData,&cLen);
    if(state || cLen != 16){
        ErrorBox(_T("Read card error"));
        return FALSE;
    }
    return TRUE;
}
