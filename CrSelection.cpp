// CrSelection.cpp : implementation file
//

#include "stdafx.h"
#include "myapp.h"
#include "db_ado.h"
#include <crpe.h>
#include "CrSelection.h"
#include "CrFieldSelection.h"
#include "CrDateSelection.h"
#include "secfmt.h"
#include "AREAfmt.h"
#include "CrSortFields.h"
#include "UXFWordw.h"
#include "UXFxls.h"
#include "UXDDisk.h"
#include "crdates.h"
#include "RptSelectionTitle.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void DB_AllSleep();
extern void SetRPR_Title(DB *scr,CRPEJob *hJob);
extern void DateStrFromDateTime(char *str,CString &m_date);
extern CString RptOrderBy,RptSelection;
/////////////////////////////////////////////////////////////////////////////
// CCrSelection dialog


CCrSelection *m_myCallDoc;
CCrSelection::CCrSelection(UINT IDD  ,CWnd* pParent /*=NULL*/)
    : CCryDlg(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCrSelection)
	m_ReportTitle = _T("");
	m_data = _T("");
	m_description = _T("");
	//}}AFX_DATA_INIT
	UINT flag = GetCurrentDirectory(1000,lpBuffer);
    m_pOldDlg = pDlg;
    scr = NULL;
    m_export = m_ExportToWinWord = m_AutoMailExport = m_FMT = FALSE;
    m_ExportToWinExcel = FALSE;
    m_PreviewEnable = m_UseDateFilter = TRUE;
    m_myCallDoc = this;
    dopen(&RepFilter,"Filter.rep");
    InternalFilter = FALSE;
    m_showrpttitle = FALSE;
    m_UseInternalSortFields = TRUE;
    m_hasPrintButton = TRUE;
    strWinWordFileName = "";
    m_RunTimeChangePrinter = FALSE;
    m_UserDescription = FALSE;
    m_DirectFormula = FALSE;
    m_hasSetReportSelectionDescription = FALSE;
    if (!m_ok.LoadBitmaps("OKU", "OKD", "OKF"))
	{
        FailCreateBitmapButton(IDS_FAILED_TO_LOAD_BITMAPS_FOR_BUTTONS);
	}
    if (!m_cancel.LoadBitmaps("CANCELU", "CANCELD", "CANCELF"))
	{
        FailCreateBitmapButton(IDS_FAILED_TO_LOAD_BITMAPS_FOR_BUTTONS);
	}
}

CCrSelection::~CCrSelection()
{
    BOOL bFlag = SetCurrentDirectory(lpBuffer);
    pDlg = m_pOldDlg;
    dclose(&RepFilter);
    CheckMemoryError("Crselection");
}

void CCrSelection::DoDataExchange(CDataExchange* pDX)
{
    CCryDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrSelection)
	DDX_Control(pDX, IDCANCEL, m_cancel);
    DDX_Control(pDX, ID_PRINT, m_ok);
	DDX_Control(pDX, IDC_LIST1, m_ListView);
	DDX_Text(pDX, IDC_EDIT1, m_data);
	DDX_Text(pDX, IDC_EDIT2, m_description);
    DDX_Check(pDX, IDC_CHECK1, m_showrpttitle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCrSelection, CCryDlg)
	//{{AFX_MSG_MAP(CCrSelection)
//	ON_BN_CLICKED(IDFILTER, OnFilter)
	ON_BN_CLICKED(ID_PRINT, OnPrint)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
    ON_BN_CLICKED(IDC_BUTTON1, OnUpdate)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
    ON_BN_CLICKED(IDEDIT, OnEdit)
    ON_BN_CLICKED(IDC_FORMAT, OnFormat)
    ON_BN_CLICKED(IDC_AREA_FORMAT, OnAreaFormat)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_BN_CLICKED(IDC_BUTTON2, OnUpdateDescription)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CCrSelection)
    DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(ID_PRINT, DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_CHECK1, DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_FORMAT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_EDIT1, DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_EDIT2, DLSZ_SIZE_X)
    DLGRESIZE_CONTROL(IDC_BUTTON1, DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_BUTTON2, DLSZ_MOVE_X)
    DLGRESIZE_CONTROL(IDC_AREA_FORMAT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
    DLGRESIZE_CONTROL(IDC_LIST1, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    DLGRESIZE_CONTROL(IDC_PROGRESS1, DLSZ_SIZE_X | DLSZ_MOVE_Y)
END_DLGRESIZE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrSelection message handlers
void CCrSelection::HideAlterFieldDescription()
{
    GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);           //18.3.2005
    GetDlgItem(IDC_LABEL2)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_EDIT2)->ShowWindow(SW_HIDE);
}

void CCrSelection::ShowReportOption(DB *df)
{
    m_ListView.DeleteAllItems();
    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.fmt = LVCFMT_LEFT;

    lvc.cx = 40;
    lvc.pszText = "No";
    lvc.iSubItem = 1;
    m_ListView.InsertColumn(0, &lvc);

    lvc.cx = 200;
    lvc.pszText = "Description";
    lvc.iSubItem = 2;
    m_ListView.InsertColumn(1, &lvc);

    lvc.cx = 40;
    lvc.pszText = "Cmd";
    lvc.iSubItem = 3;
    m_ListView.InsertColumn(2, &lvc);

    lvc.cx = 580;
    lvc.pszText = "Value";
    lvc.iSubItem = 4;
    m_ListView.InsertColumn(3, &lvc);

    m_ListView.SetExtendedStyle(LVS_EX_TRACKSELECT |
        LVS_EX_ONECLICKACTIVATE | LVS_EX_FULLROWSELECT);
    m_ListView.SetHoverTime(1500);
    HCURSOR m_hStdHotCursor = m_ListView.GetHotCursor();
	m_ListView.SetHotCursor(m_hStdHotCursor);
    Refresh(df);
}

int CCrSelection::GetFieldCommand(int i,int len,char *Op,char *Data)
{
    int  j = 0;
    BOOL flag = FALSE;
    for(; i<len; i++) {
        char ch = option->dat[1][i];
        if(ch !=' ') {
            flag = TRUE;
		}
        if(flag) {
            if(ch==' ') {
                char tmp[4024];
                strcpy(tmp,option->dat[1]);
                strcpy(Data,&tmp[i]);
                break;
            }
            Op[j++] = ch;
        }
	}
    Op[j] = 0;
    return i;
}

int CCrSelection::GetFieldName(int i,int len,char *FieldName,char *Op,char *Data)
{
    int j=0;
    FieldName[0] = Op[0] = Data[0] = 0;
    BOOL flag = FALSE;
    for(; i<len; i++) {
        char ch = option->dat[1][i];
		if(ch=='{') {
            flag = TRUE;
            continue;
		}
        if(flag) {
            if(ch=='}') {
                i = GetFieldCommand(i+1,len,Op,Data);
                break;
            }
            FieldName[j++] = ch;
        }
	}
    FieldName[j] = 0;
    return i;
}

