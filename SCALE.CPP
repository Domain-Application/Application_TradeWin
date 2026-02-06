// Scale.cpp : implementation file
//

#include "stdafx.h"
#include "menu.h"
#include "Scale.h"
#include "SETPAYDATEU.h"
/*
#define _WIN32_WINNT 0x0501  // Windows XP and later
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")
#include <afxinet.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
*/
#include "MyJpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CalAmt(DB *df);
/////////////////////////////////////////////////////////////////////////////
// CScale dialog

CScale::CScale(UINT IDD, DB *pdf, int Idx, CWnd *pParent /*=NULL*/)
	: CParent(IDD, pdf, Idx, pParent)
{
	//{{AFX_DATA_INIT(CScale)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/*
#include <afxwin.h>
#include <windows.h>
#include <wincrypt.h>
/*

// Function to load RSA public key from CString
RSA* LoadPublicKey(const CString& publicKey) {
    BIO* bio = BIO_new_mem_buf((void*)(LPCTSTR)publicKey, -1);
    if (!bio) {
        AfxMessageBox(_T("BIO creation failed!"));
        return NULL;
    }

    RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);

    if (!rsa) {
        AfxMessageBox(_T("Failed to load public key!"));
        return NULL;
    }

    return rsa;
}

// Function to encode binary data as Base64
CString Base64Encode(const BYTE* buffer, int length) {
    BIO* bio = BIO_new(BIO_s_mem());
    BIO* b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // No newline characters in output
    BIO_write(bio, buffer, length);
    BIO_flush(bio);

    char* encodedData;
    long encodedLength = BIO_get_mem_data(bio, &encodedData);

    CString result(encodedData, encodedLength);
    BIO_free_all(bio);
    return result;
}

// Encrypt using RSA public key and return Base64 output
CString EncryptRSA(const CString& publicKey, const CString& plainText) {
    RSA* rsa = LoadPublicKey(publicKey);
    if (!rsa) return _T("Key error");

    int keySize = RSA_size(rsa);
    BYTE* encryptedData = new BYTE[keySize];

    int result = RSA_public_encrypt(plainText.GetLength(), 
                                    (BYTE*)(LPCTSTR)plainText, 
                                    encryptedData, rsa, 
                                    RSA_PKCS1_OAEP_PADDING);

    RSA_free(rsa);

    if (result == -1) {
        delete[] encryptedData;
        return _T("");
    }

    // Convert encrypted binary to Base64
    CString base64Encoded = Base64Encode(encryptedData, result);
    delete[] encryptedData;

    return base64Encoded;
}

// Function to manually format a JSON-like CString with multiple key-value pairs from an array
CString CreateJsonAndHash(CString keyValuePairs[], int size)
{
    // Ensure the array has an even number of elements (key-value pairs)
    if (size % 2 != 0) {
        return _T("Error: Key-value array size must be even.");
    }

    // Manually format JSON string
    CString jsonString = _T("{");

    for (int i = 0; i < size; i += 2)
    {
        CString key = keyValuePairs[i];
        CString value = keyValuePairs[i + 1];
        CString pair;
        pair.Format(_T("\"%s\":\"%s\""), key, value);

        jsonString += pair;
        if (i < size - 2)
        {
            jsonString += _T(",");
        }
    }

    jsonString += _T("}");

	CString publicKey = _T("-----BEGIN PUBLIC KEY-----\n"
                           "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAjFULJ7hV0d+I2Ec9LqAI\n"
                           "Ukjsf6m/CaSADr6KGe2fB0u2k7eb0NZWjRJA7AE2vgMJdUTubKz+qcbLRbEbXFIP\n"
                           "oiYFKwkX8pFSNS18j9gEdJNDRq5963hzWG7ATSEqFQmMPvyV2BvKtUXZyZnGdXyj\n"
                           "13I3ROqeZyPAPtgw5ZXIWU53Jy8fSXtH4Eqx8E4RR2m8tl0abIjcvDsdxwdLhBx5\n"
                           "PRkhmKMUeNEns30HmMZwIjUJYtrB71AVOmOqmpd0glhCtfl1aYFo6OpAS0n3iK3N\n"
                           "oV8+hD8K39rnIHRmeyJCkOCFDfZilMxIYp1pu8CZlexSnLA8aMCMuet1O5gIVLWM\n"
                           "JwIDAQAB\n"
                           "-----END PUBLIC KEY-----\n");
    // Compute SHA-256 hash of the JSON string

    return EncryptRSA(publicKey, jsonString);
}

void SaveQRcode(DB *df, CString data, int jpgkey, CString fieldName) // TODO: make it dynamic on function call?
{
	CString cmdstr,cmdstr2;
	CString strBat,str,qrstr;
	char szJpg[1024],szJpg2[1024],szItem[1024],szDel[1024],szDel2[1024];
	char szFileName[1024],szFileName2[1024];
	char szPath[1024],szPath2[1024];
	
	UINT flag = GetCurrentDirectory(1000,szJpg);	
	strcpy(szFileName,szJpg);
	strcpy(szPath,szJpg);
	
    strcpy(szDel,szJpg);
	strcpy(szDel2,szJpg);
	strcat(szDel,"\\qrcode.jpg");
	
    DeleteFile(szDel);
	DeleteFile(szDel2);
	
	strcpy(szItem,data); 
	trim(szItem);
	cmdstr.Format("\\qrcode.exe -o qrcode.jpg -s 2 -l H \"%s\"",szItem);
	strcat(szJpg, cmdstr);
	
    WinExec(szJpg,SW_HIDE);
	
	cmdstr2.Format("\\qrcode.jpg");
    strcat(szFileName, cmdstr2);

	Sleep(1000);
	
	CMyJpg p;
	
	int xflag = p.textcopy("scale_qrcode", fieldName, jpgkey , szFileName, 'I');
	if(xflag==TRUE){
	}
}

void ExecuteUrlRequest(const CString& url)
{
    CInternetSession session;
    CString jsonResponse;
    
    try
    {
        CHttpFile* pFile = (CHttpFile*)session.OpenURL(url);
        if (pFile)
        {
            char buffer[8192];
            UINT bytesRead = 0;

            while ((bytesRead = pFile->Read(buffer, sizeof(buffer) - 1)) > 0)
            {
                buffer[bytesRead] = 0; // Null-terminate
                jsonResponse += buffer;
            }

            pFile->Close();
            delete pFile;
        }
    }
    catch (CInternetException* e)
    {
        TCHAR szErr[1024];
        e->GetErrorMessage(szErr, 1024);
        AfxMessageBox(_T("Error: ") + CString(szErr));
        e->Delete();
        return;
    }

	CString result = _T("Result");
	CString reason = _T("Reason");
    int start = jsonResponse.Find(_T("isSuccess"));
    int end = jsonResponse.Find(_T("showResult")) - 2;

    if (start != -1 && end != -1 && end > start)
    {
        start += 11; // Move past "<title>"
        result = jsonResponse.Mid(start, end - start);
    }

	int reasonStart = jsonResponse.Find(_T("showResult")) + 10;
	int reasonEnd = jsonResponse.Find(_T("}")) -1 ;

	if (start != -1 && end != -1 && end > start)
    {
        reason = jsonResponse.Mid(reasonStart, reasonEnd - reasonStart);
    }

	CString jsonReturn;
	jsonReturn.Format("Result: %s | Reason: %s", result, reason );

    // Display the response
    AfxMessageBox(jsonReturn);
}
*/

void CScale::OnPrint()
{
	/*
	CString strSQL, sub_db;
	if (!strcmp(df->name, "SCALE"))
	{
		// strSQL.Format("Update %s set SUBTOTAL = (SELECT SUM(AMT) FROM %s where TICKET = '%s') where ticket = '%s'", df->name, sub->name, df->dat[1], df->dat[1]);
		strSQL.Format("Update %s set SUBTOTAL = '200' where ticket = '%s'", df->name, sub->name, df->dat[1], df->dat[1]);
		RunSql(strSQL);
	}
	long rec = qd(df->dat[GetDbFieldNo(df, "ACCODE")], &debtor);
	CString str_debtor = df->dat[GetDbFieldNo(df, "ACCODE")];
	CString walletID = debtor.dat[GetDbFieldNo(&debtor, "WalletID")];
	CString IC = df->dat[GetDbFieldNo(df, "IC")];
	CString PhoneNumber = debtor.dat[GetDbFieldNo(&debtor, "TEL1")];
	CString AccessToken = debtor.dat[GetDbFieldNo(&debtor, "TNGACCESSTOKEN")];\
	int amount = df->val(GetDbFieldNo(df, "SUBTOTAL")) * 100;
	CString str_amount;
	str_debtor.TrimLeft();
	str_debtor.TrimRight();
	walletID.TrimLeft();
	walletID.TrimRight();
	IC.TrimLeft();
	IC.TrimRight();
	PhoneNumber.TrimLeft();
	PhoneNumber.TrimRight();
	AccessToken.TrimLeft();
	AccessToken.TrimRight();
	str_amount.Format("%d", amount);
	str_amount.TrimLeft();
	str_amount.TrimRight();

	CString keyValuePairs[] = {
    _T("Ticket"),        df->dat[1],
    _T("walletID"),      walletID,
    // _T("IC"),           IC,
    _T("Amount"),       str_amount,
    _T("PhoneNumber"),  PhoneNumber,
    _T("AccessToken"),  AccessToken
	};

    int arraySize = sizeof(keyValuePairs) / sizeof(keyValuePairs[0]);

    CString hashedJson = CreateJsonAndHash(keyValuePairs, arraySize);
	strSQL.Format("Update scale set Encrypted_Data = '%s' where Ticket = '%s'", hashedJson, df->dat[1]);
	RunSql(strSQL);
	int jpgkey = df->val(GetDbFieldNo(df, "jpgkey"));
	if(isblank(df->dat[GetDbFieldNo(df, "jpgkey")]))
	{
		lsetblank(&scale_qrcode);
		scale_qrcode.AddNew();
		jpgkey = scale_qrcode.val(GetDbFieldNo(&scale_qrcode, "PKKEY"));
		strSQL.Format("UPDATE scale set jpgkey = %d where ticket = '%s'", jpgkey, df->dat[1]);
		RunSql(strSQL);
	}
	CString final_output, fieldName;
	final_output.Format("https://tradewintng.domainconnect.us.kg/Api/GetAuthorizationStep?CODE=%s", str_debtor);
	fieldName = _T("Auth_QRCode");
	SaveQRcode(df, final_output, jpgkey, fieldName);

	final_output.Format("https://tradewintng.domainconnect.us.kg/Api/MakeBill?data=%s", hashedJson);
	// final_output.Format("https://192.168.1.233:3080/Api/MakeBill?data=%s", hashedJson);
	fieldName = _T("Transfer_QRCode");
	SaveQRcode(df, final_output, jpgkey, fieldName);

	//strSQL.Format("SELECT CASE WHEN TNGACCESSTOKEN IS NULL then 1 else 0 end as flag FROM tradewin_acc.dbo.debtor where accode = '%s'", str_debtor);
	//if(SqlInt(strSQL) == 0)
	//{
		ExecuteUrlRequest(final_output);
	//}
	*/
	
	
	CParent::OnPrint();
}

void CScale::DoDataExchange(CDataExchange *pDX)
{
	CParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScale)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScale, CParent)
