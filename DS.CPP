// Ds.cpp : implementation file
//

#include <afxctl.h>
#include "stdafx.h"
#include "menu.h"
#include "Ds.h"
#include "mgrid.h"
#include "openrs.h"
#include "PartyGrading.h"
#include "SETPAYDATE.h"
#include "SETPAYDATEU.h"
// #include "Scan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//20230615
// double Get_Adjustment(double subtotal)
// {
// 	char szTmp[1000];
// 	dtoa(subtotal, szTmp, 12, 0);
// 	return atof(szTmp);
// }
//END
/////////////////////////////////////////////////////////////////////////////
// CDs dialog
BOOL isCheck = TRUE;

CDs::CDs(UINT IDD, DB *pdf, int Idx, CWnd *pParent /*=NULL*/)
	: CStaff(IDD, pdf, Idx, pParent)
{
	//{{AFX_DATA_INIT(CDs)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDs::DoDataExchange(CDataExchange *pDX)
{
	CStaff::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDs)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

int CheckUserLevel(DB *df)
{
	int level = 7;

	if (df->val(UR_LEVEL) < level)
	{
		return 0;
	}
	return 1;
}

int Int(double amt)
{
	char txt[100];
	amt = int(amt + 0.00001);
	dtoa(amt, txt, 15, 0);
	trim(txt);

	return atoi(txt);
}

void FindProductCategorySerialNo(DB *df)
{
	return; //
	if (!isblank(df->dat[DP_SERIAL]))
		return;
	long rec = qd(df->dat[DP_PT1], &prodtype);
	if (rec == 0)
	{
		waitmsg("Please key in product");
		return;
	}

	rec = qd(prodtype.dat[PT_PTGROUP], &ptgroup);
	if (rec == 0)
	{
		waitmsg("Please key in product group.");
		return;
	}
	rec = qd(ptgroup.dat[PTGROUP_CATEGORY], &category);
	if (rec == 0)
	{
		waitmsg("Please key in product category at Product Group.");
		return;
	}

	char szHeader[100];
	strcpy(szHeader, category.dat[3]);
	CString str;
	str.Format("select max(serial) from [%s] where serial like '%s%%'",
			   df->m_strTable, szHeader);
	OpenRs p1(str);
	CString serial = p1.dbText(1);
	if (serial.GetLength() <= 2)
	{
		serial.Format("%s%5.5ld", szHeader, 1);
	}
	else
	{
		char szTmp[100];
		strcpy(szTmp, serial);
		int no = atoi(&szTmp[2]) + 1;
		serial.Format("%s%5.5ld", szHeader, no);
	}
	lset(df->dat[DP_SERIAL], serial);
}

void getmet(long *m, DB *df)
{
	/*
	m[1] = atol(df->dat[DP_SGROSS]);
	m[2] = atol(df->dat[DP_STARE]);
	m[5] = atol(df->dat[DP_WASTAGE]);
	m[4] = labs(m[2] - m[1]) - m[5];
	m[6] = atol(df->dat[DP_SW]);
	m[7] = m[4] - m[6];
	*/
	long weight1, weight2;

	weight1 = atol(df->dat[DP_SGROSS]);
	weight2 = atol(df->dat[DP_STARE]);
	if (weight1 > weight2)
	{
		m[2] = weight1;
		m[5] = weight2;
	}
	else
	{
		m[5] = weight2;
		m[2] = weight1;
	}
	weight1 = atol(df->dat[DP_CGROSS]);
	weight2 = atol(df->dat[DP_CTARE]);
	if (weight1 > weight2)
	{
		m[1] = weight1;
		m[4] = weight2;
	}
	else
	{
		m[4] = weight2;
		m[1] = weight1;
	}

	m[7] = atol(df->dat[DP_CWASTAGE]);
	m[8] = atol(df->dat[DP_WASTAGE]);

	m[10] = labs(m[4] - m[1]) - m[7];
	m[11] = labs(m[5] - m[2]) - m[8];

	m[3] = m[2] - m[1];
	m[6] = m[5] - m[4];
	m[9] = m[8] - m[7];
	m[12] = m[11] - m[10];
}

void PrintAddress(FILE *ptr)
{ /*
   //  if (company_no==12){  // TO hard code for FMT 20091113 CCWONG
		 fprintf(ptr,"\n");
		 fprintf(ptr,"P");
	 //	fprintf(ptr,"(FORMERLY KNOWN AS HENG HUP PAPER RECYCLE SDN. BHD.)");
		 fprintf(ptr,G_company_add1);
		 fprintf(ptr,"\n"+G_company_add2);
		 fprintf(ptr,"\n"+G_company_add3);
		 fprintf(ptr,"\n");

		 return;

 //    }
 */
	fprintf(ptr, "P");
	int i;

	// The reg no put at title on 19/07/2005 fprintf(ptr,"\nCompany Register No %s",profile.dat[1]);
	for (i = PRO_ADD1; i <= PRO_ADD3; i++)
	{
		fprintf(ptr, "\n%s", stkpf.dat[i]);
	}
	fprintf(ptr, "\n");
}

void PrintTicket(FILE *ptr, char *title)
{
	fprintf(ptr, "2C!M");
	fprintf(ptr, "ME%sF", title);
	PrintAddress(ptr);
}

BOOL CDs::IsCannotEdit()
{
	char txt[100];
	long xrec = 0;
	if (!stricmp(df->name, "ds"))
	{
		xrec = apoinv_wb.Search(df->dat[1], VIWB_DO, TRUE);
		strcpy(txt, apoinv_wb.dat[1]);
	}
	else
	{

		xrec = aroinv_wb.Search(df->dat[1], VIWB_DO, TRUE);
		strcpy(txt, aroinv_wb.dat[1]);
	}
	if (xrec)
	{
		CString str;
		str.Format("Ticket is issued to Invoice:%s\nNot allow modify.", trim(txt));
		AfxMessageBox(str);
		return TRUE;
	}
	return FALSE;
}

void CDs::after_save(int ExistFlag)
{
	/*
	BOOL flagNew;
	if (ExistFlag==NONEXIST){
		flagNew = TRUE;
	}
	else{
		flagNew = FALSE;
	}*/
	if (strcmp(df->dat[DP_PTYPE], "01   ") == 0 || strcmp(df->dat[DP_PTYPE], "CASH ") == 0)
	{ // cash
	  // DoInvoiceExportAndPayment(df); //take out auto invoice and payment 06/10/18
	}
}

void CDs::ShowInformation()
{

	double total, wt, qty, tqty, tpcent, upc;
	long net;
	int i, x, upori;
	char text[20];
	char str[20];

	CStaff::ShowInformation();
	getmet(m, df);
	net = m[11];
	ltot(m[3], 6, str);
	SetDlgItemText(IDC_GROSSDIFF, putnum(str));
	ltot(m[6], 6, str);
	SetDlgItemText(IDC_TAREDIFF, putnum(str));
	ltot(m[12], 6, str);
	SetDlgItemText(IDC_VAR, putnum(str));
	ltot(m[10], 6, str);
	SetDlgItemText(IDC_PARTYNETT, putnum(str));
	ltot(m[11], 6, str);
	SetDlgItemText(IDC_NETT, putnum(str));
	/*
	getmet(m,df);
	net = m[4];
	dtoa(net,text,9,0);
	SetDlgItemText(IDC_NETT,putnum(text));
	 dtoa(double(m[7]),text,6,0);
	 SetDlgItemText(IDC_VAR,putnum(text));
	*/
	total = qty = tqty = tpcent = 0;
	for (i = 0; i < 8; i++)
	{
		x = i * 4 + DP_PT1;
		qty = df->val(x + 1) * net;
		tpcent += df->val(x + 1);
		tqty += df->val(x + 2);
		wt = df->val(x + 2) * df->val(x + 3);

		//20230621
		if(!strncmp(df->dat[DP_PTYPE],"CASH", 4)){
				//amount = Int(amount);		// book-> if senghiap  hide
			// wt = Get_Adjustment(wt);
		}
		//END

		dtodb(wt, df, DP_AMT1 + i);
		dtoa(atof(df->dat[DP_AMT1 + i]), text, 9, 2);
		SetDlgItemText(IDC_TW1 + i, putnum(text));
		// SetDlgItemText(IDC_TW1+i,df->dat[DP_AMT1+i]);
		// total += wt;
		// dtoa(wt,text,9,2);
		// SetDlgItemText(IDC_TW1+i,putnum(text));
		
		total += df->val(DP_AMT1 + i);

		upori = i + DP_ORI_WT1;
		upc = fabs(df->val(x + 3) - df->val(upori));
		if (upc > 0)
		{
			SetDlgItemText(IDC_UPC1 + i, "*");
		}
		else
		{
			SetDlgItemText(IDC_UPC1 + i, "");
		}
	}
	dtoa(tpcent, text, 5, 0);
	SetDlgItemText(IDC_TPCENT, putnum(text));
	dtoa(tqty, text, 5, 0);
	SetDlgItemText(IDC_TQTY, putnum(text));
	dtoa(total, text, 9, 2);
	SetDlgItemText(IDC_TOTAL, putnum(text));

	long Srec = paymentdate.Search(df->dat[1], 1, TRUE);
	if (Srec)
	{
		SetDlgItemText(IDC_PAYDATE, mdate(paymentdate.dat[2]));
	}
	else
	{
		SetDlgItemText(IDC_PAYDATE, "");
	}
	/*
		 long Xrec = arriveddate.Search(df->dat[1],1,TRUE);
		 if(Xrec){
			 SetDlgItemText(IDC_ARRDATE,mdate(arriveddate.dat[2]));
		 }
		 else {
			 SetDlgItemText(IDC_ARRDATE,"");
		 }
	*/
	if (IsManual(df))
	{
		SetDlgItemText(IDC_MANUAL, "*");
	}
	else
		SetDlgItemText(IDC_MANUAL, " ");

	/*	 if (!isblank(df->dat[DP_TNG])){
			 qd(df->dat[DP_TNG],&tng);
			 SetDlgItemText(IDC_MODI,tng.dat[2]);
			 SetDlgItemText(IDC_LMODI,"MODIFIED BY TAG USER");
		 }
		 else {
			 SetDlgItemText(IDC_MODI,"");
			 SetDlgItemText(IDC_LMODI,"");
		 }
	*/
}

int TotalDspercent(DB *df)
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

int TotalDspercentParty(DB *df)
{
	return atoi(df->dat[DP_PCENT11]) +
		   atoi(df->dat[DP_PCENT12]) +
		   atoi(df->dat[DP_PCENT13]) +
		   atoi(df->dat[DP_PCENT14]) +
		   atoi(df->dat[DP_PCENT15]) +
		   atoi(df->dat[DP_PCENT16]) +
		   atoi(df->dat[DP_PCENT17]) +
		   atoi(df->dat[DP_PCENT18]);
}

void ClearContent(DB *df)
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
int OnChangePriceField(int no)
{
	int nofld = no;
	int nofldori = 0;
	nofld = nofld / 4;
	if (nofld < 19)
	{
		nofldori = DP_ORI_WT1;
		for (int i = 0; i < 8; i++)
		{
			if (nofld - i == 4)
			{
				nofldori += i;
				break;
			}
		}
	}
	else
	{
		nofldori = DP_ORI_WT11;
		for (int i = 0; i < 8; i++)
		{
			if (nofld - i == 19)
			{
				nofldori += i;
				break;
			}
		}
	}
	return nofldori;
}

/*
double ChangePrice(CString m_accode,CString m_fld)
{
	double price,cprice,sprice;
	price=cprice = sprice =0;

	CString str,pgrade;
	str.Format("select [PRODGRADE] from [spitem] where [pt]='%s' and [accode]='%s'",
		m_fld,m_accode);
	pgrade = SqlText(str);
	if (isblank(pgrade)){
		char szTmp[40];
		strcpy(szTmp,m_accode);
		qd(szTmp,&accode);
		pgrade = accode.dat[ACCODE_PRODGRADE];
	}
	str.Format("Select COST,SALES from PTSUB where PART='%s' and PRODGRADE='%s'",
		m_fld,pgrade);
	cprice = SqlValue(str,1);
	sprice = SqlValue(str,2);
	price = (Purchase_Flag) ? cprice : sprice;
	return price;
}
*/

void SetTicketAmount(DB *df)
{
	int i, x, y;
	double total = 0; //20230615
	for (i = 0; i < 8; i++)
	{
		x = i * 4 + DP_PT1;
		y = i * 4 + DP_PT11;
		if (isblank(df->dat[x]) && isblank(df->dat[y]))
			continue;
		double amount = df->val(x + 2) * df->val(x + 3);
		double amtpty = df->val(y + 2) * df->val(y + 3);
		/*
		char ptypex[100];
		strcpy(ptypex,df->dat[DP_PTYPE]);trim(ptypex);
		if(!strcmp(ptypex,"01")){  // cash
			amount = Int(amount);
			amtpty = Int(amtpty);
		}
		*/
		//20230615
		if(!strncmp(df->dat[DP_PTYPE],"CASH", 4)){
				//amount = Int(amount);		// book-> if senghiap  hide
			// amount = Get_Adjustment(amount);
		}
		//END
		dtodb(amount, df, DP_AMT1 + i);
		dtodb(amtpty, df, DP_AMT11 + i);
		total += amount; //20230615
	}
	// if(!strcmp(df->name, "ds") || !strcmp(df->name, "dp"))
	// {
		dtodb(total, df, GetDbFieldNo(df, "SUBTOTAL")); //20230615
	// }
	
}

int tell_ds(UINT nChar, int &flag, int &fld, DB *df, int sq)
{

	long net, percent, Srec;
	double qty, final;
	CString str;
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
	case DP_PO:
		if (sq)
		{
			UpdatePoInformation(df, &po);
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

		if ((percent = TotalDspercent(df)) > 100)
		{
			AfxMessageBox("Percentage over 100", MB_OK, 0);
			ltodb(100 - (percent - atol(df->dat[fld])), df, fld);
			CopyBufferToControl(fld, df);
			flag = DO;
			return TAB_REFRESH_ALL;
		}
		if (sq)
		{
			SetTicketAmount(df);
		}

		/*if (isblank(df->dat[fld]))
		   ltodb((100-percent),df,fld);
		else
			ltodb(atol(df->dat[fld]),df,fld);
			CopyBufferToControl(fld,df);
*/
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
		if (sq)
		{
			SetTicketAmount(df);
		}
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
		if (sq)
		{
			SetTicketAmount(df);
		}
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
				AfxMessageBox("Supplier Code is empty, please key in supplier code");
				return TAB_REFRESH_ALL;
			}
			else
			{
				Srec = qd(df->dat[DP_ACCODE], &accode);
				if (Srec < 1)
				{
					str.Format("Supplier Code %s not regiestered yet", df->dat[DP_ACCODE]);
					AfxMessageBox(str);
					return TAB_REFRESH_ALL;
				}
			}
		}
		//		if (isblank(df->dat[fld]) || sq){
		Srec = qd(df->dat[DP_PO], &po);
		if (Srec)
		{
			CString strSql, strWhere;
			strSql.Format("SELECT dbo.PT.PART, dbo.PT.NAME, dbo.PT.SNAME, dbo.PT.PTGROUP, dbo.PT.UNIT, dbo.PT.COST, dbo.POITEM.PRICE AS SALES, dbo.PT.REMARKS FROM dbo.POITEM LEFT OUTER JOIN dbo.PT ON dbo.POITEM.PT = dbo.PT.PART ");
			strWhere.Format("WHERE POITEM.PO = '%s' order by PT.PART", df->dat[DP_PO]);
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
		//		}
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
		if (sq)
		{
			SetTicketAmount(df);
		}
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
		if (sq)
		{
			SetTicketAmount(df);
		}
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
		if (sq)
		{
			SetTicketAmount(df);
		}
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
			//    dtodb(ChangePrice(df->dat[DP_ACCODE],df->dat[fld]),df,fld+3);
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

	ClearContent(df);
	return TAB_REFRESH_ALL;
}

int Ds_initial(DB *df, int *fld) // initial setting
{
	if (UserLevel() < 999)
	{
		fld[0] = DP_MODIFY;
		fld[1] = 0;
	}
	// ScanApproveFlag = TRUE;
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
	/*
		if (!ScanApproveFlag){
			fld[0] = DP_DATE;
			fld[1] = DP_STARE;
			fld[2] = DP_SGROSS;
			fld[3] = DP_TIMEIN;
			fld[4] = DP_TIMEOUT;
			fld[5] = 0;

		}
		*/
	return 1;
}

int Wds_initial(DB *df, int *fld) // initial setting
{
	/*	if (!ScanApproveFlag){
			fld[0] = DP_DATE;
			fld[1] = DP_STARE;
			fld[2] = DP_SGROSS;
			fld[3] = DP_TIMEIN;
			fld[4] = DP_TIMEOUT;
			fld[5] = 0;
		}
		else{
			fld[0] = DP_STARE;
			fld[1] = DP_TIMEOUT;
			fld[2] = 0;
		} */
	fld[0] = DP_STARE;
	fld[1] = DP_TIMEOUT;
	fld[2] = 0;
	return 1;
}
void ReplaceQuarryGrid(long rec, CMSFlexGrid *grid, DB *df, int IdxItem)
{
	MGRID g(grid, df, rec);
	g.SetText(df, NULL, DP_TICKET, DP_DATE, DP_VEHICLE, DP_ACCODE, DP_ACCODE | 0x100,
			  DP_PT1, DP_PT1 | 0x100, -1);
	getmet(m, df);

	char text[20];
	ltot(m[11], 6, text);
	g.SetText(text);
}

void SetQuarryGridTitle(CMSFlexGrid *grid, DB *df, int IdxItem)
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

int ds_save(DB *df, int flag)
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
	// getmet(m,df);
	if (m[11] == 0)
	{
		waitmsg("zero weight");
		return 0;
	}
	// edited SGROSS can not > old value  // ccwong 20110415 AS PER HENGHUP
	/* 23/7/2015 Ding
	double old_sgross = atof(df->bak[DP_SGROSS]);
	double newgross =df->val(DP_SGROSS);
	if(df->val(DP_SGROSS)>old_sgross){
		CString str;
		str.Format("Edited 1st weight %6.0f can not > old value %6.0f",df->val(DP_SGROSS),old_sgross);
		AfxMessageBox(str);
		return 0;
	}
	*/
	// Check Scrap Grading Note
	/* 20/4/2015
	rec = dt.Search(df->dat[1],1);
	if (rec<=0){
		if (isblank(df->dat[DP_SCRAP])){
			waitmsg("Scrap Grading Note Not Found");
		}
	}
	*/
	/* long percent = TotalDspercent(df);
	  if(!((percent == 100) || (percent == 0 ))) {
		  waitmsg("percentage error");
		  return 0;
	  }
	  */
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

	if (!CheckVarianceWeightParty(df))
		return 0;
	if (!CheckVarianceNetWeight(df))
		return 0;
	FindProductCategorySerialNo(df);
	SetTicketAmount(df);

	if (ScanApproveFlag)
	{
		lset(df->dat[DP_TNG], CardID);
	}
	CardID = "";
	ScanApproveFlag = FALSE;
	(df->e->initial)(df, df->ReadOnlyArray);

	if (strcmp(df->m_strTable, "ds") == 0)
	{
		SetModifyMark(DP_SGROSS, DP_MODIFY, df);
		SetModifyMark(DP_STARE, DP_MODIFY, df);
	}
	return 1;
}

