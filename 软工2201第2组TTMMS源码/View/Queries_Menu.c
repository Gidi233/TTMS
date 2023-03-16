#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"

void Queries_Menu(void){
	char choice;
	do {
        system("cls");
		printf("\n==================================================================\n");
		printf("******************************信息查询******************************\n");
		printf("[S]tudio   Query.\n");
		printf("[P]lay     Query.\n");
		printf("[T]icket   Query.\n");
		printf("S[c]hedule Queries\n");
		printf("[R]eturn.\n");
		printf("\n==================================================================\n");
		printf("请输入你的选择");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's'://Studio_query;
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(1);
			break;
		case 'T':
		case 't':
		    ListTickets();
			break;
		case 'C':
		case 'c':
			Schedule_UI_ListAll();
			break;
		}
    printf("按任意键继续...");
    getchar();getchar();
	} while ('R' != choice && 'r' != choice);
	}
