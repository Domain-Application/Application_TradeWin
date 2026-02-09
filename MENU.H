// menu.h : main header file for the MENU application
//

#if !defined(AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_)
#define AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES

extern DB scale_qrcode;
extern void SetTransactionType(DB *df, DB *accode);
extern DB  malaysia_state,malaysia_city,postcode;
extern DB classification;
//20230615
double Get_Adjustment(double subtotal);
//END

// Version SST
extern char txtSST_StartDate[20];
//////
extern DB  vehicle;
extern int Delete_ds(long irec,DB *df,int IdxItem);
extern void DoAutoInvoice(DB *df,DB *invmain,DB *invsub);
extern void DoInvoiceExportAndPayment(DB *df);
extern DB  stkpf;
extern DB  scalesub,scaledwsub;
extern DB  scale,scaledw;
extern DB  ptsub;
extern DB  pr_new,wpr_new,dp_rt;
extern DB  rt,wrt;
extern DB  incentivesub;
extern DB  incentive;
extern DB  dp_do;
extern DB  accodehq,salesman,vtype;
#define  RCA 14
#define  TL 11
#define  TL_NO (TL*4)
#include "myapp.h"

extern int company_no;
extern CString G_company_add1,G_company_add3;
extern CString G_company_add2,CardID;
extern CString m_FirstTicket;

extern DB *ttt,logo,po,poitem,so,soitem,spitem,dt,dtitem;
extern DB arinv_wb,aroinv_wb,apinv_wb,apoinv_wb,status;
extern DB adj,adjitem,reason,currency;
extern DB paymentdate;
extern int   PViewFlag,FacileFlag;
extern BOOL ZeroWeight1,ScanApproveFlag,IsWBZeroWt;

extern long dpKey;
extern BOOL isCheck;
extern DB logerr,taxcode;

extern void FindPercen(DB *df);
extern void FindTaxCode(DB *df, BOOL Purchase_Flag);
extern int Int(double amt);
extern int CheckNetWt(DB *df);
extern int CheckUserLevel(DB *df);
extern int CheckVarianceNetWeight(DB *df);
extern void SetSubTotal(DB *df);
extern void init_pr(DB *df);
extern void init_wpr(DB *df);
extern void getprmet(long *m,DB *df);
extern void init_dp_do(DB *df);
extern void SetTicketAmount(DB *df);
extern int UserPasswordLoginAccepted(DB *df,int level,CString str);
extern BOOL IsManual(DB *df);
extern void SaveWeightMaxDataWB(DB *df,long confirmWeight,long weight2=0);

extern void DirectDeliveryPricingUpdate();
extern void PrintPublicTicket(long irec,FILE *ptr,DB *df);
extern int tell_dp(UINT nChar,int &flag,int &fld,DB *df,int sq);
extern void init_wpr(DB *df);
extern void PublicWeighingProcess(DB *main,DB *ds);
extern void AdminDbPtentry(DB *df,DB *sub);
extern void AdminDbPTYPE(DB *df);
extern void AdminDbScale(DB *df,DB *sub);
extern void ViewStockFlow(DB *df);
extern void AdjustmentEntry();
extern void init_adj(DB *df);
extern void init_adjitem(DB *df);
extern int CheckVarianceWeightParty(DB *df);
extern int TotalDspercentParty(DB *df);
extern long VarianceWeightParty(DB *df);
extern BOOL Purchase_Flag;
extern void FindProductCategorySerialNo(DB *df);
extern void init_arinv_wb(DB *df);
extern void init_aroinv_wb(DB *df);
extern void SalesInvoiceWb();
extern void PurchaseInvoiceWb();
extern void CleanDatabase();
extern void DirectDeliveryTicket();
extern double ChangePrice(CString m_accode,CString m_fld);
extern int OnChangePriceField(int no);