void In_wds(DB *df, int fld)
{
	CString str;
	if (fld == DP_SCALE)
	{
		str.Format("SELECT * FROM SCALE WHERE TICKET NOT IN (SELECT SCALE FROM DS WHERE SCALE IS NOT NULL) AND ACCODE='%s'", df->dat[DP_ACCODE]);
		scale.m_strCmdText = str;
		scale.ReOpen(str);
	}
}

BOOL IsTicketAlreadyBilled(DB *df)
{

	long xrec = 0;
	if (stricmp(df->name, "dp") == 0 || stricmp(df->name, "rt") == 0 || stricmp(df->name, "scaledw") == 0)
	{
		xrec = aroinv_wb.Search(df->dat[1], VIWB_DO, TRUE);
	}
	else
	{
		xrec = apoinv_wb.Search(df->dat[1], VIWB_DO, TRUE);
	}
	if (xrec)
	{
		return TRUE;
	}
	return FALSE;
}

int Delete_ds(long irec, DB *df, int IdxItem)
{
	if (IsTicketAlreadyBilled(df))
	{
		CString str;
		str.Format("Error Message\n\nCannot delete Ticket No: %s\nbecause this ticket already Invoiced.\n", df->dat[1]);
		AfxMessageBox(str);
		return 0;
	}
	else
	{
		return Delete(irec, df, IdxItem);
	}
}

