#if !defined(AFX_STATE_H__B777C182_0785_11D3_BD5E_A31FA7CFA924__INCLUDED_)
#define AFX_STATE_H__B777C182_0785_11D3_BD5E_A31FA7CFA924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mgrid.h : main header file for the MENU application
//

extern void ShowInvoiceInformation();
extern void ShowIpayInformation();
extern void ShowCRInformation();
extern void ShowWACInformation();
extern void init_payment(DB *df);
extern void auditor_statement(void);
extern void bf_statement(void);
extern void get_bf(DB *df);
extern double arrage_st(DB *df);
extern double dr_amt,cr_amt;
extern void PrintStatement(FILE *ptr);
extern void EquiryData();
extern void init_tso(DB *df);
extern int FilterBfSTate(void);
extern void FindInvoiceOutstandingBalance(char *date,double offset);
extern void SetStatementInvoiceData(DB *df,DB *rep);
extern void ComputeOverFourMonthAgeing();
extern int  GetAnalysisBalance(void(*gst)(void));
extern void GetAuditorStateData(void);
extern int  NoOfDaysOutstanding(char *);
extern void stateTitle(void);
extern void statement_report(void);
extern void GetSalesmanStateData(void);
extern void GetStateData(void);
extern void ComputeStateBalance();
extern DB   tvo,ciq;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATE_H__B777C182_0785_11D3_BD5E_A31FA7CFA924__INCLUDED_)