//{{AFX_MSG_MAP(CScale)
ON_WM_DESTROY()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_SETPAYDATE, OnSetpaydate)
ON_BN_CLICKED(IDC_UNSET, OnUnset)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScale message handlers
BOOL CScale::IsCannotEdit()
{
	char txt[100];
	long xrec = 0;
	if (!stricmp(df->name, "scale"))
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

void CScale::SetAttribute(int flag, int fld)
{
	if (flag)
	{
		if (atoi(user.dat[UR_LEVEL]) >= 99)
		{
			GetDlgItem(IDC_SETPAYDATE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_UNSET)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_SETPAYDATE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_UNSET)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		GetDlgItem(IDC_SETPAYDATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_UNSET)->ShowWindow(SW_HIDE);
	}
	CParent::SetAttribute(flag, fld);
}

void CScale::after_save(int ExistFlag)
{
	/*
	BOOL flagNew;
	if (ExistFlag==NONEXIST){
		flagNew = TRUE;
	}
	else{
		flagNew = FALSE;
	}*/
	CString strSQL, sub_db;
	if (!strcmp(df->name, "SCALE"))
	{
		strSQL.Format("Update %s set SUBTOTAL = (SELECT SUM(AMT) FROM %s where TICKET = '%s') where ticket = '%s'", df->name, sub->name, df->dat[1], df->dat[1]);
		RunSql(strSQL);
	}

	ShowInformation();
}

// 20230717
double Get_Adjustment(double subtotal)
{
	char szTmp[1000];
	dtoa(subtotal, szTmp, 12, 0);
	return atof(szTmp);
}

CString Get_Rounding_Adjustment(double subtotal)
{
	CString result;
	result.Format("%.2f", Get_Adjustment(subtotal) - subtotal);
	return result;
}

CString Get_Final_Amount(double subtotal)
{
	CString result;
	result.Format("%.2f", Get_Adjustment(subtotal));
	return result;
}
// END

void CScale::ShowInformation()
{
	subtotal_amt = subtotal_rounding_adj = subtotal_final_amt = 0; // 20230717
	CParent::ShowInformation();
	long Srec = paymentdate.Search(df->dat[1], 1, TRUE);
	if (Srec)
	{
		SetDlgItemText(IDC_PAYDATE, mdate(paymentdate.dat[2]));
	}
	else
	{
		SetDlgItemText(IDC_PAYDATE, "");
	}
	CString str_final_amt;
	GetDlgItemText(IDC_TOTAL6, str_final_amt);
	// 20230717
	if (strcmp(df->dat[SCA_PTYPE], "01   ") == 0 || strcmp(df->dat[SCA_PTYPE], "CASH ") == 0)
	{
		SetDlgItemText(IDC_ROUNDING_ADJ, Get_Rounding_Adjustment(subtotal_amt));
		SetDlgItemText(IDC_Final_Amt, Get_Final_Amount(subtotal_amt));
	}
	else
	{
		SetDlgItemText(IDC_ROUNDING_ADJ, "0.00");
		SetDlgItemText(IDC_Final_Amt, str_final_amt);
	}
	// END
}

void CScale::OnReqitemInitDialog(CDialog *p, int status)
{
	double upc = 0;
	int fld0 = GetDbFieldNo(&scalesub, "PKKEY");

	long irec = scalesub.Search(sub->dat[SCASB_PKKEY], fld0, TRUE);
	if (irec > 0)
	{
		upc = fabs(sub->val(SCASB_UPRICE) - sub->val(SCASB_UPRICE_ORI));
		if (upc > 0)
		{
			p->SetDlgItemText(IDC_UPC, "*");
		}
		else
		{
			p->SetDlgItemText(IDC_UPC, "");
		}
	}
}

BOOL CScale::OnInitDialog()
{
	CParent::OnInitDialog();
	if (Purchase_Flag)
	{
		SetDlgItemText(IDC_STATIC1, "RN NO");
	}
	else
	{
		SetDlgItemText(IDC_STATIC1, "DN NO");
		SetDlgItemText(IDC_STATIC6, "CUSTOMER");
	}

	// TODO: Add extra initialization here

	return TRUE; // return TRUE unless you set the focus to a control
				 // EXCEPTION: OCX Property Pages should return FALSE
}

void CScale::OnDestroy()
{
	CParent::OnDestroy();

	// TODO: Add your message handler code here
}

void CScale::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CParent::OnTimer(nIDEvent);
}

