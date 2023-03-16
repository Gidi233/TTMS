

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
	 * Description: ��ʾ���е��ݳ��ƻ���Ϣ,���в�ѯ��
	 * Input:       ��
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ��
	 */
void Schedule_UI_ListAll(void) {
/*
typedef struct{
	int totalRecords;�����ݼ�¼�� 
	int offset����ǰҳ������ڵ�һ����¼��ƫ�Ƽ�¼��
	int pageSize;ҳ���С
	void *curPos;��ǰҳ��ʼ��¼�������еĽڵ��ַ
	} Pagination_t�� 
*/
    const int STUDIO_PAGE_SIZE = 6;

    int i, id;
	char choice;
	char playName[33];
	schedule_list_t head;
	schedule_node_t *pos;//�����ݳ���Ŀ¼������ 
	Pagination_t paging;//�����ҳ�� 

	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;//��ʼ����ҳ������ 

	//��������
	paging.totalRecords = Schedule_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);
	//����ҳ��paging��λ������ĵ�һҳ 

	do {

       system("cls");//���� 
		printf("\n==================================================================\n");
		printf("**********************( ~^�����Ϣ�б�^~ ) **********************\n");
		printf("%5s  %5s  %5s  %18s  %18s  %5s\n", "ID", "��Ŀid", "�ݳ���ID",
				"��ӳ����", "��ӳʱ��","��λ��");
		printf("==================================================================\n");
		//��ʾ����
		int i;
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%2d  %2d  %2d    %4d-%02d-%02d   %2d-%2d-%2d  %d\n", pos->data.id,
					pos->data.play_id,pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day, pos->data.time.hour,
					pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
			pos = pos->next;//ÿ�ζ�����һ�� 
		}

		printf("--------- �ܼ�¼:%2d -------------------------ҳ���� %2d/%2d ------\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ|          [R]����             [N]��һҳ |  |");
		printf(
				"\n==================================================================\n");
		printf("���ѡ��:");
		fflush(stdin);//��������뻺����
		scanf("%c", &choice);
		fflush(stdin);//ͬ�� 

		switch (choice) {

		/*case 'q':
		case 'Q':
            printf("��������Ҫ��ѯ���ݳ��ƻ��ľ�Ŀ���ƣ�");
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
    //printf("�����������...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, schedule_node_t);

}
/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ��ؾ�Ŀ��ID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Schedule_UI_Add(int play_id) {


        int rtn = 0;
        schedule_t rec;
        //��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Schedule");
        printf("\n=======================================================\n");
		printf("****************  ����µ��ݳ��ƻ�  ****************\n");
		printf("-------------------------------------------------------\n");
     	printf("\t\t\t�����������ŵ��ݳ���ID��");
    	scanf("%10d",&rec.studio_id);
	    printf("\t\t\t�����������ŵ��ݳ���Ŀ���ݳ����ڣ�yyyy mm dd���ո����������");
	    scanf("%4d%2d%2d",&rec.date.year,&rec.date.month,&rec.date.day);
	    printf("\t\t\t�����������ŵľ�Ŀ���ݳ�ʱ�䣺(hh mm ss���ո������):");
	    scanf("%2d%2d%2d",&rec.time.hour,&rec.time.minute,&rec.time.second);
	    printf("\t\t\t���������ƻ����ŵ���λ������");
	    scanf("%10d",&rec.seat_count);
		printf("=======================================================\n");
        rec.play_id =play_id;


    if(Schedule_Srv_Add(&rec)){
        printf("\t\t\t��ӳɹ���  ( ^___^ )y\n");
        Ticket_Srv_AddBatch(rec.id, rec.studio_id);
    }
    rtn++;
	return rtn;

}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: ����ID���޸��ݳ��ƻ���Ϣ
 * Input:       ���޸ĵľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      �޸ĵľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_UI_Modify(int id){

    schedule_t rec;
	int rtn = 0;
	/*Load record*/
	if (!Schedule_Srv_FetchByID(id, &rec)) {//��ѯ���� 
		printf("���ݳ���������!\n�� [Enter] ��������!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("****************  �����ݳ��ƻ�  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ���ݳ���ĿID��\n");
	scanf("%d",&rec.play_id);
    printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ���ݳ����ķ�ӳID�ţ�\n");
    scanf("%d",&rec.studio_id);
	printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ���ݳ�������ݣ�\n");
	scanf("%d",&rec.date.year);
	printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ���ݳ������·ݣ�\n");
	scanf("%d",&rec.date.month);
	printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ���ݳ����ڣ�\n");
	scanf("%d",&rec.date.day);
	printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ���ݳ�ʱ�䣨ʱ����\n");
	scanf("%d",&rec.time.hour);
	printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ���ݳ�ʱ�䣨�֣���\n");
	scanf("%d",&rec.time.minute);
	printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ���ݳ�ʱ�䣨�룩��\n");
	scanf("%d",&rec.time.second);
	printf("\t\t\t��������Ҫ�޸ĵ��ݳ���Ŀ����λ������\n");
	scanf("%d",&rec.seat_count);



	printf("-------------------------------------------------------\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"���³ɹ�!\n�� [Enter] ��������!\n");
	} else
		printf("����ʧ��!\n�� [Enter] ��������!\n");

	getchar();
	return rtn;


	return 0;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_UI_Delete(int id){

    //schedule_t *id;
	int rtn = 0;
	if(Schedule_Srv_DeleteByID(id)){
        printf("ɾ���ɹ���  ( ^___^ )y\n");
        rtn=1;
        return rtn;
    }else{
        printf("ɾ��ʧ��...  (x___x)");
    }

	return rtn;

	/*if (Schedule_Srv_DeleteByID(id)) {
		//��ɾ����ӳ��ʱ��ͬʱ���ݷ�ӳ��idɾ����λ�ļ���
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
 * Description: ����ID�Ų�ѯ�ݳ��ƻ���Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */

int Schedule_UI_Query(int id){
    schedule_t buf;

    if(!Schedule_Srv_FetchByID(id, &buf)){
        printf("\t\t\t������~~û���ҵ����ID����Ϣ������\n");
        return 0;
    }

        printf("\n==================================================================\n");
		printf("********************** �ݳ��ƻ���Ϣ�б� **********************\n");
		printf("%5s  %5s  %5s  %18s  %18s  %5s", "ID", "��Ŀid", "�ݳ���ID",
				"��ӳ����", "��ӳʱ��","��λ��");
		printf("\n==================================================================\n");
		//��ʾ����

        printf("%2d  %2d  %2d    %4d-%02d-%02d   %2d-%2d-%2d  %d\n", buf.id,buf.play_id,buf.studio_id, buf.date.year,buf.date.month,buf.date.day, buf.time.hour,buf.time.minute,buf.time.second,buf.seat_count);
		printf(
				"******************************************************************\n");



	return 1;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: ���б�ģʽ��ʾ������Ŀ���ݳ��ƻ���Ϣ
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬playΪ��Ŀ��Ϣ��pagingΪ��ʾ��ҳ���ò���
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){







}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: �ݳ��ƻ�������棬�ݳ��ƻ����վ�Ŀ���й����Ծ�ĿID��Ϊ����
 * Input:       ��ؾ�ĿID��
 * Output:      �ݳ��ƻ���Ϣ
 * Return:      ��
 */
void Schedule_UI_MgtEntry(int play_id){

    const int SCHEDULE_PAGE_SIZE = 6;
    play_t buf;
    if(!Play_Srv_FetchByID(play_id,&buf)){//���ܲ�ѯ 
        printf("\t\t\tû�и�ID�ľ�Ŀ~\n");
        return ;
    }

	int id2;
    int i, id;
	char choice;
	schedule_list_t head;
	schedule_node_t *pos;
	Pagination_t paging;//�����ҳ�� 

	List_Init(head, schedule_node_t);
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;//��ʼ����ҳ������ 

	//��������
	paging.totalRecords = Schedule_Srv_FetchByPlay(head,play_id);

	Paging_Locate_FirstPage(head, paging);//ʹ�ú꺯�����嵽��һҳ 

	do {

       system("cls");
		printf("\n==================================================================\n");
		printf("********************** �ݳ��ƻ���Ϣ�б� **********************\n");
		printf("%s  %s  %s  %10s  %10s  %s\n", "ID", "��Ŀid", "�ݳ���ID",
				"��ӳ����", "��ӳʱ��","��λ��");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		int i;
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%-2d %5d %5d         %5d-%2d-%2d%5d-%2d-%2d  %d\n", pos->data.id,
					pos->data.play_id,pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day, pos->data.time.hour,
					pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
			pos = pos->next;
		}

		printf("------- �ܼƻ���:%2d ----------------------- ҳ�� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ  [N]��һҳ  [A]���   [D]ɾ��   [U]����   [Q]��ѯ  [R]����  [K]����Ʊ");
		printf(
				"\n==================================================================\n");
		printf("���������ѡ��");
		fflush(stdin);//��������� 
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Schedule_UI_Add(play_id)) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, schedule_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("������ ID:");
			scanf("%d", &id);
			if (Schedule_UI_Delete(id)) {	//������������
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("������ID:");
			scanf("%d", &id);
			if (Schedule_UI_Modify(id)) {	//������������
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'q':
		case 'Q':
			printf("������ID:");
			scanf("%d", &id);
			if(Schedule_UI_Query(id)){
				paging.totalRecords = Schedule_Srv_FetchAll(head);
				List_Paging(head, paging, schedule_node_t);
			}
			break;
		case 'K':
		case 'k':
			//printf("�����ڿ�����...\n");
			printf("������ƻ�ID:");
			scanf("%d", &id2);
			printf("�������ݳ���ID:");
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
    printf("�����������...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, schedule_node_t);


}