extern void init_po(DB *df);
extern void init_poitem(DB *df);
extern void init_dt(DB *df);
extern void init_dtitem(DB *df);
extern void CrystalReport(DB *scr,DB *rep,int *field,int printerName);
extern void SalesOrderData();
extern void PurchaseOrderData();
extern void AccodeMaintenance();
extern void CheckUserRight(DB *user,int key);
extern void ToTicketPrinter(DB *rep,int printerName);
extern int FacileFlag;
extern long GwstWeight,ActualWeight;
extern void VarianceDetailReport(DB *df);
extern void Weighing_Despatch(DB *main,DB *dp);
extern void FileChangeReport(DB *df);
extern void WeighingProcess(DB *main,DB *ds);
extern void PrintTicketSlip(char *rep,FILE *ptr,DB *df);
extern void GST_Rate_Setting(void);
extern int facile(void);
extern long weight1,weight2;
extern void MonthlyProductDetailReport(DB *df,char *rep);
extern char mesure[20];
extern int CheckVarianceWeight(DB *df);
extern long VarianceWeight(DB *df);
extern void Pcode(long irec,FILE *ptr,DB *df);
extern DB   accode,prodtype,driver,lorry,wdp,wds,ds,category,unit,report;
extern DB   two,one,login,user,daily,dp,profile,gst,transp,ptype;
extern DB   country,grade,actype,ptgroup;
extern char *DateStrMsg,*ToStrMsg,*DsMainScr[],*DpMainScr[],*dsview[],*dpview[],*fmismenu[3];
extern char *DsMainScr[25],*wdsmenu[3];
extern char *MainScr[],*FirstScr[],*tranmenu[],*RcvMainScr[],*dsort_scr[];
extern char *VehicleMenu[],*FirstD[],*m00[],*nsave[],*cyview[3],*tmenu[3],*fmmenu[3];
extern char *connect_msg,*company,*sfmenu[],*win[],*wdsmenu[],*wdpmenu[];
extern char *dsavemenu,*specmenu[3],TypeVar[],*VehicleMenu[],*Hot01,*daily_scr[];
extern char *editcust[19],*custmenu[6], *packemenu[3], *Typemenu[3],*Broview[3];
extern char *h00,*hfirst,*drivermenu[6],*hfirst,*FirstD[3],*mview[3];
extern char *prodmenu[3],*firstmenu[3],*ErrorMenu[],*dtrmenu[3],*snmenu[3],*PmfScr[];
extern char *rview[3],*sfview[3],*pfview[3],*agmenu[3],*pl_view[3];
extern char *setprod[], *Resetmenu[], *setdtr[3] , *Ct_menu[3],*setct[3];
extern char ShortFigure[20],*ConSumScr[],*cons_view[],*lyview[3];
extern char timebuf[10],mesure[20],*l_view[],*DpMainScr[25],*wdpmenu[3],*nsave[3];
extern int  ContractFlag,EmulationFlag,PsFlag;
extern int  FirstFd_row[32],FirstFd_row2[39],Fd_row[39];
extern int DoPCR(DB *df,DB *pc,int flag);
extern int FirstFarrow[], MFirstFarrow[], Farrow[];
extern int FirstFd_row[],Fd_row[];
extern int VehicleNoEntry(int fld,DB *df);
extern int Wmode,Parting,Omode;
extern int _flag,_offset;
extern int dspec(DB *);
extern int get_avery(char *);
extern int key_ddws(void);
extern int key_detail(char *tl);
extern int key_mdsh(char *);
extern int key_tcv(char *);
extern int keydaily(char *);
extern int keymonthly(char *);
extern int keypcr1(DB *df,char *tl,int (*tell)(KB ,int *,int *,int ,int ,int ,DB *,char *));
extern int keystat,tickethandle;
extern int porla_no;
extern int wait_zero(void);
extern long   LorryRec;
extern long SetCtrBalance(DB *rep,long weight);
extern long m[15];
extern long part1data(DB *scr,DB *df,DB *rep,int field,void(*save)(long rec,DB *df,DB *rep));
extern long put_dc_pk_balance(DB *rep,DB *pf);
extern long put_pcbr_balance(DB *rep,DB *pf);
extern void AcTotalWeight(DB *df,char *rep);
extern void AutoCreateCollectionOrder(void);
extern void CheckVehicleNo(char *);
extern void DPProductSummary(DB *,char *);
extern void DailyDPSummary(DB *df);
extern void DailyDespatchSummary(DB *);
extern void DailyDespatchWeighingSummary(void);
extern void DailyDpWS(void);
extern void DailyODPSummary(DB *df);
extern void DailyPurchasingComposite(void);
extern void DespatchDs(DB *df);
extern void DespatchPack(DB *);
extern void DetailReport(DB *df,char *rep);
extern void FileChangeReport(DB *df);
extern void DispDpFirst(int x,int y);
extern void DispDsFirst(int x,int y);
extern void Fill_Sum(DB *rep);
extern void Lorry_Data(void);
extern void MonthlyDespatchDetailReport(DB *);
extern void MonthlyDespatchShortageReport(DB *);
extern void MonthlyDespatchSpecReport(DB *);
extern void MonthlyReceiptDetailReport(void);
extern void MonthlyReceiptShortageReport(void);
extern void MonthlyReceiptSpecReport(void);
extern void Pcust(long ,FILE *,DB *);
extern void ProductDespatchedPosition(void);
extern void ProfileSetup(void);
extern void ScbRSave(DB *MainData,DB *report);
extern void VaranceSummary(DB *df,char *rep);
extern void accode_maintenance(void);
extern void despatch_weight_checking(DB *);
extern void disp_weight(int x,int y,char *mesure);
extern void display_no_vehicle_message(void);
extern void driver_code(void);
extern void getmet(long *m,DB *df);
extern void init_transp(DB *df);
extern void init_unit(DB *df);
extern void init_category(DB *df);
extern void init_accode(DB *df);
extern void init_driver(DB *df);
extern void init_lorry(DB *df);
extern void init_country(DB *df);
extern void init_grade(DB *df);
extern void init_pt(DB *df);
extern void init_ptentry(DB *df);
extern void init_ptype(DB *df);
extern void init_gst(DB *df);
extern void init_ds(DB *df);
extern void init_dp(DB *df);
extern void init_wds(DB *df);
extern void init_scale(DB *df);
extern void init_scalesub(DB *df);
extern void init_wdp(DB *df);
extern void init_stdlist(DB *df);
extern void msumcust_report(DB *df);
extern void msummary_report(DB *df);
extern void pfdb(long irec,FILE *ptr,DB *df);
extern void pfmi(long irec,FILE *ptr,DB *df);
extern void pfrb(long irec,FILE *ptr,DB *df);
extern void pmis(long irec,FILE *ptr,DB *df);
extern void prec(long irec,FILE *ptr,DB *df);
extern void prodtype_code(void);
extern void production_entry(void);
extern void pslip1(long irec,FILE *ptr,DB *df);
extern void pslip2(long irec,FILE *ptr,DB *df);
extern void PrintSlipDs(long irec,FILE *ptr,DB *df);
extern void PrintSlipScale(long irec,FILE *ptr,DB *df);
extern void PrintLDoTitle(DB *scr,DB *df);
extern void purge_data(DB *,int);
extern void receive_weight_checking(DB *);
extern void receiving_modify(DB *,char *);
extern void receiving_parting(DB *,char *);
extern void rep_pcr1(DB *df,DB *pc,int flag);
extern void rsumcust_report(DB *);
extern void rsummary_report(DB *);
extern void sumcust_report(DB *);
extern void summary_dp(DB *,char *);
extern void summary_report(DB *);
extern void tellscr(long Srec,double *total,DB *df);
extern void tellsq(long Srec,double *total,DB *df);
extern void weighting_dp(DB *);
extern void weighting_ds(DB *);
extern void StdPrintFormat(long irec,FILE *ptr,DB *df);
extern void CheckUserRight(DB *user,int key);
extern void PurchaseOrderData();
extern void PrintAddress(FILE *ptr);
extern void init_accodes(DB *df);
extern void ViewViolation();
extern int TotalDsQty(DB *df);
extern int curweightZeroCount;
/*
enum accode {ACCODE_CODENO=1,ACCODE_CUSTOMER,ACCODE_ADD1,ACCODE_ADD2,ACCODE_ADD3,
            ACCODE_TEL,ACCODE_FAX,ACCODE_CONTACT,ACCODE_COUNTRY,ACCODE_REMARKS,ACCODE_ACTYPE,ACCODE_HQSALES,ACCODE_HQPURCHASE,ACCODE_INCENTIVE,ACCODE_BASIS,ACCODE_PRODGRADE,
			ACCODE_GST_NO,ACCODE_TAXCODE};
*/
enum accode {ACCODE_CODENO=1,ACCODE_CUSTOMER,ACCODE_ADD1,ACCODE_ADD2,ACCODE_ADD3,
ACCODE_TEL,ACCODE_FAX,ACCODE_CONTACT,ACCODE_COUNTRY,ACCODE_REMARKS,
			ACCODE_GST_NO,ACCODE_TAXCODE};
#define ACCODE_AREA ACCODE_COUNTRY 

enum accodehq {HQ_CODENO=1,HQ_NAME,HQ_REMARK1,HQ_REMARK2};

enum pr_new {PR_TICKET=1,PR_SERIAL,PR_DATE,PR_VEHICLE,PR_DRIVER,PR_ACCODE,PR_PRODUCT,PR_REMARKS,PR_MODIFY,PR_TIMEIN,PR_TIMEOUT,
PR_SGROSS,PR_STARE,PR_AMOUNT,
PR_USER1,PR_USER2,PR_W1,PR_W2,PR_Z1,PR_Z2};