void CScale::ShowDetail()
{
	CParent::ShowDetail();
}

void CScale::OnDetail()
{
	CParent::OnDetail();
}

void CScale::ShowInitFrame(CMSFlexGrid *grid, DB *sub)
{
	fg.Initial(grid, sub, 2, grid_ratio);
	fg.SetTitle("No", 400);
	fg.SetTitle("Product Code", 1500);
	fg.SetTitle("Product Description", 4500);
	fg.SetTitle("Quantity", 1000, 7);
	fg.SetTitle("Unit Price", 1000, 7);
	fg.SetTitle("*", 300, 4);
	fg.SetTitle("Amount", 1300, 7);
	// //20230717
	// if(!strcmp(df->name, "SCALE"))
	// {
	// 	fg.SetTitle("Rounding Adj",1300,7);
	// 	fg.SetTitle("Final Amount",1300,7);
	// }
	// //END
}

void CScale::ReplaceSubGrid(long rec, long irec, CMSFlexGrid *m_grid, DB *sub, int IdxItem)
{
	fg.show(rec);
	qd(sub->dat[SCASB_PT], &prodtype);
	fg.SetText(sub, NULL, 3, -1);
	fg.SetText(&prodtype, PT_NAME, -1);
	fg.SetText(sub, 4, 5, -1);
	double upc = 0;
	upc = fabs(sub->val(SCASB_UPRICE) - sub->val(SCASB_UPRICE_ORI));
	if (upc > 0)
	{
		fg.SetText("*");
	}
	else
	{
		fg.SetText(" ");
	}
	fg.SetText(sub, 6, -1);
	subtotal_amt += sub->val(6);
}

