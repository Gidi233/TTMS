#include "Query_Play_UI.h"
#include "../Service/play.h"
#include "../Persistence/Query_Persist.h"
#include "../Service/schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Service/studio.h"
#include "../Persistence/Ticket_Persist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void DisplayQueryPlay(void)
{
	fflush(stdin);
	char fileName[30];
	play_t play;

	char choice;
do {
    system("cls");
	fflush(stdin);
	memset(fileName, 0, sizeof(fileName));
	printf("请输入剧目名称：");
	scanf("%s",fileName);
	printf("\n=======================================================\n");
	printf("\n****************  剧目信息列表  ***********\n");
	if (Query_PlayName(fileName,&play)) {
		printf("ID\t\t名称\t\t区域\t 分级\t\t开始日期\t 结束日期\t价格\n");
		printf("-------------------------------------------------------\n");
		printf("%d\t\t%s\t\t%s\t%s\t%d-%d-%d\t%d-%d-%d\t%d\n", play.id,
							play.name,	play.area,
							(play.rating==1?"儿童   ":(play.rating==2?"青少年":"成人   ")),
							play.start_date.year,play.start_date.month,
							play.start_date.day,play.end_date.year,
							play.end_date.month,play.end_date.day,play.price);
		schedule_t  sched;
		studio_t studio = {0};
//		seat_list_t list, int studio_id
		if (Query_Schedule_By_PlayId(play.id, &sched)) {
			Query_Studio_By_Schedule(sched.studio_id, &studio);
			printf("剧目将在 %d年%d月%d日 %d:%d 演出于 %s 影厅\n", sched.date.year, sched.date.month,
																		sched.date.day,sched.time.hour,
																		sched.time.minute, studio.name);
			printf("有座位 %d 个, 已售 %d 个, %d未售出 \n", Seat_Number_Count(sched.studio_id), Sold_Ticket_Count(sched.id),
																		Seat_Number_Count(sched.studio_id)-Sold_Ticket_Count(sched.id));
		} else {
			printf("抱歉，该剧目没有演出计划！\n");
		}
		printf("[R]返回  [B]预订   [A]重新查询:");
		fflush(stdin);
		scanf("%c", &choice);
		switch(choice) {
		case 'a':
		case 'A':
			break;
		case 'B':
		case 'b':
			//出售票
			break;
		}
	} else {
		printf("抱歉，该剧目不存在！\n");
		printf("[R]返回  [B]预订   [A]重新查询:");
		fflush(stdin);
		scanf("%c", &choice);
		switch(choice) {
		case 'a':
		case 'A':
			break;
		case 'B':
		case 'b':
			//出售票
			break;
		}
	}
printf("按任意键继续...");
getchar();getchar();
} while (choice != 'r' && choice != 'R');

}


