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
//��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
int SysLogin() {

     // ��������
    account_list_t pNew;
   // ���ڽ�������ı���
   char ch;
   int count=0;
   char username[20];
   char password[20];
    account_t buf;
    printf("\t\t\t========================\n");
	printf("\n");
	printf("\t\t\t   �������˺ţ�");
    scanf("%s",username);
   	printf("\t\t\t   ���������룺");
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
        printf("��¼�ɹ���\n");
        Account_Srv_FetchByName(username, &buf);
        strcpy(gl_CurUser.username , buf.username);
        strcpy(gl_CurUser.password , buf.password);
        gl_CurUser.id = buf.id;
        gl_CurUser.type = buf.type;
        return 1;
    }

	return 0;

}

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
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

	//��������
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("**********************( ~^�û���Ϣ^~ ) **********************\n");
		printf("%5s  %18s  %10s  %10s  \n", "ID", "�û�����","����", "����"
				);
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (account_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18d  %10s  %10s  \n", pos->data.id,
					 pos->data.type, pos->data.username,
					pos->data.password);
			pos = pos->next;
		}
		printf("-\t\t\t------------------ �� %2d/%2d ҳ---------------------\n",
				 Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]��һҳ  [N]��һҳ   [A]���  [D]ɾ��   [U]����  [S]��ѯ  [R]������һҳ");
		printf(
				"\n==================================================================\n");
		printf("\t\t\t���빦��ǰ����ĸ��ѡ����:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(head)) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
            printf("\t\t\t����Ҫɾ�����û���:");
			scanf("%s", username);
			if (Account_UI_Delete(head,username)) {	//������������
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			 printf("\t\t\t����Ҫ�޸ĵ��û���:");
			scanf("%s", username);
			if (Account_UI_Modify(head ,username)) {	//������������
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 's':
		case 'S':
			 printf("\t\t\t�������û�����:");
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
    printf("�����������...");
    getchar();getchar();
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, account_node_t);



}


//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list ) {
     // ��������
    account_list_t pNew;
   // ���ڽ�������ı���
   char ch;
   int count=0;
   char password[20];
   int type;

    pNew = (account_list_t)malloc(sizeof(account_node_t));

    printf("\t\t\t�������µ��˺ţ�\n");
    scanf("%s",pNew->data.username);


    if(Account_Srv_FindByUsrName(list,pNew->data.username)!=NULL){
        printf("\t\t���˺��Ѿ���ע�ᣡע��ʧ�ܣ� (x___x)\n");
        free(pNew);
        return 0;

    }


    printf("\t\t\t�������µ����룺\n");
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
  //  printf("���������˺ŵ�id��\n");
   // scanf("%d",&pNew->data.id);


    printf("���������˺ŵ����ͣ�(1.��ƱԱ 2.��Ժ���� 9.ϵͳ����Ա)\n");
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
        printf("�����������,�Զ��˳��˹���~\n");
        free(pNew);
        return ;
    }

    pNew->data.id = EntKey_Srv_CompNewKey("Account");

    Account_Srv_Add(&pNew->data);
    List_AddTail(list,pNew);
    printf("ע��ɹ���( ^___^ )y\n");
	return 1;

}

//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
int Account_UI_Modify(account_list_t list ,char usrName[]) {


    account_list_t p = Account_Srv_FindByUsrName(list,usrName);
    if(p==NULL){
        printf("\t\t\t���˺Ų����ڣ� (x___x)\n");
        return 0;

    }

    //printf("%s %s\n",p->data.username,p->data.password);
    //int id;
    char pwd[20];
    printf("\t\t\t�������޸ĺ���û�����:");
    scanf("%s",pwd);
    strcpy(p->data.password,pwd);

   //Account_Perst_Update

     Account_Srv_Modify(&p->data);
    printf("\t\t\t�޸ĳɹ�  ( ^___^ )y\n");
    return 1;
    }


//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {

    account_list_t p = Account_Srv_FindByUsrName(list,usrName);
    if(p==NULL){
        printf("\t\t\t���˺Ų����ڣ� (x___x)\n");
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
                printf("ɾ���ɹ�");
                return 1;

                }

        }
    }



	return 0;
}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t  list ,char usrName[]) {
    account_list_t pTemp = Account_Srv_FindByUsrName(list,usrName);
    if(pTemp==NULL){
        printf("���˺Ų����ڣ� (x___x)\n");
        return 0;
    }

    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		printf("**********************( ~^�û���Ϣ^~ ) **********************\n");
		printf("%5s  %18s  %10s  %10s  \n", "ID", "����","����", "����"
				);
		printf("------------------------------------------------------------------\n");
        printf("%5d  %18d  %10s  %10s  \n", pTemp->data.id,
					 pTemp->data.type, pTemp->data.username,
					pTemp->data.password);
	return 1;
}