void CCrSelection::GetCrystalReportSelection()
{
    zap(&RepFilter);

    if (!app->printJob->GetSelectionFormula(selecttext)) {
        ErrorBox(app->printJob->GetErrorText());
	}
    int len = selecttext.GetLength();
    if(len==0) return;
    int i=0;
    char cmsg[500];
    char text[5000];
    strcpy(text,selecttext);

    int jlen = 0;
    int j = 0;
    while(1) {
        j += i;
        i = instr(j+1,text, "and");
        if(i<=0){
            strcpy(cmsg,&text[j]);
            RepFilter.lsetblank();
            lset(RepFilter.dat[1],cmsg);
            RepFilter.AddNew();
            break;
        }
        i--;
        jlen += i;
		if(text[j]==10){ j++; i--;}						//3-3-2002
        strncpy(cmsg,&text[j],i);
        cmsg[i] = 0;
        RepFilter.lsetblank();
        lset(RepFilter.dat[1],cmsg);
        RepFilter.AddNew();
        if(jlen > len) break;
		i+=4;
    }
}

BOOL CCrSelection::OnInitDialog() 
{
    CCryDlg::OnInitDialog();
	InitResizing(FALSE, FALSE, WS_CLIPCHILDREN);
	UpdateDialogControls(this, FALSE);

    if(m_ExportToWinExcel || m_ExportToWinWord) {
        m_export=TRUE;
    }
    OnPrintAndSetDescription();
    if( m_hasSetReportSelectionDescription == FALSE) {
        GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
    }
	
    if(UserLevel()<9) {
        GetDlgItem(IDC_FORMAT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_AREA_FORMAT)->ShowWindow(SW_HIDE);
    }
    if(UserLevel()< app->level_AlterFieldDescription) {
        HideAlterFieldDescription();
    }
    CString strTitle;
    strTitle.Format("Report Filter Console -- %s -- %s",
        m_CrystalReportName,SubTitle);
    SetWindowText(strTitle);
    HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_FILTER);
	this->SetIcon(m_hIcon, TRUE);			// Set big icon

    GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
    if(m_DirectFormula) {
        if (!app->printJob->SetSelectionFormula(selecttext)) {
            ErrorBox(app->printJob->GetErrorText());
            OnCancel();
            return TRUE;
        }
        this->ShowWindow(SW_HIDE);
        PostMessage(WM_COMMAND,ID_PRINT);
        return TRUE;
    }
    GetCrystalReportSelection();
    option = &RepFilter;
    option->rec = 0;
    if(OnDateFilter()==FALSE) return TRUE;
    ShowReportOption(&RepFilter);
    if(m_Direct==TRUE || option->t->sumrecord==0) {
        this->ShowWindow(SW_HIDE);
        PostMessage(WM_COMMAND,ID_PRINT);
    }
    if(m_UserDescription == FALSE) {
        HideAlterFieldDescription();
    }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCrSelection::ErrorMsg()
{
     msg += "\nCrystal Report\n\n";
     msg += app->printJob->GetErrorText();
     ErrorBox(msg);
     OnCancel();
}

void CCrSelection::OnStdplan()
{
    CString strQuery;
    BOOL pFlag = app->printJob->GetSQLQuery(strQuery);

#ifdef _DEBUG
	FILE *ptr = fopen("crselect.txt","a");
	fprintf(ptr,"%s",strQuery);
	fclose(ptr);
#endif //_DEBUG

    char szFile[1024];
    strcpy(szFile,m_CrystalReportName);
    int i = instr(1, szFile, ".");
    if(i) szFile[i-1] = 0;

	DB adoTmp;
    BOOL flag = ado(&adoTmp, szFile, NULL, strQuery);
	if(!flag) return;

    short nSortFlds = app->printJob->GetNGroups();

    short condition;
    CRPEGroupOptions groupOptions;

    CString fieldName="";
    for(i=0; i<nSortFlds; i++) {
        BOOL bSgFlag = app->printJob->GetGroupOptions(i,&groupOptions);
        fieldName = groupOptions.m_fieldName;
        break;
    }


    char szTmp[1024];
    strcpy(szTmp, fieldName);
    int len = instr(1,fieldName,".");
    strcpy(FieldName, &szTmp[len]);
    len = strlen(FieldName);
    FieldName[len-1] = 0;

    space_str(szTmp, 24);
    for(int j=1; j<= adoTmp.sumfield; j++) {
        if(adoTmp.fieldName[j].CompareNoCase(FieldName)==0) {
            _PlanField = j;
            rset(szTmp, FieldName);
            break;
        }
    }
    _PL[3] = szTmp;

    stdplan(&two, &adoTmp);

	dclose(&adoTmp);
}

void CCrSelection::OnPrint()
{
    UpdateData();

    OnPrintAndSetDescription();

    CRPEJobInfo jobStatus;
    short status = app->printJob->GetJobStatus(&jobStatus);
    if(status == PEP_JOBHALTED) {
        MessageBox("too many records or too much time");
    }
    else if(status != PEP_JOBNOTSTARTED) {
        msg.Format("invalid %s report",m_CrystalReportName);
        ErrorMsg();
        return;
    }

    if(m_DirectFormula==FALSE) {
        if( OnFilter()==FALSE ) return;
    }

    if(m_hasSetReportSelectionDescription && m_showrpttitle) {
        CRptSelectionTitle h1;
        h1.DoModal();
    }
	
    GetDlgItem(ID_PRINT)->ShowWindow(SW_HIDE);
    GetNGroupSortFields();
    GetNSortFields();
    OnStart();
}

