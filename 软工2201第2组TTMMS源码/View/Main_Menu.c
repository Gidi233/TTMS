#include <stdio.h>
#include<conio.h> 
#include "Main_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "Queries_Menu.h"
#include "Account_UI.h"
#include "Sale_UI.h"
#include "SalesAnalysis_UI.h"


account_t gl_CurUser;
void Main_Menu(void) {
	char choice;
	int id;
	do {
        system("cls");
		printf("\n==================================================================\n");
	printf("**********************( ~^票务管理信息^~ ) **********************\n");
		printf("[S]tudio Management. [演出厅管理]\n");
		printf("[P]lay Management. [剧目管理]\n");
		printf("[T]icket Sale. [售票]\n");
		printf("[R]eturn Ticket. [退票]\n");
		printf("[Q]ueries [查询]\n");
		printf("Ra[n]king and Statistics. [排行和统计]\n");
		printf("[A]ccount Management. [账户管理]\n");
		printf("[E]xist. [退出系统]\n");
		printf("[Y]Schedule. [演出计划管理]\n");
		
		printf("\n==================================================================\n");
		printf("Please input your choice [请选择]:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's':
			//Studio_UI_MgtEntry();
			Schedule_UI_ListAll();
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(0);
			break;
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'T':
		case 't':
			Sale_UI_MgtEntry();
			break;
		case 'R':
		case 'r':
			Sale_UI_ReturnTicket();
			break;
		case 'N':
		case 'n':
			SalesAanalysis_UI_MgtEntry();
			break;
		case 'A':
		case 'a':
            Account_UI_MgtEntry();
			break;
		case 'y':
		case 'Y':
		    printf("请输入play的ID：");
		    scanf("%d",&id);
			Schedule_UI_MgtEntry(id);
			//Schedule_UI_ListAll();
			break;



		}
    printf("按任意键继续...");
    getchar();getchar();
	} while ('E' != choice && 'e' != choice);
}

 
void Main_MenuFirst(){

    system("color 0F");
	printf("\t\t\t   ----------------------------------------------------------------------------\n");
	printf("\t\t\t                                 电影院票务管理系统                            \n");
	printf("\t\t\t    ********************                                    ********************\n"
	       "\t\t\t    *                  *                                    *                  *\n"
		   "\t\t\t    * 1.系统管理员登录 *                                    * 2.电影院经理登录 *\n"
		   "\t\t\t    *                  *                                    *                  *\n"
		   "\t\t\t    ********************                                    ********************\n"
		   
		   
		   "\t\t\t    ********************                                    ********************\n"
		   "\t\t\t    *                  *                                    *                  *\n"
		   "\t\t\t    * 3.售票员登录     *                                    *  0.退出系统      *\n"
		   "\t\t\t    *                  *                                    *                  *\n"
		   "\t\t\t    ********************                                    ********************\n");

}

void Menu_UI_AdminFunction()
{
    system("color 0F");
	printf("\n\n					**************系统管理员界面*************\n");
	printf("\n					0 .返回上一层");
	printf("\n					1 .管理演出厅");
	printf("\n					2 .管理系统用户");
	printf("\n					3 .设置座位");
	printf("\n					4 .修改密码\n");
	printf("\n					======================================\n");
	printf("				       ---------------------------------------\n");

}
void Menu_UI_MangFunction()
{
    system("color 0F");
	printf("\n\n					**************剧目经理界面*************\n");
    printf("\n					0 .返回上一层"); 
	printf("\n					1 .管理剧目");
	printf("\n					2 .安排演出");
	printf("\n					3 .查询演出");
	printf("\n					4 .查询演出票");
	printf("\n					5 .统计销售额");
	printf("\n					6 .统计票务");
	printf("\n					7 .修改密码\n");
	printf("\n					====================================\n");
	printf("				       ---------------------------------------\n");

}
void Menu_UI_ClerkFunction()
{
    system("color 0F");
	printf("\n\n					**************售票员界面*************\n");
	printf("\n					 0 .返回上一层");
	printf("\n				 	 1 .查询演出");
	printf("\n					 2 .查询演出票");
	printf("\n				 	 3 .售票");
	printf("\n					 4 .退票");
	printf("\n					 5 .统计销售额");
	printf("\n					 6 .修改密码\n");
	printf("\n					======================================\n");
	printf("				       ---------------------------------------\n");

}

int SysLoginTemp()
{
    Account_Srv_InitSys();
     // 新链表结点
    account_list_t pNew;
   // 用于接收密码的变量
   char ch;
   int count=0;
   char username[20];
   char password[20];
    account_t buf;
    printf("请输入登录账号：\n");
    scanf("%s",username);
    printf("请输入登录密码：\n");
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
        Account_Srv_FetchByName(username, &buf);
        strcpy(gl_CurUser.username , buf.username);
        strcpy(gl_CurUser.password , buf.password);
        gl_CurUser.id = buf.id;
        gl_CurUser.type = buf.type;

        return 1;
    }

	return 0;

}

