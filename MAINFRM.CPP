// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "menu.h"
#include "MainFrm.h"
#include "Staff.h"
#include "about.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

extern void CheckUserRight(DB *user,int key);
IMPLEMENT_DYNCREATE(CMainFrame, CMyFrame)

BEGIN_MESSAGE_MAP(CMainFrame, CMyFrame)
	//{{AFX_MSG_MAP(CMainFrame)
    ON_COMMAND(ID00009, On00001)
	ON_COMMAND(ID00011, On00001)
    ON_COMMAND(ID00012, On00001)
    ON_COMMAND(ID00013, On00001)
    ON_COMMAND(ID00014, On00001)
	ON_COMMAND(ID00015, On00001)
    ON_COMMAND(ID00021, On00001)
    ON_COMMAND(ID00022, On00001)
    ON_COMMAND(ID00023, On00001)
	ON_COMMAND(ID00024, On00001)
    ON_COMMAND(ID00031, On00001)
    ON_COMMAND(ID00032, On00001)
    ON_COMMAND(ID00411, On00001)
    ON_COMMAND(ID00412, On00001)
    ON_COMMAND(ID00413, On00001)
    ON_COMMAND(ID00414, On00001)
    ON_COMMAND(ID00415, On00001)
    ON_COMMAND(ID00416, On00001)
    ON_COMMAND(ID00417, On00001)
    ON_COMMAND(ID00418, On00001)
    ON_COMMAND(ID00419, On00001)
    ON_COMMAND(ID00421, On00001)
	ON_COMMAND(ID00422, On00001)
    ON_COMMAND(ID00423, On00001)
	ON_COMMAND(ID00424, On00001)
	ON_COMMAND(ID00425, On00001)
    ON_COMMAND(ID00431, On00001)
    ON_COMMAND(ID00432, On00001)
    ON_COMMAND(ID00433, On00001)
    ON_COMMAND(ID00434, On00001)
	ON_COMMAND(ID00441, On00001)
	ON_COMMAND(ID00442, On00001)
	ON_COMMAND(ID00443, On00001)
	ON_COMMAND(ID00444, On00001)
	ON_COMMAND(ID00445, On00001)
	ON_COMMAND(ID00446, On00001)
	ON_COMMAND(ID00447, On00001)
	ON_COMMAND(ID00448, On00001)
	ON_COMMAND(ID00449, On00001)
	ON_COMMAND(ID00061, OnPassword)
    ON_COMMAND(ID00062, On00001)
    ON_COMMAND(ID00063, On00001)
    ON_COMMAND(ID00064, On00001)
    ON_COMMAND(ID00065, On00001)
    ON_COMMAND(ID00066, On00001)
    ON_COMMAND(ID00005, On00001)
	ON_COMMAND(ID00071, On00001)
	ON_COMMAND(ID00072, On00001)
    ON_COMMAND(ID00811, On00001)
    ON_COMMAND(ID00812, On00001)
    ON_COMMAND(ID00813, On00001)
    ON_COMMAND(ID00814, On00001)
    ON_COMMAND(ID00815, On00001)
    ON_COMMAND(ID00816, On00001)
    ON_COMMAND(ID00817, On00001)
    ON_COMMAND(ID00818, On00001)
    ON_COMMAND(ID00819, On00001)
    ON_COMMAND(ID00820, On00001)
    ON_COMMAND(ID00821, On00001)
    ON_COMMAND(ID00822, On00001)
    ON_COMMAND(ID00823, On00001)
    ON_COMMAND(ID00824, On00001)
    ON_COMMAND(ID00825, On00001)
    ON_COMMAND(ID00826, On00001)
    ON_COMMAND(ID00827, On00001)
    ON_COMMAND(ID00828, On00001)
    ON_COMMAND(ID00829, On00001)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	memset(ProfileHiddenMenu,0,sizeof(ProfileHiddenMenu));
//    m_System.Format("%s Ver 4.00",title);
//    m_System.Format("%s Ver 5.00",title);  Include AR,AP
//	  m_System.Format("%s Ver 5.10",title); // can change file change data
//	  m_System.Format("%s Ver 5.20",title); // Company Title add company registration no
//	  m_System.Format("%s Ver 6.3a",title); // Main + 1st weight,2nd weight, wastage,unock period
//	m_System.Format("%s Ver 7.00",title); // Add Amount Fields at Ds and Dp , HQ Accode
//	m_System.Format("%s Ver 8.00",title); // Goods Rejected, Public Weighing
	// m_System.Format("%s VER 1.00",title);
    // m_System.Format("%s VER 1.00",title); // Kiosk System Integration
	m_System.Format("%s VER 1.01",title); // scale receive change issued by from bala to login user, added container for scale despatch with no logo and logo version
	m_System.Format("%s VER 1.02",title); // 20230717 added rounding adj, final amount and actual amount at scale receiving screen 
    bCommOpen = TRUE;
}

void CMainFrame::OnOpenOtherFiles()
{
	int j=0;
	if(company_no>=2 && company_no<=9){	 //small scale only
		ProfileHiddenMenu[j++] = 11;
		ProfileHiddenMenu[j++] = 12;
		ProfileHiddenMenu[j++] = 13;
		ProfileHiddenMenu[j++] = 14;
		ProfileHiddenMenu[j++] = 15;
		ProfileHiddenMenu[j++] = 16;
		ProfileHiddenMenu[j++] = 17;
		ProfileHiddenMenu[j++] = 2;
	}
    dopen(&one,"one.scr");
    dopen(&two,"two.scr");

//	MifareInitial();
	
    memset(k, 0, sizeof(k));
    for(int i=0; i<6; i++) {
        k[2][i] = 0x0ff;
    }
    k[1][0] = 0xa0;
    k[1][1] = 0xa1;
    k[1][2] = 0xa2;
    k[1][3] = 0xa3;
    k[1][4] = 0xa4;
    k[1][5] = 0xa5;
    k[1][6] = 0;
	
    k[0][0] = 0x37;
    k[0][1] = 0x89;
    k[0][2] = 0x00;
    k[0][3] = 0x72;
    k[0][4] = 0x88;
    k[0][5] = 0x00;
    k[0][6] = 0;
	
    k[3][0] = k[3][1] = k[3][2] = k[3][3] = k[3][4] = k[3][5] = k[3][6] = 0;
}

//HIDDEN MODULE
void CMainFrame::AddProfileHiddenPoint(int &j)
{
    for(int i=0; i<9; i++){
        int node = ProfileHiddenMenu[i];
        if(node==0) break;
        HiddenPoint[j++] = node;
    }
}

void CMainFrame::OnClose()
{
	if(m_hInstMaster) FreeLibrary(m_hInstMaster);
	CMyFrame::OnClose();
}