extern DB logo;
void CCrSelection::ResetFiles(CRPEJob *hJob)
{
    short nTable = hJob->GetNTables();
    CRPETableType tableType;
    short tableN = 0;
    BOOL bFlag = hJob->GetNthTableType(tableN,&tableType);
    CRPETableLocation tableLocation;

    DB_AllSleep();
    for(tableN = 0; tableN<nTable; tableN++) {
        bFlag = hJob->GetNthTableLocation(tableN,&tableLocation);
        CString repname,newname;
        repname.Format("%s",tableLocation.m_location);
        repname.MakeUpper();
        char szRptName[1024],szNewName[1024];
        strcpy(szRptName,repname);
        int nCount = instr(1,szRptName,"DBO.PROC");                 //3.7.2003
        if(nCount) continue;
        nCount = instr(1,szRptName,"DBO.");                 //20.3.2002
        if(nCount) {
            strcpy(szNewName, &szRptName[nCount+3]);
            strcpy(tableLocation.m_location,szNewName);
            bFlag = hJob->SetNthTableLocation(tableN,&tableLocation);
			if(bFlag == FALSE) {
                msg.Format("File: %s Set Nth Table Location error\n",logo.name);
                ErrorMsg();
                return;
			}
            repname = szNewName;
        }

        if(instr(1,repname,logo.name)) {
            strcpy(tableLocation.m_location,logo.actualFilename);
            bFlag = hJob->SetNthTableLocation(tableN,&tableLocation);
			if(bFlag == FALSE) {
                msg.Format("File: %s Set Nth Table Location error\n",logo.name);
                ErrorMsg();
                return;
			}
            bFlag = hJob->VerifyDatabase();
			if(bFlag == FALSE) {
                msg.Format("File: %s VerifyDatabase error\n",logo.name);
                ErrorMsg();
                return;
			}
        }
        else if(app->RptResetFiles(m_CrystalReportName,repname,newname)) {
            strcpy(tableLocation.m_location, newname);
            bFlag = hJob->SetNthTableLocation(tableN,&tableLocation);
			if(bFlag == FALSE) {
                msg.Format("File: %s Set Nth Table Location error\n",newname);
                ErrorMsg();
                return;
			}
            bFlag = hJob->VerifyDatabase();
			if(bFlag == FALSE) {
                msg.Format("File: %s VerifyDatabase error\n",newname);
                ErrorMsg();
                return;
			}
        }
    }

    CRPESessionInfo sessionInfo;
    bFlag = hJob->GetNthTableSessionInfo(tableN,&sessionInfo);

    CRPETablePrivateInfo privateInfo;
    hJob->GetNthTablePrivateInfo (tableN, &privateInfo);
}

BOOL CCrSelection::SetSelectionFormula(CString m_Selection_Formula)
{
     if (!app->printJob->SetSelectionFormula(m_Selection_Formula)) {
         ErrorBox(app->printJob->GetErrorText());
         OnCancel();
         return FALSE;
     }
    BOOL flag2 = app->printJob->CheckSelectionFormula();
    if (flag2==FALSE) {
        CString errText;
        errText.Format("Formula: %s\n\n%s",m_Selection_Formula,app->printJob->GetErrorText());
        ErrorBox(errText);
        OnCancel();
        return FALSE;
    }
    return TRUE;
}

void CCrSelection::DelayLoop()
{
}

void CCrSelection::OnUpdate() 
{
    UpdateData();
    if (option->rec) {
        lset(option->dat[1],m_data);
        fastwrt(option->rec,option);
        ShowReportOption(option);
        OnFilter();
    }
}

void CCrSelection::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    PostMessage(WM_COMMAND,IDEDIT);
	*pResult = 0;
}

void CCrSelection::OnFormat()
{
    if(m_PreviewEnable==FALSE) return;
	CSectionFormat mytest(app->printJob);
	mytest.DoModal();
}

void CCrSelection::OnAreaFormat()
{
    if(m_PreviewEnable==FALSE) return;
	CAreaFormat mytest(app->printJob);
	mytest.DoModal();
}

BOOL CCrSelection::OnDateFilter()
{
    if(m_UseDateFilter==FALSE) return TRUE;

    CString m_Selection_Formula="";
    BOOL bFlag = FALSE;
    BOOL bStartFlag = TRUE;

    for (long rec=1 ; rec <= option->t->sumrecord; rec++) {
        readrec(rec,option);
        if(bStartFlag) {
            int len = strlen(option->dat[1]);
            GetFieldName(0,len,FieldName,Op,Data);

            int dotExisted = instr(1,FieldName,".");
            if(dotExisted) {
                int dateTimeCommand = instr(1,Data,"DateTime");
                if(dateTimeCommand) {
                    CString cmd,date1,date2;
                    DateStrFromDateTime(mdate(two.dat[1]),date1);
                    DateStrToDateTime(mdate(two.dat[2]),date2);
                    cmd.Format("{%s} in %s to %s", FieldName ,date1, date2);
                    if(bFlag) {
                        m_Selection_Formula += " and ";
                    }
                    m_Selection_Formula += cmd;
                    bStartFlag = FALSE;
					bFlag = TRUE;
                    lset(option->dat[1],cmd);
                    fastwrt(rec,option);
                    if(InternalFilter == FALSE) break;
                    continue;
                }
                else {
                    dateTimeCommand = instr(1,Data,"Date");
                    if(dateTimeCommand) {
                        CString cmd,date1,date2;
                        DateStrToDate(mdate(two.dat[1]),date1);
                        DateStrToDate(mdate(two.dat[2]),date2);
                        cmd.Format("{%s} in %s to %s", FieldName ,date1, date2);
                        if(bFlag) {
                            m_Selection_Formula += " and ";
                        }
                        m_Selection_Formula += cmd;
                        bStartFlag = FALSE;
						bFlag = TRUE;
                        lset(option->dat[1],cmd);
                        fastwrt(rec,option);
                        if(InternalFilter == FALSE) break;
                        continue;
                    }
                }
            }
        }
        if(InternalFilter == TRUE) {
            strcpy(text,option->dat[1]);
            rtrim(text);
            if(!isblank(text)) {
                if(bFlag) {
                    m_Selection_Formula += " and ";
                }
                m_Selection_Formula += text;
                bFlag = TRUE;
            }
        }
    }

#ifdef _DEBUG
    if(m_Selection_Formula.GetLength()) {
        MessageBox(m_Selection_Formula);
    }
#endif //_DEBUG

    if (SetSelectionFormula(m_Selection_Formula)==FALSE) {
        return FALSE;
    }	
    return TRUE;
}

void CCrSelection::OnEditCopy()
{
    CRPEJobInfo jobStatus;
    while(1) {
        short status = app->printJob->GetJobStatus(&jobStatus);
        if(status == PEP_JOBINPROGRESS) {
			continue;
		}
        if(status == PEP_JOBINPROGRESS) {
		}
        BOOL bJob = app->printJob->IsJobFinished();
        if(bJob==TRUE) break;
		break;
    }
    OnCancel();
}

void CCrSelection::GetNGroupSortFields()
{
    if(m_PreviewEnable==FALSE) return;
    short nSortFlds = app->printJob->GetNGroupSortFields();

    for(short int i=0; i<nSortFlds; i++) {
        short int direction;
        CString fieldName,msg;
        BOOL bFlag = app->printJob->GetNthGroupSortField (i,fieldName,&direction);
        if(bFlag == FALSE) {
            msg.Format("Field %i: %s Get Nth Sort Field error\n",i,fieldName);
            MessageBox(msg);
        }
    }
}

