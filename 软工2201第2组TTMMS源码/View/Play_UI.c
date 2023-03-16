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
	 * Description: ���б�ģʽ��ʾ��Ŀ��Ϣ
	 * Input:       list��Ŀ��Ϣ�����ͷָ�룬paging��ҳ���ò���
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ���ز��ҵ��ļ�¼��Ŀ
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
	Pagination_t paging;//�����ҳ�� 

	List_Init(head, play_node_t);//��ʼ����Ŀ��Ϣ���� 
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;//��ʼ����ҳ������ 

	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);//��ҳ�������ڵ�һҳ 

	do {
		system("cls");//���� 
		printf("\n==================================================================\n");
		printf("**********************( ~^��Ŀ��Ϣ����^~ ) **********************\n");
		printf("ID\t��Ŀ����\t��Ʒ����\t��Ŀ�ȼ�\t��Ŀ����\t��Ŀʱ��\t ��ʼ��ӳʱ��\t ������ӳʱ��\t��ĿƱ��\n");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
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
								(post->data.rating==1?"��ͯ   ":(post->data.rating==2?"������":"����   ")),
								(post->data.type==1?" ��Ӱ  ":(post->data.type==2?"���":"���ֻ�   ")),
								post->data.duration,
								post->data.start_date.year,post->data.start_date.month,
								post->data.start_date.day,post->data.end_date.year,
								post->data.end_date.month,post->data.end_date.day,post->data.price);
			post = post->next;
		}
		printf("\t\t\t------- �� %2d �� ----------------------- �� %2d/%2d ҳ ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ  [N]��һҳ  [A]����  [D]ɾ��   [U]�޸�   [S]��ѯ  [R]����");
		printf(
				"\n==================================================================\n");
		printf("���������ѡ��");
		fflush(stdin);//��������� 
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Play_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Play_Srv_FetchAll(head);//���»�ȡ���о�Ŀ��Ϣ 
				Paging_Locate_LastPage(head, paging, play_node_t);//��ҳ���̶����������һҳ 
			}
			break;
		case 'd':
		case 'D':
			printf("����ID:");
			scanf("%d", &id);
			if (Play_UI_Delete(id)) {	//������������
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);//���ݷ�ҳ��paging ��ƫ��������ҳ���̶���head �Ķ�Ӧλ�� 
			}
			break;
		case 'u':
		case 'U':
			printf("����ID:");
			scanf("%d", &id);
			if (Play_UI_Modify(id)) {	//������������
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("����ID:");
			scanf("%d", &id);
			if(Play_UI_Query(id)){

            paging.totalRecords = Play_Srv_FetchAll(head);
			List_Paging(head, paging, play_node_t);
			}

			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {//��ǰҳ�� 
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
    printf("�����������...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, play_node_t);









}


/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: ���һ����Ŀ��Ϣ
 * Input:       ��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Play_UI_Add(void)
{   // ��¼��
    int newCount = 0;

    play_list_t pNew =NULL;
    pNew = (play_list_t)malloc(sizeof(play_node_t));

	printf("\t\t\t==================================================================\n");
    printf("\t\t\t�������Ŀ���ƣ�");
    scanf("%s",pNew->data.name);
    printf("\t\t\t�������Ŀ����(1.��Ӱ 2.��� 3.���ֻ�)��");
    scanf("%d",&pNew->data.type);getchar();
    printf("\t\t\t�������Ŀ��Ʒ������");
    scanf("%s",pNew->data.area);
    printf("\t\t\t�������Ŀ�ȼ���(1.��ͯ 2.������ 3.����)");
    scanf("%d",&pNew->data.rating);
    printf("\t\t\t�������Ŀʱ��(����)��");
    scanf("%d",&pNew->data.duration);
    printf("\t\t\t�������Ŀ��ʼ��ӳ���ڣ������ʽ��yyyy mm dd����");

    scanf("%d",&pNew->data.start_date.year);
    scanf("%d",&pNew->data.start_date.month);
    scanf("%d",&pNew->data.start_date.day);

    printf("\t\t\t�������Ŀ�������ڣ������ʽ��yyyy mm dd����");
    scanf("%d",&pNew->data.end_date.year);
    scanf("%d",&pNew->data.end_date.month);
    scanf("%d",&pNew->data.end_date.day);

    printf("\t\t\t�������Ŀ��Ʊ�ۣ�");
    scanf("%d",&pNew->data.price);
    pNew->data.id = EntKey_Srv_CompNewKey("Play");


    if(Play_Srv_Add(&pNew->data)){
        printf("\t\t\t��ӳɹ���\n");
    }
    free(pNew);
    newCount++;
	return newCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: �޸ľ�Ŀ��Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Play_UI_Modify(int id){
int rnt=0;
    play_t data;

    int rating;
    int type;
    if(!Play_Srv_FetchByID(id, &data)){
        printf("\t\t\tû�д�ID�ľ�Ŀ��Ϣ");
        return 0;
    }

    printf("\t\t\t�������޸ĺ�ľ�Ŀ���ƣ�");
    scanf("%s",data.name);
    printf("\t\t\t�������޸ĺ�ľ�Ŀ����(1.��Ӱ 2.��� 3.���ֻ�)��");
    scanf("%d",&type);getchar();
    if(type==1){
        data.type = PLAY_TYPE_FILE;
    }else if(type == 2){
        data.type = PLAY_TYPE_OPEAR;
    }else if(type == 3){
        data.type = PLAY_TYPE_CONCERT;
    }else{
        printf("\t\t\t�����������,�Զ��˳��˹���~\n");
        return ;
    }

    printf("\t\t\t�������޸ĺ�ľ�Ŀ��Ʒ������");
    scanf("%s",data.area);
    printf("\t\t\t�������޸ĺ�ľ�Ŀ�ȼ�(1.��ͯ 2.������ 3.����)��");
    scanf("%d",&rating);

    if(rating==1){
        data.rating = PLAY_RATE_CHILD;
    }else if(rating == 2){
        data.rating = PLAY_RATE_TEENAGE;
    }else if(rating == 3){
        data.rating = PLAY_RATE_ADULT;
    }else{
        printf("\t\t\t�����������,�Զ��˳��˹���~\n");
        return ;
    }



    printf("\t\t\t�������޸ĺ�ľ�Ŀʱ��(����)��");
    scanf("%d",&data.duration);
    printf("\t\t\t�������޸ĺ�ľ�Ŀ��ʼ��ӳ���ڣ������ʽ��yyyy mm dd����");

    scanf("%d",&data.start_date.year);
    scanf("%d",&data.start_date.month);
    scanf("%d",&data.start_date.day);

    printf("\t\t\t�������޸ĺ�ľ�Ŀ�������ڣ������ʽ��yyyy mm dd����");
    scanf("%d",&data.end_date.year);
    scanf("%d",&data.end_date.month);
    scanf("%d",&data.end_date.day);

    printf("\t\t\t�������޸ĺ�ľ�Ŀ��Ʊ�ۣ�");
    scanf("%d",&data.price);

    if(Play_Srv_Modify(&data)){

        printf("\t\t\t�޸ĳɹ���\n");
        rnt=1;
       
    }

    return rnt;

}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: ����ID��ɾ����Ŀ��Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Play_UI_Delete(int id){
int rtn=0;
    if(Play_Srv_DeleteByID(id)){
        printf("ɾ���ɹ���\n");
        rtn=1;
        return rtn;
    }else{
        printf("ɾ��ʧ��...");
    }

	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: ����ID�Ų�ѯ��Ŀ��Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Play_UI_Query(int id){
	int rtn=0;
    play_t data;
    if(Play_Srv_FetchByID(id,&data)){
        printf("�ҵ��ˣ�\n");
        rtn=1;
        return rtn;
    }else{
        printf("û�ҵ�..\n");
    }

	return rtn;
}

/* ---ly*/
void Play_UI_FetchByName(play_list_t list){

   // play_list_t list;
    char condt[33];
    printf("\t\t\t��������Ҫ���ҵľ�Ŀ���ƣ�ģ�����ң�:");
    scanf("%s",condt);
    Play_Srv_FetchByName(list,condt);

}

