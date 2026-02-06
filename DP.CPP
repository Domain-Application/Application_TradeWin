// Dp.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "mgrid.h"
#include "Dp.h"
#include "PartyGrading.h"
#include "AskPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDp dialog

CDp::CDp(UINT IDD, DB *pdf, int Idx, CWnd *pParent /*=NULL*/)
	: CStaff(IDD, pdf, Idx, pParent)
{
	//{{AFX_DATA_INIT(CDp)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDp::DoDataExchange(CDataExchange *pDX)
{
	CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDp)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDp, CStaff)
//{{AFX_MSG_MAP(CDp)
ON_BN_CLICKED(IDC_PARTY_GRADING, OnPartyGrading)
ON_BN_CLICKED(IDC_DO, OnDo)
ON_BN_CLICKED(IDC_SETPAYDATE, OnSetpaydate)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDp message handlers
void CDp::SetAttribute(int flag, int fld)
{
	if (flag)
	{
		// GetDlgItem(IDC_DO)->ShowWindow(SW_SHOW);
	}
	else
	{
		// GetDlgItem(IDC_DO)->ShowWindow(SW_HIDE);
	}
	CStaff::SetAttribute(flag, fld);
}

void CDp::ShowInformation()
{
	CStaff::ShowInformation();
}

long VarianceWeight(DB *df)
{
	long wt, net;

	getmet(m, df);
	wt = atol(df->dat[DP_QTY1]) +
		 atol(df->dat[DP_QTY2]) +
		 atol(df->dat[DP_QTY3]) +
		 atol(df->dat[DP_QTY4]) +
		 atol(df->dat[DP_QTY5]) +
		 atol(df->dat[DP_QTY6]) +
		 atol(df->dat[DP_QTY7]) +
		 atol(df->dat[DP_QTY8]);
	net = m[11] - wt;
	return net;
}

long VarianceWeightParty(DB *df)
{
	long wt, net;

	getmet(m, df);
	wt = atol(df->dat[DP_QTY11]) +
		 atol(df->dat[DP_QTY12]) +
		 atol(df->dat[DP_QTY13]) +
		 atol(df->dat[DP_QTY14]) +
		 atol(df->dat[DP_QTY15]) +
		 atol(df->dat[DP_QTY16]) +
		 atol(df->dat[DP_QTY17]) +
		 atol(df->dat[DP_QTY18]);
	if (m[10] > 0 && wt <= 0)
	{
		return m[10];
	}
	if (wt <= 0)
		return 0;

	if (m[10] > 0)
	{
		net = m[10] - wt;
	}
	else
	{
		net = m[11] - wt;
	}
	return net;
}

int TotalDppercent(DB *df)
{
	return atoi(df->dat[DP_PCENT1]) +
		   atoi(df->dat[DP_PCENT2]) +
		   atoi(df->dat[DP_PCENT3]) +
		   atoi(df->dat[DP_PCENT4]) +
		   atoi(df->dat[DP_PCENT5]) +
		   atoi(df->dat[DP_PCENT6]) +
		   atoi(df->dat[DP_PCENT7]) +
		   atoi(df->dat[DP_PCENT8]);
}

void ClearDpContent(DB *df)
{
	long rec;

	for (int i = 0; i < 8; i++)
	{
		rec = qd(df->dat[DP_PT1 + i * 4], &prodtype);
		if (rec == 0)
		{
			lset(df->dat[DP_PCENT1 + i * 4], "");
			lset(df->dat[DP_QTY1 + i * 4], "");
			lset(df->dat[DP_WT1 + i * 4], "");
		}
	}
}

void UpdatePoInformation(DB *df, DB *podf)
{
	lset(df->dat[DP_ACCODE], podf->dat[PO_SUPPLIER]);
}

