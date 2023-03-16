#include "Account_UI.h"
#include "../Common/list.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
#include<windows.h>
#include <stdio.h>
#include<assert.h>
#include<string.h>
#include <unistd.h>
#include <sys/time.h>
extern account_t gl_CurUser;
static const int ACCCOUNT_PAGE_SIZE = 5;
//登录函数，提示用户输入用户名和密码，登录成功return 1，否则提示重新登录，超过3次，登录失败
int SysLogin() {

     // 新链表结点
    account_list_t pNew;
   // 用于接收密码的变量
   char ch;
   int count=0;
   char username[20];
   char password[20];
    account_t buf;
    printf("\t\t\t========================\n");
	printf("\n");
	printf("\t\t\t   请输入账号：");
    scanf("%s",username);
   	printf("\t\t\t   请输入密码：");
   	printf("\t\t\t========================\n");
    getchar();
    while(ch=getch()){
        if(ch=='\r'){
            printf("\n");
            break;
        }
        if(ch=='\b'){
            printf("\b \b");
            count--;
        }else{
            password[count++]=ch;
            printf("*");
        }

    }
    password[count]=0;

    if(Account_Srv_Verify(username,password)){
        printf("登录成功！\n");
        Account_Srv_FetchByName(username, &buf);
        strcpy(gl_CurUser.username , buf.username);
        strcpy(gl_CurUser.password , buf.password);
        gl_CurUser.id = buf.id;
        gl_CurUser.type = buf.type;
        return 1;
    }

	return 0;

}

//管理系统用户功能入口函数，显示用户账号管理菜单
void Account_UI_MgtEntry() {
    int i, id;
	char choice;
    char username[30];
	account_list_t head;
	account_node_t *pos;
	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCCOUNT_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("**********************( ~^用户信息^~ ) **********************\n");
		printf("%5s  %18s  %10s  %10s  \n", "ID", "用户类型","姓名", "密码"
				);
		printf("------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (account_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18d  %10s  %10s  \n", pos->data.id,
					 pos->data.type, pos->data.username,
					pos->data.password);
			pos = pos->next;
		}
		printf("-\t\t\t------------------ 第 %2d/%2d 页---------------------\n",
				 Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页  [N]下一页   [A]添加  [D]删除   [U]更新  [S]查询  [R]返回上一页");
		printf(
				"\n==================================================================\n");
		printf("\t\t\t输入功能前的字母，选择功能:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(head)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
            printf("\t\t\t输入要删除的用户名:");
			scanf("%s", username);
			if (Account_UI_Delete(head,username)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			 printf("\t\t\t输入要修改的用户名:");
			scanf("%s", username);
			if (Account_UI_Modify(head ,username)) {	//从新载入数据
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 's':
		case 'S':
			 printf("\t\t\t请输入用户姓名:");
			scanf("%s", username);
			if(Account_UI_Query(head ,username)){

            paging.totalRecords = Account_Srv_FetchAll(head);
			List_Paging(head, paging, account_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
    printf("按任意键继续...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, account_node_t);



}


//添加一个用户账号信息，如果账号名存在，提示出错信息
int Account_UI_Add(account_list_t list ) {
     // 新链表结点
    account_list_t pNew;
   // 用于接收密码的变量
   char ch;
   int count=0;
   char password[20];
   int type;

    pNew = (account_list_t)malloc(sizeof(account_node_t));

    printf("\t\t\t请输入新的账号：\n");
    scanf("%s",pNew->data.username);


    if(Account_Srv_FindByUsrName(list,pNew->data.username)!=NULL){
        printf("\t\t该账号已经被注册！注册失败！ (x___x)\n");
        free(pNew);
        return 0;

    }


    printf("\t\t\t请输入新的密码：\n");
    getchar();
    while(ch=getch()){
        if(ch=='\r'){
            printf("\n");
            break;
        }
        if(ch=='\b'){
            printf("\b \b");
            count--;
        }else{
            password[count++]=ch;
            printf("*");
        }

    }
    password[count]=0;
    strcpy(pNew->data.password,password);

    //scanf("%s",pNew->data.pwd);
  //  printf("请输入新账号的id：\n");
   // scanf("%d",&pNew->data.id);


    printf("请输入新账号的类型：(1.售票员 2.剧院经理 9.系统管理员)\n");
    scanf("%d",&type);
    if(type==1){
        pNew->data.type = USR_CLERK;
    }else if(type == 2){
        pNew->data.type = USR_MANG;
    }else if(type == 9){
        pNew->data.type = USR_ADMIN;
    }else if(type==0){
        pNew->data.type = USR_ANOMY;
    }else{
        printf("类型输入错误,自动退出此功能~\n");
        free(pNew);
        return ;
    }

    pNew->data.id = EntKey_Srv_CompNewKey("Account");

    Account_Srv_Add(&pNew->data);
    List_AddTail(list,pNew);
    printf("注册成功！( ^___^ )y\n");
	return 1;

}

//根据用户账号名修改用户账号密码，不存在这个用户账号名，提示出错信息
int Account_UI_Modify(account_list_t list ,char usrName[]) {


    account_list_t p = Account_Srv_FindByUsrName(list,usrName);
    if(p==NULL){
        printf("\t\t\t该账号不存在！ (x___x)\n");
        return 0;

    }

    //printf("%s %s\n",p->data.username,p->data.password);
    //int id;
    char pwd[20];
    printf("\t\t\t请输入修改后的用户密码:");
    scanf("%s",pwd);
    strcpy(p->data.password,pwd);

   //Account_Perst_Update

     Account_Srv_Modify(&p->data);
    printf("\t\t\t修改成功  ( ^___^ )y\n");
    return 1;
    }


//根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(account_list_t list ,char usrName[]) {

    account_list_t p = Account_Srv_FindByUsrName(list,usrName);
    if(p==NULL){
        printf("\t\t\t该账号不存在！ (x___x)\n");
        return 0;
    }

    account_list_t pTemp = NULL;

    List_ForEach(list,pTemp)
    {
        if(strcmp(pTemp->data.username,usrName)==0){
                if(Account_Srv_DeleteByID(pTemp->data.id)){

                pTemp->prev->next = pTemp->next;
                pTemp->next->prev = pTemp->prev;
                free(pTemp);
                printf("删除成功");
                return 1;

                }

        }
    }



	return 0;
}

//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息
int Account_UI_Query(account_list_t  list ,char usrName[]) {
    account_list_t pTemp = Account_Srv_FindByUsrName(list,usrName);
    if(pTemp==NULL){
        printf("该账号不存在！ (x___x)\n");
        return 0;
    }

    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		printf("**********************( ~^用户信息^~ ) **********************\n");
		printf("%5s  %18s  %10s  %10s  \n", "ID", "类型","姓名", "密码"
				);
		printf("------------------------------------------------------------------\n");
        printf("%5d  %18d  %10s  %10s  \n", pTemp->data.id,
					 pTemp->data.type, pTemp->data.username,
					pTemp->data.password);
	return 1;
}


