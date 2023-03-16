


#ifndef PLAY_UI_H_
	#define PLAY_UI_H_
	#include "../Common/list.h"
	#include "../Service/play.h"

	static const int PLAY_PAGE_SIZE=6;


	
	 // 标识符:    PlaTMS_SCU_Play_UI_Add
	 //功能: 添加一条剧目信息
	
	int Play_UI_Add(void);

	
	 //标识符:	TTMS_SCU_Play_UI_Mod
	 //功能: 修改剧目信息
	 
	int Play_UI_Modify(int id);

	
	 //标识符:	TTMS_SCU_Play_UI_Del
	 //功能: 按照ID号删除剧目信息
	int Play_UI_Delete(int id);

	
	 //标识符:	TTMS_SCU_Play_UI_Qry
	 //功能: 按照ID号查询剧目信息
	 
	int Play_UI_Query(int id);

	
	 //标识符:	TTMS_SCU_Play_UI_Show
	 //功能: 以列表模式显示剧目信息
	 
	void Play_UI_ShowList(play_list_t list, Pagination_t paging);

	
	 //标识符:	TTMS_SCU_Play_UI_MgtEnt
	 //功能: 剧目信息管理界面
	void Play_UI_MgtEntry(int flag);


  //  void Play_UI_FetchByName(play_list_t list, char condt[]);

#endif /* PLAY_UI_H_ */