void CCrSelection::GetNSortFields()
{
    if(m_PreviewEnable==FALSE) return;
    if(m_UseInternalSortFields==FALSE) return;
    short nSortFlds = app->printJob->GetNSortFields ();
    if(nSortFlds==0) return;

    for(short int i=0; i<nSortFlds; i++) {
        short int direction;
        CString fieldName,msg;
        BOOL bFlag = app->printJob->GetNthSortField (i,fieldName,&direction);
        if(bFlag == FALSE) {
            msg.Format("Field %i: %s Get Nth Sort Field error\n",i,fieldName);
            MessageBox(msg);
        }
    }
    CCrSortFields h2;
    h2.DoModal();

}

void CCrSelection::OnAutoMailExport()
{
    if(m_AutoMailExport==TRUE) {
        BOOL toMail = TRUE;
        BOOL nExport = app->printJob->ExportPrintWindow(toMail); // export the preview window
    }
}

void CCrSelection::OnExport()
{
    if(m_export==FALSE) return;

	if(strWinWordFileName.GetLength()==0) {
		strWinWordFileName = m_CrystalReportName;
		if(m_ExportToWinExcel) {
			strWinWordFileName.Replace(".rpt", ".xls");
		}
		else if(m_ExportToWinWord) {
			strWinWordFileName.Replace(".rpt", ".doc");
		}
	}
    int len = strWinWordFileName.GetLength() + 2;
    if(len == 2) m_ExportToWinWord = FALSE;

    CRPEExportOptions options;
    if(m_ExportToWinWord) {

        strcpy(&destinationOptions[2],strWinWordFileName);
        destinationOptions[0] = char(len);
        destinationOptions[1] = char(0x80);

        options.m_StructSize = 150;               // initialize to PE_SIZEOF_EXPORT_OPTIONS
        strcpy(options.m_formatDLLName,"u2fwordw");
        options.m_formatType = UXFWordWinType;
        options.m_formatOptions = 0;
        strcpy(options.m_destinationDLLName,"u2ddisk");
        options.m_destinationType = UXDDiskType;
        options.m_destinationOptions = destinationOptions;
        options.m_nFormatOptionsBytes= 0;      // Set by 'PEGetExportOptions'
        options.m_nDestinationOptionsBytes = len; // Set by 'PEGetExportOptions'
    }
    else if(m_ExportToWinExcel) {
        len += 5;
        strcpy(&destinationOptions[2],strWinWordFileName);
        destinationOptions[0] = char(len);
        destinationOptions[1] = char(0x80);

        options.m_StructSize = 150;               // initialize to PE_SIZEOF_EXPORT_OPTIONS
        strcpy(options.m_formatDLLName,"u2fxls");
        options.m_formatType = UXFXls5Type;
        options.m_formatOptions = 0;
        strcpy(options.m_destinationDLLName,"u2ddisk");
        options.m_destinationType = UXDDiskType;
        options.m_destinationOptions = destinationOptions;
        options.m_nFormatOptionsBytes= 0;      // Set by 'PEGetExportOptions'
        options.m_nDestinationOptionsBytes = len ; // Set by 'PEGetExportOptions'
    }
    else {
        BOOL bExportOptions = app->printJob->GetExportOptions (&options);
        if(bExportOptions == FALSE) return;
    }
    BOOL bDone = app->printJob->ExportTo(&options);
    SetActiveWindow();
}

void CCrSelection::Refresh(DB *df)
{
    m_UserDescription = FALSE;
    if(cnn) {
        if(app->m_strConnection.Compare("DSN=ODBC")!=0) {
            if (IsAdoSystem()) {
                 if(app->IsOnlineProgram) {
                     CString str = "select count(*) from [ReportFilterDescription]";
                     double v = SqlValue(str);
                     if(v) m_UserDescription = TRUE;
                 }
                 else {
                     m_UserDescription = TRUE;
                 }
             }
        }
    }

    LV_ITEM lvi;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
    lvi.iSubItem = 0;

    unsigned long BkColor = RGB(0,0,0);
    unsigned long FgColor;
    for (long rec=1 ; rec <= df->t->sumrecord; rec++) {
        readrec(rec,df);
        if(rec % 2 ) {
            FgColor = RGB(190,255,255);
        }
        else {
            FgColor = RGB(255,255,255);
        }

        ltot(rec,2,text);
        lvi.iItem      = rec-1;
        lvi.cchTextMax = 1;
        lvi.pszText    = text;
        m_ListView.InsertItem(&lvi);

        int len = strlen(df->dat[1]);
        GetFieldName(0,len,FieldName,Op,Data);
        CString strDescription = FindCrselectDescription(m_CrystalReportName,FieldName,m_UserDescription);
        m_ListView.SetItemText(rec-1, 1, strDescription , BkColor, FgColor);

        strcpy(text,Op);
        rtrim(text);
        m_ListView.SetItemText(rec-1, 2, text , BkColor, FgColor);

        strcpy(text,Data);
        rtrim(text);
        m_ListView.SetItemText(rec-1, 3, text , BkColor, FgColor);
    }
}

void CCrSelection::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

    GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	if(phdn->iItem>=0) {
        option->rec = phdn->iItem + 1;
        if(option->rec <= option->t->sumrecord) {
            readrec(option->rec,option);
            strcpy(text,option->dat[1]);
            rtrim(text);
            SetDlgItemText(IDC_EDIT1,text);
            int len = strlen(option->dat[1]);
            GetFieldName(0,len,FieldName,Op,Data);
            CString strDescription = FindCrselectDescription(m_CrystalReportName,FieldName,m_UserDescription);
            SetDlgItemText(IDC_EDIT2,strDescription);

            GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
        }
    }
	*pResult = 0;
}

void CCrSelection::OnUpdateDescription() 
{
    if(m_UserDescription==FALSE) return;

    int iPos = instr(1,FieldName,".");
    if(iPos==0) return;

    UpdateData();
    _strtime(szTime);
    strcpy(szTable,FieldName);
    szTable[iPos-1] = 0;
    strcpy(szField,&FieldName[iPos]);

    CString strSql;
    strSql.Format("delete from [ReportFilterDescription] where [Field] = '%s' and [Table] = '%s' and [Rpt]='%s' and [pref] = '%s'",
        szField,szTable,m_CrystalReportName,Pref);
    RunSql(strSql);
    strSql.Format("insert into [ReportFilterDescription] ([Field],[Table],[Rpt],[pref],[Description],[user],[date]) values ('%s','%s','%s','%s','%s','%s','%s %s') ",
        szField,szTable,m_CrystalReportName,Pref,m_description,user.dat[1], DateStr(today), szTime);
    RunSql(strSql);
    ShowReportOption(option);
}

