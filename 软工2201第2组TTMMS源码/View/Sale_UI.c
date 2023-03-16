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

//引用登陆用户的全局数据结构
extern account_t gl_CurUser;


//根据计划ID，显示演出票
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

	//载入数据
	paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_id);
	Paging_Locate_FirstPage(head, paging);


    //载入数据（演出计划、座位、剧目）
	Schedule_Srv_FetchByID(schedule_id,&schedule_rec);
    // 获得所有座位
    if(Seat_Srv_FetchByRoomID(seatList, schedule_rec.studio_id)<=0){
        printf("\t\t\t未生成座位！  ( ●-● )\n");
        return ;
    }



	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);

	do {
        system("cls");
		printf("\n=======================================================\n");
		printf("****************     演出票信息表    ****************\n");
		printf("ID\t\t剧目名称\t\t座位行数 \t座位列数\t 日期\t时间\t\t票价\t票的状态\n");
		printf("-------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (ticket_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
            Seat_Srv_FetchByID(pos->data.seat_id, &seat_rec);
			printf("%d\t%s\t%d\t%d\t%d-%d-%d\t%d:%d:%d\t%d\t%s\n", pos->data.id, play_rec.name,
					seat_rec.row, seat_rec.column,
					schedule_rec.date.year,schedule_rec.date.month,
					schedule_rec.date.day,schedule_rec.time.hour,
					schedule_rec.time.minute,schedule_rec.time.second,pos->data.price,
					pos->data.status==0?"未卖":"已售出");
			pos = pos->next;
		}
		printf(
				"==  总  计 :%d =========================== 页 %d/%d ==\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("[P]上一页|[N]下一页 | [B]买票 | [R]返回 ");
		fflush(stdin);

		printf("\t\t\t请输入您的选择：");
		scanf("%c", &choice);
		fflush(stdin);
		switch (choice) {
		case 'b':
		case 'B':
			printf("\t\t\tI请输入票的 ID:");
			scanf("%d", &id);
			Sale_UI_SellTicket(head, seatList);	//从新载入数据
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
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, ticket_node_t);

}

// 售票
inline int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList){


    int row,conlum;
    int money;
    ticket_node_t *ticket =NULL;
    printf("\t\t\t请确定您选择的座位的行列号：");
    scanf("%d%d",&row,&conlum);
    seat_node_t * seat =Seat_Srv_FindByRowCol(seatList,row,conlum);

    if(NULL == seat){
        printf("\t\t\t该座位不存在，可能是正在维修...");
        return 0;
    }


     ticket =  Ticket_Srv_FindBySeatID(tickList, seat->data.id);

    if(ticket==NULL){
        printf("该票不存在  (x___x)\n");
        return 0;
    }

    if(ticket->data.status == TICKET_SOLD){
        printf("该票已经售出了...  ( ●-● )\n");
        return 0;
    }

    printf("该票目前的状态为：%s\n",ticket->data.status==0?"待售":"已售");
    printf("\t\t\t您需要支付：%d元\n",ticket->data.price);
    printf("\t\t\t请输入您支付的钱数：");
    scanf("%d",&money);
    if(money < ticket->data.price){
        printf("\t\t\t钱数不够....  (x___x)\n");
        return;
    }

    ticket->data.status = TICKET_SOLD;
    Ticket_Srv_Modify(&ticket->data);


    sale_t saleTicket;

    // 主键


    saleTicket.ticket_id = ticket->data.id;
    saleTicket.time = TimeNow();
    saleTicket.date = DateNow();
    saleTicket.type = SALE_SELL;
    saleTicket.user_id = gl_CurUser.id;
    saleTicket.value = ticket->data.price;
    saleTicket.id = EntKey_Srv_CompNewKey("Sale");

    Sale_Srv_Add(&saleTicket);
	printf("\n\t\t\t==========================================================\n");
    printf("\t\t\t支付成功！影票信息已自动生成!\n");
    printf("\t\t\t该票目前的状态为：%s\n",ticket->data.status==0?"待售":"已售");
	return ticket->data.seat_id;

}

