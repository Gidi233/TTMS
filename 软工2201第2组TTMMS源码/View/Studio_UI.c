
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

	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("**********************( ~^�ݳ����б�^~ ) **********************\n");
		printf("%5s  %18s  %10s  %10s  %10s\n", "ID", "����", "��λ����",
				"��λ����", "��λ����");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (studio_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
			pos = pos->next;
		}
		printf("------- ��Ŀ����:%2d ----------------------- ҳ���� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ  [N]��һҳ   [1]���   [2]ɾ��   [3]�޸�    [4]��λ   [0]�˳���ǰҳ��");
		printf(
				"\n==================================================================\n");
		printf("\t\t\t��ѡ�񣨹���ǰ����ĸ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1':
			if (Studio_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case '2':
			printf("\t\t\t������Ҫɾ����ӰԺID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;

		case '3':
			printf("\t\t\t������Ҫ�޸ĵ�ӰԺID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;

		case '4':
			printf("\t\t\t������ӰԺID:");
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
    printf("�����������...");
    getchar();getchar();
	} while (choice != '0' );
	//�ͷ�����ռ�
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("****************  ���һ���ݳ���  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("\t\t\t�ݳ������ƣ�");
		fflush(stdin);
		gets(rec.name);
		printf("\t\t\tԤ����λ����:");
		scanf("%d", &(rec.rowsCount));
		printf("\t\t\tԤ����λ����:");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = 0;
		printf("=======================================================\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Studio");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\t\t\t��ӳɹ�...  ( ^___^ )y\n");
		} else
			printf("\t\t\t���ʧ��...!  (x___x)\n");
		printf("-------------------------------------------------------\n");
		printf("\t\t\t[1]��Ӹ���, [2]����:");
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
	printf("****************  �޸��ݳ�����Ϣ  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("\t\t\t�ݳ���ID[%d]:\n", rec.id);
	printf("\t\t\t�ݳ�������[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if(seatcount){
		do{			//�����λ�ļ���������λ��Ϣ������µ����б������ǰ�󣬷����������
			printf("\t\t\t�޸ĺ����λ���� >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("\t\t\t�޸ĺ����λ���� >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
		}while(	newrow<rec.rowsCount||newcolumn<rec.colsCount);
		rec.rowsCount=newrow;
		rec.colsCount=newcolumn;
		rec.seatsCount=seatcount;
	}
	else{
		printf("\t\t\t�޸ĺ����λ����:");
		scanf("%d", &rec.rowsCount);
		printf("\t\t\t�޸ĺ����λ����:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount=0;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\t\t\t�ݳ������³ɹ�..  ( ^___^ )y\n�����������...\n");
	} else
		printf("\t\t\t�ݳ�������ʧ��..(x___x)\n�����������...\n");

	getchar();
	return rtn;
}


int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//��ɾ����ӳ��ʱ��ͬʱ���ݷ�ӳ��idɾ����λ�ļ��е���λ
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("\t\t\t��λɾ���ɹ�...  ( ^___^ )y\n");
		printf(
				"\t\t\t�ݳ���ɾ���ɹ�... Y(^_^)Y\n�����������\n");
		rtn = 1;
	} else {
		printf("\t\t\t���ݳ���������...(x___x)\n");
	}

	getchar();
	return rtn;
}

