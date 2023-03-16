
#ifndef SALESANALYSIS_UI_H_
#define SALESANALYSIS_UI_H_
#include "../Common/list.h"
#include "../Service/account.h"
#include "../Service/play.h"
#include "../Service/Sale.h"
#include "../Service/salesanalysis.h"


void SalesAanalysis_UI_BoxOffice();
void SalesAanalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate);
void SalesAanalysis_UI_MgtEntry();


//////////////////////////////////////////////////////////////////
static const int STASALES_PAGE_SIZE=6;

//统计销售额界面
void StaSales_UI_MgtEntry ();

//统计个人销售额界面
void StaSales_UI_Self();

//统计售票员销售额界面
void StaSales_UI_Clerk();

//////////////////////////////////////////////////////////////////
#endif /* SALESANALYSIS_UI_H_ */


