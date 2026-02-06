// DuplicateCR.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "DuplicateCR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDuplicateCR dialog


CDuplicateCR::CDuplicateCR(CWnd* pParent /*=NULL*/)
	: CDialog(CDuplicateCR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDuplicateCR)
	m_crno = _T("");
	m_nextno = _T("");
    m_noofcopy = _T("1");
	//}}AFX_DATA_INIT
}


void CDuplicateCR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDuplicateCR)
	DDX_Text(pDX, IDC_EDIT1, m_crno);
	DDX_Text(pDX, IDC_EDIT2, m_nextno);
	DDX_Text(pDX, IDC_EDIT3, m_noofcopy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDuplicateCR, CDialog)
	//{{AFX_MSG_MAP(CDuplicateCR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDuplicateCR message handlers

BOOL CDuplicateCR::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    backup(&arcr);
    long no_cr = ReadTicketNo(&arcr,1);
    lset(arcr.dat[1],"CN");
    arcr.CodeNoDefault(&arcr,1);
    m_nextno = arcr.dat[1];
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDuplicateCR::OnDuplicateCopy()
{
    lset(arcr.dat[1], szNo);
    long xrec = arcr.Search(arcr.dat[1],1);
    if(xrec>0) {
        str.Format("Cr no: %s existed",arcr.dat[1]);
        return;
    }

    lset(arcr.dat[1], m_crno);
    arcr.Search(arcr.dat[1],1,TRUE);
    lset(arcr.dat[1], szNo);
    long crec = viewidx(&debtor);
    if(crec==0) return;

    lset(arcr.dat[Cr_CUSTOMER], debtor.dat[1]);
    lset(arcr.dat[Cr_STAFF],debtor.dat[STAFF]);
    qd(debtor.dat[AREA],&area);
    lset(arcr.dat[Cr_PCCODE],area.dat[4]);
    arcr.AddNew();

    cnn->BeginTrans();
    strSql.Format("select * into [#newcr] from [argldat] where [refno]='%s'",m_crno);
    if(RunSql(strSql)==FALSE) {
        cnn->RollbackTrans();
        return;
    }

    str = "insert into [argldat] ([REFNO],[SQNO],[YOUR_REF],[GLCODE],[DESCRIPT],[ADJ_CAT],[TOTAL],[INVOICE]) ";
    str += "select '%s',[SQNO],[YOUR_REF],[GLCODE],[DESCRIPT],[ADJ_CAT],[TOTAL],[INVOICE] ";
    str += "from [#newcr] ";
    strSql.Format(str, szNo, m_crno);
    if(RunSql(strSql)==FALSE) {
        cnn->RollbackTrans();
        return;
    }

    strSql.Format("drop table [#newcr]");
    if(RunSql(strSql)==FALSE) {
        cnn->RollbackTrans();
        return;
    }
    cnn->CommitTrans();
}

void CDuplicateCR::OnDuplicate()
{
    long no_of_copy = atol(m_noofcopy);

    strcpy(szNo, m_nextno);
    for(long rec=1; rec<=no_of_copy; rec++) {
        OnDuplicateCopy();
        incno(szNo);
    }
}

void CDuplicateCR::OnOK() 
{
    UpdateData();

    if(m_nextno.IsEmpty()) {
        MessageBox("New cr no can not be blank");
        return;
    }
    lset(arcr.dat[1], m_crno);
    long xrec = arcr.Search(arcr.dat[1],1);
    if(xrec==0) {
        MessageBox("Original cr no not found");
        return;
    }
    if( atol(m_noofcopy) > 0) {
        OnDuplicate();
    }
	
	CDialog::OnOK();
}
