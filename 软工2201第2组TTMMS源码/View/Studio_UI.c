
#include "Studio_UI.h"

#include "../Common/list.h"
#include "../Service/studio.h"
#include "../Service/seat.h"
#include "../Service/EntityKey.h"
#include "Seat_UI.h"

static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

void Studio_UI_MgtEntry(void) {
	int i, id;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("**********************( ~^演出厅列表^~ ) **********************\n");
		printf("%5s  %18s  %10s  %10s  %10s\n", "ID", "名称", "座位行数",
				"座位列数", "座位总数");
		printf("------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (studio_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
			pos = pos->next;
		}
		printf("------- 剧目总数:%2d ----------------------- 页数： %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页  [N]下一页   [1]添加   [2]删除   [3]修改    [4]座位   [0]退出当前页面");
		printf(
				"\n==================================================================\n");
		printf("\t\t\t请选择（功能前的字母）:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1':
			if (Studio_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case '2':
			printf("\t\t\t请输入要删除的影院ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;

		case '3':
			printf("\t\t\t请输入要修改的影院ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;

		case '4':
			printf("\t\t\t请输入影院ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != '0' );
	//释放链表空间
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("****************  添加一个演出厅  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("\t\t\t演出厅名称：");
		fflush(stdin);
		gets(rec.name);
		printf("\t\t\t预算座位行数:");
		scanf("%d", &(rec.rowsCount));
		printf("\t\t\t预算座位列数:");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = 0;
		printf("=======================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("Studio");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\t\t\t添加成功...  ( ^___^ )y\n");
		} else
			printf("\t\t\t添加失败...!  (x___x)\n");
		printf("-------------------------------------------------------\n");
		printf("\t\t\t[1]添加更多, [2]返回:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('1' == choice );
	return newRecCount;
}

int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("The room does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("****************  修改演出厅信息  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("\t\t\t演出厅ID[%d]:\n", rec.id);
	printf("\t\t\t演出厅名字[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if(seatcount){
		do{			//如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
			printf("\t\t\t修改后的座位行数 >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("\t\t\t修改后的座位列数 >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
		}while(	newrow<rec.rowsCount||newcolumn<rec.colsCount);
		rec.rowsCount=newrow;
		rec.colsCount=newcolumn;
		rec.seatsCount=seatcount;
	}
	else{
		printf("\t\t\t修改后的座位行数:");
		scanf("%d", &rec.rowsCount);
		printf("\t\t\t修改后的座位列数:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount=0;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\t\t\t演出厅更新成功..  ( ^___^ )y\n按任意键继续...\n");
	} else
		printf("\t\t\t演出厅更新失败..(x___x)\n按任意键继续...\n");

	getchar();
	return rtn;
}


int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中的座位
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("\t\t\t座位删除成功...  ( ^___^ )y\n");
		printf(
				"\t\t\t演出厅删除成功... Y(^_^)Y\n按任意键继续\n");
		rtn = 1;
	} else {
		printf("\t\t\t该演出厅不存在...(x___x)\n");
	}

	getchar();
	return rtn;
}

