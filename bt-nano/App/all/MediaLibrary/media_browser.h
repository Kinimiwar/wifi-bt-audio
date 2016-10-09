/*
********************************************************************************************
*
*        Copyright (c): Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\music\media_browser.h
* Owner: ctf
* Date: 2016.1.26
* Time: 17:29:00
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*      ctf      2016.1.26    17:29:00      1.0
********************************************************************************************
*/


#ifndef __APP_MEDIA_BROWSER_H__
#define __APP_MEDIA_BROWSER_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define _APP_MEDIA_BROWSER_COMMON_  __attribute__((section("app_media_browser_task_common")))
#define _APP_MEDIA_BROWSER_INIT_    __attribute__((section("app_media_browser_task_common")))

#define MAX_MUSIC_DIR_DEPTH 4

#define MEDIA_PLAY_WITH_FILE_NAME     0
#define MEDIA_PLAY_WITH_FILE_NUM      1

#define  FIND_SUM_STARTFILEID       0 //��ȡ�ļ�����ʼid��
#define  FIND_SUM_SORTSTART         1 //�˵���ʾ��Ŀ����ʼ�ļ����
#define  FIND_SUM_ITEMNUM           2 //�˵�ӵ�е���Ŀ����
#define  FIND_SUM_FILENUM           4 //��ȡ�ļ�����

typedef struct _SORT_INFO_ADDR_STRUCT
{ 
    UINT32 ulFileFullInfoSectorAddr;    //�����ļ���ϸ��Ϣ����ʼsector��ַ(������Ϣ����ʵ��ַ)
    UINT16 uiSortInfoAddrOffset[4];     //ͬһ�ļ���ͬID3��Ϣ�ڻ�����Ϣ���е�ƫ�Ƶ�ַ
    UINT32 ulFileSortInfoSectorAddr;    //�����ļ�������Ϣ����ʼsector��ַ(�洢������ĸ����ڻ�����Ϣ���е�ID)
    UINT32 ulSortSubInfoSectorAddr[3];  //ID3��Ϣ���ౣ���ַ(��ID3�����SORTINFO_STRUCT�ṹ����Ϣ)
}SORT_INFO_ADDR_STRUCT;

typedef struct _MUSIC_DIR_TREE_STRUCT{

    UINT16 MusicDirTotalItem;
    UINT16 MusicDirBaseSortId[MAX_MUSIC_DIR_DEPTH];
    UINT16 MusicDirDeep;
    UINT16 Cursor[MAX_MUSIC_DIR_DEPTH];    //���λ��: 0~7
    UINT16 CurItemId[MAX_MUSIC_DIR_DEPTH]; //�����ָ���item�ڵ�ǰĿ¼�µ��ļ���
}MUSIC_DIR_TREE_STRUCT;
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t MediaBrowser_Init(void *pvParameters, void *arg);
extern rk_err_t MediaBrowser_DeInit(void *pvParameters);
extern void MediaBrowser_Enter(void);
extern void GetMediaItemInfo(UINT16 *pFileName, SORT_INFO_ADDR_STRUCT AddrInfo, UINT16 uiSortId, UINT16 uiCharNum, UINT16 uiCurDeep, UINT16 Flag);
extern UINT16 GetSummaryInfo(UINT32 ulSumSectorAddr, UINT16 uiSumId, UINT16 uiFindSumType, UINT16 SortInfoAddrOffset, UINT16 MusicDirDeep);

#endif