void AdminDbScale(DB *df, DB *sub)
{
	if (df->ID == 0)
	{
		AfxMessageBox("Database not open");
		return;
	}
	CScale p1(df->ID, df, df->f[1].idx & 0x01);
	if (!stricmp(df->name, "scale"))
	{
		tado(&paymentdate, "paymentdate");
	}
	else
	{
		tado(&paymentdate, "paymentdated");
	}
	// tado(&paymentdate,"paymentdate");
	p1.sub = sub;
	// p1.NonDupArray[0] = SCASB_PT;
	p1.DoModal();
	dclose(&paymentdate);
}

int scale_initial(DB *df, int *fld) // initial setting
{
	if (UserLevel() < 9)
	{
		fld[0] = SCA_DATE;
		fld[1] = 0;
	}
	return 1;
}

int tell_scale(UINT nChar, int &flag, int &fld, DB *df, int sq)
{
	long net, percent;
	double qty;
	qty = net = percent = 0;

	switch (fld)
	{
		/*
	case SCA_ACCODE:
		if (sq){
			lset(df->dat[SCA_BASIS],accode.dat[ACCODE_BASIS]);
		}
		break; */
	case SCA_VEHICLE:
		if (strcmp(df->dat[fld], df->bak[fld]))
		{
			qd(df->dat[SCA_VEHICLE], &lorry);
			lset(df->dat[SCA_IC], lorry.dat[LO_DRIVER1]);
			lset(df->dat[SCA_ACCODE], lorry.dat[LO_DRIVER2]);
			lset(df->dat[SCA_TRANSP], lorry.dat[LO_TRANSP]);
			qd(df->dat[SCA_IC], &driver);
			lset(df->dat[SCA_DRIVER], driver.dat[2]);
			CopyBufferToControl(SCA_IC, df);
			CopyBufferToControl(SCA_ACCODE, df);
			CopyBufferToControl(SCA_TRANSP, df);
			CopyBufferToControl(SCA_DRIVER, df);
			return TAB_REFRESH_ALL;
		}
		break;
	case SCA_IC:
		if (strcmp(df->dat[fld], df->bak[fld]))
		{
			qd(df->dat[SCA_IC], &driver);
			lset(df->dat[SCA_DRIVER], driver.dat[2]);
			CopyBufferToControl(SCA_DRIVER, df);
			return TAB_REFRESH_ALL;
		}
		break;
		/*
		case SCA_PTYPE:
			{
			CString strSQL;
			if(strcmp(df->dat[SCA_PTYPE],"01   ")==0 || strcmp(df->dat[SCA_PTYPE],"CASH ")==0 )
			{  // cash
				//DoInvoiceExportAndPayment(df); //take out auto invoice and payment 06/10/18
				strSQL.Format("Update %s set AMT = ROUND(AMT) where TICKET = '%s'", sub->name, df->dat[1]);
				RunSql(strSQL);
			}
			strSQL.Format("Update %s set SUBTOTAL = (SELECT SUM(AMT) FROM %s where TICKET = '%s') where ticket = '%s'", df->name, sub->name, df->dat[1], df->dat[1]);
			RunSql(strSQL);
			break;
			}
		*/
		/*
		case SCA_BASIS:
			if(sq){
				if (strncmp(df->dat[fld],"O",1))
					if (strncmp(df->dat[fld],"P",1)){
						waitmsg("Basis - O(Own) or P(Party)");
						lset(df->dat[fld],"O");
						return 2;
					}
			}
			break; */
	}
	return 2;
}

