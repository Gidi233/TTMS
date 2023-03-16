
#include "Play_Persist.h"
#include "../Service/play.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

static const char PLAY_DATA_FILE[] = "Play.dat";//�����Ŀ��Ϣ
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";//ɾ�������ʱ����ʱ�����Ŀ��Ϣ

/*
 * Function:    Play_Perst_Insert
 * Function ID:	TTMS_SCU_Play_Perst_Insert
 * Description: �ھ�Ŀ��Ϣ�ļ�ĩβд��һ����Ŀ��Ϣ
 * Input:       �������ļ��ľ�Ŀ��Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      д���ļ��ļ�¼��
 */
int Play_Perst_Insert(const play_t *data) {
    assert(NULL!=data);  
    FILE *fp = fopen(PLAY_DATA_FILE,"ab");//׷�ӷ�ʽ���ļ� 
    int rtn;
    if (NULL == fp) {
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}
    rtn = fwrite(data,sizeof(play_t),1,fp);//����fwrite����д���µľ�Ŀ��Ϣ 
    fclose(fp);

  return rtn;
}

/*
 * Function:    Play_Perst_Update
 * Function ID:	TTMS_SCU_Play_Perst_Mod
 * Description: ���վ�ĿID�Ÿ����ļ��еľ�Ŀ��Ϣ
 * Input:       �����ļ��и��µľ�Ŀ��Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Play_Perst_Update(const play_t *data) {

    assert(NULL!=data);  
    FILE *fp = fopen(PLAY_DATA_FILE,"rb+");//��д��ʽ���ļ� 
    if(NULL == fp){
        printf("Cannot open file %s!\n",PLAY_DATA_FILE);
        return 0;
    }

    play_t buf;// ���建����
    int found = 0;

    while(!feof(fp)){//�ж��Ƿ����ļ�ĩβΪλ�� 
        if(fread(&buf,sizeof(play_t),1,fp)){//��ȡһ����Ŀ��Ϣ���浽buf�� 
            if(buf.id == data->id){
                fseek(fp,-sizeof(play_t),SEEK_CUR);//�ļ�ָ��س�һ����¼ 
                fwrite(data, sizeof(play_t), 1, fp);
				found = 1;
				break;
            }
        }

    }
    fclose(fp);
	return found;
}

/*
 * Function:    Play_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Play_Perst_DelByID
 * Description: ���վ�ĿID��ɾ����Ŀ����Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Play_Perst_DeleteByID(int ID) {

    //��ԭʼ�ļ���������Ȼ���ȡ��������д�뵽�����ļ��У�����Ҫɾ����ʵ����˵���

	//��ԭʼ�����ļ�������
	if(rename(PLAY_DATA_FILE, PLAY_DATA_TEMP_FILE)<0){//���ļ����� 
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(PLAY_DATA_TEMP_FILE, "rb");//ֻ�����ļ� 
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(PLAY_DATA_FILE, "wb");//ֻд���ļ� 
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", PLAY_DATA_TEMP_FILE);
		return 0;
	}


	play_t buf;

	int found = 0;
	while (!feof(fpSour)) {//�����ļ����� 
		if (fread(&buf, sizeof(play_t), 1, fpSour)) {//��PlayTemp.dat����Ŀ��Ϣ�����浽buf 
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(play_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//ɾ����ʱ�ļ�
	remove(PLAY_DATA_TEMP_FILE);
	return found;

}

/*
 * Function:    Play_Perst_SelectByID
 * Function ID:	TTMS_SCU_Play_Perst_SelByID
 * Description: ���վ�ĿID�Ų��Ҿ�Ŀ����Ϣ
 * Input:       �����ҵľ�ĿID�ţ�������ҽ�����ڴ�ĵ�ַ
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Play_Perst_SelectByID(int ID, play_t *buf) {

    assert(NULL!=buf);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");//ֻ����ʽ���ļ� 
	if (NULL == fp) {
		return 0;
	}

	play_t data;
	int found = 0;

	while (!feof(fp)) {//δ�����ļ����� 
		if (fread(&data, sizeof(play_t), 1, fp)) {//��ȡһ����Ŀ��Ϣ���浽date 
			if (ID == data.id) {
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;

}


/*
 * Function:    Play_Perst_SelectAll
 * Function ID:	TTMS_SCU_Play_Perst_SelAll
 * Description: �����о�Ŀ��Ϣ������һ������
 * Input:       list��Ŀ��Ϣ�����ͷָ��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Play_Perst_SelectAll(play_list_t list) {
    play_node_t *newNode;
	play_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, play_node_t);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp)) {
			newNode = (play_node_t*) malloc(sizeof(play_node_t));
			if (!newNode) {
				printf(
						"Warning, Memory OverFlow!!!\n Cannot Load more Data into memory!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}


/*
 * Function:    Play_Perst_SelectByName
 * Function ID:	TTMS_SCU_Play_Perst_SelByName
 * Description: ���վ�Ŀ���Ʋ��Ҿ�Ŀ����Ϣ
 * Input:       listΪ���ҵ��ľ�Ŀ��Ϣ����condtΪģ����ѯ�Ĺؼ���
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */

 //---lyģ���ؼ���
int Play_Perst_SelectByName(play_list_t list, char condt[]) {

    assert(NULL!=list);
    //��ʼ��

    List_Destroy(list,play_node_t);
    List_Init(list,play_node_t);



    play_list_t pTemp;

    int recCount = 0; //��¼��
    int count = 0;
    play_t data;
    int i = 0;
    FILE *fp = fopen(PLAY_DATA_FILE,"rb");

    if (NULL == fp) { //�ļ�������
		return 0;
	}

    while(!feof(fp)){

        if(fread(&data,sizeof(play_t),1,fp)){
            for(i = 0;i<strlen(data.name);i++){
                if(data.name[i]==condt[i]){
                	//��ȡһ����Ŀ��Ϣ������data�У�
					//data.name��������condt �е��ַ���������ӽ�㵽list����Ľ�β
		 			//reccount+1
                   pTemp = (play_list_t)malloc(sizeof(play_node_t));
                   if(!pTemp){
                    printf("����ռ�ʧ�ܣ�\n");
                   }
                   pTemp->data = data;
                   List_AddTail(list,pTemp);
                    count++;
                    break;
                }
            }
            if(count!=0){
                count = 0;
                recCount++;
            }
        }
    }

    fclose(fp);
   return recCount;
}


//-----ly�������ֻ�ȡID
int Play_Perst_SelectPlayByName(char playName[]) {



	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	play_t data;
	int found = -1;

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp)) {
			if (strcpy(data.name,playName)) {
                //printf("%s----%d\n",data.name,data.id);
				found = data.id;
				break;
			};

		}
	}
	fclose(fp);

	return found;

}


