#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/list.h"
#include "../Service/play.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>
//static const int PLAY_PAGE_SIZE = 5;

	/*
	 * Function:    Play_UI_ShowList
	 * Function ID:	TTMS_SCU_Play_UI_Show
	 * Description: 以列表模式显示剧目信息
	 * Input:       list剧目信息链表的头指针，paging分页设置参数
	 * Output:      所有剧目的信息
	 * Return:      返回查找到的记录数目
	 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	
}
void Play_UI_MgtEntry(int flag){

    static const int PLAY_PAGE_SIZE = 5;

    if(flag==1){
        DisplayQueryPlay();
        return ;
    }

    int i, id;
	char choice;

	play_list_t head;
	play_node_t *post;
	Pagination_t paging;//定义分页器 

	List_Init(head, play_node_t);//初始化剧目信息链表 
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;//初始化分页器参数 

	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);//分页器定义在第一页 

	do {
		system("cls");//清屏 
		printf("\n==================================================================\n");
		printf("**********************( ~^剧目信息管理^~ ) **********************\n");
		printf("ID\t剧目名称\t出品地区\t剧目等级\t剧目类型\t剧目时长\t 开始放映时间\t 结束放映时间\t剧目票价\n");
		printf("------------------------------------------------------------------\n");
		//显示数据
	/*Paging_ViewPage_ForEach(head,paging,play_node_t,pose,i)
	
	{
		printf("%5d%20s%8d\n", post->data.id,post->data.name,	post->data.area,post->date.rating,post->date.type,post->date.duration,post->data.start_date.year,post->data.start_date.month,
								post->data.start_date.day,post->data.end_date.year,
								post->data.end_date.month,post->data.end_date.day,post->data.price);
	}*/
		for (i = 0, post = (play_node_t *) (paging.curPos);
				post != head && i < paging.pageSize; i++) {
			printf("%d\t%-10s\t%-10s\t%10s\t%-10s\t%-10d\t%4d-%2d-%2d\t%4d-%2d-%2d\t%d\n", post->data.id,
								post->data.name,	post->data.area,
								(post->data.rating==1?"儿童   ":(post->data.rating==2?"青少年":"成人   ")),
								(post->data.type==1?" 电影  ":(post->data.type==2?"歌剧":"音乐会   ")),
								post->data.duration,
								post->data.start_date.year,post->data.start_date.month,
								post->data.start_date.day,post->data.end_date.year,
								post->data.end_date.month,post->data.end_date.day,post->data.price);
			post = post->next;
		}
		printf("\t\t\t------- 共 %2d 项 ----------------------- 第 %2d/%2d 页 ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页  [N]下一页  [A]增加  [D]删除   [U]修改   [S]查询  [R]返回");
		printf(
				"\n==================================================================\n");
		printf("请输入你的选择");
		fflush(stdin);//清除缓存区 
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Play_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Play_Srv_FetchAll(head);//重新获取所有剧目信息 
				Paging_Locate_LastPage(head, paging, play_node_t);//分页器固定到链表最后一页 
			}
			break;
		case 'd':
		case 'D':
			printf("输入ID:");
			scanf("%d", &id);
			if (Play_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);//根据分页器paging 的偏移量将分页器固定到head 的对应位置 
			}
			break;
		case 'u':
		case 'U':
			printf("输入ID:");
			scanf("%d", &id);
			if (Play_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("输入ID:");
			scanf("%d", &id);
			if(Play_UI_Query(id)){

            paging.totalRecords = Play_Srv_FetchAll(head);
			List_Paging(head, paging, play_node_t);
			}

			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {//当前页号 
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			break;
		}
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, play_node_t);









}


