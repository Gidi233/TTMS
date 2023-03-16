
#include "SalesAnalysis_UI.h"
#include "../Common/list.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include <conio.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//外部变量gl_CurUser 用于存储登录用户信息
account_t gl_CurUser;

//定义分页机制中页面大小为5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//剧院销售排行榜函数,降序显示截止目前剧院电影票房排行榜
void SalesAnalysis_UI_BoxOffice() {

}

//显示售票员在给定日期区间的售票情况
void SalesAnalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate){





}

//销售分析入口函数，显示菜单，菜单包含"降序显示截止目前剧院电影票房排行榜"，“显示或查询当日售票员售票情况”，
//查询给定日期区间某售票员售票情况
void SalesAanalysis_UI_MgtEntry() {
    int i, id;
	char choice;

	salesanalysis_list_t head;
	salesanalysis_node_t *pos;
	Pagination_t paging;

	List_Init(head, salesanalysis_node_t);
	paging.offset = 0;
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;

	//载入数据 获取分析票房的链表
	//分页的总记录数
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	//降序排序
	SalesAnalysis_Srv_SortBySale(head);

	Paging_Locate_FirstPage(head, paging);



	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("**********************( ~^票房排行榜^~ ) **********************\n");
		printf("%5s  %18s  %18s  %10s  %10s %10s %10s %9s %9s\n", "剧目ID", "剧目名称", "剧目区域",
				"剧目时长", "剧目上座数量","剧目票房","剧目票价","剧目上映日期","剧目下映日期");
		printf("------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (salesanalysis_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18s  %18s  %10d  %10d %10d %10d %5d-%2d-%2d %5d-%2d-%2d\n", pos->data.play_id,
					pos->data.name, pos->data.area, pos->data.duration,
					pos->data.totaltickets,pos->data.sales,pos->data.price,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
		}
		printf("\t\t\t---------- 共 %2d 项 --------------------------- 第 %2d/%2d 页 --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
				printf(
				"\t[P]上一页  [N]下一页  [R]返回\n");
		printf(
				"\n==================================================================\n");
		printf("请输入你的选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
			}
			break;
		}
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, salesanalysis_node_t);

}


//统计销售额界面
void StaSales_UI_MgtEntry (account_t CurUser){

        strcpy(gl_CurUser.username , CurUser.username);
        strcpy(gl_CurUser.password , CurUser.password);
        gl_CurUser.id = CurUser.id;
        gl_CurUser.type = CurUser.type;

        //printf("%s----%s\n",gl_CurUser.username,CurUser.username);

        getchar();
        getchar();

        char choice;

	do {
		system("cls");
		printf("\n==================================================================\n");

        printf(
				"2.统计个人销售额...\n");

		printf("------------------------------------------------------------------\n");
		//显示数据
		printf(
				"******************************************************************\n");
		printf(
				"\n[1]:查询任意售票员营业额 ;	[2]查询自身营业额 ;	[R]退出\n");
		printf(
				"\n==================================================================\n");
		printf("请输入你的选择");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);


		switch (choice) {
		case '1':
		     if(gl_CurUser.type == USR_MANG){
                StaSales_UI_Clerk();
		     }else{
                printf("\t\t\t抱歉，该账号没有权限...  (x___x)\n");
		     }


			break;

		case '2':
		    if(gl_CurUser.type == USR_CLERK){
                StaSales_UI_Self();
		     }else{
                printf("\t\t\t抱歉，该账号没有权限...  (x___x)\n");
		     }
			break;

		}
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
}

//统计个人销售额界面
void StaSales_UI_Self(){

    int id = gl_CurUser.id;
    user_date_t curdate,startdata,enddate;

    curdate = DateNow();
    startdata.year = curdate.year;
    startdata.month = curdate.month;
    startdata.day = 1;

    enddate.year = curdate.year;
    enddate.month = curdate.month;
    enddate.day = 31;

    int moneyByMySelf = 0;

	char choice;

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("********************** Projection Room List **********************\n");
		//printf("1.查询当日的销售统计");
		printf("\t\t\t1.查询当月的销售统计");
		printf("------------------------------------------------------------------\n");
		//显示数据
        printf("--------------------------[0]返回上一页-------------------------------\n");
		printf(
				"******************************************************************\n");
		printf(
				"\n==================================================================\n");
		printf("请输入你的选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1':
			moneyByMySelf = StaSales_Srv_CompSaleVal(id, startdata, enddate);
			printf("\t\t\t您今日的销售额为：%d\n",moneyByMySelf);
			break;
		/*case '2':

            printf("正在开发中.....\n");


			break;*/
		}
    printf("按任意键继续.....");
    getchar();getchar();
	} while (choice != '0' );

}

//统计售票员销售额界面
void StaSales_UI_Clerk(){

    int id,money;
    char Usrname[33];
    user_date_t startdata,enddate;
    account_t user ;
    printf("\t\t\t请输入售货员姓名：");
    scanf("%s",Usrname);
    if(!Account_Srv_FetchByName(Usrname,&user)){
        printf("\t\t\t该用户名信息不存在！\n");
        return;
    }
    if(user.type != USR_CLERK){
        printf("\t\t\t抱歉，该用户不是售票员...  ( ●-● )\n");
        return;
    }

    id = user.id;
    printf("\t\t\t请输入起始时间（年 月 日（中间有空格））:");
    scanf("%d%d%d",&startdata.year,&startdata.month,&startdata.day);

    printf("\t\t\t请输入末了时间（年 月 日（中间有空格））:");
    scanf("%d%d%d",&enddate.year,&enddate.month,&enddate.day);

    money = StaSales_Srv_CompSaleVal(id,  startdata, enddate);
    if(money==-1){
            printf("\t\t\t目前还没有销售记录...  ( ●-● )\n");
        return;
    }

    printf("\t\t\t销售员：%s的销售额为：%d\n",Usrname,money);
    return;
}