void init_ds(DB *df)
{
	// df->e->prec		 = pslip1;
	//	df->e->prec      = PrintSlipDs;
	df->e->save = ds_save;
	df->e->tell = tell_ds;
	df->e->initial = Ds_initial;
	df->e->rep = ReplaceQuarryGrid;
	df->e->GridTitle = SetQuarryGridTitle;
	df->e->In = In_wds;
	df->e->Delete = Delete_ds;
}

void init_wds(DB *df)
{
	df->e->tell = tell_ds;
	df->e->rep = ReplaceQuarryGrid;
	df->e->save = ds_save;
	df->e->GridTitle = SetQuarryGridTitle;
	df->e->prec = unprint;
	df->e->initial = Wds_initial; // 08/07/2004 MR LOW Sungai Besi & Bukit Kemuning  // ccwong 20110125
	df->e->In = In_wds;
}

BEGIN_MESSAGE_MAP(CDs, CStaff)
//{{AFX_MSG_MAP(CDs)
ON_BN_CLICKED(IDC_PARTY_GRADING, OnPartyGrading)
ON_BN_CLICKED(IDC_SETPAYDATE, OnSetpaydate)
ON_BN_CLICKED(IDC_UNSET, OnUnset)
ON_BN_CLICKED(IDC_DO, OnDo)
ON_BN_CLICKED(IDC_SETARRDATE, OnSetarrdate)
ON_BN_CLICKED(IDC_UNSETARR, OnUnsetarr)
ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDs message handlers
void CDs::SetAttribute(int flag, int fld)
{
	if (Purchase_Flag)
	{
		// GetDlgItem(IDC_DO)->ShowWindow(SW_HIDE);
		if (flag)
		{
			//			if(strcmp(df->m_strTable,"wds")==0) GetDlgItem(IDC_SCAN)->ShowWindow(SW_HIDE);
			//			else GetDlgItem(IDC_SCAN)->ShowWindow(SW_HIDE);
		}
		else
		{
			//			GetDlgItem(IDC_SCAN)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		//		GetDlgItem(IDC_SCAN)->ShowWindow(SW_HIDE);
		if (flag)
		{
			// GetDlgItem(IDC_DO)->ShowWindow(SW_SHOW);
		}
		else
		{
			// GetDlgItem(IDC_DO)->ShowWindow(SW_HIDE);
		}
	}

	if (strcmp(df->m_strTable, "dp") != 0)
	{
		//		GetDlgItem(IDC_SETARRDATE)->ShowWindow(SW_HIDE);
		//		GetDlgItem(IDC_UNSETARR)->ShowWindow(SW_HIDE);
	}

	CStaff::SetAttribute(flag, fld);
}

void CDs::OnPartyGrading()
{
	if (SaveFlag == CANNOTSAVE)
	{
		waitmsg("View Only\r\nAny Changes to Ticket Information need to edit the ticket First");
		long rec = df->Search(df->dat[1], 1, TRUE);
	}
	isCheck = FALSE;

	CPartyGrading t1(IDD_PARTY_GRADING);

	t1.FirstField = DP_PT11;
	t1.LastField = DP_WT18;
	t1.df = df;
	t1.askSave = TRUE;
	t1.DoModal();

	isCheck = TRUE;
}

int CheckVarianceNetWeight(DB *df)
{
	if (!isCheck)
	{
		return 1;
	}

	getmet(m, df);
	double diff = abs(m[10] - m[11]);
	double variance = m[10] * stkpf.val(PRO_VARIANCE) / 100;
	if ((!isblank(df->dat[DP_CGROSS]) || !isblank(df->dat[DP_CTARE]) || !isblank(df->dat[DP_CWASTAGE])) && diff > variance)
	{
		CString str;
		str.Format("Reason: Net Weight Variance Overflow.\r\nMinimum User");
		/*
		int flag = UserPasswordLoginAccepted(df,5,str);
		if(flag==0){
			return 0;
		}
		*/
		return 1;
	}
	return 1;
}

void CDs::OnSetpaydate()
{
	/*	long Srec = paymentdate.Search(df->dat[1],1,TRUE);
		if (Srec>0){
			CScan p1;
			p1.df = &tng;
			int flag = p1.DoModal();
			if(flag==IDCANCEL) {
				return;
			}
		}*/
	CSETPAYDATEU d;
	d.b_ticket = df->dat[1];
	d.main = df;
	d.updatepay = &paymentdate;
	// d.DoModal();
	int flag = d.DoModal();
	if (flag == IDCANCEL)
	{
		return;
	}
	if (Purchase_Flag)
	{
		DoAutoInvoice(df, &apinv_wb, &apoinv_wb);
	}
	else
	{
		DoAutoInvoice(df, &arinv_wb, &aroinv_wb);
	}
	ShowInformation();
}

void CDs::OnUnset()
{
	long Srec = paymentdate.Search(df->dat[1], 1, TRUE);
	if (Srec > 0)
	{
		/*	CScan p1;
			p1.df = &tng;
			int flag = p1.DoModal();
			int flag
			if(flag==IDCANCEL) {
				return;

			}*/
		paymentdate.Delete(df->dat[1]);
		AfxMessageBox("UNSET COMPLETED");
	}
	else
	{
		AfxMessageBox("RECORD DOES NOT EXIST");
	}
	ShowInformation();
}
#include "DpDo.h"
extern void Despatch_DO(DB *df);
void CDs::OnDo()
{
	qd(df->dat[1], &dp);
	char key[100];
	strcpy(key, df->dat[1]);

	///
	dpKey = 1;
	long xrec = dp_do.Search(df->dat[1], DPDO_DP, TRUE);

	if (xrec > 0)
	{
		app->UpdateTableLastTransaction(&dp_do, FALSE);
		app->UpdateBrowseLastTransaction(&dp_do, FALSE);
		dpKey = 2;
	}
	CString str, result;
	str.Format("Select * FROM DP_DO where DP_DO='%s' AND DP='%s'", dp.dat[DP_DO], dp.dat[1]);
	result = SqlText(str, 1);
	if (isblank(result))
	{
		long Srec = dp_do.Search(dp.dat[DP_DO], 1);
		if (Srec)
		{
			CString str;
			str.Format("D/O No %s existed in system. Please revise.", dp.dat[DP_DO]);
			AfxMessageBox(str);
			return;
		}
	}
	CString dpticket;
	dpticket = dp.dat[1];
	dpticket.TrimRight();
	// Despatch_DO(&dp_do);
	CDpDo p1(dp_do.ID, &dp_do, dp_do.f[1].idx & 0x01);
	p1.OneRecordOnly = TRUE;
	p1.ticketfrdp = dpticket;
	p1.DoModal();
	ShowInformation();
}

void CDs::OnSetarrdate()
{
	// TODO: Add your control notification handler code here
	CSETPAYDATEU d;
	d.b_ticket = df->dat[1];
	d.main = df;
	d.updatepay = &arriveddate;
	d.ispaydate = FALSE;
	d.DoModal();
	ShowInformation();
}

void CDs::OnUnsetarr()
{
	long Srec = arriveddate.Search(df->dat[1], 1, TRUE);
	if (Srec > 0)
	{
		arriveddate.Delete(df->dat[1]);
		AfxMessageBox("UNSET COMPLETED");
	}
	else
	{
		AfxMessageBox("RECORD DOES NOT EXIST");
	}
	ShowInformation();
}

void CDs::OnDestroy()
{
	CStaff::OnDestroy();
}

BOOL IsManual(DB *df)
{
	if (strcmp(df->m_strTable, "wds") == 0 || strcmp(df->m_strTable, "wdp") == 0)
		return FALSE; // elvern

	if (isblank(df->dat[DP_W1]) || isblank(df->dat[DP_W2]))
	{
		return TRUE;
	}
	return FALSE;
}
/*
void CDs::OnScan()
{
	CScan p1;
	p1.df = &tng;
	int flag = p1.DoModal();
	if(flag==IDCANCEL) {
		return;
	}
	memset(df->ReadOnlyArray,0,sizeof(int) * (TotalField+1) );
	(df->e->initial)(df,df->ReadOnlyArray);
	ChangeToEditMode(2);
	CurrentField = GetFirstAvailableField();
	GotoEdit( CurrentField);

}
*/
int TotalDsQty(DB *df)
{
	return atoi(df->dat[DP_QTY1]) +
		   atoi(df->dat[DP_QTY2]) +
		   atoi(df->dat[DP_QTY3]) +
		   atoi(df->dat[DP_QTY4]) +
		   atoi(df->dat[DP_QTY5]) +
		   atoi(df->dat[DP_QTY6]) +
		   atoi(df->dat[DP_QTY7]) +
		   atoi(df->dat[DP_QTY8]);
}