/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: 添加一条剧目信息
 * Input:       无
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Play_UI_Add(void)
{   // 记录数
    int newCount = 0;

    play_list_t pNew =NULL;
    pNew = (play_list_t)malloc(sizeof(play_node_t));

	printf("\t\t\t==================================================================\n");
    printf("\t\t\t请输入剧目名称：");
    scanf("%s",pNew->data.name);
    printf("\t\t\t请输入剧目类型(1.电影 2.歌剧 3.音乐会)：");
    scanf("%d",&pNew->data.type);getchar();
    printf("\t\t\t请输入剧目出品地区：");
    scanf("%s",pNew->data.area);
    printf("\t\t\t请输入剧目等级：(1.儿童 2.青少年 3.成人)");
    scanf("%d",&pNew->data.rating);
    printf("\t\t\t请输入剧目时长(分钟)：");
    scanf("%d",&pNew->data.duration);
    printf("\t\t\t请输入剧目开始放映日期（输入格式：yyyy mm dd）：");

    scanf("%d",&pNew->data.start_date.year);
    scanf("%d",&pNew->data.start_date.month);
    scanf("%d",&pNew->data.start_date.day);

    printf("\t\t\t请输入剧目结束日期（输入格式：yyyy mm dd）：");
    scanf("%d",&pNew->data.end_date.year);
    scanf("%d",&pNew->data.end_date.month);
    scanf("%d",&pNew->data.end_date.day);

    printf("\t\t\t请输入剧目的票价：");
    scanf("%d",&pNew->data.price);
    pNew->data.id = EntKey_Srv_CompNewKey("Play");


    if(Play_Srv_Add(&pNew->data)){
        printf("\t\t\t添加成功！\n");
    }
    free(pNew);
    newCount++;
	return newCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: 修改剧目信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_UI_Modify(int id){
int rnt=0;
    play_t data;

    int rating;
    int type;
    if(!Play_Srv_FetchByID(id, &data)){
        printf("\t\t\t没有此ID的剧目信息");
        return 0;
    }

    printf("\t\t\t请输入修改后的剧目名称：");
    scanf("%s",data.name);
    printf("\t\t\t请输入修改后的剧目类型(1.电影 2.歌剧 3.音乐会)：");
    scanf("%d",&type);getchar();
    if(type==1){
        data.type = PLAY_TYPE_FILE;
    }else if(type == 2){
        data.type = PLAY_TYPE_OPEAR;
    }else if(type == 3){
        data.type = PLAY_TYPE_CONCERT;
    }else{
        printf("\t\t\t类型输入错误,自动退出此功能~\n");
        return ;
    }

    printf("\t\t\t请输入修改后的剧目出品地区：");
    scanf("%s",data.area);
    printf("\t\t\t请输入修改后的剧目等级(1.儿童 2.青少年 3.成人)：");
    scanf("%d",&rating);

    if(rating==1){
        data.rating = PLAY_RATE_CHILD;
    }else if(rating == 2){
        data.rating = PLAY_RATE_TEENAGE;
    }else if(rating == 3){
        data.rating = PLAY_RATE_ADULT;
    }else{
        printf("\t\t\t类型输入错误,自动退出此功能~\n");
        return ;
    }



    printf("\t\t\t请输入修改后的剧目时长(分钟)：");
    scanf("%d",&data.duration);
    printf("\t\t\t请输入修改后的剧目开始放映日期（输入格式：yyyy mm dd）：");

    scanf("%d",&data.start_date.year);
    scanf("%d",&data.start_date.month);
    scanf("%d",&data.start_date.day);

    printf("\t\t\t请输入修改后的剧目结束日期（输入格式：yyyy mm dd）：");
    scanf("%d",&data.end_date.year);
    scanf("%d",&data.end_date.month);
    scanf("%d",&data.end_date.day);

    printf("\t\t\t请输入修改后的剧目的票价：");
    scanf("%d",&data.price);

    if(Play_Srv_Modify(&data)){

        printf("\t\t\t修改成功！\n");
        rnt=1;
       
    }

    return rnt;

}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: 按照ID号删除剧目信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_UI_Delete(int id){
int rtn=0;
    if(Play_Srv_DeleteByID(id)){
        printf("删除成功！\n");
        rtn=1;
        return rtn;
    }else{
        printf("删除失败...");
    }

	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: 按照ID号查询剧目信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Play_UI_Query(int id){
	int rtn=0;
    play_t data;
    if(Play_Srv_FetchByID(id,&data)){
        printf("找到了！\n");
        rtn=1;
        return rtn;
    }else{
        printf("没找到..\n");
    }

	return rtn;
}

/* ---ly*/
void Play_UI_FetchByName(play_list_t list){

   // play_list_t list;
    char condt[33];
    printf("\t\t\t请输入您要查找的剧目名称（模糊查找）:");
    scanf("%s",condt);
    Play_Srv_FetchByName(list,condt);

}