#define PR_LORRY PR_VEHICLE
#define PR_PGROSS PR_SGROSS

enum dp {DP_TICKET=1,DP_SERIAL,DP_DATE,DP_VEHICLE,DP_PO,DP_UNIT,
         DP_ACCODE,DP_PTYPE,DP_SUBCON,
         DP_TRANSP,DP_DO,DP_SCRAP,DP_CONTRACT,DP_IC,DP_DRIVER,
         DP_PT1,DP_PCENT1,DP_QTY1,DP_WT1,DP_PT2,DP_PCENT2,DP_QTY2,DP_WT2,
         DP_PT3,DP_PCENT3,DP_QTY3,DP_WT3,DP_PT4,DP_PCENT4,DP_QTY4,DP_WT4,
         DP_PT5,DP_PCENT5,DP_QTY5,DP_WT5,DP_PT6,DP_PCENT6,DP_QTY6,DP_WT6,
         DP_PT7,DP_PCENT7,DP_QTY7,DP_WT7,DP_PT8,DP_PCENT8,DP_QTY8,DP_WT8,
         DP_REMARKS,DP_MODIFY,DP_TIMEIN,DP_TIMEOUT,
         DP_CGROSS,DP_CTARE,DP_CWASTAGE,
         DP_SGROSS,DP_STARE,DP_WASTAGE,
         DP_USER1,DP_USER2,DP_SRNNO,DP_SRNDATE,DP_BASIS,DP_NOTSHOW,DP_GRADE,DP_SCALE,DP_SALESMAN,DP_VTYPE,DP_OTHQTY,
		 DP_W1,DP_W2,DP_Z1,DP_Z2,
         DP_PT11,DP_PCENT11,DP_QTY11,DP_WT11,DP_PT12,DP_PCENT12,DP_QTY12,DP_WT12,
         DP_PT13,DP_PCENT13,DP_QTY13,DP_WT13,DP_PT14,DP_PCENT14,DP_QTY14,DP_WT14,
         DP_PT15,DP_PCENT15,DP_QTY15,DP_WT15,DP_PT16,DP_PCENT16,DP_QTY16,DP_WT16,
         DP_PT17,DP_PCENT17,DP_QTY17,DP_WT17,DP_PT18,DP_PCENT18,DP_QTY18,DP_WT18,
		 DP_AMT1,DP_AMT2,DP_AMT3,DP_AMT4,DP_AMT5,DP_AMT6,DP_AMT7,DP_AMT8,
		 DP_AMT11,DP_AMT12,DP_AMT13,DP_AMT14,DP_AMT15,DP_AMT16,DP_AMT17,DP_AMT18,
		 DP_PUSER,DP_TNG,
		 DP_ORI_WT1,DP_ORI_WT2,DP_ORI_WT3,DP_ORI_WT4,DP_ORI_WT5,DP_ORI_WT6,DP_ORI_WT7,DP_ORI_WT8,
		 DP_ORI_WT11,DP_ORI_WT12,DP_ORI_WT13,DP_ORI_WT14,DP_ORI_WT15,DP_ORI_WT16,DP_ORI_WT17,DP_ORI_WT18,
		 DP_REC_STATUS,DP_NEWUSER,DP_NEWTIME,DP_MODIFYUSER,DP_MODIFYTIME,DP_PRINT_COUNTER};
#define DP_LORRY DP_VEHICLE
#define DP_PGROSS DP_SGROSS
#define DP_DPTICKET DP_SCALE


enum ptgroup {PTGROUP_CODE=1,PTGROUP_NAME,PTGROUP_CATEGORY};
enum monthly {S_DATE1=1,S_DATE2,S_CODE,S_PRODUCT,S_PAGE};
enum daily {Da_D1=1,Da_D2,Da_ACCODE,Da_VEHICLE,Da_IC,Da_PT,Da_TRANSP,Da_CONTRACT,Da_PTYPE,Da_COUNTRY,Da_PAGE};
//enum fchange {FC_D1=1,FC_D2,FC_USER,FC_SMF,FC_PAGE};
enum lorry {LO_VEHICLE=1,LO_DRIVER1,LO_DRIVER2,LO_TRANSP,LO_TARE,LO_REMARKS};
enum pt {PT_PART=1,PT_NAME,PT_SNAME,PT_PTGROUP,PT_UNIT,PT_COST,PT_SALES,PT_REMARKS,PT_SALESGL,PT_PURCHASEGL,PT_LASTUPDATE,PT_C_ADJ,PT_S_ADJ};
enum transp {Trn_TR_NO=1,Trn_NAME,Trn_SNAME,Trn_TR_TEL,Trn_TR_FAX,
             Trn_TR_ADD1,Trn_TR_ADD2,Trn_TR_ADD3,Trn_CONTACT,Trn_COMPUTE,
             Trn_REM};
//enum profile {Pro_ADD1=1,Pro_ADD2,Pro_ADD3,Pro_ADD4,Pro_ZERO,Pro_STABLE};
enum logo {Logo_CODE=1,Logo_NAME,Logo_ADD1,Logo_ADD2,Logo_ADD3,Logo_ADD4,
           Logo_DATE,Logo_DATE1,Logo_DATE2,Logo_TIME1,Logo_TIME2,
           Logo_SELECT1,Logo_SELECT2,Logo_SELECT3,Logo_RTITLE,Logo_GROUP,
           Logo_USER,Logo_USERNAME,Logo_PROGRAM_ID,Logo_ACCODE,Logo_BRANCH};
enum spitem {Sp_CUSTOMER=1,Sp_SQNO,Sp_ITEM,Sp_PRODGRADE,Sp_BUYERPTNAME};
enum po {PO_PO=1,PO_DATE,PO_ETA,PO_SUPPLIER,PO_TERMS,PO_REMARKS};
enum poitem  {POD_REFNO=1,POD_SQNO,POD_ITEM,POD_NAME,POD_CATEGORY,POD_QTY,POD_PRICE,POD_AMOUNT};
enum dt {Dt_TICKET=1,Dt_DATE,Dt_VEHICLE,Dt_PO,Dt_SUPPLIER,Dt_SO,Dt_BUYER,
             Dt_IC,Dt_DRIVER,Dt_REMARKS,Dt_DO,Dt_TIMEIN,Dt_TIMEOUT,Dt_PW1,Dt_PW2,Dt_PW3,Dt_SW1,Dt_SW2,Dt_SW3};
enum dtitem  {Dti_REFNO=1,Dti_SQNO,
              Dti_ITEM,Dti_QTY1,Dti_PRICE1,Dti_AMOUNT1,
              Dti_ITEM2,Dti_QTY2,Dti_PRICE2,Dti_AMOUNT2};
