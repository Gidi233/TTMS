

#include "Schedule_Persist.h"
#include "../Service/schedule.h"
#include "../Common/list.h"
#include "../Service/ticket.h"
//#include "../Persistence/Common_Persist.h"
#include "../View/Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";//保存演出计划信息
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";////删除或更新时，暂时保存演出计划信息

/*
 * Function:    Schedule_Perst_Insert
 * Function ID:	TTMS_SCU_Schedule_Perst_Insert
 * Description: 在演出计划信息文件末尾写入一条演出计划信息
 * Input:       待加入文件的演出计划信息数据
 * Output:      打开文件失败的信息
 * Return:      写入文件的记录数
 */
int Schedule_Perst_Insert(const schedule_t *data)
{
    assert(NULL!=data);
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "ab");// 以追加方式打开文件 
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(schedule_t), 1, fp);//调用fwrite 函数写入新的信息 

	fclose(fp);
	return rtn;

}

/*
 * Function:    Schedule_Perst_Update
 * Function ID:	TTMS_SCU_Schedule_Perst_Mod
 * Description: 按照演出计划ID号修改文件中的演出计划信息
 * Input:       待在文件中修改的演出计划信息数据
 * Output:      打开文件失败的信息
 * Return:      修改的演出计划信息数，0表示未找到，1表示找到并更新
 */
int Schedule_Perst_Update(const schedule_t *data){
    assert(NULL!=data);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");//以读写方式打开文件 
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	schedule_t buf;//定义缓冲区 
	int found = 0;

	while (!feof(fp)) {//判断是否处于文件末尾为位置 
		if (fread(&buf, sizeof(schedule_t), 1, fp)) {//读取一条剧目信息保存到buf中 
			if (buf.id == data->id) {
				fseek(fp, -sizeof(schedule_t), SEEK_CUR);////文件指针回撤一条记录  
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;

}

/*
 * Function:    Schedule_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Perst_DelByID
 * Description: 按照演出计划ID号删除演出计划的信息
 * Input:       待删除的演出计划ID号
 * Output:      打开文件失败的信息
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_Perst_DeleteByID(int ID){
    //将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if(rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE)<0){
		perror("Error: ");
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_TEMP_FILE);
		return 0;
	}
	schedule_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(schedule_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, fpTarg);
		}
	}
	
	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;

}

/*
 * Function:    Schedule_Perst_SelectByID
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByID
 * Description: 按照演出计划ID号删除演出计划的信息
 * Input:       待查找的演出计划ID号，保存查找结果的内存的地址
 * Output:      无
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_Perst_SelectByID(int ID, schedule_t *buf){
    assert(NULL!=buf);
//assert这个宏只是帮助我们调试代码的，它的作用是：让用户在调试函数的时候把错误排除掉，而。assert可以帮助定位错误，而不是排除错误
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)) {
		//文件结束没，结束了1，没结束返回0 
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
		//在fp中读1个大小为sizeof（）的数据放到指针date里面 
			if (ID == data.id) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}
/*
 * Function:    Schedule_Perst_SelectAll
 * Function ID:	TTMS_SCU_Schedule_Perst_SelAll
 * Description: 将所有演出计划信息建立成一条链表
 * Input:       list为演出计划信息链表的头指针
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Schedule_Perst_SelectAll(schedule_list_t list){
   schedule_node_t *newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
			if (!newNode) {
				printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}

/*
 * Function:    Schedule_Perst_SelectByPlay
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByPlay
 * Description: 将同意剧目的演出计划信息搜索出来，建立一条链表
 * Input:       list为满足条件的演出计划信息链表的头指针，剧目的ID号
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id){

    //先销毁，在创建
    List_Destroy(list,schedule_node_t);
    List_Init(list,schedule_node_t);

    FILE *fp = fopen(SCHEDULE_DATA_FILE,"rb");
    schedule_t data;
    schedule_list_t pNew =NULL;
    int rtn = 0;
    if(NULL==fp){
        printf("文件是不存在哒~\n");
        return rtn;
    }


    while(!feof(fp)){

        if(fread(&data,sizeof(schedule_t),1,fp)){
            if(data.play_id==play_id){
            pNew = (schedule_list_t)malloc(sizeof(schedule_node_t));
            if(NULL == pNew){

                printf("内存不足，无法申请空间！\n");
                return rtn;
            }
               pNew->data = data;
               List_AddTail(list,pNew);
               rtn++;
            }
        }
    }
   return rtn;
}
