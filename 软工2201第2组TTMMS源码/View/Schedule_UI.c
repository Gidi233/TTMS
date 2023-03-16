

#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/list.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>

	/*
	 * Function:    Schedule_UI_ListAll
	 * Function ID:	TTMS_SCU_Schedule_UI_List
	 * Description: 显示所有的演出计划信息,进行查询。
	 * Input:       无
	 * Output:      所有剧目的信息
	 * Return:      无
	 */
void Schedule_UI_ListAll(void) {
/*
typedef struct{
	int totalRecords;总数据记录数 
	int offset；当前页面相对于第一条记录的偏移记录数
	int pageSize;页面大小
	void *curPos;当前页起始记录在链表中的节点地址
	} Pagination_t； 
*/
    const int STUDIO_PAGE_SIZE = 6;

    int i, id;
	char choice;
	char playName[33];
	schedule_list_t head;
	schedule_node_t *pos;//链表将演出的目录连起来 
	Pagination_t paging;//定义分页器 

	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;//初始化分页器参数 

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);
	//将分页器paging定位到链表的第一页 

	do {

       system("cls");//清屏 
		printf("\n==================================================================\n");
		printf("**********************( ~^相关信息列表^~ ) **********************\n");
		printf("%5s  %5s  %5s  %18s  %18s  %5s\n", "ID", "剧目id", "演出厅ID",
				"放映日期", "放映时间","座位数");
		printf("==================================================================\n");
		//显示数据
		int i;
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%2d  %2d  %2d    %4d-%02d-%02d   %2d-%2d-%2d  %d\n", pos->data.id,
					pos->data.play_id,pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day, pos->data.time.hour,
					pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
			pos = pos->next;//每次都向下一个 
		}

		printf("--------- 总记录:%2d -------------------------页数： %2d/%2d ------\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页|          [R]返回             [N]下一页 |  |");
		printf(
				"\n==================================================================\n");
		printf("你的选择:");
		fflush(stdin);//是清空输入缓冲区
		scanf("%c", &choice);
		fflush(stdin);//同上 

		switch (choice) {

		/*case 'q':
		case 'Q':
            printf("请输入您要查询的演出计划的剧目名称：");
            scanf("%s",playName);
			if(Schedule_UI_QueryByPlayName(playName)){
                paging.totalRecords = Schedule_Srv_FetchAll(head);
                List_Paging(head, paging, schedule_node_t);
			}

			break;*/
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			}
			break;
		}
    //printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, schedule_node_t);

}
/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: 添加一条演出计划信息
 * Input:       相关剧目的ID号
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Schedule_UI_Add(int play_id) {


        int rtn = 0;
        schedule_t rec;
        //获取主键
		rec.id = EntKey_Srv_CompNewKey("Schedule");
        printf("\n=======================================================\n");
		printf("****************  添加新的演出计划  ****************\n");
		printf("-------------------------------------------------------\n");
     	printf("\t\t\t请输入您安排的演出厅ID：");
    	scanf("%10d",&rec.studio_id);
	    printf("\t\t\t请输入您安排的演出剧目的演出日期（yyyy mm dd“空格隔开”）：");
	    scanf("%4d%2d%2d",&rec.date.year,&rec.date.month,&rec.date.day);
	    printf("\t\t\t请输入您安排的剧目的演出时间：(hh mm ss“空格隔开”):");
	    scanf("%2d%2d%2d",&rec.time.hour,&rec.time.minute,&rec.time.second);
	    printf("\t\t\t请输入您计划安排的座位数量：");
	    scanf("%10d",&rec.seat_count);
		printf("=======================================================\n");
        rec.play_id =play_id;


    if(Schedule_Srv_Add(&rec)){
        printf("\t\t\t添加成功！  ( ^___^ )y\n");
        Ticket_Srv_AddBatch(rec.id, rec.studio_id);
    }
    rtn++;
	return rtn;

}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: 按照ID号修改演出计划信息
 * Input:       待修改的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      修改的剧目信息数，0表示未找到，1表示找到并更新
 */