enum arinv_wb {IVWB_INVOICE=1,IVWB_DATE,IVWB_ACCODE,IVWB_TERMS,IVWB_YOUR_REP,IVWB_OUR_REF,IVWB_REMARKS,IVWB_PAYDATE,IVWB_STATUS,
				IVWB_CURRENCY,IVWB_RATE,IVWB_UPLOAD_ONHOLD,
				IVWB_AMOUNT,IVWB_TAXCODE,IVWB_INCLUDE,IVWB_PERCEN,IVWB_TAXAMT,IVWB_SUBTOTAL,IVWB_PO};
enum aroinv_wb {VIWB_INVOICE=1,VIWB_SQNO,VIWB_DO,VIWB_QTY,VIWB_AMOUNT,VIWB_SCALE, VIWB_SCALE_AMT};

enum adj     {Aj_REFNO=1,Aj_DATE,Aj_REASON,Aj_REMARKS};
enum adjitem {ADJ_REFNO=1,ADJ_SQNO,ADJ_ITEM,ADJ_QTY,
ADJ_PRICE,ADJ_AMOUNT,ADJ_PKKEY};

enum setprice {ML_REFNO=1,ML_DATE1,ML_DATE2,ML_ACCODETYPE,ML_ACCODE,ML_PT,ML_PRICE,ML_UPDATE};

//Account System
#define STATEMENT_BALANCE 13
#define RCA 14
#define TL 11
#define TL_NO (TL*4)

extern void ExportPurchaseInvoiceTransactions();
extern  void ExportInvoiceTransactions();
extern int Listening();
extern void TransferToOldAccountingSystem();
extern void Receipt_Entry();
extern void SupplierCreditNote();
extern void AgeingStatement();
extern void init_payment(DB *df);
extern void init_purchase(DB *df);
extern double date_tally(char *text,DB *df,int IdxItem,int field,char *datefld,char *date);
extern void PDebitNote(long irec,FILE *ptr,DB *df);
extern void PCreditNote(long irec,FILE *ptr,DB *df);
extern void GldatChildControl(UINT IDD,DB *df,DB *psub,int IdxItem);
extern void init_WacBank(DB *df);
extern void PurchaseOrderData();
extern void UpdatePoInformation(DB *df,DB *podf);
extern int db_AdminDb(DB *df,int flag);
extern void stateTitle();
extern void SQL_Method();
extern void GetPaydatGainLoss(DB *df,DB *sub);
extern void PurchaseOrderData();
extern int IsArinvStable(DB *df);
extern void Receipt_Entry();
extern void CloseAC();
extern double price_unit;
extern CString FindMarginCode();
extern void ProductCostingDataEntry();
extern void OfficeUseOpeningEntry();
extern void init_ooitem(DB *df);
extern void BankReconciliation();
extern void OfficeStockItemOut();
extern void WarehouseStockInward();
extern void FindEnquiryDetailInformation(DB *rep);
extern void BankReconciliationOpeningData();
extern void PrintBankReconciliation(DB *df,int i);
extern void PutBankReconciliation(DB *df,int i);
extern void ViewEditBankReconciliation(void);
extern void init_blist(DB *df);
extern void SaveArDB(DB *df,DB *data);
extern void GLCodeInitialSetting();
extern void PostingData();
extern void SupplierInquired();
extern void init_Gst_type(DB *df);
extern void AR_ChequeReturn();
extern void AP_ChequeReturn();
extern void TOServer();
extern void ViewAR_ChequeReturn();
extern void ViewAP_ChequeReturn();
extern char Ob[RCA+1][16];
extern double arrage_st(DB *df);
extern long custidx;
extern char TmpTco[500];
extern char p2date[130];
extern double Over_Balance[RCA+1];
extern void init_st1(DB *df);
extern long viewidx_ciq(DB *df,UINT ID);
extern void ComputeOverFourMonthAgeing();
extern void ViewCIQ();
extern DB  ciq,blist,tww,enquiry,ppf,ppfitem,size,ptcost,ptcitem;
extern void put_blist(DB *df,int mode);
extern int enquiry_flag;