void CCrSelection::OnEdit() 
{
    if(m_PreviewEnable==FALSE) return;
    if (option->rec==0) return;

    int len = strlen(option->dat[1]);
    GetFieldName(0,len,FieldName,Op,Data);

    int dotExisted = instr(1,FieldName,".");
    if(dotExisted==0) return;

    int dateTimeCommand = instr(1,Data,"DateTime");
    if(dateTimeCommand) {
        CCrDateSelection h1;
        h1.m_cmd  = Op;
        h1.m_data = Data;
        if(h1.DoModal()==IDOK) {
            CString cmd;
            cmd.Format("{%s} %s %s", FieldName ,h1.m_cmd , h1.m_data);
            lset(option->dat[1],cmd);
            fastwrt(option->rec,option);
            ShowReportOption(option);
            OnFilter();
        }
        return;
    }
    else {
        dateTimeCommand = instr(1,Data,"Date");
        if(dateTimeCommand) {
            CCrDateSelection h1;
            h1.m_cmd  = Op;
            h1.m_data = Data;
            h1.m_DateTimeFormat = FALSE;
            if(h1.DoModal()==IDOK) {
                CString cmd;
                cmd.Format("{%s} %s %s", FieldName ,h1.m_cmd , h1.m_data);
                lset(option->dat[1],cmd);
                fastwrt(option->rec,option);
                ShowReportOption(option);
                OnFilter();
            }
            return;
        }
    }

    char tmpField[200],TrimFieldName[200];
    strcpy(tmpField,&FieldName[dotExisted]);
    trim_field(TrimFieldName,tmpField);                     //17.1.2002
    CString strField = app->ConfirmCrystalTableFile(TrimFieldName,m_CrystalReportName);
    DB *pf = GetMatchFile(strField);
    if(!pf) {
        char szFileName[1024];
        strncpy(szFileName, FieldName, dotExisted-1);
        szFileName[dotExisted-1] = 0;
        strField = app->ConfirmCrystalTableFile(szFileName,m_CrystalReportName);
        pf = GetMatchFile(strField);
        if(pf) {
            int field_no = fldNo(pf,tmpField);
            if(field_no !=1 ) pf = NULL;
        }
    }

    if(pf) {
        void (*tmp_SetGridRows)(CMSFlexGrid *grid);
        tmp_SetGridRows = pf->e->SetGridRows;
        pf->e->SetGridRows = SetGridRows;
        CCrFieldSelection b1(IDD_CRSELECTION_EDIT);
        b1.df       = pf;
        b1.IdxItem  = pf->f[1].idx & 1;
        b1.EditFlag = 0;
        b1.mSingleClick = FALSE;   //MUST DOUBLE CLICK
        b1.m_ButtonOk   = TRUE;
        b1.m_cmd        = Op;
        b1.FieldName    = FieldName;
        if(b1.DoModal()==IDOK) {
            CString cmd;
            cmd.Format("{%s} %s %s", FieldName ,b1.m_cmd , b1.m_data);
            lset(option->dat[1],cmd);
            fastwrt(option->rec,option);
            ShowReportOption(option);
            OnFilter();
        }
        pf->e->SetGridRows = tmp_SetGridRows;
    }
}

BOOL CCrSelection::DoCreatingRpt()
{
    if(m_FMT){
        OnStdplan();
        return TRUE;
    }
	CRPEWindowOptions m_windowOptions;
    BOOL flag = app->printJob->GetWindowOptions(&m_windowOptions);
    app->CheckCrystalReportStatus(flag);
    short nGroup = app->printJob->GetNGroups();
    if(nGroup>0) {
        m_windowOptions.m_hasGroupTree = TRUE;
    }
    else {
        m_windowOptions.m_hasGroupTree = FALSE;
    }
    m_windowOptions.m_showDocumentTips = TRUE;
    m_windowOptions.m_showToolbarTips  = TRUE;
    m_windowOptions.m_hasPrintButton      = m_hasPrintButton;
    m_windowOptions.m_hasExportButton     = m_hasPrintButton;
    m_windowOptions.m_hasPrintSetupButton = m_hasPrintButton;

    m_windowOptions.m_hasZoomControl = TRUE;
    m_windowOptions.m_hasProgressControls = TRUE;
    m_windowOptions.m_hasSearchButton = TRUE;
    m_windowOptions.m_canDrillDown = TRUE;
    if(app->IsHasStoreProcedure==TRUE){
        m_windowOptions.m_hasRefreshButton = FALSE;
    }
    else {
        m_windowOptions.m_hasRefreshButton = TRUE;
    }
    m_windowOptions.m_hasNavigationControls = TRUE;
    m_windowOptions.m_hasCancelButton = TRUE;
    m_windowOptions.m_hasLaunchButton = FALSE;
    if(!app->printJob->SetWindowOptions(&m_windowOptions)){
         msg.Format("File: %s error SetWindowOptions\n",m_CrystalReportName);
         ErrorMsg();
         return FALSE;
    }
    OnExport();
    return app->printJob->Start();
}

void CCrSelection::OpenSubReport()
{
    short SubreportsInSection[99];

    memset(SubreportsInSection, 0, sizeof(SubreportsInSection));
    short nSection = app->printJob->GetNSections();
    for(int y=0; y<nSection && y<99; y++) {
        short nSectionCode = app->printJob->GetSectionCode(y);
        SubreportsInSection[y] = nSectionCode;
    }
    SubreportsInSection[y] = 0;
    nSubreport = 0;

    CString m_Selection_Formula;
    if (!app->printJob->GetSelectionFormula(m_Selection_Formula)) {
        ErrorBox(app->printJob->GetErrorText());
	}
    for(short z=0; ; z++) {
        short sectionCode = SubreportsInSection[z];
        if(sectionCode ==0) break;
        short nSR = app->printJob->GetNSubreportsInSection(sectionCode);
        for(short y=0; y<nSR; y++) {
            if(nSubreport>=40) continue;
            short handle = app->printJob->GetJobHandle();
            DWORD subreportHandle = app->printJob->GetNthSubreportInSection(sectionCode,y);
            CRPESubreportInfo subreportInfo;
            BOOL st = app->printJob->GetSubreportInfo (subreportHandle,&subreportInfo);
            app->CheckCrystalReportStatus(st);

            hSubJob[nSubreport] = app->printJob->OpenSubreportJob (subreportInfo.m_name);
            ResetFiles(hSubJob[nSubreport]);
            app->OnLogonserver(hSubJob[nSubreport],m_CrystalReportName);
            SetRPR_Title(scr,hSubJob[nSubreport]);
// 20.10.2003
            CString m_subSelection_Formula;
            if (!hSubJob[nSubreport]->GetSelectionFormula(m_subSelection_Formula)) {
                ErrorBox(hSubJob[nSubreport]->GetErrorText());
            }
            if(sectionCode>=8000 && sectionCode<9000) {
                if(m_subSelection_Formula.GetLength()>0) {
                    if (!hSubJob[nSubreport]->SetSelectionFormula(m_Selection_Formula)) {
                        ErrorBox(hSubJob[nSubreport]->GetErrorText());
                    }
                }
            }
            nSubreport++;
        }
    }
}