int Schedule_UI_Modify(int id){

    schedule_t rec;
	int rtn = 0;
	/*Load record*/
	if (!Schedule_Srv_FetchByID(id, &rec)) {//查询不到 
		printf("该演出厅不存在!\n按 [Enter] 键来返回!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("****************  更新演出计划  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("\t\t\t请输入你要修改的演出剧目的演出剧目ID：\n");
	scanf("%d",&rec.play_id);
    printf("\t\t\t请输入你要修改的演出剧目的演出厅的放映ID号：\n");
    scanf("%d",&rec.studio_id);
	printf("\t\t\t请输入你要修改的演出剧目的演出日期年份：\n");
	scanf("%d",&rec.date.year);
	printf("\t\t\t请输入你要修改的演出剧目的演出日期月份：\n");
	scanf("%d",&rec.date.month);
	printf("\t\t\t请输入你要修改的演出剧目的演出日期：\n");
	scanf("%d",&rec.date.day);
	printf("\t\t\t请输入你要修改的演出剧目的演出时间（时）：\n");
	scanf("%d",&rec.time.hour);
	printf("\t\t\t请输入你要修改的演出剧目的演出时间（分）：\n");
	scanf("%d",&rec.time.minute);
	printf("\t\t\t请输入你要修改的演出剧目的演出时间（秒）：\n");
	scanf("%d",&rec.time.second);
	printf("\t\t\t请输入你要修改的演出剧目的座位数量：\n");
	scanf("%d",&rec.seat_count);



	printf("-------------------------------------------------------\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"更新成功!\n按 [Enter] 键来返回!\n");
	} else
		printf("更新失败!\n按 [Enter] 键来返回!\n");

	getchar();
	return rtn;


	return 0;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: 按照ID号删除演出计划信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_UI_Delete(int id){

    //schedule_t *id;
	int rtn = 0;
	if(Schedule_Srv_DeleteByID(id)){
        printf("删除成功！  ( ^___^ )y\n");
        rtn=1;
        return rtn;
    }else{
        printf("删除失败...  (x___x)");
    }

	return rtn;

	/*if (Schedule_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中
		if(Ticket_Srv_DeleteBatch(id))
			printf("The ticket of the Schedule id deleted successfully!\n");
		printf(
				"The Schedule deleted successfully!\nPress [Enter] key to return!\n");
		rtn = 1;
	} else {
		printf("The Schedule does not exist!\nPress [Enter] key to return!\n");
	}

	getchar();
	return rtn;*/

}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: 按照ID号查询演出计划信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */

int Schedule_UI_Query(int id){
    schedule_t buf;

    if(!Schedule_Srv_FetchByID(id, &buf)){
        printf("\t\t\t呜呜呜~~没有找到这个ID的信息！！！\n");
        return 0;
    }

        printf("\n==================================================================\n");
		printf("********************** 演出计划信息列表 **********************\n");
		printf("%5s  %5s  %5s  %18s  %18s  %5s", "ID", "剧目id", "演出厅ID",
				"放映日期", "放映时间","座位数");
		printf("\n==================================================================\n");
		//显示数据

        printf("%2d  %2d  %2d    %4d-%02d-%02d   %2d-%2d-%2d  %d\n", buf.id,buf.play_id,buf.studio_id, buf.date.year,buf.date.month,buf.date.day, buf.time.hour,buf.time.minute,buf.time.second,buf.seat_count);
		printf(
				"******************************************************************\n");



	return 1;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: 以列表模式显示给定剧目的演出计划信息
 * Input:       list为查找到的演出计划信息链表的头指针，play为剧目信息，paging为显示分页设置参数
 * Output:      无
 * Return:      返回查找到的记录数目
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){







}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: 演出计划管理界面，演出计划按照剧目进行管理，以剧目ID号为输入
 * Input:       相关剧目ID号
 * Output:      演出计划信息
 * Return:      无
 */
void Schedule_UI_MgtEntry(int play_id){

    const int SCHEDULE_PAGE_SIZE = 6;
    play_t buf;
    if(!Play_Srv_FetchByID(play_id,&buf)){//不能查询 
        printf("\t\t\t没有该ID的剧目~\n");
        return ;
    }

	int id2;
    int i, id;
	char choice;
	schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;//定义分页器 

	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;//初始化分页器参数 

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchByPlay(head,play_id);

	Paging_Locate_FirstPage(head, paging);//使用宏函数定义到第一页 

	do {

       system("cls");
		printf("\n==================================================================\n");
		printf("********************** 演出计划信息列表 **********************\n");
		printf("%s  %s  %s  %10s  %10s  %s\n", "ID", "剧目id", "演出厅ID",
				"放映日期", "放映时间","座位数");
		printf("------------------------------------------------------------------\n");
		//显示数据
		int i;
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%-2d %5d %5d         %5d-%2d-%2d%5d-%2d-%2d  %d\n", pos->data.id,
					pos->data.play_id,pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day, pos->data.time.hour,
					pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
			pos = pos->next;
		}

		printf("------- 总计划数:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页  [N]下一页  [A]添加   [D]删除   [U]更新   [Q]查询  [R]返回  [K]生成票");
		printf(
				"\n==================================================================\n");
		printf("请输入你的选择");
		fflush(stdin);//清除缓存区 
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Schedule_UI_Add(play_id)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("请输入 ID:");
			scanf("%d", &id);
			if (Schedule_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("请输入ID:");
			scanf("%d", &id);
			if (Schedule_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'q':
		case 'Q':
			printf("请输入ID:");
			scanf("%d", &id);
			if(Schedule_UI_Query(id)){
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'K':
		case 'k':
			//printf("功能在开发中...\n");
			printf("请输入计划ID:");
			scanf("%d", &id2);
			printf("请输入演出厅ID:");
			scanf("%d", &id);
			Ticket_Srv_AddBatch(id2, id);
			paging.totalRecords = Schedule_Srv_FetchAll(head);
			List_Paging(head, paging, schedule_node_t);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			}
			break;
		}
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, schedule_node_t);


}