extern void CustomerInquired();
extern void PrintVoucher(long irec,FILE *ptr,DB *df);
extern long viewidx_ciq(DB *df,UINT ID=IDD_VIEWCIQ);
extern long viewinv(DB *df,UINT ID=IDD_VIEWINV);
extern void OpenAPData();
extern void OpenARData();
extern void OpenGLData();
extern void CREntry();
extern void init_trdat(DB *df);
extern void CollectReceipt(DB *data,char *cust, char *cur,char *paydate);
extern void CreditNoteRequestForm();
extern void init_cnq(DB *df);
extern void init_acrdat(DB *df);
extern void ViewTR_Entry(int flag=1);
extern DB cp,cpdat,acr,acrdat,ctype,cnq,cnqdat,st,st1,st2,pgcode;
extern CProgressCtrl *m_p1,*m_p2;
extern char FrmCurrency[10];
extern char *not_bal_msg;
extern char *AcStr[3];
extern void SetPaperSize(FILE *ptr);
extern int OpenFlag;
extern int XprintFormat;
extern int SelectFormatMode();
extern void init_Gst_type(DB *df);
extern void DoWAC(DB *wac,DB *wacitem,int id1,int id2);
extern int CheckIssuedChequeNo(int fld,int flag,DB *df);
extern void init_wac(DB *df);
extern void init_wacitem(DB *df);
extern void init_prefix(DB *df);
extern void init_bank(DB *df);
extern void init_emode(DB *df);
extern void init_margin(DB *df);
extern BOOL bTaxInclude;
extern void Ppay(long irec,FILE *ptr,DB *df);
extern void Preceipt(long irec,FILE *ptr,DB *df);
extern int  ProgramMenuNo;
extern int ProduresID;
extern void PurchaseInvoice();
extern void SalesInvoice();
extern double dr_amt, cr_amt;
extern void CreditNote();
extern void TodayRIDataEntry(int flag);
extern void DebitNote();
extern void AllInvoiceDataEntry();
extern void CPLEntry();
extern void ViewConfirmInvoiceDataEntry();
extern void TodayInvoiceDataEntry(int flag);
extern void CustomerCompliantMessage(UINT IDD,DB *df,DB *psub,int IdxItem);
extern void MonthlyStockBalance();
extern void ViewGoodsTransfer();
extern void ViewGoodsppf();
extern void ViewCurrentStockBalance();
extern void ViewOpeningEntry();
extern void ViewReturnIn();
extern void ItemGroupDataEntry();
extern void ViewInward();
extern void ReturnDODataEntry();
extern DB penditem,ppinv,pc,ReturnDO,reason,directors_data,tso,tvo,tst;
extern DB bcwac,bcitem;
extern void TransferTableFromOLDSystem();
extern void ViewAfterPendingEntry();
extern void ExportINVTransactions();
extern void DirectPrintINVOICE(DB *df, DB *sub);
extern void SetInvoiceTitle(DB *df);
extern void Pinv(long irec,FILE *ptr,DB *df);
extern void Print_Transfer_From_Outlet(long irec,FILE *ptr,DB *df);
extern void PIentry(long irec,FILE *ptr,DB *df);
extern void PrintReturnInward(long irec,FILE *ptr,DB *df);
extern void PrintINVOICE(long irec,long Srec, DB *df, DB *sub);
extern void ComputeBankCommission(DB *df);
extern void TransferTableToOLDSystem();
extern void GetInvoiceInformation(DB* df);
extern void ResetReceiptBalance();
extern double OinvAmt;
extern void init_scheme(DB *df);
extern void init_oinv(DB *df);
extern void init_cpl(DB *df);
extern void init_cplitem(DB *df);
extern void init_penditem(DB *df);
extern void init_glcode(DB *df);
extern void init_glgroup(DB *df);
extern double INV_total,INV_pay,INV_cr,INV_rc,INV_bal;
extern double InvoiceRealBalance(DB *df,char *text,char *date);
extern double InvoiceBalance(DB *df,char *text);
extern void init_OffsetInv(DB *df);
extern int STCR,STDR;
extern int tcl_key;
extern void ViewSOData();
extern double FindSpecialPrice(DB *df);
extern double customer_percent;
extern int tell_invitem(UINT nChar,int &flag,int &fld,DB *df,int sq);
extern char *AltServerName,*AltinternetUser,*AltinternetPassword;
extern char *Alt2ServerName,*Alt2internetUser,*Alt2internetPassword;
extern void ImportTopLegionInvoice();
extern char *InvServerName,*InvinternetUser,*InvinternetPassword;
extern double FindFIFOValue(char *tnk,double qty);
extern void TransferSalesInvoiceToOLDSystem();
extern DB   taccode,imon,period,period_acc;
extern DB   *customer,*ipay,*paydat,*cr,*crdat,newinv;
extern DB   aripay,arpaydat,arcr,arcrdat,*gldat,scheme,tmpinv,creditor;
extern DB   cpl,cplitem,glgroup,glcode,*inv,*oinv,ar_oinv,ap_oinv,prefix;
extern DB   tt,bank,dc,wac,wacitem,emode;
extern DB   Gst_type,bcwac,bcitem;
extern DB   ar_adj_cat,ap_adj_cat,*rc,*rcdat;
extern DB   apcr,apcrdat,aprc,aprcdat,apipay,appaydat,apinv,ap_gldat,ap_oinv;
extern DB   arcr,arcrdat,arrc,arrcdat,aripay,arpaydat,arinv,ar_gldat,ar_oinv;
extern DB   gl_init,glitem,ido,locate,pt,acccat,whstkin;
//extern DB   tng;
//extern void init_Tng(DB *df);
extern void FormatSmartCard();

extern int IsPeriodLock(char *date);
extern void AccountInvoice();
extern void Pitem(long irec,FILE *ptr,DB *df);
extern BOOL FileCompare(const char *sourceFile,const char *cmpFile);
extern void OpeningEntry();
extern void OpenPin(char *file, char *hfile);
extern void DoDebtorDataMaintenance(DB *df);
extern void init_accode(DB *df);
extern void init_receipt(DB *df);
extern void init_arinv(DB *df);
extern void init_ri(DB *df);
extern void init_tr(DB *df);
extern void init_trdat(DB *df);
extern void init_ob(DB *df);
extern void init_tmo(DB *df);
extern void init_ientry(DB *df);
extern void init_initem(DB *df);
extern void init_item(DB *df);
extern void ItemDataEntry(int);
extern void init_tfo(DB *df);
extern void init_tfoitem(DB *df);
extern void TransferDataEntry();
extern void ppfDataEntry();
extern void IentryEntry();
extern void init_pending(DB *df);
extern void InvoiceDataEntry();
extern void ReturnIn();
extern void PendingEntry();
extern void init_riitem(DB *df);
extern void init_invitem(DB *df);
extern void init_invoice(DB *df);
extern void CreateItemFIFO();
extern void ClosePin();
extern void SendPayment_Entry();
extern double ItemAmt;
extern double FindMinimumPrice();
extern char *ServerName;
extern void PriceTableDataEntry();
extern char *internetUser,*internetPassword;
extern void Payment_Entry();
extern void TR_Entry();
extern void ActiveRouter();
extern HINTERNET hIS,hIC;
extern void InvoiceEntry();
extern void ServiceCenter();
extern void SpecialPriceDataEntry();
extern void CleanFtp();
extern BOOL FindServiceAsk();
extern void UnPackServiceFiles();
extern BOOL MyFtpPutFile(IN HINTERNET hFtpSession,IN LPCSTR lpszLocalFile,
    IN LPCSTR lpszNewRemoteFile,IN DWORD dwFlags,IN DWORD dwContext);

extern DB sr_tmp,sritem_tmp,so,soitem,tr,trdat,location,currency,area,pccode,tmo;
extern DB ptable,ptp,paytype,ri,riitem,itype,itemcust,receipt,stklog;
extern DB officeout,ooitem;
extern void init_debtor(DB *df);
extern void init_creditor(DB *df);
extern void init_ipay(DB *df);
extern void init_paydat(DB *df);
extern void init_crdat(DB *df);
extern void init_cr(DB *df);
extern void init_apcr(DB *df);
extern void TodayPriceDataEntry();
extern void PendingSREntry();
extern void ContractListingReport();
extern void init_salesman(DB *df);
extern void MasterGrossTareNetReport(DB *df);
extern DB *ttt,Status,sg,arinv;
extern void ProfileData(DB *df);
extern int   PViewFlag;
extern void DownLoadProcess();
extern void UpLoadProcess();
extern void init_stdlist(DB *df);
extern void PrintLoadingUnloadingForm(long irec,FILE *ptr,DB *df);
extern void DetailGrossTareNetListingReport(DB *df);
extern void DoSalemanDataMaintenance(DB *df);

extern void SetStdCompanyLogo();
extern int keytscr(void);
extern int  AccessNo;
extern void ProductSummaryReport(DB *df);
extern void PorlaReport(void);
extern void BuyerTransportFeeSummary(DB *df);
extern char newpatn[50][72];
extern char *dsavemenu,*company;
extern void prec(long irec,FILE *ptr,DB *df);
extern double amount,dr_total,cr_total,bal_total;
extern void PrintLogo(FILE *ptr,char *tl);

