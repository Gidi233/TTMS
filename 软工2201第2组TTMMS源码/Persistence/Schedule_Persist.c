

#include "Schedule_Persist.h"
#include "../Service/schedule.h"
#include "../Common/list.h"
#include "../Service/ticket.h"
//#include "../Persistence/Common_Persist.h"
#include "../View/Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <assert.h>


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";//�����ݳ��ƻ���Ϣ
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";////ɾ�������ʱ����ʱ�����ݳ��ƻ���Ϣ

/*
 * Function:    Schedule_Perst_Insert
 * Function ID:	TTMS_SCU_Schedule_Perst_Insert
 * Description: ���ݳ��ƻ���Ϣ�ļ�ĩβд��һ���ݳ��ƻ���Ϣ
 * Input:       �������ļ����ݳ��ƻ���Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      д���ļ��ļ�¼��
 */
int Schedule_Perst_Insert(const schedule_t *data)
{
    assert(NULL!=data);
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "ab");// ��׷�ӷ�ʽ���ļ� 
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(schedule_t), 1, fp);//����fwrite ����д���µ���Ϣ 

	fclose(fp);
	return rtn;

}

/*
 * Function:    Schedule_Perst_Update
 * Function ID:	TTMS_SCU_Schedule_Perst_Mod
 * Description: �����ݳ��ƻ�ID���޸��ļ��е��ݳ��ƻ���Ϣ
 * Input:       �����ļ����޸ĵ��ݳ��ƻ���Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      �޸ĵ��ݳ��ƻ���Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_Perst_Update(const schedule_t *data){
    assert(NULL!=data);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");//�Զ�д��ʽ���ļ� 
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	schedule_t buf;//���建���� 
	int found = 0;

	while (!feof(fp)) {//�ж��Ƿ����ļ�ĩβΪλ�� 
		if (fread(&buf, sizeof(schedule_t), 1, fp)) {//��ȡһ����Ŀ��Ϣ���浽buf�� 
			if (buf.id == data->id) {
				fseek(fp, -sizeof(schedule_t), SEEK_CUR);////�ļ�ָ��س�һ����¼  
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;

}

/*
 * Function:    Schedule_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Perst_DelByID
 * Description: �����ݳ��ƻ�ID��ɾ���ݳ��ƻ�����Ϣ
 * Input:       ��ɾ�����ݳ��ƻ�ID��
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_Perst_DeleteByID(int ID){
    //��ԭʼ�ļ���������Ȼ���ȡ��������д�뵽�����ļ��У�����Ҫɾ����ʵ����˵���

	//��ԭʼ�����ļ�������
	if(rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE)<0){
		perror("Error: ");
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", SCHEDULE_DATA_TEMP_FILE);
		return 0;
	}
	schedule_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(schedule_t), 1, fpSour)) {
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, fpTarg);
		}
	}
	
	fclose(fpTarg);
	fclose(fpSour);

	//ɾ����ʱ�ļ�
	remove(SCHEDULE_DATA_TEMP_FILE);
	return found;

}

/*
 * Function:    Schedule_Perst_SelectByID
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByID
 * Description: �����ݳ��ƻ�ID��ɾ���ݳ��ƻ�����Ϣ
 * Input:       �����ҵ��ݳ��ƻ�ID�ţ�������ҽ�����ڴ�ĵ�ַ
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_Perst_SelectByID(int ID, schedule_t *buf){
    assert(NULL!=buf);
//assert�����ֻ�ǰ������ǵ��Դ���ģ����������ǣ����û��ڵ��Ժ�����ʱ��Ѵ����ų���������assert���԰�����λ���󣬶������ų�����
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)) {
		//�ļ�����û��������1��û��������0 
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
		//��fp�ж�1����СΪsizeof���������ݷŵ�ָ��date���� 
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
 * Function:    Schedule_Perst_SelectAll
 * Function ID:	TTMS_SCU_Schedule_Perst_SelAll
 * Description: �������ݳ��ƻ���Ϣ������һ������
 * Input:       listΪ�ݳ��ƻ���Ϣ�����ͷָ��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Schedule_Perst_SelectAll(schedule_list_t list){
   schedule_node_t *newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) { //�ļ�������
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
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
 * Function:    Schedule_Perst_SelectByPlay
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByPlay
 * Description: ��ͬ���Ŀ���ݳ��ƻ���Ϣ��������������һ������
 * Input:       listΪ�����������ݳ��ƻ���Ϣ�����ͷָ�룬��Ŀ��ID��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id){

    //�����٣��ڴ���
    List_Destroy(list,schedule_node_t);
    List_Init(list,schedule_node_t);

    FILE *fp = fopen(SCHEDULE_DATA_FILE,"rb");
    schedule_t data;
    schedule_list_t pNew =NULL;
    int rtn = 0;
    if(NULL==fp){
        printf("�ļ��ǲ�������~\n");
        return rtn;
    }


    while(!feof(fp)){

        if(fread(&data,sizeof(schedule_t),1,fp)){
            if(data.play_id==play_id){
            pNew = (schedule_list_t)malloc(sizeof(schedule_node_t));
            if(NULL == pNew){

                printf("�ڴ治�㣬�޷�����ռ䣡\n");
                return rtn;
            }
               pNew->data = data;
               List_AddTail(list,pNew);
               rtn++;
            }
        }
    }
   return rtn;
}
