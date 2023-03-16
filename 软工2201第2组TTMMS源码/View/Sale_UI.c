#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "Ticket_UI.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include "../Service/studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//���õ�½�û���ȫ�����ݽṹ
extern account_t gl_CurUser;


//���ݼƻ�ID����ʾ�ݳ�Ʊ
void Sale_UI_ShowTicket(int schID){ 

    int i, id;
    int schedule_id = schID;
	schedule_t schedule_rec;
	play_t play_rec;
	seat_t seat_rec;
	char choice;

	ticket_list_t head;
	ticket_node_t *pos;
	Pagination_t paging;
    seat_list_t seatList;


	List_Init(head, ticket_node_t);
	List_Init(seatList,seat_node_t);
	paging.offset = 0;
	paging.pageSize = TICKET_PAGE_SIZE;

	//��������
	paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_id);
	Paging_Locate_FirstPage(head, paging);


    //�������ݣ��ݳ��ƻ�����λ����Ŀ��
	Schedule_Srv_FetchByID(schedule_id,&schedule_rec);
    // ���������λ
    if(Seat_Srv_FetchByRoomID(seatList, schedule_rec.studio_id)<=0){
        printf("\t\t\tδ������λ��  ( ��-�� )\n");
        return ;
    }



	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);

	do {
        system("cls");
		printf("\n=======================================================\n");
		printf("****************     �ݳ�Ʊ��Ϣ��    ****************\n");
		printf("ID\t\t��Ŀ����\t\t��λ���� \t��λ����\t ����\tʱ��\t\tƱ��\tƱ��״̬\n");
		printf("-------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (ticket_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
            Seat_Srv_FetchByID(pos->data.seat_id, &seat_rec);
			printf("%d\t%s\t%d\t%d\t%d-%d-%d\t%d:%d:%d\t%d\t%s\n", pos->data.id, play_rec.name,
					seat_rec.row, seat_rec.column,
					schedule_rec.date.year,schedule_rec.date.month,
					schedule_rec.date.day,schedule_rec.time.hour,
					schedule_rec.time.minute,schedule_rec.time.second,pos->data.price,
					pos->data.status==0?"δ��":"���۳�");
			pos = pos->next;
		}
		printf(
				"==  ��  �� :%d =========================== ҳ %d/%d ==\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("[P]��һҳ|[N]��һҳ | [B]��Ʊ | [R]���� ");
		fflush(stdin);

		printf("\t\t\t����������ѡ��");
		scanf("%c", &choice);
		fflush(stdin);
		switch (choice) {
		case 'b':
		case 'B':
			printf("\t\t\tI������Ʊ�� ID:");
			scanf("%d", &id);
			Sale_UI_SellTicket(head, seatList);	//������������
				paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_rec.id);
				List_Paging(head, paging, ticket_node_t);

			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, ticket_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, ticket_node_t);
			}
			break;
		case 'r':
		case 'R':
			break; 
		}
    printf("�����������...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, ticket_node_t);

}

// ��Ʊ
inline int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList){


    int row,conlum;
    int money;
    ticket_node_t *ticket =NULL;
    printf("\t\t\t��ȷ����ѡ�����λ�����кţ�");
    scanf("%d%d",&row,&conlum);
    seat_node_t * seat =Seat_Srv_FindByRowCol(seatList,row,conlum);

    if(NULL == seat){
        printf("\t\t\t����λ�����ڣ�����������ά��...");
        return 0;
    }


     ticket =  Ticket_Srv_FindBySeatID(tickList, seat->data.id);

    if(ticket==NULL){
        printf("��Ʊ������  (x___x)\n");
        return 0;
    }

    if(ticket->data.status == TICKET_SOLD){
        printf("��Ʊ�Ѿ��۳���...  ( ��-�� )\n");
        return 0;
    }

    printf("��ƱĿǰ��״̬Ϊ��%s\n",ticket->data.status==0?"����":"����");
    printf("\t\t\t����Ҫ֧����%dԪ\n",ticket->data.price);
    printf("\t\t\t��������֧����Ǯ����");
    scanf("%d",&money);
    if(money < ticket->data.price){
        printf("\t\t\tǮ������....  (x___x)\n");
        return;
    }

    ticket->data.status = TICKET_SOLD;
    Ticket_Srv_Modify(&ticket->data);


    sale_t saleTicket;

    // ����


    saleTicket.ticket_id = ticket->data.id;
    saleTicket.time = TimeNow();
    saleTicket.date = DateNow();
    saleTicket.type = SALE_SELL;
    saleTicket.user_id = gl_CurUser.id;
    saleTicket.value = ticket->data.price;
    saleTicket.id = EntKey_Srv_CompNewKey("Sale");

    Sale_Srv_Add(&saleTicket);
	printf("\n\t\t\t==========================================================\n");
    printf("\t\t\t֧���ɹ���ӰƱ��Ϣ���Զ�����!\n");
    printf("\t\t\t��ƱĿǰ��״̬Ϊ��%s\n",ticket->data.status==0?"����":"����");
	return ticket->data.seat_id;

}