#define TDATE Date
#define ACCODE CUST
extern DB   logo,user,login,report,one,two,profile,vtype,category,cgroup;
extern DB   pending,penddat,item,accode,debtor,xclass,staff,invoice,invitem;
extern DB   ob,obitem,arriveddate,prodgrade;
extern void init_sr(DB *df);
extern void init_sritem(DB *df);
extern void SREntry();
extern void ApprovedSREntry();
extern void RejectSREntry();
extern void SendItemSREntry();
extern void CustomerInquired();
extern BOOL MoveParentRecords(char *key,DB *df,DB *sub,DB *psr,DB *psritem);
extern void init_spitem(DB *df);
extern void InvoiceReport();
extern void MarginPriceDataEntry();
extern void AdminAccodeDb(DB *df);
extern void ResetDatabase();



enum  debtor { CU_CODE=1,CU_NAME,CU_SNAME,CU_ADD1,CU_ADD2,CU_ADD3,CU_TEL0,CU_TEL1,
               CU_RESTEL,CU_HTEL,CU_FAX,CU_EMAIL,CU_AREA,CU_TYPE,CU_CURRENCY,
               CU_STAFF,CU_CONTACT,
               CU_GLCODE,CU_GLCODE2,CU_SCHEME,CU_PREFIX,CU_TERMS,
               CU_LIMIT,CU_BAL,CU_ODATE,CU_STATEMENT,CU_ZERO,
               CU_REM,CU_REM2,
               CU_CUST_BAL,CU_COUNTRY,CU_ACTYPE};
#define ENAME     2
#define ADD1      CU_ADD1
#define ADD2      CU_ADD2
#define ADD3      CU_ADD3
#define STAFF     CU_STAFF
#define CUR       CU_CURRENCY
#define AREA      CU_AREA
#define TEL0      CU_TEL1
#define GLCODE    CU_GLCODE
#define GLCODE2   CU_GLCODE2
#define STATEMENT CU_STATEMENT
#define TERMS     CU_TERMS
#define FAX       CU_FAX

enum profile {PF_COMPANY_NO=1,PF_ADD1,PF_ADD2,PF_ADD3,PF_TAXID,
              PF_DATE,PF_DEBTOR,
              PF_SALES,PF_CREDITOR,PF_PURCHASE,PF_PC,PF_BANK,PF_BC,PF_APF,PF_PF,
              PF_VAT,PF_AR_CGL,PF_AP_CGL,PF_CURRENCY,PF_ZERO,PF_STABLE,PF_MINIMUM,PF_BRANCH,
			  PF_TCPIP_SERVERNAME,PF_TCPIP_USERNAME,PF_TCPIP_PASSWORD,PF_PORT,PF_VARTOLERANCE,
              PF_ARPAY,PF_APPAY,PF_GLWAC,PF_ALIGN,PF_RPRINTER,
              PF_AR,PF_AP,PF_GL,PF_TAX,PF_GLTAX,PF_PREVIEW,PF_CUSTOMER_BANK,
              PF_TEMP_REC,PF_TAX_NO,PF_ETA_DATE,PF_PCCODE,
              PF_CUST_PO,PF_SUPP_INV,PF_COLLECT_BY,PF_PAID_BY,PF_PAYTYPE,
              PF_EMODE,PF_MULTI_CURR,PF_SCHEME,PF_TT,PF_PREFIX,
              PF_SHOW_OVER_CREDIT_LIMIT,PF_AREA,PF_SALESMAN,PF_PD_CHQ,
              PF_CONTRA,PF_RET_CHQ_RECORDS,PF_HOLD_PAYMENT,PF_PERIOD,
              PF_CN_CATEGORY,PF_MULTI_USER,PF_CLIENT_SERVER,PF_SERVER_NAME,
			  };

#define Pro_SITE Pro_CODE

//from copper FFB 
/*
enum  ipay    {IP_BILL=1,IP_CUSTOMER,IP_RECEIPT,IP_DATE,IP_STAFF,IP_CURR,IP_RATE,
IP_PAYTYPE,IP_BANK2,IP_CHEQUE,IP_CQ_DATE,
IP_GLCODE,IP_TOTAL,IP_RM,IP_BC,IP_REMARKS,
IP_BANKIN,IP_RETURN,IP_CGL,IP_VATNO,IP_DISAMT,IP_PAYER,IP_FOREIGN_AMT,IP_FOREIGN_RATE,IP_CGL_DATE};
#define IP_CURRENCY IP_CURR
#define IP_BILLNO   IP_BILL
#define IP_PARTICULAR   IP_REMARKS
#define IP_ACCODE   IP_CUSTOMER
*/
enum  ipay     {IP_BILL=1,IP_CUSTOMER,IP_RECEIPT,IP_DATE,IP_STAFF,IP_CURR,IP_RATE,
               IP_PAYTYPE,IP_BANK2,IP_CHEQUE,IP_CQ_DATE,
               IP_GLCODE,IP_TOTAL,IP_BC,IP_REMARKS,
               IP_BANKIN,IP_RETURN,IP_CGL,IP_VATNO};

#define IP_CURRENCY IP_CURR
#define IP_BILLNO   IP_BILL

enum staff { CODENO=1,NAME,SEX,STATUS,TEL,POSITION2,POSTCODE1,HADD1,
             INCOMETAX,SOCSO,ICNO,EPF,ACNO,EDUCATION,MEMO,
             BIRTH,HIRE,TERMINAT,COMM1,COMM2,
             };


enum offset  {OFI_INVOICE=1,OFI_DATE,OFI_DAY,
               OFI_TOTAL,OFI_DISCOUNT,OFI_IBAL,OFI_BAL,OFI_ACC,OFI_PAY,OFI_OVERDUE};
enum accode1 {AC_CODE=1,AC_NAME,AC_ADD1,AC_ADD2,AC_ADD3,AC_PTABLE,
             AC_TEL1,AC_FAX,AC_LIMIT,AC_TERMS,AC_BAL};
enum area {Area_CODE=1,Area_NAME,Area_REMARK,Area_PCCODE,Area_LOCATION};

extern DB itemrec;
enum itemrec {Itr_ITEM=1,Itr_DATE,Itr_REFNO,Itr_TYPE,Itr_I1,Itr_O1,
              Itr_B1,Itr_I2,Itr_O2,Itr_B2,Itr_I3,Itr_O3,Itr_B3,
              Itr_I4,Itr_O4,Itr_B4};
enum tmo {Tm_D1=1,Tm_D2,Tm_ITEM1,Tm_ITEM2,Tm_CATEGORY,Tm_STYPE,Tm_LOCATION,Tm_PAGE};
enum tcl {Tcl_DEBTOR1=1,Tcl_DEBTOR2,Tcl_BL,Tcl_AREA,Tcl_STAFF,Tcl_GLCODE,Tcl_PAGE};
enum paytype {Pty_CODE=1,Pty_NAME,Pty_DESC,Pty_COMM,Pty_ARMINBC,Pty_APCOMM,Pty_APMINBC,Pty_PTYPE,Pty_REMARKS};

