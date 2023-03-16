#include "Account.h"
#include "EntityKey.h"
#include "../Common/list.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>
#include<stdio.h>
account_t gl_CurUser;

static const char ACCOUNT_DATA_FILE[] = "Account.dat";

//创建系统初始化账号admin
void Account_Srv_InitSys(){
    FILE *fp = fopen(ACCOUNT_DATA_FILE,"ab");
    if(NULL==fp){
        printf("文件创建失败！");
        return ;
    }
    account_t data;
    account_t buf;
    strcpy(data.username,"admin");
    strcpy(data.password,"admin");
    data.type = USR_ADMIN;
    int count = Account_Srv_FetchByName(data.username, &buf);
    if(count==0){

        data.id = EntKey_Srv_CompNewKey("Account");
        fwrite(&data,sizeof(account_t),1,fp);
        fclose(fp);
        printf("写入admin数据...\n");
        return ;
    }

    fclose(fp);

}

//验证登录账号是否已存在，存在，保存登录用户信息到全局变量gl_CurUser，return 1；否则return 0
inline int Account_Srv_Verify(char usrName[], char pwd[]){

    FILE *fp = fopen(ACCOUNT_DATA_FILE,"rb");
    int found = 0;
    if(NULL==fp){
        return 0;
    }
    account_t data;

    while(!feof(fp)){
        if(fread(&data,sizeof(account_t),1,fp)){

            if(strcmp(data.password,pwd)==0 && strcmp(data.username,usrName)==0){
                gl_CurUser = data;
                found = 1;
                break;
            }

        }

    }

    fclose(fp);
    return found;
}


//验证用户名是否存在，遍历list，若存在，返回对应结点指针；否则，返回null
account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) {
    account_list_t pTemp;
   // account_node_t *newNode;
	/*account_t data;
	int recCount = 0;
    FILE *fp = fopen(ACCOUNT_DATA_FILE,"rb");
    if(NULL == fp){
        return 0;
    }

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			if(strcmp(data.username,usrName)==0){
                fclose(fp);
                //printf("%s %s\n",data.username,data.password);
                return &data;
			}
		}
	}
	fclose(fp);

*/
    List_ForEach(list,pTemp){

       if(strcmp(pTemp->data.username,usrName)==0){
                return pTemp;
                //printf("%s %s\n",data.username,data.password);
                //return &data;
        }

    }

    return NULL;


}

//添加一个用户账号，通过调用Account_Perst_Insert(data)函数实现
inline int Account_Srv_Add(const account_t *data){

	 return Account_Perst_Insert(data);
}

//修改一个用户账号，通过调用Account_Perst_Update(data)函数实现
inline int Account_Srv_Modify(const account_t *data){
	return Account_Perst_Update(data);
}

//删除一个用户账号，通过调用Account_Perst_DeleteByID(usrID)函数实现
inline int Account_Srv_DeleteByID(int usrID){
	return Account_Perst_DeleteByID(usrID);
}

//提取usrID对应的用户账号信息，通过调用Account_Perst_SelectByID(usrID, buf)函数实现
inline int Account_Srv_FetchByID(int usrID, account_t *buf){
	return Account_Perst_SelectByID(usrID, buf);
}

//提取usrName对应的用户账号信息，通过调用Account_Perst_SelByName(usrName, buf)函数实现
inline int Account_Srv_FetchByName(char usrName[], account_t *buf){
	return Account_Perst_SelByName(usrName, buf);
}

//提取所有用户账号信息，保存到list链表中，通过调用Account_Perst_SelectAll(list)函数实现
inline int Account_Srv_FetchAll(account_list_t list){
	return Account_Perst_SelectAll(list);
}


