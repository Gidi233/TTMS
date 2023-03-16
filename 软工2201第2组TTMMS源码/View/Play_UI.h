


#ifndef PLAY_UI_H_
	#define PLAY_UI_H_
	#include "../Common/list.h"
	#include "../Service/play.h"

	static const int PLAY_PAGE_SIZE=6;


	
	 // ��ʶ��:    PlaTMS_SCU_Play_UI_Add
	 //����: ���һ����Ŀ��Ϣ
	
	int Play_UI_Add(void);

	
	 //��ʶ��:	TTMS_SCU_Play_UI_Mod
	 //����: �޸ľ�Ŀ��Ϣ
	 
	int Play_UI_Modify(int id);

	
	 //��ʶ��:	TTMS_SCU_Play_UI_Del
	 //����: ����ID��ɾ����Ŀ��Ϣ
	int Play_UI_Delete(int id);

	
	 //��ʶ��:	TTMS_SCU_Play_UI_Qry
	 //����: ����ID�Ų�ѯ��Ŀ��Ϣ
	 
	int Play_UI_Query(int id);

	
	 //��ʶ��:	TTMS_SCU_Play_UI_Show
	 //����: ���б�ģʽ��ʾ��Ŀ��Ϣ
	 
	void Play_UI_ShowList(play_list_t list, Pagination_t paging);

	
	 //��ʶ��:	TTMS_SCU_Play_UI_MgtEnt
	 //����: ��Ŀ��Ϣ�������
	void Play_UI_MgtEntry(int flag);


  //  void Play_UI_FetchByName(play_list_t list, char condt[]);

#endif /* PLAY_UI_H_ */

