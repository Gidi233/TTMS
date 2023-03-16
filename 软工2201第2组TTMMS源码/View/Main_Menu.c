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
	printf("**********************( ~^Ʊ�������Ϣ^~ ) **********************\n");
		printf("[S]tudio Management. [�ݳ�������]\n");
		printf("[P]lay Management. [��Ŀ����]\n");
		printf("[T]icket Sale. [��Ʊ]\n");
		printf("[R]eturn Ticket. [��Ʊ]\n");
		printf("[Q]ueries [��ѯ]\n");
		printf("Ra[n]king and Statistics. [���к�ͳ��]\n");
		printf("[A]ccount Management. [�˻�����]\n");
		printf("[E]xist. [�˳�ϵͳ]\n");
		printf("[Y]Schedule. [�ݳ��ƻ�����]\n");
		
		printf("\n==================================================================\n");
		printf("Please input your choice [��ѡ��]:");
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
		    printf("������play��ID��");
		    scanf("%d",&id);
			Schedule_UI_MgtEntry(id);
			//Schedule_UI_ListAll();
			break;



		}
    printf("�����������...");
    getchar();getchar();
	} while ('E' != choice && 'e' != choice);
}

 
void Main_MenuFirst(){

    system("color 0F");
	printf("\t\t\t   ----------------------------------------------------------------------------\n");
	printf("\t\t\t                                 ��ӰԺƱ�����ϵͳ                            \n");
	printf("\t\t\t    ********************                                    ********************\n"
	       "\t\t\t    *                  *                                    *                  *\n"
		   "\t\t\t    * 1.ϵͳ����Ա��¼ *                                    * 2.��ӰԺ�����¼ *\n"
		   "\t\t\t    *                  *                                    *                  *\n"
		   "\t\t\t    ********************                                    ********************\n"
		   
		   
		   "\t\t\t    ********************                                    ********************\n"
		   "\t\t\t    *                  *                                    *                  *\n"
		   "\t\t\t    * 3.��ƱԱ��¼     *                                    *  0.�˳�ϵͳ      *\n"
		   "\t\t\t    *                  *                                    *                  *\n"
		   "\t\t\t    ********************                                    ********************\n");

}

void Menu_UI_AdminFunction()
{
    system("color 0F");
	printf("\n\n					**************ϵͳ����Ա����*************\n");
	printf("\n					0 .������һ��");
	printf("\n					1 .�����ݳ���");
	printf("\n					2 .����ϵͳ�û�");
	printf("\n					3 .������λ");
	printf("\n					4 .�޸�����\n");
	printf("\n					======================================\n");
	printf("				       ---------------------------------------\n");

}
void Menu_UI_MangFunction()
{
    system("color 0F");
	printf("\n\n					**************��Ŀ�������*************\n");
    printf("\n					0 .������һ��"); 
	printf("\n					1 .�����Ŀ");
	printf("\n					2 .�����ݳ�");
	printf("\n					3 .��ѯ�ݳ�");
	printf("\n					4 .��ѯ�ݳ�Ʊ");
	printf("\n					5 .ͳ�����۶�");
	printf("\n					6 .ͳ��Ʊ��");
	printf("\n					7 .�޸�����\n");
	printf("\n					====================================\n");
	printf("				       ---------------------------------------\n");

}
void Menu_UI_ClerkFunction()
{
    system("color 0F");
	printf("\n\n					**************��ƱԱ����*************\n");
	printf("\n					 0 .������һ��");
	printf("\n				 	 1 .��ѯ�ݳ�");
	printf("\n					 2 .��ѯ�ݳ�Ʊ");
	printf("\n				 	 3 .��Ʊ");
	printf("\n					 4 .��Ʊ");
	printf("\n					 5 .ͳ�����۶�");
	printf("\n					 6 .�޸�����\n");
	printf("\n					======================================\n");
	printf("				       ---------------------------------------\n");

}

int SysLoginTemp()
{
    Account_Srv_InitSys();
     // ��������
    account_list_t pNew;
   // ���ڽ�������ı���
   char ch;
   int count=0;
   char username[20];
   char password[20];
    account_t buf;
    printf("�������¼�˺ţ�\n");
    scanf("%s",username);
    printf("�������¼���룺\n");
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
        printf("��ѡ����(0-3):");
		scanf("%c", &choice);
		if(choice=='0'){
            printf("��ӭ�ٴ�ʹ��>>>\n");
            return;
		}
		if(choice!='1'&&choice!='2'&&choice!='3'){
            continue;
		}
		fflush(stdin);
		//��¼����
		if(SysLoginTemp()==0){
            cnt++;
            printf("�˺Ż������������(%d��)\n",cnt);
            if(cnt==3){
                printf("ϵͳǿ���˳�");
                return;
            }
            getchar();
            continue;
		}
		switch (choice) {
		case '1' :
		    if(gl_CurUser.type == USR_ADMIN){
                printf("��¼�ɹ�!  ( ^___^ )y");
                 Main_UI_AdminFunction();
		    }else{
                printf("���˺�û��Ȩ��...\n");
            }

			break;
		case '2':
		    if(gl_CurUser.type == USR_MANG){
                printf("��¼�ɹ�!  ( ^___^ )y");
                Main_UI_MangFunction();
		    }else{
                printf("���˺�û��Ȩ��...\n");
		    }

			break;
		case '3':
		    if(gl_CurUser.type == USR_CLERK){
                printf("��¼�ɹ�!  ( ^___^ )y");
                Main_UI_ClerkFunction();
		    }else{
                printf("���˺�û��Ȩ��...\n");
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
		 printf("��ѡ����(0-4):");
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
		    printf("������Ҫ������λ���ݳ���ID��");
		    scanf("%d",&roomID);
            Seat_UI_MgtEntry(roomID);
			break;
        case '4' :
            ModifyByUsname();
            break;
		case '0': 
	    	choice = '0';
		}
    printf("�����������....");
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
		 printf("��ѡ����(0-7):");
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1' :
            Play_UI_MgtEntry(0);
			break;
		case '2':
           printf("�������Ŀ��ID��");
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
    printf("�����������....");
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
		 printf("��ѡ����(0-6):");
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case '1' :
             Schedule_UI_ListAll();
			break;
		case '2':
		    printf("�������ݳ���ID��");
		    scanf("%d",&schID);
            Sale_UI_ShowTicket(schID);
			break;
		case '3':
            Sale_UI_MgtEntry();//��Ʊ
			break;
		case '4':
            Sale_UI_ReturnTicket();//��Ʊ
            break;
        case '5':
            StaSales_UI_MgtEntry(gl_CurUser);//ͳ������..
            break;
         case '6' :
            ModifyByUsname();
            break;
        case '0':;
            break;
		}
    printf("�����������....");
    getchar();getchar();
	} while (choice != '0');

}

void ModifyByUsname()
{

   char password1[33];
   char password[33];
   printf("������ԭʼ���룺");
   scanf("%s",password1);

   if(!Account_Srv_Verify(gl_CurUser.username, password1)){
    printf("�����������,�޸�ʧ�ܣ�\n");
    return ;
  }

   printf("���������������루�����Ҫ�μ����룩��");
   scanf("%s",password);


   account_t data;

   strcpy(data.username,gl_CurUser.username);
   strcpy(data.password,password);
   data.type = gl_CurUser.type;
   data.id = gl_CurUser.id;

   if(Account_Srv_Modify(&data)){
    printf("�޸ĳɹ���  ( ^___^ )y\n");

   }else{
        printf("�޸�ʧ��...\n");
   }
}