int scale_save(DB *df, int flag)
{
	if (isblank(df->dat[SCA_VEHICLE]))
	{
		waitmsg("Please key in vehicle no");
		return 0;
	}
	return 1;
}

int start_scale(DB *df)
{
	dbasedate(df->dat[SCA_DATE]);
	_strtime(df->dat[SCA_TIMEIN]);
	return 1;
}

/*
//20230622
void Print_Scale(long irec,FILE *ptr,DB *df)
{
	df->m_PrintOneFormOnly = TRUE;
	df->m_PreviewCrystalReport = TRUE;
	CString rpt_name;
	rpt_name.Format("%s.rpt", df->name);
	PrintOneForm(df, rpt_name);
	df->IncPrinterCounter();       //inc print counter
}
//END
*/

void init_scale(DB *df)
{
	df->e->start = start_scale;
	df->e->tell = tell_scale;
	df->e->save = scale_save;
	// df->e->prec      = PrintSlipScale;
	df->e->initial = scale_initial;
	df->e->Delete = Delete_ds;
	/*
	if(!strcmp(df->name, "SCALE"))
	{
	df->e->prec      = Print_Scale; //20230622
	}
	*/
}

// 20230623 revert back for tradewin
void ScalesubChangePrice(DB *df, int fld)
{
	double price, cprice, sprice;
	price = cprice = sprice = 0;

	CString str, pgrade;
	// str.Format("select [PRODGRADE] from [spitem] where [pt]='%s' and [accode]='%s'",
	//     df->dat[fld],scale.dat[SCA_ACCODE]);
	// pgrade = SqlText(str);
	// if (isblank(pgrade)){
	// 	qd(scale.dat[SCA_ACCODE],&accode);
	// 	pgrade = accode.dat[ACCODE_PRODGRADE];
	// }
	// str.Format("Select COST,SALES from PTSUB where PART='%s' and PRODGRADE='%s'",
	// 	df->dat[fld],pgrade);
	str.Format("Select COST,SALES from PTSUB where PART='%s'", df->dat[fld]);
	cprice = SqlValue(str, 1);
	sprice = SqlValue(str, 2);
	price = (Purchase_Flag) ? cprice : sprice;
	dtodb(price, df, SCASB_UPRICE);
}
// END