void CCrSelection::CloseSubReport()
{
    if(nSubreport>0) {
        BOOL st = app->printJob->CloseSubreport();
        if(!st) {
            ErrorBox(app->printJob->GetErrorText());
        }
		else {
			for(int x=0; x<nSubreport; x++) {
				delete hSubJob[x];
			}
		}
    }
}

BOOL CCrSelection::ChangePrinter()
{
    DEVMODE *mode = 0;

    CString driverName,printerName,portName;
    if(m_RunTimeChangePrinter) {
        BOOL bFlag = app->printJob->GetSelectedPrinter (driverName,printerName,portName,&mode);
        app->CheckCrystalReportStatus(bFlag);
        m_printerName = printerName;
        if(app->SelectPrinter(m_CrystalReportName,m_printerName)==TRUE) {
            if(m_printerName.GetLength()==0){
                CPrintDialog dlg(FALSE);
                dlg.DoModal();
                m_printerName = dlg.GetDeviceName();
            }
            if(m_printerName.Compare(printerName)) {
                bFlag = app->printJob->SelectPrinter("",m_printerName,"");
                if(bFlag==FALSE) {
                     msg.Format("Error select printer (%s printer name)\n",m_printerName);
                     ErrorMsg();
                     return FALSE;
                }
            }
        }
    }
    return TRUE;
}

BOOL CCrSelection::SetSelection_Formula()
{
	CString m_Selection_Formula;
    if (!app->printJob->GetSelectionFormula(m_Selection_Formula)) {
        ErrorBox(app->printJob->GetErrorText());
        OnCancel();
        return FALSE;
	}
    if(app->SelectFilter(m_CrystalReportName,m_Selection_Formula)==TRUE) {
        if (SetSelectionFormula(m_Selection_Formula)==FALSE) {
            return FALSE;
        }
    }
    return TRUE;
}

void CCrSelection::OnStart()
{
    CRPEJobInfo jobStatus;
    short status = app->printJob->GetJobStatus(&jobStatus);
    if(status != PEP_JOBNOTSTARTED) {
        msg.Format("invalid %s report",m_CrystalReportName);
        ErrorMsg();
        return;
    }
    BOOL bFlag = SetSelection_Formula();
    if(bFlag==FALSE) return;

    if (ChangePrinter()==FALSE) {
        return;
    }
    app->ExecStoreProcedure(m_CrystalReportName,option);
    if (!app->CreateView(m_CrystalReportName,selecttext)) {
	}

    ResetFiles(app->printJob);
    SetRPR_Title(scr,app->printJob);

    OpenSubReport();

    if(m_PreviewEnable==TRUE) {
        msg.Format("Preview %s",m_CrystalReportName);
		CRect rect;
		app->m_pMainWnd->GetWindowRect(&rect);
        bFlag = app->printJob->OutputToWindow(msg, 0,0,rect.right,rect.bottom,0, (CWnd *) 0);
        if(bFlag == FALSE) {
            msg.Format("File: %s Preview Windows error\n",m_CrystalReportName);
            ErrorMsg();
            return;
        }
    }
    else {
        bFlag = app->printJob->OutputToPrinter();
        if(bFlag == FALSE) {
            msg.Format("File: %s Output to printer error\n",m_CrystalReportName);
            ErrorMsg();
            return;
        }
    }
    DoSetDefaultparameterfield();

    bFlag = DoCreatingRpt();
    if(bFlag == FALSE) {
        msg = app->printJob->GetErrorText();
        ErrorBox(msg);
    }
    OnSaveRptLog();
    CloseSubReport();
    if(m_ExportToWinExcel || m_ExportToWinWord) {
        CString lpFile = strWinWordFileName;
		lpFile.TrimRight();
        HINSTANCE flag = ShellExecute(
            NULL,
            "OPEN",
            lpFile,
            NULL,
            NULL,
            SW_SHOW
            );
    }
    if(bFlag == FALSE) {
        OnCancel();
        return;
    }

    if(m_PreviewEnable==FALSE) {
        DelayLoop();
        SetActiveWindow();
    }
    OnAutoMailExport();
    app->printJob->IsJobFinished();
    RestMsg();
    CCryDlg::OnOK();
}

BOOL CCrSelection::StringtoValinfo(CString& input,short valueType, PEValueInfo* valInfo)
{
switch(valueType){
	case PE_PF_NUMBER:
	
		valInfo->viNumber = atof(input);
		
		valInfo->valueType = PE_VI_NUMBER; 
		break;
	case PE_PF_CURRENCY:{
	
			return FALSE;
		}
		valInfo->viCurrency = atof(input.Mid(1, (input.GetLength() - 1)));
		
		valInfo->valueType = PE_VI_CURRENCY;
		break;
						
	case PE_PF_BOOLEAN:

		if(!input.CompareNoCase("TRUE") || !input.CompareNoCase("FALSE"))
			valInfo->viBoolean = (!input.CompareNoCase("TRUE"))? TRUE: FALSE;
		else{
			return FALSE;
		}
		valInfo->valueType = PE_VI_BOOLEAN;
		break;
/*	case PE_PF_INTEGER:
		valInfo->viInteger = atoi(input)
		if(valInfo->viInteger == 0){
			DisplayError(1, IDS_HINTINTEGER);
			return FALSE;
		}
		break; */
	case PE_PF_DATE:  
	
		if(!(input.Mid(0, 4)).CompareNoCase("DATE") && input.GetLength() <= 17 )
		{
			switch(input.GetLength()) 
			{
			case 17:
				{
					// Negative date
					valInfo->viDate[0] = atoi(input.Mid(5, 5));
					valInfo->viDate[1] = atoi(input.Mid(11, 2));
					valInfo->viDate[2] = atoi(input.Mid(14, 2));
					if(valInfo->viDate[0] == 0)
					{
						return FALSE;
					}
				} break;
			case 16: 
				{
					// normal date (non-negative)
					valInfo->viDate[0] = atoi(input.Mid(5, 4));
					valInfo->viDate[1] = atoi(input.Mid(10, 2));
					valInfo->viDate[2] = atoi(input.Mid(13, 2));
					if(valInfo->viDate[0] == 0)
					{
						return FALSE;
					}
				} break;
			case 15: 
				{
					// normal date (non-negative)
					valInfo->viDate[0] = atoi(input.Mid(5, 3));
					valInfo->viDate[1] = atoi(input.Mid(9, 2));
					valInfo->viDate[2] = atoi(input.Mid(12, 2));
					if(valInfo->viDate[0] == 0)
					{
						return FALSE;
					}
				} break;
			default:
				{
					return FALSE;
				}	
			} // end switch
					
			if(valInfo->viDate[1] == 0){
				return FALSE;
			}
			
			if(valInfo->viDate[2] == 0){
				return FALSE;
			}
		}
		else{
				return FALSE;
		}
		valInfo->valueType = PE_VI_DATE;
		break;

		
	case PE_PF_STRING:
	
		if(input.GetLength() > PE_VI_STRING_LEN){
				return FALSE;
		}

		lstrcpy(valInfo->viString, input);
	
		valInfo->valueType = PE_VI_STRING;
		break;
	/*case PE_VI_DATETIME:
		value.Format("%d-%d-%d %d:%d:%d", valInfo->viDateTime[0],valInfo->viDateTime[1],valInfo->viDateTime[2],valInfo->viDateTime[3], valInfo->viDateTime[4], valInfo->viDateTime[5]);
		break;
	case PE_VI_TIME:
		value.Format("%d:%d:%d", valInfo->viTime[0],valInfo->viTime[1],valInfo->viTime[2]);
		break;
	case PE_VI_COLOR:
		value = "Color";
		break;
	case PE_VI_CHAR:
		value.Format("%c", valInfo->viC);
		break;
	case PE_VI_LONG:
		value.Format("%d", valInfo->viLong);
		break;
	case PE_VI_NOVALUE:
		value = "No Value";
		break; */
	default:
		break;
	}
	return TRUE;
}

