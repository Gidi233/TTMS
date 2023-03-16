
#include "Sale_Persist.h"
#include "../Common/list.h"
#include "../Service/Sale.h"
#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>


static const char SALE_DATA_FILE[] = "Sale.dat";
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat";

int Sale_Perst_Insert(const sale_t *data) {
	assert(NULL!=data);
	FILE *fp = fopen(SALE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SALE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(sale_t), 1, fp);

	fclose(fp);
	return rtn;
}


int Sale_Perst_DeleteByID(int saleID) {
    //将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if(rename(SALE_DATA_FILE, SALE_DATA_TEMP_FILE)<0){
		printf("Cannot open file %s!\n", SALE_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SALE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", SALE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SALE_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", SALE_DATA_TEMP_FILE);
		return 0;
	}


	sale_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(sale_t), 1, fpSour)) {
			if (saleID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(sale_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SALE_DATA_TEMP_FILE);
	return found;
}

//根据用户ID载入给定时间区间内的销售记录
int Sale_Perst_SelectByUsrID(sale_list_t list, int usrID, user_date_t stDate,
		user_date_t endDate) {

     //先销毁，在创建
    List_Destroy(list,sale_node_t);
    List_Init(list,sale_node_t);


    FILE *fp = fopen(SALE_DATA_FILE,"rb");
    sale_t data;
    sale_list_t pNew =NULL;
    int rtn = 0;
    if(NULL==fp){
        printf("文件不存在~\n");
        return rtn;
    }

    while(!feof(fp)){

        if(fread(&data,sizeof(sale_t),1,fp)){
            if(data.user_id==usrID&&(data.date.year >= stDate.year &&data.date.year<=endDate.year)
               &&(data.date.month >= stDate.month &&data.date.month<=endDate.month)
               &&(data.date.day >= stDate.day &&data.date.day<=endDate.day)){
               pNew = (sale_list_t)malloc(sizeof(sale_node_t)) ;
                if(pNew==NULL){
                    printf("空间不足！\n");
                }

                pNew->data = data;
               List_AddTail(list,pNew);
               rtn++;
            }

        }

    }

   return rtn;
}


//根据给定时间区间内的销售记录
int Sale_Perst_SelectByDate(sale_list_t list, user_date_t stDate,
		user_date_t endDate) {

     //先销毁，在创建
    List_Destroy(list,sale_node_t);
    List_Init(list,sale_node_t);


    FILE *fp = fopen(SALE_DATA_FILE,"rb");
    sale_t data;
    sale_list_t pNew =NULL;
    int rtn = 0;
    if(NULL==fp){
        printf("文件不存在~\n");
        return rtn;
    }

    while(!feof(fp)){

        if(fread(&data,sizeof(sale_t),1,fp)){
            if((data.date.year >= stDate.year &&data.date.year<=endDate.year)
               &&(data.date.month >= stDate.month &&data.date.month<=endDate.month)
               &&(data.date.day >= stDate.day &&data.date.day<=endDate.day)){
               pNew = (sale_list_t)malloc(sizeof(sale_node_t)) ;
                if(pNew==NULL){
                    printf("空间不足！\n");
                }

                pNew->data = data;
               List_AddTail(list,pNew);
               rtn++;
            }

        }

    }

   return rtn;

}

/*

//queryUid = null,query all the uid Sale information return the all uid Sale information number
//queryUid =  uid, query the uid Sale information return the uid Sale  number
 *
int  Sale_Infor_By_Uid(char * queryUid, Sale_list_t head)
{

	return 0;
}

int Sale_Infor_By_Time(char  queryTime[][30], Sale_list_t head)
{


	return 0;
}

//根据票的id获得票的基本信息
int Select_Price_By_Playid(int id, ticket_t *buf)
{

		return 0;
}
*/