void Main_UI_First()//???? 
{
    int cnt = 0;
    char choice;
	do {
		system("cls");
		Main_MenuFirst();
		fflush(stdin);
        printf("请选择功能(0-3):");
		scanf("%c", &choice);
		if(choice=='0'){
            printf("欢迎再次使用>>>\n");
            return;
		}
		if(choice!='1'&&choice!='2'&&choice!='3'){
            continue;
		}
		fflush(stdin);
		//登录界面
		if(SysLoginTemp()==0){
            cnt++;
            printf("账号或密码输入错误(%d次)\n",cnt);
            if(cnt==3){
                printf("系统强制退出");
                return;
            }
            getchar();
            continue;
		}
		switch (choice) {
		case '1' :
		    if(gl_CurUser.type == USR_ADMIN){
                printf("登录成功!  ( ^___^ )y");
                 Main_UI_AdminFunction();
		    }else{
                printf("该账号没有权限...\n");
            }

			break;
		case '2':
		    if(gl_CurUser.type == USR_MANG){
                printf("登录成功!  ( ^___^ )y");
                Main_UI_MangFunction();
		    }else{
                printf("该账号没有权限...\n");
		    }

			break;
		case '3':
		    if(gl_CurUser.type == USR_CLERK){
                printf("登录成功!  ( ^___^ )y");
                Main_UI_ClerkFunction();
		    }else{
                printf("该账号没有权限...\n");
		    }

			break;
		case '0': ;

            break;
		}
	} while (choice != '0');
}



void Main_UI_AdminFunction()
{
    char choice;
    int roomID;
	do {
		system("cls");
		Menu_UI_AdminFunction();
		fflush(stdin);
		 printf("请选择功能(0-4):");
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1' :
		    Studio_UI_MgtEntry();
			break;
		case '2':
            Account_UI_MgtEntry();
			break;
		case '3':
		    printf("请输入要设置座位的演出厅ID：");
		    scanf("%d",&roomID);
            Seat_UI_MgtEntry(roomID);
			break;
        case '4' :
            ModifyByUsname();
            break;
		case '0': 
	    	choice = '0';
		}
    printf("按任意键继续....");
    getch();
	} while (choice != '0');

}


void Main_UI_MangFunction()
{
    char choice;
    int id;
	do {
		system("cls");
		Menu_UI_MangFunction();
		fflush(stdin);
		 printf("请选择功能(0-7):");
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1' :
            Play_UI_MgtEntry(0);
			break;
		case '2':
           printf("请输入剧目的ID：");
		    scanf("%d",&id);
			Schedule_UI_MgtEntry(id);
			break;
		case '3':
            Schedule_UI_ListAll();
			break;
        case '4':
           ListTickets();
			break;
        case '5':
           StaSales_UI_MgtEntry (gl_CurUser);
			break;
		case '6':
           SalesAanalysis_UI_MgtEntry();
			break;
         case '7' :
            ModifyByUsname();
            break;
		case '0': ;
            break;
		}
    printf("按任意键继续....");
    getchar();
	} while (choice != '0');

}



void Main_UI_ClerkFunction()
{
    char choice;
    int schID;
	do {
		system("cls");
		Menu_UI_ClerkFunction();
		fflush(stdin);
		 printf("请选择功能(0-6):");
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1' :
             Schedule_UI_ListAll();
			break;
		case '2':
		    printf("请输入演出厅ID：");
		    scanf("%d",&schID);
            Sale_UI_ShowTicket(schID);
			break;
		case '3':
            Sale_UI_MgtEntry();//售票
			break;
		case '4':
            Sale_UI_ReturnTicket();//退票
            break;
        case '5':
            StaSales_UI_MgtEntry(gl_CurUser);//统计销售..
            break;
         case '6' :
            ModifyByUsname();
            break;
        case '0':;
            break;
		}
    printf("按任意键继续....");
    getchar();getchar();
	} while (choice != '0');

}

void ModifyByUsname()
{

   char password1[33];
   char password[33];
   printf("请输入原始密码：");
   scanf("%s",password1);

   if(!Account_Srv_Verify(gl_CurUser.username, password1)){
    printf("密码输入错误,修改失败！\n");
    return ;
  }

   printf("请输入您的新密码（请务必要牢记密码）：");
   scanf("%s",password);


   account_t data;

   strcpy(data.username,gl_CurUser.username);
   strcpy(data.password,password);
   data.type = gl_CurUser.type;
   data.id = gl_CurUser.id;

   if(Account_Srv_Modify(&data)){
    printf("修改成功！  ( ^___^ )y\n");

   }else{
        printf("修改失败...\n");
   }
}