BOOL CCrSelection::StringtoPFinfo(CString& input,char* block)
{
    switch(m_paramField.ValueType){
        case PE_PF_NUMBER:

            *(double *)block = atof(input);
            break;
        case PE_PF_CURRENCY:

            if(input[0] != '$'){
                    return FALSE;
            }
            *(double *)block = atof(input.Mid(1, (input.GetLength() - 1)));


            break;

        case PE_PF_BOOLEAN:

            if(!input.CompareNoCase("TRUE") || !input.CompareNoCase("FALSE"))
                *(BOOL *) block = (!input.CompareNoCase("TRUE"))? TRUE: FALSE;
            else{
                return FALSE;
            }

            break;
        case PE_PF_DATE:{
            CRInt16s year;
            CRInt16u months, days;
            CRDate date;
            if(!(input.Mid(0, 4)).CompareNoCase("DATE") && input.GetLength() == 16 ){
                year = atoi(input.Mid(5, 4));
                months = atoi(input.Mid(10, 2));
                days = atoi(input.Mid(13, 2));
                if(year == 0){
                    return FALSE;
                }

                if(months == 0){
                    return FALSE;
                }

                if(days == 0){
                    return FALSE;
                }
            }
            else{
                    return FALSE;
            }
        date = CRYearMonthDayToDate(year, months, days);
        *(CRDate *) block = date;
            break;
                        }

        case PE_PF_STRING:

            if(input.GetLength() > PE_VI_STRING_LEN){
                    return FALSE;
            }

            lstrcpy(block, input);


            break;
        default:
            break;
    }
    return TRUE;
}

void CCrSelection::OnSetparameterfield()
{
    // Needed Structures
	PEValueInfo valInfo;
	valInfo.StructSize = PE_SIZEOF_VALUE_INFO;
	
    // Set the current value information
    if( StringtoValinfo(m_currentValue, m_paramField.ValueType, &valInfo) ) {
		if(!PEConvertVInfoToPFInfo(&valInfo, &m_paramField.ValueType, m_paramField.CurrentValue)) {
			if(!StringtoPFinfo(m_currentValue, m_paramField.CurrentValue)) {
   
			    // Set the default value inforation
				if(StringtoValinfo(m_defaultValue, m_paramField.ValueType, &valInfo)) {
					if(!PEConvertVInfoToPFInfo(&valInfo, &m_paramField.ValueType, m_paramField.DefaultValue)) {
						if(!StringtoPFinfo(m_defaultValue, m_paramField.DefaultValue)) {
						}
					}
				}
			}
		}
	}

    m_paramField.DefaultValueSet = 1;
    m_paramField.CurrentValueSet = 1;

    if(!PESetNthParameterField(app->printJob->GetJobHandle(), iPos, &m_paramField))
    {
		int k=1;
    }
	else
    {
		int k=2;
	}
} // End Set ParameterField Button


void CCrSelection::DoSetDefaultparameterfield()
{

    m_paramField.StructSize = PE_SIZEOF_PARAMETER_FIELD_INFO;

	int totalParams = PEGetNParameterFields(app->printJob->GetJobHandle());

    for(int count = 0; count < totalParams; count++) {
        iPos = count;
        PEGetNthParameterField(app->printJob->GetJobHandle(),count,&m_paramField);
        if(!stricmp(m_paramField.Name,"ONE")) {
            int year = year_no(one.dat[1]);
            int month = month_no(one.dat[1]);
            int day = day_no(one.dat[1]);
            m_currentValue.Format("date(%4.4i,%2.2i,%2.2i)",year,month,day);
            OnSetparameterfield();
        }
        else if(!stricmp(m_paramField.Name,"TWO1")) {
            int year = year_no(two.bak[1]);
            int month = month_no(two.bak[1]);
            int day = day_no(two.bak[1]);
            m_currentValue.Format("date(%4.4i,%2.2i,%2.2i)",year,month,day);
            OnSetparameterfield();
        }
        else if(!stricmp(m_paramField.Name,"TWO2")) {
            int year = year_no(two.bak[2]);
            int month = month_no(two.bak[2]);
            int day = day_no(two.bak[2]);
            m_currentValue.Format("date(%4.4i,%2.2i,%2.2i)",year,month,day);
            OnSetparameterfield();
        }
        else if(!stricmp(m_paramField.Name,"@TWO1")) {
            int year = year_no(two.bak[1]);
            int month = month_no(two.bak[1]);
            int day = day_no(two.bak[1]);
            m_currentValue.Format("datetime(%4.4i,%2.2i,%2.2i,0,0,0)",year,month,day);
            OnSetparameterfield();
        }
        else if(!stricmp(m_paramField.Name,"@TWO2")) {
            int year = year_no(two.bak[2]);
            int month = month_no(two.bak[2]);
            int day = day_no(two.bak[2]);
            m_currentValue.Format("date(%4.4i,%2.2i,%2.2i,0,0,0)",year,month,day);
            OnSetparameterfield();
        }
        else if(!stricmp(m_paramField.Name,"START_DATE")) {
            int year = year_no(two.dat[1]);
            int month = month_no(two.dat[1]);
            int day = day_no(two.dat[1]);
            m_currentValue.Format("date(%4.4i,%2.2i,%2.2i)",year,month,day);
            OnSetparameterfield();
        }
        else if(!stricmp(m_paramField.Name,"END_DATE")) {
            int year = year_no(two.dat[2]);
            int month = month_no(two.dat[2]);
            int day = day_no(two.dat[2]);
            m_currentValue.Format("date(%4.4i,%2.2i,%2.2i)",year,month,day);
            OnSetparameterfield();
        }
        else {
            app->SetParameter(this);
        }
	}
}

