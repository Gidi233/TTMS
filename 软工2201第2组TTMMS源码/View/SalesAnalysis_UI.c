
#include "SalesAnalysis_UI.h"
#include "../Common/list.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include <conio.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//�ⲿ����gl_CurUser ���ڴ洢��¼�û���Ϣ
account_t gl_CurUser;

//�����ҳ������ҳ���СΪ5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//��Ժ�������а���,������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�
void SalesAnalysis_UI_BoxOffice() {

}

//��ʾ��ƱԱ�ڸ��������������Ʊ���
void SalesAnalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate){





}

//���۷�����ں�������ʾ�˵����˵�����"������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�"������ʾ���ѯ������ƱԱ��Ʊ�������
//��ѯ������������ĳ��ƱԱ��Ʊ���
void SalesAanalysis_UI_MgtEntry() {
    int i, id;
	char choice;

	salesanalysis_list_t head;
	salesanalysis_node_t *pos;
	Pagination_t paging;

	List_Init(head, salesanalysis_node_t);
	paging.offset = 0;
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;

	//�������� ��ȡ����Ʊ��������
	//��ҳ���ܼ�¼��
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	//��������
	SalesAnalysis_Srv_SortBySale(head);

	Paging_Locate_FirstPage(head, paging);



	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("**********************( ~^Ʊ�����а�^~ ) **********************\n");
		printf("%5s  %18s  %18s  %10s  %10s %10s %10s %9s %9s\n", "��ĿID", "��Ŀ����", "��Ŀ����",
				"��Ŀʱ��", "��Ŀ��������","��ĿƱ��","��ĿƱ��","��Ŀ��ӳ����","��Ŀ��ӳ����");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (salesanalysis_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18s  %18s  %10d  %10d %10d %10d %5d-%2d-%2d %5d-%2d-%2d\n", pos->data.play_id,
					pos->data.name, pos->data.area, pos->data.duration,
					pos->data.totaltickets,pos->data.sales,pos->data.price,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
		}
		printf("\t\t\t---------- �� %2d �� --------------------------- �� %2d/%2d ҳ --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
				printf(
				"\t[P]��һҳ  [N]��һҳ  [R]����\n");
		printf(
				"\n==================================================================\n");
		printf("���������ѡ��:");
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
    printf("�����������...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, salesanalysis_node_t);

}


//ͳ�����۶����
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
				"2.ͳ�Ƹ������۶�...\n");

		printf("------------------------------------------------------------------\n");
		//��ʾ����
		printf(
				"******************************************************************\n");
		printf(
				"\n[1]:��ѯ������ƱԱӪҵ�� ;	[2]��ѯ����Ӫҵ�� ;	[R]�˳�\n");
		printf(
				"\n==================================================================\n");
		printf("���������ѡ��");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);


		switch (choice) {
		case '1':
		     if(gl_CurUser.type == USR_MANG){
                StaSales_UI_Clerk();
		     }else{
                printf("\t\t\t��Ǹ�����˺�û��Ȩ��...  (x___x)\n");
		     }


			break;

		case '2':
		    if(gl_CurUser.type == USR_CLERK){
                StaSales_UI_Self();
		     }else{
                printf("\t\t\t��Ǹ�����˺�û��Ȩ��...  (x___x)\n");
		     }
			break;

		}
    printf("�����������...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
}

//ͳ�Ƹ������۶����
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
		//printf("1.��ѯ���յ�����ͳ��");
		printf("\t\t\t1.��ѯ���µ�����ͳ��");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
        printf("--------------------------[0]������һҳ-------------------------------\n");
		printf(
				"******************************************************************\n");
		printf(
				"\n==================================================================\n");
		printf("���������ѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1':
			moneyByMySelf = StaSales_Srv_CompSaleVal(id, startdata, enddate);
			printf("\t\t\t�����յ����۶�Ϊ��%d\n",moneyByMySelf);
			break;
		/*case '2':

            printf("���ڿ�����.....\n");


			break;*/
		}
    printf("�����������.....");
    getchar();getchar();
	} while (choice != '0' );

}

//ͳ����ƱԱ���۶����
void StaSales_UI_Clerk(){

    int id,money;
    char Usrname[33];
    user_date_t startdata,enddate;
    account_t user ;
    printf("\t\t\t�������ۻ�Ա������");
    scanf("%s",Usrname);
    if(!Account_Srv_FetchByName(Usrname,&user)){
        printf("\t\t\t���û�����Ϣ�����ڣ�\n");
        return;
    }
    if(user.type != USR_CLERK){
        printf("\t\t\t��Ǹ�����û�������ƱԱ...  ( ��-�� )\n");
        return;
    }

    id = user.id;
    printf("\t\t\t��������ʼʱ�䣨�� �� �գ��м��пո񣩣�:");
    scanf("%d%d%d",&startdata.year,&startdata.month,&startdata.day);

    printf("\t\t\t������ĩ��ʱ�䣨�� �� �գ��м��пո񣩣�:");
    scanf("%d%d%d",&enddate.year,&enddate.month,&enddate.day);

    money = StaSales_Srv_CompSaleVal(id,  startdata, enddate);
    if(money==-1){
            printf("\t\t\tĿǰ��û�����ۼ�¼...  ( ��-�� )\n");
        return;
    }

    printf("\t\t\t����Ա��%s�����۶�Ϊ��%d\n",Usrname,money);
    return;
}