//根据剧目ID显示演出计划
void Sale_UI_ShowScheduler(int playID) {


    const int SALESANALYSIS_PAGE_SIZE = 5; // 设置页面大小
    int id;
    char choice;
    Pagination_t paging;
    play_t buf;
    schedule_list_t head;

    schedule_node_t *pos;
    //设置页面大小
    paging.pageSize = SALESANALYSIS_PAGE_SIZE;
    paging.offset = 0;


    Play_Srv_FetchByID(playID,&buf);

    List_Init(head,schedule_node_t);
    // 根据剧目获取剧目链表
    int count = Schedule_Srv_FetchByPlay(head,buf.id);
    if(!count){
        printf("未有演出安排！");
        return ;
    }




    paging.totalRecords = count;
	Paging_Locate_FirstPage(head, paging);

    // 修改
	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("********************** Schedule Info List **********************\n");
		printf("%5s  %5s  %5s  %18s  %18s  %5s\n", "ID", "剧目ID", "演出厅ID",
				"放映日期", "放映时间","座位数");
		printf("------------------------------------------------------------------\n");
		//显示数据
		int i;
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%2d  %2d  %2d    %4d-%02d-%02d   %02d:%02d:%02d  %d\n", pos->data.id,
					pos->data.play_id,pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day, pos->data.time.hour,
					pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
			pos = pos->next;
		}
		printf("\n\t\t\t------------ 共 %2d 项-------------------- %2d/%2d 页---------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页   [N]下一页    [T]票       [R]返回");
		printf(
				"\n==================================================================\n");
		printf("请输入您的选项:");
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
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
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

	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("\n****************     剧目信息表     ***********\n");
		printf("ID\t\t剧目名称\t\t 地区\t  类型\t\t 上映日期\t  结束日期\t  票价\n");
		printf("------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, post = (play_node_t *) (paging.curPos);
				post != head && i < paging.pageSize; i++) {
			printf("%d\t\t%s\t\t%s\t%s\t%d-%d-%d\t%d-%d-%d\t%d\n", post->data.id,
								post->data.name,	post->data.area,
								(post->data.rating==1?"儿童   ":(post->data.rating==2?"青少年":"成人   ")),
								post->data.start_date.year,post->data.start_date.month,
								post->data.start_date.day,post->data.end_date.year,
								post->data.end_date.month,post->data.end_date.day,post->data.price);
			post = post->next;
		}
		printf("-------  总  计 :%2d ----------------------- 页 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页  [N]下一页   [C]查询演出计划    [R]返回");
		printf(
				"\n==================================================================\n");
		printf("请输入你的选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'c':
		case 'C':
		    printf("\t\t\t请输入您要观看的电影ID：");
			scanf("%d", &id);
			Sale_UI_ShowScheduler(id); //新添加成功，跳到最后一页显示

            paging.totalRecords = Play_Srv_FetchAll(head);
            Paging_Locate_LastPage(head, paging, play_node_t);

			break;
		case 's':
		case 'S':
			if (Play_UI_FetchByName(head)){	//从新载入数据
				paging.totalRecords = Play_Srv_FetchAll(head);
				List_Paging(head, paging, play_node_t);
			}
			break;
		case 'f':
		case 'F':
			printf("请输入ID:");
			scanf("%d", &id);
			if (1) {	//从新载入数据
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
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, play_node_t);








}

//退票
void Sale_UI_ReturnTicket(){

    int id;
    ticket_t buf;
    printf("\t\t\t输入票的ID");
    scanf("%d",&id);
    if(!Ticket_Srv_FetchByID(id,&buf)){
        printf("\t\t\t该票不存在  (x___x)\n");
        return ;
    }
    if(buf.status != TICKET_SOLD){
        printf("\t\t\t此票还没有售出呢！");
        return;
    }

    buf.status = TICKET_AVL;
    // 修改票的状态
    Ticket_Srv_Modify(&buf);
    printf("\t\t\t退票成功...  ( ●-● )\n");
    printf("\t\t\t该票目前的状态为：%s\n",buf.status==0?"待售":"已售");

    sale_t refond;

    refond.date = DateNow();
    refond.id = EntKey_Srv_CompNewKey("Sale");
    refond.ticket_id = buf.id;
    refond.time =TimeNow();
    refond.type = SALE_RETURN;
    refond.user_id = gl_CurUser.id;
    refond.value = -buf.price;

    // 记录并保存销售记录
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
    //获得schedule 的所有票
    printf("\t\t\t请输入您要选择查看那个演出计划下的票:");
    scanf("%d",&id);
    if(!Schedule_Srv_FetchByID(id, &data)){
        printf("\t\t\t没有此演出计划id\n");
        return;
    }


    int ticket_count = Ticket_Srv_FetchBySchID(ticketList, data.id);
    if(ticket_count<=0){
        printf("还没有票呢");
        return ;
    }



    int seat_count = Seat_Srv_FetchByRoomID(seatList, data.studio_id);
    if(seat_count<=0){
        printf("未生成座位！  (x___x)");
        return ;
    }
    Sale_UI_ShowTicket(id);
    //Sale_UI_SellTicket(ticketList, seatList);

}
//查看订单记录 
/*void Sale_UI_ShowSaleRecord()
{
    int usrID = 0, i;
    char choice;
    salesanalysis_node_t *sale_head = NULL, *sale_temp = NULL;

    printf("\n==================================================================\n");
    printf("***************************** 销售记录 *****************************\n");
    printf("------------------------------------------------------------------\n");
    printf("%5s  %-10s  %-10s  %-10s  %-8s  %-12s  %-12s  %-10s\n", "ID", "售票员ID", "电影ID", "放映厅ID", "票数", "售票时间", "影片价格", "销售额");
    printf("------------------------------------------------------------------\n");

    sale_head = SalesAnalysis_Srv_FindAllByUsrID(usrID);
    if (!sale_head)
    {
        printf("\t\t没有销售记录！\n");
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
    printf("\n按任意键继续...");
    getchar();
}
*/

