#include "Account.h"
#include "EntityKey.h"
#include "../Common/list.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>
#include<stdio.h>
account_t gl_CurUser;

static const char ACCOUNT_DATA_FILE[] = "Account.dat";

//����ϵͳ��ʼ���˺�admin
void Account_Srv_InitSys(){
    FILE *fp = fopen(ACCOUNT_DATA_FILE,"ab");
    if(NULL==fp){
        printf("�ļ�����ʧ�ܣ�");
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
        printf("д��admin����...\n");
        return ;
    }

    fclose(fp);

}

//��֤��¼�˺��Ƿ��Ѵ��ڣ����ڣ������¼�û���Ϣ��ȫ�ֱ���gl_CurUser��return 1������return 0
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


//��֤�û����Ƿ���ڣ�����list�������ڣ����ض�Ӧ���ָ�룻���򣬷���null
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

//���һ���û��˺ţ�ͨ������Account_Perst_Insert(data)����ʵ��
inline int Account_Srv_Add(const account_t *data){

	 return Account_Perst_Insert(data);
}

//�޸�һ���û��˺ţ�ͨ������Account_Perst_Update(data)����ʵ��
inline int Account_Srv_Modify(const account_t *data){
	return Account_Perst_Update(data);
}

//ɾ��һ���û��˺ţ�ͨ������Account_Perst_DeleteByID(usrID)����ʵ��
inline int Account_Srv_DeleteByID(int usrID){
	return Account_Perst_DeleteByID(usrID);
}

//��ȡusrID��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelectByID(usrID, buf)����ʵ��
inline int Account_Srv_FetchByID(int usrID, account_t *buf){
	return Account_Perst_SelectByID(usrID, buf);
}

//��ȡusrName��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelByName(usrName, buf)����ʵ��
inline int Account_Srv_FetchByName(char usrName[], account_t *buf){
	return Account_Perst_SelByName(usrName, buf);
}

//��ȡ�����û��˺���Ϣ�����浽list�����У�ͨ������Account_Perst_SelectAll(list)����ʵ��
inline int Account_Srv_FetchAll(account_list_t list){
	return Account_Perst_SelectAll(list);
}