int tell_scalesub(UINT nChar, int &flag, int &fld, DB *df, int sq)
{
	switch (fld)
	{
	case SCASB_PT:
		if (sq)
		{
			ScalesubChangePrice(df, fld); // 20230623 revert back for tradewin
			// dtodb(ChangePrice(scale.dat[SCA_ACCODE],df->dat[fld]),df,SCASB_UPRICE);
			// dtodb(ChangePrice(scale.dat[SCA_ACCODE],df->dat[fld]),df,SCASB_UPRICE_ORI);
		}
		break;
	case SCASB_AMT:
		flag = DO;
		break;
	// 20230621
	case SCASB_BIN:
		if (sq)
		{
			if (df->val(SCASB_QTY) < df->val(SCASB_BIN))
			{
				AfxMessageBox("Bin cannot more than quantity");
				lset(df->dat[SCASB_BIN], "");
				return 0;
			}
		}
		break;
	case SCASB_WASTAGE:
		if (sq)
		{
			if (df->val(SCASB_QTY) < df->val(SCASB_WASTAGE))
			{
				AfxMessageBox("Wastage cannot more than quantity");
				lset(df->dat[SCASB_WASTAGE], "");
				return 0;
			}
		}
		break;
		// END
	}
	CalAmt(df);
	return 2;
}

int scalesub_save(DB *df, int flag)
{
	if (isblank(df->dat[SCASB_PT]))
	{
		waitmsg("Please select Product");
		return 0;
	}
	if (df->val(SCASB_QTY) < 0 || isblank(df->dat[SCASB_QTY]))
	{
		waitmsg("Quantity is Zero.\r\nNot allowed to save.");
		return 0;
	}
	if (df->val(SCASB_UPRICE) < 0 || isblank(df->dat[SCASB_UPRICE]))
	{
		waitmsg("Unit Price is Zero.\r\nNot allowed to save.");
		return 0;
	}
	// TODO: when amount is negative -> prevent save (Not exist at 201906 system)
	CalAmt(df);
	return 1;
}

void init_scalesub(DB *df)
{
	df->e->tell = tell_scalesub;
	df->e->save = scalesub_save;
}

void CalAmt(DB *df)
{
	if (!isblank(df->dat[SCASB_QTY]) && !isblank(df->dat[SCASB_UPRICE]))
	{
		// double amt = (df->val(SCASB_QTY)*df->val(SCASB_UPRICE)); 20230621
		double amt = (df->val(SCASB_QTY) - df->val(SCASB_WASTAGE) - df->val(SCASB_BIN)) * df->val(SCASB_UPRICE);
		dtodb(amt, df, SCASB_AMT);
	}
}

void CScale::OnSetpaydate()
{
	CSETPAYDATEU d;
	d.b_ticket = df->dat[1];
	d.main = df;
	d.updatepay = &paymentdate;
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

void CScale::OnUnset()
{
	long Srec = paymentdate.Search(df->dat[1], 1, TRUE);
	if (Srec > 0)
	{
		paymentdate.Delete(df->dat[1]);
		AfxMessageBox("UNSET COMPLETED");
	}
	else
	{
		AfxMessageBox("RECORD DOES NOT EXIST");
	}
	ShowInformation();
}