enum  paydat  {PY_BILL=1,PY_SQNO,PY_INVOICE,PY_DATE,PY_TOTAL,PY_GL};
enum  gldat   {CG_REFNO=1,CG_SQNO,CG_YOUR_REF,CG_GLCODE,CG_DESCRIPT,
               CG_ADJ_CAT,CG_TOTAL,CG_INVOICE,CG_PPF};
enum  crdat   {CN_BILL=1,CN_SQNO,CN_INVOICE,CN_DATE,CN_TOTAL,CN_GL};
enum cr {Cr_BILL=1,Cr_CUSTOMER,Cr_DATE,Cr_STAFF,Cr_PCCODE,Cr_TOTAL,
         Cr_CURR,Cr_RATE,Cr_REMARKS,Cr_CGL,Cr_TR,Cr_OR};
enum adj_cat {Ct_CODE=1,Ct_CATEGORY,Ct_REMARKS,Ct_SQNO,Ct_GLCODE};
#define Cr_CURRENCY Cr_CURR
enum  st      { St_DATE=1,St_INVNO,St_NAME,St_BILLNO,St_FLAG,
                St_DR,St_CR,St_BALANCE,St_IDATE,St_RECEIVABLE,St_TYPE,St_INVOICE};
enum scheme {Sch_REFNO=1,Sch_DATE,Sch_REM1,Sch_REM2,Sch_REM3,Sch_GLCODE,
             Sch_D1,Sch_P1,Sch_D2,Sch_P2,Sch_D3,Sch_P3,Sch_D4,Sch_P4,
             Sch_D5,Sch_P5,Sch_D6,Sch_P6,Sch_D7,Sch_P7,Sch_D8,Sch_P8,Sch_D9,Sch_P9,Sch_D10,Sch_P10};
enum currency {CY_CODE=1,CY_CURRENCY,CY_PERFIX,CY_SPERFIX,CY_RATE,CY_DATE,CY_REMARKS};
enum glgroup {Gp_CODE=1,Gp_DESCRIPT,Gp_TYPE,Gp_SQNO};
enum glcode {C_CODE=1,C_DESCRIPT,C_GLGROUP,C_TYPE,C_DBCR, CBAL,C_PRIMARY};
/*
enum arinv {IV_BILLNO=1,IV_INVOICE,IV_DATE,IV_ETA,IV_CUSTOMER,
            IV_TYPE,IV_STAFF,IV_PREFIX,IV_TERMS,
            IV_CURR,IV_RATE,IV_PCCODE,IV_REM,IV_HOLD_PAY,IV_REASON,IV_TOTAL,
            IV_BANK,IV_CHEQUE,IV_CQ_DATE,IV_BANKIN};
#define IV_TT       IV_TYPE
#define IV_CURRENCY IV_CURR
			enum oinv {Oiv_REFNO=1,Oiv_SQNO,Oiv_GLCODE,Oiv_NAME,
			Oiv_QTY,Oiv_PRICE,Oiv_AMT,Oiv_T,Oiv_VAT};
#define Oiv_AMOUNT Oiv_AMT
*/
enum arinv {IV_BILLNO=1,IV_INVOICE,IV_PONO,IV_DATE,IV_ETA,IV_CUSTOMER,
IV_TYPE,IV_STAFF,IV_PREFIX,IV_TERMS,
IV_CURR,IV_RATE,IV_PCCODE,IV_DEPT,IV_REM,IV_HOLD_PAY,IV_REASON,IV_TOTAL,
IV_BANK,IV_CHEQUE,IV_CQ_DATE,IV_BANKIN,IV_POB_JOBFILE,IV_CONTRAREF};
#define IV_TT       IV_TYPE
#define IV_CURRENCY IV_CURR

enum oinv {Oiv_REFNO=1,Oiv_SQNO,Oiv_GLCODE,Oiv_NAME,
Oiv_QTY,Oiv_PRICE,Oiv_AMT,Oiv_ANA,Oiv_ACODE,Oiv_PCCODE,
Oiv_DEPT,Oiv_FA,Oiv_T,Oiv_PERCEN,Oiv_VAT,Oiv_INCLUDE,Oiv_JOBKEY,Oiv_JOBGRP};
#define Oiv_AMOUNT Oiv_AMT


enum wac   {I_REFNO=1,I_EMODE,I_DATE,I_CHEQUE,I_CQ_DATE,I_BANKIN,
            I_PCCODE,I_PARTICULAR,I_REMARKS,I_POSTING};
enum wacitem {Wi_INVNO=1,Wi_SQNO,Wi_GLCODE,Wi_NAME,Wi_DC,Wi_AMT,Wi_T,Wi_GST};
enum t {Vat_VTTYPE=1,Vat_VTNAME,Vat_TAX,Vat_REM,Vat_GLCODE1,Vat_GLCODE2,Vat_GLCODE3};
enum emode {Em_CODE=1,Em_PARTICULAR,Em_REMARKS,Em_TYPE,Em_GLCODE1,Em_GLCODE2,
            Em_AMT,Em_REMINDER,Em_PREFIX,Em_NEXTNO,Em_DATE,Em_PERIOD,Em_REM1,Em_REM2,Em_REM3};
enum  receipt {RE_INVOICE=1,RE_DATE,RE_DAY,
               RE_TOTAL,RE_DISCOUNT,RE_IBAL,
               RE_BAL,RE_ACC,RE_PAY,RE_OVERDUE,RE_MONTH};
enum ibal {Ibal_REFNO=1,Ibal_DEBTOR,Ibal_DATE,Ibal_TERMS,Ibal_AMOUNT,Ibal_PAYSUM,Ibal_CNSUM,Ibal_RCSUM};
enum {DD_DEBTOR=1,DD_SQNO,DD_DIRECTOR,DD_JOIN,DD_CAPITAL};
enum acr {CR_REFNO=1,CR_DEBTOR,CR_DATE,CR_ADJ_CAT,CR_AMOUNT,CR_REM1,CR_REM2,CR_REM3,CR_REM4,CR_REM5,CR_STATUS,CR_STAFF};
#define CR_BILLNO CR_REFNO
#define CR_CUSTOMER CR_DEBTOR
#define CR_TOTAL CR_AMOUNT

enum cnq {Cnq_TR=1,Cnq_CRF,Cnq_DATE,Cnq_DEBTOR,Cnq_ADJ_CAT,Cnq_AMOUNT,
          Cnq_REM1,Cnq_REM2,Cnq_REM3,Cnq_REM4,Cnq_REM5,Cnq_STATUS,Cnq_STAFF};