int tell_dp(UINT nChar, int &flag, int &fld, DB *df, int sq)
{
	long net, percent, Srec;
	double qty, final;
	CString str, result;
	int i = 0;
	int x = 0;

	qty = net = percent = 0;
	switch (fld)
	{
	case DP_VEHICLE:
		if (strcmp(df->dat[fld], df->bak[fld]))
		{
			qd(df->dat[DP_VEHICLE], &lorry);
			lset(df->dat[DP_IC], lorry.dat[LO_DRIVER1]);
			lset(df->dat[DP_ACCODE], lorry.dat[LO_DRIVER2]);
			lset(df->dat[DP_TRANSP], lorry.dat[LO_TRANSP]);
			qd(df->dat[DP_IC], &driver);
			lset(df->dat[DP_DRIVER], driver.dat[2]);
			CopyBufferToControl(DP_IC, df);
			CopyBufferToControl(DP_ACCODE, df);
			CopyBufferToControl(DP_TRANSP, df);
			CopyBufferToControl(DP_DRIVER, df);
			return TAB_REFRESH_ALL;
		}
		break;
	case DP_ACCODE:
		if (sq)
		{
			//			lset(df->dat[DP_BASIS],accode.dat[ACCODE_BASIS]);
		}
		break;
	case DP_TIMEIN:
	case DP_TIMEOUT:
		if (isblank(df->dat[fld]))
		{
			_strtime(df->dat[fld]);
		}
		break;
	case DP_PO:
		if (sq)
		{
			UpdatePoInformation(df, &so);
			return TAB_REFRESH_ALL;
		}
		break;
	case DP_IC:
		if (strcmp(df->dat[fld], df->bak[fld]))
		{
			qd(df->dat[DP_IC], &driver);
			lset(df->dat[DP_DRIVER], driver.dat[2]);
			CopyBufferToControl(DP_DRIVER, df);
			return TAB_REFRESH_ALL;
		}
		break;
	case DP_SGROSS:
	case DP_STARE:
		getmet(m, df);
		final = 0;
		final = double(m[11]);
		for (i = 0; i < 8; i++)
		{
			x = i * 4 + DP_QTY1;
			if (!isblank(df->dat[x - 1]))
			{
				qty = df->val(x - 1) * final / 100;
				dtodb(qty, df, x);
			}
		}
		SetTicketAmount(df);
		//      flag = DO;
		break;
	case DP_SERIAL:
		//        flag = DO;
		break;
	case DP_PCENT1:
	case DP_PCENT2:
	case DP_PCENT3:
	case DP_PCENT4:
	case DP_PCENT5:
	case DP_PCENT6:
	case DP_PCENT7:
	case DP_PCENT8:
		if (isblank(df->dat[fld - 1]))
			flag = DO;

		if ((percent = TotalDppercent(df)) > 100)
		{
			AfxMessageBox("Percentage over 100", MB_OK, 0);
			ltodb(100 - (percent - atol(df->dat[fld])), df, fld);
			CopyBufferToControl(fld, df);
			flag = DO;
			return TAB_REFRESH_ALL;
		}
		SetTicketAmount(df);
		break;
	case DP_QTY1:
	case DP_QTY2:
	case DP_QTY3:
	case DP_QTY4:
	case DP_QTY5:
	case DP_QTY6:
	case DP_QTY7:
	case DP_QTY8:
		if (isblank(df->dat[fld - 2]))
			flag = DO;
		getmet(m, df);
		final = double(m[11]);
		// if (m[10]>0) final =  double (m[10]);
		if (!isblank(df->dat[fld - 1]))
		{
			qty = df->val(fld - 1) * final / 100;
			dtodb(qty, df, fld);
		}
		SetTicketAmount(df);
		return TAB_REFRESH_ALL;
	case DP_WT1:
	case DP_WT2:
	case DP_WT3:
	case DP_WT4:
	case DP_WT5:
	case DP_WT6:
	case DP_WT7:
	case DP_WT8:
		if (isblank(df->dat[fld - 1]))
			flag = DO;
		SetTicketAmount(df);
		break;
	case DP_PT1:
	case DP_PT2:
	case DP_PT3:
	case DP_PT4:
	case DP_PT5:
	case DP_PT6:
	case DP_PT7:
	case DP_PT8:
		if (sq)
		{
			if (isblank(df->dat[DP_ACCODE]))
			{
				AfxMessageBox("Customer Code is empty, please key in customer code");
				return TAB_REFRESH_ALL;
			}
			else
			{
				Srec = qd(df->dat[DP_ACCODE], &accode);
				if (Srec < 1)
				{
					str.Format("Customer Code %s not regiestered yet", df->dat[DP_ACCODE]);
					AfxMessageBox(str);
					return TAB_REFRESH_ALL;
				}
			}
		}
		// if (isblank(df->dat[fld]) || sq){
		Srec = qd(df->dat[DP_PO], &so);
		if (Srec)
		{
			CString strSql, strWhere;
			strSql.Format("SELECT dbo.PT.PART, dbo.PT.NAME, dbo.PT.SNAME, dbo.PT.PTGROUP, dbo.PT.UNIT, dbo.PT.COST, dbo.SOITEM.PRICE AS SALES, dbo.PT.REMARKS FROM dbo.SOITEM LEFT OUTER JOIN dbo.PT ON dbo.SOITEM.PT = dbo.PT.PART ");
			strWhere.Format("WHERE SOITEM.SO = '%s' order by PT.PART", df->dat[DP_PO]);
			strSql += strWhere;
			prodtype.m_strCmdText = strSql;
			prodtype.ReOpen();
		}
		else
		{
			CString strSql, strWhere;
			strSql.Format("SELECT * FROM PT ");
			prodtype.m_strCmdText = strSql;
			prodtype.ReOpen();
		}
		//}
		if (sq)
		{
			//    dtodb(ChangePrice(df->dat[DP_ACCODE],df->dat[fld]),df,fld+3);
			//	int no = OnChangePriceField(fld);
			//	dtodb(ChangePrice(df->dat[DP_ACCODE],df->dat[fld]),df,no);
			//	SetTicketAmount(df);
		}
		break;
	case DP_PCENT11:
	case DP_PCENT12:
	case DP_PCENT13:
	case DP_PCENT14:
	case DP_PCENT15:
	case DP_PCENT16:
	case DP_PCENT17:
	case DP_PCENT18:
		if (isblank(df->dat[fld - 1]))
			flag = DO;

		if ((percent = TotalDspercentParty(df)) > 100)
		{
			AfxMessageBox("Percentage over 100", MB_OK, 0);
			ltodb(100 - (percent - atol(df->dat[fld])), df, fld);
			CopyBufferToControl(fld, df);
			flag = DO;
			return TAB_REFRESH_ALL;
		}
		SetTicketAmount(df);
		break;
	case DP_QTY11:
	case DP_QTY12:
	case DP_QTY13:
	case DP_QTY14:
	case DP_QTY15:
	case DP_QTY16:
	case DP_QTY17:
	case DP_QTY18:
		if (isblank(df->dat[fld - 2]))
			flag = DO;

		getmet(m, df);
		final = double(m[11]);
		if (m[10] > 0)
			final = double(m[10]);
		if (!isblank(df->dat[fld - 1]))
		{
			qty = df->val(fld - 1) * final / 100;
			dtodb(qty, df, fld);
		}
		SetTicketAmount(df);
		return TAB_REFRESH_ALL;
	case DP_WT11:
	case DP_WT12:
	case DP_WT13:
	case DP_WT14:
	case DP_WT15:
	case DP_WT16:
	case DP_WT17:
	case DP_WT18:
		if (isblank(df->dat[fld - 1]))
			flag = DO;
		SetTicketAmount(df);
		break;
	case DP_PT11:
	case DP_PT12:
	case DP_PT13:
	case DP_PT14:
	case DP_PT15:
	case DP_PT16:
	case DP_PT17:
	case DP_PT18:
		if (sq)
		{
			//	dtodb(ChangePrice(df->dat[DP_ACCODE],df->dat[fld]),df,fld+3);
			//	int no = OnChangePriceField(fld+3);
			//	dtodb(ChangePrice(df->dat[DP_ACCODE],df->dat[fld]),df,no);
			//	SetTicketAmount(df);
		}
		break;
		/*
	case DP_BASIS:
		if(sq){
			if (strncmp(df->dat[fld],"O",1))
				if (strncmp(df->dat[fld],"P",1)){
					waitmsg("Basis - O(Own) or P(Party)");
					lset(df->dat[fld],"O");
					return 2;
				}
		}
		break;
		*/
	case DP_DO:
		str.Format("Select * FROM DP_DO where DP_DO='%s' AND DP='%s'", dp.dat[DP_DO], dp.dat[1]);
		result = SqlText(str, 1);
		if (!isblank(result))
			flag = DO;
		break;

	case DP_PTYPE:
	{
		// 20230615
		if (sq)
		{
			if (!strncmp(df->dat[DP_PTYPE], "CASH", 4))
			{
				if (df->val(DP_AMT1) > 0)
				{
					if (AfxMessageBox("Pay Type Change to Cash.\r\nDo you want to refresh Amount", MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						SetTicketAmount(df);
						return 2;
					}
				}
			}
		}
		// END
		break;
	}
	}
	ClearDpContent(df);
	return TAB_REFRESH_ALL;
}

int Dp_initial(DB *df, int *fld) // initial setting
{
	if (UserLevel() < 999)
	{
		fld[0] = DP_MODIFY;
		fld[1] = 0;
	}
	if (UserLevel() < 9)
	{
		fld[0] = DP_DATE;
		fld[1] = 0;
	}
	if (UserLevel() < 5)
	{
		fld[1] = DP_STARE;
		fld[2] = DP_SGROSS;
		fld[3] = DP_TIMEIN;
		fld[4] = DP_TIMEOUT;
		fld[5] = 0;
	}
	return 1;
}

void ReplaceDpQuarryGrid(long rec, CMSFlexGrid *grid, DB *df, int IdxItem)
{
	MGRID g(grid, df, rec);
	g.SetText(df, NULL, DP_TICKET, DP_DATE, DP_VEHICLE, DP_ACCODE, DP_ACCODE | 0x100,
			  DP_PT1, DP_PT1 | 0x100, -1);
	getmet(m, df);

	char text[20];
	ltot(m[11], 6, text);
	g.SetText(text);
}

void SetDpQuarryGridTitle(CMSFlexGrid *grid, DB *df, int IdxItem)
{
	MGRID fg(grid, df, 0, 2);

	fg.SetTitle("No", 480, 1);
	fg.SetTitle("Ticket", 800, 1);
	fg.SetTitle("Date", 1000, 1);
	fg.SetTitle("Vehicle No", 1030, 1);
	fg.SetTitle("A/C Code", 885, 1);
	fg.SetTitle("Name", 2885, 1);
	fg.SetTitle("Product", 885, 1);
	fg.SetTitle("Description", 2000, 1);
	fg.SetTitle("Net Weight", 990, 7);
}

int CheckVarianceWeightParty(DB *df)
{
	if (!isCheck)
	{
		return 1;
	}

	long wt;

	getmet(m, df);
	wt = atol(df->dat[DP_QTY11]) +
		 atol(df->dat[DP_QTY12]) +
		 atol(df->dat[DP_QTY13]) +
		 atol(df->dat[DP_QTY14]) +
		 atol(df->dat[DP_QTY15]) +
		 atol(df->dat[DP_QTY16]) +
		 atol(df->dat[DP_QTY17]) +
		 atol(df->dat[DP_QTY18]);

	if (m[10] != wt)
	{
		CString str;
		str.Format("Reason: Net Weight(Party) <> Total Qty(Party).\r\nMinimum User");
		/* 7/5/2015
		int flag = UserPasswordLoginAccepted(df,5,str);
		if(flag==0){
			return 0;
		}
		*/
		return 1;
	}
	return 1;
}

int dp_save(DB *df, int flag)
{
	int x, z, i, j;
	long rec;

	getmet(m, df);
	if (TotalDsQty(df) != m[11])
	{
		AfxMessageBox("Total Qty is not tally with Nett weight.");
		return 0;
	}
	if (isblank(df->dat[DP_VEHICLE]))
	{
		waitmsg("Please key in vehicle no");
		return 0;
	}
	// Zero Weight checking
	//  getmet(m,df);
	if (m[11] == 0)
	{
		waitmsg("zero weight");
		return 0;
	}

	// Product Existing checking
	if (!(isblank(df->dat[DP_STARE])))
	{
		rec = qd(df->dat[DP_PT1], &prodtype);
		if (rec == 0)
		{
			waitmsg("Please key in product");
			return 0;
		}
	}
	// Duplicate product code checking

	if (!isblank(df->dat[DP_PT2]))
	{
		for (i = 0; i < 8; i++)
		{
			x = i * 4 + DP_PT1;
			if (!(isblank(df->dat[x + 4])))
			{
				for (j = i + 1; j < 9; j++)
				{
					z = j * 4 + DP_PT1;
					if (!(isblank(df->dat[z])))
					{
						if (!(strcmp(df->dat[x], df->dat[z])))
						{
							waitmsg("Duplicate Product Code");
							return 0;
						} // strcmp
					}	  // isblank z
				}		  // for j loop
			}			  // isblank x
		}				  // for i loop
	}					  // is blank
	if (!stricmp(df->name, "wrt") || !stricmp(df->name, "rt"))
	{
		if (isblank(df->dat[DP_DPTICKET]))
		{
			AfxMessageBox("Despatch Ticket cannot empty");
			return 0;
		}
		rec = qd(df->dat[DP_DPTICKET], &dp);
		if (rec <= 0)
		{
			AfxMessageBox("Despatch Ticket No. Not Existed");
			return 0;
		}
	}
	if (!CheckVarianceWeightParty(df))
		return 0;
	FindProductCategorySerialNo(df);
	SetTicketAmount(df);

	if (strcmp(df->m_strTable, "dp") == 0)
	{
		SetModifyMark(DP_SGROSS, DP_MODIFY, df);
		SetModifyMark(DP_STARE, DP_MODIFY, df);
	}
	return 1;
}

void init_dp(DB *df)
{
	//    df->e->prec		 = PrintSlipDs;//pslip2;
	df->e->save = dp_save;
	df->e->tell = tell_dp;
	df->e->initial = Dp_initial;
	df->e->rep = ReplaceDpQuarryGrid;
	df->e->GridTitle = SetDpQuarryGridTitle;
	df->e->Delete = Delete_ds;
}

void init_wdp(DB *df)
{
	df->e->tell = tell_dp;
	df->e->save = dp_save;
	df->e->rep = ReplaceDpQuarryGrid;
	df->e->GridTitle = SetDpQuarryGridTitle;
	df->e->prec = unprint;
	df->e->initial = Dp_initial; // 08/07/2004 MR LOW Sungai Besi & Bukit Kemuning
}

void CDp::OnPartyGrading()
{
	if (IsSaveConditionOk(df) == 0)
		return;

	CPartyGrading t1(IDD_PARTY_GRADING);

	t1.FirstField = DP_PT11;
	t1.LastField = DP_WT18;
	t1.df = df;
	t1.DoModal();
}

extern void Despatch_DO(DB *df);
void CDp::OnDo()
{
	// ooi
	ReplaceRecord();

	char key[8000];
	strcpy(key, df->dat[1]);

	///

	long xrec = dp_do.Search(df->dat[1], DPDO_DP, TRUE);
	if (xrec > 0)
	{
		app->UpdateTableLastTransaction(&dp_do, FALSE);
		app->UpdateBrowseLastTransaction(&dp_do, FALSE);
	}

	Despatch_DO(&dp_do);
	ShowInformation();
}

void CDp::OnSetpaydate()
{
}
