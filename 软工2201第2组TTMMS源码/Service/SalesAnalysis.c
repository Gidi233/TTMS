
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


//����Ա��usrID�ڸ���ʱ����������۶�������۶�
inline int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate){

	return 1;
}

//��Ͼ�ĿPlay.dat,ͳ���������ݣ��������۷���salesanalysis_list_t list��������list������
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list) {

    if(list==NULL){
        return 0;
    }
    play_list_t playList;
    play_node_t *pos;
    salesanalysis_list_t newNode;

    int soldCount = 0;  // ��ȡ������ЧƱ��
    int totalCount = 0;//totalCountΪ��Ʊ��
    int rtn = 0;//������
    List_Free(list,salesanalysis_node_t);

    List_Init(playList,play_node_t);
    Play_Srv_FetchAll(playList); //ȡ�����о�Ŀ

    List_ForEach(playList,pos){
        newNode = (salesanalysis_list_t)malloc(sizeof(salesanalysis_node_t));
        if(!newNode){
            printf("������˼���ڴ����...\n");
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
        newNode->data.totaltickets = soldCount; // data�����titalΪ������ЧƱ��
        List_AddTail(list,newNode);
        Salesanalysis_Perst_Insert(&newNode->data);

        rtn++;

    }

	return rtn;
}

//�Ծ�Ժ��ӰƱ������
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list){

    salesanalysis_node_t *p, *listLeft;
	assert(list!=NULL);

	if(List_IsEmpty(list))
		return ;

	//��nextָ�빹�ɵ�ѭ����������һ�����Ͽ�
	list->prev->next = NULL;

	//listLeftָ���һ�����ݽڵ�
	listLeft = list->next;

	//��list������Ϊ��
	list->next = list->prev = list;

	while (listLeft != NULL ) {
		//ȡ����һ�����
		p = listLeft;
		listLeft = listLeft->next;

		//�����p���뵽����������list��
		SalesAnalysis_Srv_AddToSoftedList(list, p);
	}

}

//�����node���뵽����������list��
void SalesAnalysis_Srv_AddToSoftedList(salesanalysis_list_t list, salesanalysis_node_t *node) {

	salesanalysis_node_t *p;

	assert(list!=NULL && node!=NULL);

	if(List_IsEmpty(list))	{
		List_AddTail(list, node);
	}else{
		//Ѱ�Ҳ���λ��
		p=list->next;

		while(p!=list && (p->data.sales > node->data.sales)){
			p=p->next;
		}

		//�����node���뵽p֮ǰ
		List_InsertBefore(p, node);
	}
}





//����ID��ȡ���۶�ͳ��
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