void CCrSelection::OnSaveRptLog()
{
    if(cnn==NULL) return;
    if(app->m_strConnection.CompareNoCase("DSN=ODBC")==0) return;
    if(IsSqlFile == FALSE) {
        if(app->m_strConnection.Find("DSN")>=0) return;
    }
    BOOL bflag = IsAdoTableExisted("RptLog");
    if(!bflag) return;

    _strtime(szTime);
    CString strSql,m_Selection_Formula;

    bflag = app->printJob->GetSelectionFormula(m_Selection_Formula);
    app->CheckCrystalReportStatus(bflag);

    strSql.Format("insert into [RptLog] ([Name],[Date],[User],[Formula]) values ('%s','%s %s','%s','%s') ",
        m_CrystalReportName,DateStr(today),szTime,user.dat[1],
        adoString(m_Selection_Formula));
    RunSql(strSql);

}

void CCrSelection::OnCancel()
{
    app->printJob->Close();
    CCryDlg::OnCancel();
}

CString CCrSelection::ChangeFormulaText(char *text)
{
    int len = strlen( text );
    GetFieldName(0,len,FieldName,Op,Data);

    int dotExisted = instr(1,FieldName,".");
    if(dotExisted==0) return text;
    int dateTimeCommand = instr(1,Data,"DateTime");
    if(dateTimeCommand) {
        return text;
    }
    dateTimeCommand = instr(1,Data,"Date");
    if(dateTimeCommand) {
        return text;
    }

    char szFileName[1024];
    strncpy(szFileName, FieldName, dotExisted-1);
    szFileName[dotExisted-1] = 0;

    DB *pf = GetMatchFile(szFileName);
    if(!pf) return text;

    char tmpField[200],TrimFieldName[200];
    strcpy(tmpField,&FieldName[dotExisted]);
    int fld = GetDbFieldNo(pf,tmpField);
    if(fld==0) return text;

    return text;                                            //10.11.2003
    if( pf->Foreign[fld]==0 ) return text;

    trim_field(TrimFieldName,tmpField);                     //17.1.2002
    DB *qf = GetMatchFile(TrimFieldName);
    if(!qf) return text;

    char NewFieldName[1024];
    sprintf(NewFieldName,"%s.%s",qf->m_strTable, qf->fieldName[1]);
    CString str = strupr(text);
    str.Replace(FieldName,NewFieldName);
	strcpy(text, str);

    return text;
}

BOOL CCrSelection::OnFilter()
{
    CString m_Selection_Formula="";
    BOOL bFlag = FALSE;

    char szWild[10];
	space_str(szWild,8);
    for(int i=0; i<20; i++) {
        RptSelectionTitle[i] = "";
    }
    i = 0;
    for (long rec=1 ; rec <= option->t->sumrecord; rec++) {
        readrec(rec,option);
        strcpy(text,option->dat[1]);
        rtrim(text);
		rset(szWild,text);
		if(stricmp(szWild,"like \"*\"")==0) continue;
        if(isblank(text)) continue;
        if(bFlag) {
            m_Selection_Formula += " and ";
        }
        m_Selection_Formula += ChangeFormulaText(text);
        bFlag = TRUE;
        if(i>=20) continue;

        int len = strlen(option->dat[1]);
        GetFieldName(0,len,FieldName,Op,Data);
        int dateTimeCommand = instr(1,Data,"DateTime");
        if(dateTimeCommand) continue;
        dateTimeCommand = instr(1,Data,"Date");
        if(dateTimeCommand) continue;
        dateTimeCommand = instr(1,Data,"If"); // OOI
        if(dateTimeCommand) continue;

        CString strDescription = FindCrselectDescription(m_CrystalReportName,FieldName,m_UserDescription);
		if(i>0) {
//            RptSelectionTitle[i] += "      ";
		}
        RptSelectionTitle[i] += strDescription;
        RptSelectionTitle[i] += " ";

        strcpy(text,Op);
        rtrim(text);
        if(text[0]=='=') {
            RptSelectionTitle[i] += ": ";
            strcpy(text,Data);
            trim(text);
            int xlen = strlen(text);
            text[xlen-1] = 0;
            RptSelectionTitle[i] += &text[1];

            char TrimFieldName[1000];
            trim_field(TrimFieldName,szField);                     //17.1.2002
            DB *pf = GetMatchFile(TrimFieldName);
            if(pf) {
                lset(pf->dat[1], &text[1]);
                long xrec = pf->Search(pf->dat[1],1, TRUE);
                if(xrec>0) {
                    RptSelectionTitle[i] += "  ";           //15.07.2004
                    RptSelectionTitle[i] += pf->dat[2];
                }
            }
        }
        else {
            RptSelectionTitle[i] += text;
            RptSelectionTitle[i] += " ";
            strcpy(text,Data);
            trim(text);
            int xlen = strlen(text);
            text[xlen-1] = 0;
            RptSelectionTitle[i] += &text[1];
        }
        RptSelectionTitle[i].TrimRight();
        i++;
    }

#ifdef _DEBUG
    if(m_Selection_Formula.GetLength()) {
        MessageBox(m_Selection_Formula);
    }
#endif //_DEBUG
    return SetSelectionFormula(m_Selection_Formula);
}

void CCrSelection::OnPrintAndSetDescription()
{
    short SubreportsInSection[99];

    memset(SubreportsInSection, 0, sizeof(SubreportsInSection));
    short nSection = app->printJob->GetNSections();
    for(int y=0; y<nSection && y<99; y++) {
        short nSectionCode = app->printJob->GetSectionCode(y);
        SubreportsInSection[y] = nSectionCode;
    }
    SubreportsInSection[y] = 0;
    nSubreport = 0;

    for(short z=0; ; z++) {
        short sectionCode = SubreportsInSection[z];
        if(sectionCode ==0) break;
        short nSR = app->printJob->GetNSubreportsInSection(sectionCode);
        for(short y=0; y<nSR; y++) {
            if(nSubreport>=40) continue;
            short handle = app->printJob->GetJobHandle();
            DWORD subreportHandle = app->printJob->GetNthSubreportInSection(sectionCode,y);
            CRPESubreportInfo subreportInfo;
            BOOL st = app->printJob->GetSubreportInfo (subreportHandle,&subreportInfo);
            if(strcmpNI(subreportInfo.m_name, "RptSelectionTitle", 17)==0) {
                m_hasSetReportSelectionDescription = TRUE;
            }
            nSubreport++;
        }
    }
}