#define Cnq_TOTAL Cnq_AMOUNT
#define Cnq_CUSTOMER Cnq_DEBTOR
enum cnqdat {Cnd_TR=1,Cnd_SQNO,Cnd_MEMO,Cnd_AMOUNT};
#define Cnd_TOTAL Cnd_AMOUNT
enum tso { Ts_D1=1,Ts_D2,Ts_CUSTOMER1,Ts_CUSTOMER2,Ts_STAFF,
           Ts_AREA,Ts_TYPE,Ts_CONVERT,Ts_PAGE};
enum satb {SaT_CUSTOMER=1,SaT_NAME,SaT_TEL,SaT_M0,SaT_M1,
           SaT_M2,SaT_M3,SaT_M4,SaT_CR,SaT_BALANCE,SaT_MARK};
enum tst {TsT_D1=1,TsT_D2,TsT_CUSTOMER1,TsT_CUSTOMER2,TsT_STAFF,TsT_STAFF2,TsT_PCCODE,
          TsT_CURRENCY,TsT_GLCODE,TsT_AREA,TsT_CONVERT,TsT_SHOW_ZERO,TsT_PAGE};
enum tsat {TsaT_D1=1,TsaT_CUSTOMER1,TsaT_CUSTOMER2,TsaT_STAFF,TsaT_STAFF2,TsaT_PCCODE,
          TsaT_CURRENCY,TsaT_GLCODE,TsaT_CONVERT,TsaT_SHOW_ZERO,TsaT_PAGE};
enum tvo { Tv_D1=1,Tv_D2,Tv_TICKET1,Tv_TICKET2,Tv_CUSTOMER,Tv_STAFF,Tv_STAFF2,
           Tv_PCCODE,Tv_GLCODE,Tv_GLCODE2,Tv_T,
           Tv_CURRENCY,Tv_PREFIX,Tv_TT,Tv_SG,Tv_RECEIVABLE,
           Tv_CONVERT,Tv_PAGE};
enum tout{ Tout_D2=1,Tout_TICKET1,Tout_TICKET2,Tout_CUSTOMER,Tout_STAFF,Tout_STAFF2,
           Tout_PCCODE,Tout_GLCODE,Tout_GLCODE2,Tout_T,
           Tout_CURRENCY,Tout_PREFIX,Tout_TT,Tout_SG,Tout_RECEIVABLE,
           Tout_CONVERT,Tout_PAGE};
enum gl_init {AD_REFNO=1,AD_DATE,AD_PARTICULAR,AD_REMARKS};
enum glitem {Gi_INVNO=1,Gi_SQNO,Gi_GLCODE,Gi_DEBIT,Gi_CREDIT};
enum enquiry {EN_DATE=1,EN_REFNO,EN_CHEQUE,EN_BANKIN,EN_PARTICULAR,
              EN_DEBIT,EN_CREDIT,EN_BAL,EN_PATH,EN_TYPE};

enum pgcode {GL_CODE=1,GL_DATE,GL_REFNO,GL_PARTICULAR,GL_AMOUNT,GL_PATH,GL_TYPE,GL_BANKIN,GL_CHEQUE,GL_PCCODE};
enum blist {Bl_CHEQUE=1,Bl_REFNO,Bl_DATE,Bl_PARTICULAR,
            Bl_DR,Bl_CR,Bl_BANKIN,Bl_PATH,Bl_OB};

enum ciq {CIQ_DATE=1,CIQ_INVNO,CIQ_NAME,CIQ_REFNO,CIQ_FLAG,
          CIQ_DEBIT,CIQ_CREDIT,CIQ_BALANCE,CIQ_IDATE,
          CIQ_RECEIVABLE,CIQ_TYPE,CIQ_ACCODE,CIQ_PERIOD1,CIQ_PERIOD2,CIQ_KEY};

enum logerr {LGR_DATE=1,LGR_CASHIER,LGR_ACCESS,LGR_TICKET,LGR_TIME,LGR_PASS_TYPE,LGR_PROGRAM};
enum dp_do {DPDO_DO=1,DPDO_DATE,DPDO_DP,DPDO_PO_NO,DPDO_LC_NO,DPDO_DELIVERY,DPDO_REMARK,DPDO_RAINING,DPDO_WATER,DPDO_TYPEDIRTY,DPDO_GAS,DPDO_GASNO,DPDO_OTHERS};
enum ptsub {PTSB_PART=1,PTSB_SQNO,PTSB_PRODGRADE,PTSB_COST,PTSB_SALES,PTSB_PKKEY};
enum scale {SCA_TICKET=1,SCA_SERIAL,SCA_DATE,SCA_VEHICLE,SCA_UNIT,SCA_ACCODE,SCA_PTYPE,SCA_TRANSP,SCA_DO,SCA_SCRAP,SCA_IC,SCA_DRIVER,SCA_REMARKS,SCA_TIMEIN,
            SCA_SUBTOTAL, SCA_API_SCANNED_QR, SCA_API_DISPENSED // 20230717
};
enum scalesub {SCASB_TICKET=1,SCASB_SQNO,SCASB_PT,SCASB_QTY,SCASB_UPRICE,SCASB_AMT,SCASB_UPRICE_ORI, SCASB_WASTAGE, SCASB_BIN,SCASB_PKKEY};
enum stkpf {PRO_CODE=1,PRO_ADD1,PRO_ADD2,PRO_ADD3,PRO_ACCODE,PRO_ZERO,PRO_STABLE,
            PRO_GATEPASS,PRO_VARIANCE,PRO_GST_NO, PRO_GST_DATE};

/////////////////////////////////////////////////////////////////////////////
// CMenuApp:
// See menu.cpp for the implementation of this class
//

class CMenuApp : public CMyApp
{
public:
	CMenuApp();
	long rememberWt,zeroCount;
    virtual BOOL Domain();
	virtual void Profile();
	virtual void OpenAllDataFile();
    virtual void SetStdCompanyLogo();
    virtual int SubProgram(int key);
	virtual void SetDBNodeNo();
	virtual void SetDBRelations();
	//Account Syste
	BOOL RptResetFiles(const CString m_CrystalReportName,const CString repname,CString &newname);
	CString ConfirmCrystalTableFile(char *FieldName,CString m_CrystalReportName);
	virtual void ToDisplay(HANDLE hComm,long weight, long GwstWeight);
	virtual void OnLogAlram4DriveThrough(long cMaxWt1, const char *cMaxDate1, const char *cMaxTime1, const char *WeighbridgeNo);
    virtual void OnLogAlram4OverMaxWt(long cMaxWt1,long TicketWt,const char *cMaxDate1, const char *cMaxTime1, const char *WeighbridgeNo);
	
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENU_H__D6EA9606_A1F2_11D0_8F37_945AC87D3839__INCLUDED_)
