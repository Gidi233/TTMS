
#include "Play_Persist.h"
#include "../Service/play.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

static const char PLAY_DATA_FILE[] = "Play.dat";//保存剧目信息
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";//删除或更新时，暂时保存剧目信息

/*
 * Function:    Play_Perst_Insert
 * Function ID:	TTMS_SCU_Play_Perst_Insert
 * Description: 在剧目信息文件末尾写入一条剧目信息
 * Input:       待加入文件的剧目信息数据
 * Output:      打开文件失败的信息
 * Return:      写入文件的记录数
 */
int Play_Perst_Insert(const play_t *data) {
    assert(NULL!=data);  
    FILE *fp = fopen(PLAY_DATA_FILE,"ab");//追加方式打开文件 
    int rtn;
    if (NULL == fp) {
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}
    rtn = fwrite(data,sizeof(play_t),1,fp);//调用fwrite函数写入新的剧目信息 
    fclose(fp);

  return rtn;
}

/*
 * Function:    Play_Perst_Update
 * Function ID:	TTMS_SCU_Play_Perst_Mod
 * Description: 按照剧目ID号更新文件中的剧目信息
 * Input:       待在文件中更新的剧目信息数据
 * Output:      打开文件失败的信息
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_Perst_Update(const play_t *data) {

    assert(NULL!=data);  
    FILE *fp = fopen(PLAY_DATA_FILE,"rb+");//读写方式打开文件 
    if(NULL == fp){
        printf("Cannot open file %s!\n",PLAY_DATA_FILE);
        return 0;
    }

    play_t buf;// 定义缓冲区
    int found = 0;

    while(!feof(fp)){//判断是否处于文件末尾为位置 
        if(fread(&buf,sizeof(play_t),1,fp)){//读取一条剧目信息保存到buf中 
            if(buf.id == data->id){
                fseek(fp,-sizeof(play_t),SEEK_CUR);//文件指针回撤一条记录 
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
 * Description: 按照剧目ID号删除剧目的信息
 * Input:       待删除的剧目ID号
 * Output:      打开文件失败的信息
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_Perst_DeleteByID(int ID) {

    //将原始文件重命名，然后读取数据重新写入到数据文件中，并将要删除的实体过滤掉。

	//对原始数据文件重命名
	if(rename(PLAY_DATA_FILE, PLAY_DATA_TEMP_FILE)<0){//将文件改名 
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(PLAY_DATA_TEMP_FILE, "rb");//只读打开文件 
	if (NULL == fpSour ){
		printf("Cannot open file %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(PLAY_DATA_FILE, "wb");//只写打开文件 
	if ( NULL == fpTarg ) {
		printf("Cannot open file %s!\n", PLAY_DATA_TEMP_FILE);
		return 0;
	}


	play_t buf;

	int found = 0;
	while (!feof(fpSour)) {//读到文件结束 
		if (fread(&buf, sizeof(play_t), 1, fpSour)) {//从PlayTemp.dat读剧目信息并保存到buf 
			if (ID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(play_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(PLAY_DATA_TEMP_FILE);
	return found;

}

/*
 * Function:    Play_Perst_SelectByID
 * Function ID:	TTMS_SCU_Play_Perst_SelByID
 * Description: 按照剧目ID号查找剧目的信息
 * Input:       待查找的剧目ID号，保存查找结果的内存的地址
 * Output:      无
 * Return:      0表示未找到，1表示找到了
 */
int Play_Perst_SelectByID(int ID, play_t *buf) {

    assert(NULL!=buf);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");//只读方式打开文件 
	if (NULL == fp) {
		return 0;
	}

	play_t data;
	int found = 0;

	while (!feof(fp)) {//未读到文件结束 
		if (fread(&data, sizeof(play_t), 1, fp)) {//读取一条剧目信息保存到date 
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
 * Description: 将所有剧目信息建立成一条链表
 * Input:       list剧目信息链表的头指针
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */
int Play_Perst_SelectAll(play_list_t list) {
    play_node_t *newNode;
	play_t data;
	int recCount = 0;

	assert(NULL!=list);

	List_Free(list, play_node_t);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) { //文件不存在
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
 * Description: 按照剧目名称查找剧目的信息
 * Input:       list为查找到的剧目信息链表，condt为模糊查询的关键字
 * Output:      提示建立链表时，申请空间失败
 * Return:      返回查找到的记录数目
 */

 //---ly模糊关键字
int Play_Perst_SelectByName(play_list_t list, char condt[]) {

    assert(NULL!=list);
    //初始化

    List_Destroy(list,play_node_t);
    List_Init(list,play_node_t);



    play_list_t pTemp;

    int recCount = 0; //记录数
    int count = 0;
    play_t data;
    int i = 0;
    FILE *fp = fopen(PLAY_DATA_FILE,"rb");

    if (NULL == fp) { //文件不存在
		return 0;
	}

    while(!feof(fp)){

        if(fread(&data,sizeof(play_t),1,fp)){
            for(i = 0;i<strlen(data.name);i++){
                if(data.name[i]==condt[i]){
                	//读取一条剧目信息到变量data中，
					//data.name包含参数condt 中的字符串，就添加结点到list链表的结尾
		 			//reccount+1
                   pTemp = (play_list_t)malloc(sizeof(play_node_t));
                   if(!pTemp){
                    printf("申请空间失败！\n");
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


//-----ly根据名字获取ID
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


