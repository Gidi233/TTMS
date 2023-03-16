

#include "Schedule.h"
#include "../Common/list.h"
#include "../Persistence/Schedule_Persist.h"
#include "Ticket.h"
#include <stdio.h>

/*
 * Function:    Schedule_Srv_Add
 * Function ID:	TTMS_SCU_Schedule_Srv_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ����ӵ��ݳ��ƻ���Ϣ����
 * Output:      ��
 * Return:      ��ӵļ�¼��
 */
inline int Schedule_Srv_Add(const schedule_t *data) {
	return Schedule_Perst_Insert(data);
}

/*
 * Function:    Schedule_Srv_Modify
 * Function ID:	TTMS_SCU_Schedule_ Srv _Mod
 * Description: �޸�һ���ݳ��ƻ���Ϣ
 * Input:       ���޸ĵ��ݳ��ƻ���Ϣ����
 * Output:      ��
 * Return:      �޸ĵ��ݳ��ƻ���Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
inline int Schedule_Srv_Modify(const schedule_t *data) {
	return Schedule_Perst_Update(data);
}

/*
 * Function:    Schedule_Srv_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Srv_DelByID
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ�����ݳ��ƻ�ID��
 * Output:      ��
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
inline int Schedule_Srv_DeleteByID(int ID) {
	return Schedule_Perst_DeleteByID(ID);
}

/*
 * Function:    Schedule_Srv_FetchByID
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByID
 * Description: ����ID�Ų���һ���ݳ��ƻ���Ϣ������buf��ָ��Ԫ
 * Input:       �����ҵ��ݳ��ƻ�ID�ţ������ҵ�����Ϣ���浽buf��
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
inline int Schedule_Srv_FetchByID(int ID, schedule_t *buf) {
	return Schedule_Perst_SelectByID(ID, buf);

}

/*
 * Function:    Schedule_Srv_FetchAll
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchAll
 * Description: ȡ�������ݳ��ƻ���Ϣ������һ������
 * Input:       list�ݳ��ƻ���Ϣ�����ͷָ��
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
inline int Schedule_Srv_FetchAll(schedule_list_t list) {
	return Schedule_Perst_SelectAll(list);
}

/*
 * Function:    Schedule_Srv_FetchByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByPlay
 * Description: ���վ�ĿID�ţ�������ݳ��ƻ���Ϣȡ��������һ������
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬play_idΪ��Ŀ��ID��
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
inline int Schedule_Srv_FetchByPlay(schedule_list_t list,int play_id) {
	return Schedule_Perst_SelectByPlay(list, play_id);
}

/*
 * Function:    Schedule_Srv_StatRevByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_StatByPlay
 * Description: ���ݾ�ĿID��ͳ�������ʼ�Ʊ��������Ʊ��������
 * Input:       play_idΪ��ͳ�ƾ�Ŀ��ID�ţ�soldCountΪ������Ʊ����totalCountΪ��Ʊ��
 * Output:      ��
 * Return:      ����Ʊ������
 */
int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount, int *totalCount) {

    schedule_list_t list = NULL;

    // ������ʱ����soldCountΪ������Ʊ����totalCountΪ��Ʊ��
    int tempSoldCount = 0;
    int tempTotalCount = 0;
    int value = 0;

    schedule_node_t *pos;
    List_Init(list,schedule_node_t);//��ʼ��


   if( Schedule_Srv_FetchByPlay( list,play_id)<=0){
        printf("û�ж�Ӧ���ݳ��ƻ�...\n");
        return ;
   }

    List_ForEach(list,pos){
        value += Ticket_Srv_StatRevBySchID(pos->data.id, &tempSoldCount, &tempTotalCount);
        *soldCount+=tempSoldCount;
        *totalCount+=tempTotalCount;
    }
    List_Destroy(list,schedule_node_t);

	return value;
}

