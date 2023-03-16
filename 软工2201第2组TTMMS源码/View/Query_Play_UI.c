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
	printf("�������Ŀ���ƣ�");
	scanf("%s",fileName);
	printf("\n=======================================================\n");
	printf("\n****************  ��Ŀ��Ϣ�б�  ***********\n");
	if (Query_PlayName(fileName,&play)) {
		printf("ID\t\t����\t\t����\t �ּ�\t\t��ʼ����\t ��������\t�۸�\n");
		printf("-------------------------------------------------------\n");
		printf("%d\t\t%s\t\t%s\t%s\t%d-%d-%d\t%d-%d-%d\t%d\n", play.id,
							play.name,	play.area,
							(play.rating==1?"��ͯ   ":(play.rating==2?"������":"����   ")),
							play.start_date.year,play.start_date.month,
							play.start_date.day,play.end_date.year,
							play.end_date.month,play.end_date.day,play.price);
		schedule_t  sched;
		studio_t studio = {0};
//		seat_list_t list, int studio_id
		if (Query_Schedule_By_PlayId(play.id, &sched)) {
			Query_Studio_By_Schedule(sched.studio_id, &studio);
			printf("��Ŀ���� %d��%d��%d�� %d:%d �ݳ��� %s Ӱ��\n", sched.date.year, sched.date.month,
																		sched.date.day,sched.time.hour,
																		sched.time.minute, studio.name);
			printf("����λ %d ��, ���� %d ��, %dδ�۳� \n", Seat_Number_Count(sched.studio_id), Sold_Ticket_Count(sched.id),
																		Seat_Number_Count(sched.studio_id)-Sold_Ticket_Count(sched.id));
		} else {
			printf("��Ǹ���þ�Ŀû���ݳ��ƻ���\n");
		}
		printf("[R]����  [B]Ԥ��   [A]���²�ѯ:");
		fflush(stdin);
		scanf("%c", &choice);
		switch(choice) {
		case 'a':
		case 'A':
			break;
		case 'B':
		case 'b':
			//����Ʊ
			break;
		}
	} else {
		printf("��Ǹ���þ�Ŀ�����ڣ�\n");
		printf("[R]����  [B]Ԥ��   [A]���²�ѯ:");
		fflush(stdin);
		scanf("%c", &choice);
		switch(choice) {
		case 'a':
		case 'A':
			break;
		case 'B':
		case 'b':
			//����Ʊ
			break;
		}
	}
printf("�����������...");
getchar();getchar();
} while (choice != 'r' && choice != 'R');

}


