
#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
#include "../Persistence/Sale_Persist.h"
#include "../Persistence/SalesAnalysis_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/list.h"


//计算员工usrID在给定时间区间的销售额，返回销售额
inline int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate){

	return 1;
}

//结合剧目Play.dat,统计销售数据，构建销售分析salesanalysis_list_t list链表，返回list链表长度
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list) {

    if(list==NULL){
        return 0;
    }
    play_list_t playList;
    play_node_t *pos;
    salesanalysis_list_t newNode;

    int soldCount = 0;  // 存取已售有效票数
    int totalCount = 0;//totalCount为总票数
    int rtn = 0;//链表长度
    List_Free(list,salesanalysis_node_t);

    List_Init(playList,play_node_t);
    Play_Srv_FetchAll(playList); //取出所有剧目

    List_ForEach(playList,pos){
        newNode = (salesanalysis_list_t)malloc(sizeof(salesanalysis_node_t));
        if(!newNode){
            printf("不好意思，内存溢出...\n");
            return;
        }
        strcpy(newNode->data.area ,pos->data.area);
        newNode->data.duration = pos->data.duration;
        newNode->data.end_date = pos->data.end_date;
        strcpy(newNode->data.name , pos->data.name);
        newNode->data.play_id = pos->data.id;
        newNode->data.price = pos->data.price;
        newNode->data.start_date = pos->data.start_date;


        newNode->data.sales = Schedule_Srv_StatRevByPlay(pos->data.id,&soldCount,&totalCount);
        newNode->data.totaltickets = soldCount; // data里面的tital为已售有效票数
        List_AddTail(list,newNode);
        Salesanalysis_Perst_Insert(&newNode->data);

        rtn++;

    }

	return rtn;
}

//对剧院电影票房排序
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list){

    salesanalysis_node_t *p, *listLeft;
	assert(list!=NULL);

	if(List_IsEmpty(list))
		return ;

	//将next指针构成的循环链表从最后一个结点断开
	list->prev->next = NULL;

	//listLeft指向第一个数据节点
	listLeft = list->next;

	//将list链表置为空
	list->next = list->prev = list;

	while (listLeft != NULL ) {
		//取出第一个结点
		p = listLeft;
		listLeft = listLeft->next;

		//将结点p加入到已排序链表list中
		SalesAnalysis_Srv_AddToSoftedList(list, p);
	}

}

//将结点node加入到已排序链表list中
void SalesAnalysis_Srv_AddToSoftedList(salesanalysis_list_t list, salesanalysis_node_t *node) {

	salesanalysis_node_t *p;

	assert(list!=NULL && node!=NULL);

	if(List_IsEmpty(list))	{
		List_AddTail(list, node);
	}else{
		//寻找插入位置
		p=list->next;

		while(p!=list && (p->data.sales > node->data.sales)){
			p=p->next;
		}

		//将结点node加入到p之前
		List_InsertBefore(p, node);
	}
}





//根据ID获取销售额统计
int StaSales_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate){
    int amount = 0;
    sale_list_t saleList =NULL;

    sale_node_t pSale;

    List_Init(saleList,sale_node_t);

    if(Sale_Perst_SelectByUsrID(saleList, usrID, stDate,endDate) <=0){

		return -1;
    }
    sale_list_t pTemp;
    List_ForEach(saleList,pTemp){
        amount+=pTemp->data.value;
    }

    return amount;

}