//���ݾ�ĿID��ʾ�ݳ��ƻ�
void Sale_UI_ShowScheduler(int playID) {


    const int SALESANALYSIS_PAGE_SIZE = 5; // ����ҳ���С
    int id;
    char choice;
    Pagination_t paging;
    play_t buf;
    schedule_list_t head;

    schedule_node_t *pos;
    //����ҳ���С
    paging.pageSize = SALESANALYSIS_PAGE_SIZE;
    paging.offset = 0;


    Play_Srv_FetchByID(playID,&buf);

    List_Init(head,schedule_node_t);
    // ���ݾ�Ŀ��ȡ��Ŀ����
    int count = Schedule_Srv_FetchByPlay(head,buf.id);
    if(!count){
        printf("δ���ݳ����ţ�");
        return ;
    }




    paging.totalRecords = count;
	Paging_Locate_FirstPage(head, paging);

    // �޸�
	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("********************** Schedule Info List **********************\n");
		printf("%5s  %5s  %5s  %18s  %18s  %5s\n", "ID", "��ĿID", "�ݳ���ID",
				"��ӳ����", "��ӳʱ��","��λ��");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		int i;
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%2d  %2d  %2d    %4d-%02d-%02d   %02d:%02d:%02d  %d\n", pos->data.id,
					pos->data.play_id,pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day, pos->data.time.hour,
					pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
			pos = pos->next;
		}
		printf("\n\t\t\t------------ �� %2d ��-------------------- %2d/%2d ҳ---------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ   [N]��һҳ    [T]Ʊ       [R]����");
		printf(
				"\n==================================================================\n");
		printf("����������ѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
        printf("%c",choice);
		case 't':
		case 'T':

		    Sale_UI_SellTicketBySchID();
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

void Sale_UI_MgtEntry() {

    static const int PLAY_PAGE_SIZE = 5;

    int i, id;
	char choice;

	play_list_t head;
	play_node_t *post;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;

	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("\n****************     ��Ŀ��Ϣ��     ***********\n");
		printf("ID\t\t��Ŀ����\t\t ����\t  ����\t\t ��ӳ����\t  ��������\t  Ʊ��\n");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, post = (play_node_t *) (paging.curPos);
				post != head && i < paging.pageSize; i++) {
			printf("%d\t\t%s\t\t%s\t%s\t%d-%d-%d\t%d-%d-%d\t%d\n", post->data.id,
								post->data.name,	post->data.area,
								(post->data.rating==1?"��ͯ   ":(post->data.rating==2?"������":"����   ")),
								post->data.start_date.year,post->data.start_date.month,
								post->data.start_date.day,post->data.end_date.year,
								post->data.end_date.month,post->data.end_date.day,post->data.price);
			post = post->next;
		}
		printf("-------  ��  �� :%2d ----------------------- ҳ %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ  [N]��һҳ   [C]��ѯ�ݳ��ƻ�    [R]����");
		printf(
				"\n==================================================================\n");
		printf("���������ѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'c':
		case 'C':
		    printf("\t\t\t��������Ҫ�ۿ��ĵ�ӰID��");
			scanf("%d", &id);
			Sale_UI_ShowScheduler(id); //����ӳɹ����������һҳ��ʾ

            paging.totalRecords = Play_Srv_FetchAll(head);
            Paging_Locate_LastPage(head, paging, play_node_t);

			break;
		case 's':
		case 'S':
			if (Play_UI_FetchByName(head)){	//������������
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'f':
		case 'F':
			printf("������ID:");
			scanf("%d", &id);
			if (1) {	//������������
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
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

//��Ʊ
void Sale_UI_ReturnTicket(){

    int id;
    ticket_t buf;
    printf("\t\t\t����Ʊ��ID");
    scanf("%d",&id);
    if(!Ticket_Srv_FetchByID(id,&buf)){
        printf("\t\t\t��Ʊ������  (x___x)\n");
        return ;
    }
    if(buf.status != TICKET_SOLD){
        printf("\t\t\t��Ʊ��û���۳��أ�");
        return;
    }

    buf.status = TICKET_AVL;
    // �޸�Ʊ��״̬
    Ticket_Srv_Modify(&buf);
    printf("\t\t\t��Ʊ�ɹ�...  ( ��-�� )\n");
    printf("\t\t\t��ƱĿǰ��״̬Ϊ��%s\n",buf.status==0?"����":"����");

    sale_t refond;

    refond.date = DateNow();
    refond.id = EntKey_Srv_CompNewKey("Sale");
    refond.ticket_id = buf.id;
    refond.time =TimeNow();
    refond.type = SALE_RETURN;
    refond.user_id = gl_CurUser.id;
    refond.value = -buf.price;

    // ��¼���������ۼ�¼
    Sale_Srv_Add(&refond);
}


void Sale_UI_SellTicketBySchID()
{
    schedule_t data;
    seat_list_t seatList;
    ticket_list_t ticketList;

    List_Init(seatList,seat_node_t);
    List_Init(ticketList,ticket_node_t);

    int id;
    //���schedule ������Ʊ
    printf("\t\t\t��������Ҫѡ��鿴�Ǹ��ݳ��ƻ��µ�Ʊ:");
    scanf("%d",&id);
    if(!Schedule_Srv_FetchByID(id, &data)){
        printf("\t\t\tû�д��ݳ��ƻ�id\n");
        return;
    }


    int ticket_count = Ticket_Srv_FetchBySchID(ticketList, data.id);
    if(ticket_count<=0){
        printf("��û��Ʊ��");
        return ;
    }



    int seat_count = Seat_Srv_FetchByRoomID(seatList, data.studio_id);
    if(seat_count<=0){
        printf("δ������λ��  (x___x)");
        return ;
    }
    Sale_UI_ShowTicket(id);
    //Sale_UI_SellTicket(ticketList, seatList);

}
//�鿴������¼ 
/*void Sale_UI_ShowSaleRecord()
{
    int usrID = 0, i;
    char choice;
    salesanalysis_node_t *sale_head = NULL, *sale_temp = NULL;

    printf("\n==================================================================\n");
    printf("***************************** ���ۼ�¼ *****************************\n");
    printf("------------------------------------------------------------------\n");
    printf("%5s  %-10s  %-10s  %-10s  %-8s  %-12s  %-12s  %-10s\n", "ID", "��ƱԱID", "��ӰID", "��ӳ��ID", "Ʊ��", "��Ʊʱ��", "ӰƬ�۸�", "���۶�");
    printf("------------------------------------------------------------------\n");

    sale_head = SalesAnalysis_Srv_FindAllByUsrID(usrID);
    if (!sale_head)
    {
        printf("\t\tû�����ۼ�¼��\n");
    }
    else
    {
        sale_temp = sale_head;
        while (sale_temp != NULL)
        {
            printf("%5d  %10d  %10d  %10d  %8d  %4d-%02d-%02d %2d:%02d:%02d  %10.2f  %10.2f\n", sale_temp->data.id, sale_temp->data.user_id, sale_temp->data.movie_id, sale_temp->data.hall_id, sale_temp->data.ticket_count, sale_temp->data.date.year, sale_temp->data.date.month, sale_temp->data.date.day, sale_temp->data.time.hour, sale_temp->data.time.minute, sale_temp->data.time.second, sale_temp->data.unit_price, sale_temp->data.total_price);
            sale_temp = sale_temp->next;
        }
        printf("------------------------------------------------------------------\n");
        printf("[P]rint the records with charts, [R]eturn:");
        fflush(stdin);
        scanf("%c", &choice);
        if ('p' == choice || 'P' == choice)
        {
            Sale_UI_MgtEntry(3);
        }
    }
    printf("\n�����������...");
    getchar();
}
*/

