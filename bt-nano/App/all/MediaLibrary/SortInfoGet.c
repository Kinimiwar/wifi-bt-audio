/*
********************************************************************************************
*
*        Copyright (c):Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\MediaLibrary\SortInfoGet.c
* Owner: ctf
* Date: 2015.8.20
* Time: 17:29:00
* Version: 1.0
* Desc: ý���uiʱ����
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    ctf        2016.1.26     17:29:00   1.0
********************************************************************************************
*/
#include "BspConfig.h"
#ifdef _MEDIA_MODULE_

#define NOT_INCLUDE_OTHER
#include "typedef.h"
#include "RKOS.h"
#include "Bsp.h"
#include "global.h"
#include "AddrSaveMacro.h"
#include "media_browser.h"

//tiantian, δʵ��MDReadData, ��ʱ��FW_ReadFirmwaveByByte�滻MDReadData

#if 0
/*
--------------------------------------------------------------------------------
  Function name : void GetSavedMusicPath(UINT8 *pPathBuffer, UINT32 ulFullInfoSectorAddr, UINT32 ulSortSectorAddr, UINT16 uiSortId)

  Author        : anzhiguo
  Description   : ��ȡ·����Ϣ�����ļ��ţ��Լ����ļ���

  Input         : pPathBuffer -- ��ȡ��·����Ϣ���buf
                  ulFullInfoSectorAddr --- flash�д����ϸ�ļ���Ϣ����ʼ��ַ
                  ulSortSectorAddr --- flash�д�ŷ���������Ϣ����ʼ��ַ
                  uiSortId -- ��Ҫ��ȡ���ļ��ڷ�����Ϣ�е������
                  Filenum -- ��ǰҪ��ȡ���ļ��������ļ��е����(�� 1 ��ʼ)���������ղؼ�������ļ����ж��Ƿ��ظ����
  Return        :

  History:     <author>         <time>         <version>
                anzhiguo     2009/06/02         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
_APP_MEDIA_BROWSER_COMMON_
COMMON API void GetSavedMusicDir(FIND_DATA * pFindData, UINT32 ulFullInfoSectorAddr, UINT32 ulSortSectorAddr, UINT16 uiSortId )
{
    UINT32 temp1;
    UINT8 ucBufTemp[2];

    FW_ReadDataBaseByByte((ulSortSectorAddr<<9)+((UINT32)uiSortId)*2, ucBufTemp, 2);
    temp1 = (UINT16)ucBufTemp[0]+(((UINT16)ucBufTemp[1])<<8);
    FW_ReadDataBaseByByte((ulFullInfoSectorAddr<<9)+(UINT32)(temp1)*BYTE_NUM_SAVE_PER_FILE + (UINT32)DIR_CLUS_SAVE_ADDR_OFFSET, (uint8 *)pFindData, 8);

#ifdef _RK_CUE_
    FW_ReadDataBaseByByte((ulFullInfoSectorAddr<<9)+(UINT32)(temp1)*BYTE_NUM_SAVE_PER_FILE + (UINT32)CUE_START_SAVE_ADDR_OFFSET, (uint8 *)&(pFindData->CueStartTime), 4);
    FW_ReadDataBaseByByte((ulFullInfoSectorAddr<<9)+(UINT32)(temp1)*BYTE_NUM_SAVE_PER_FILE + (UINT32)CUE_END_SAVE_ADDR_OFFSET, (uint8 *)&(pFindData->CueEndTime), 4);

    if(pFindData->CueStartTime != 0 || pFindData->CueEndTime != 0)
    {
        pFindData->IsCue = 1;
    }
    else
    {
        pFindData->IsCue = 0;
    }
#endif
}
#endif

/*
--------------------------------------------------------------------------------
  Function name :
  void GetSavedMusicFileName(unsigned char *pFileName, SORT_INFO_ADDR_STRUCT AddrInfo, unsigned int uiSortId, unsigned int uiCharNum, unsigned int uiCurDeep)
  Author        : anzhiguo
  Description   : ��ȡý�����ʾ��Ŀ����Ϣ(����������������ר���������ļ�����ID3Tilte)

  Input         : AddrInfo -- �ļ���Ϣ��ŵ�ַ�ṹ�����
                : uiSortId -- �ļ���
                : uiCharNum --- Ҫ��ȡ���ֽ���
                : uiCurDeep --- ͨ���ñ���ȷ��ƫ�Ƶ�ַ
                : Flag --- �ж��Ƿ��ȡ�ļ�����ID3Title Ϊ1��ʾ�Ƕ�ȡ�ļ�����ID3Title��Ϊ0��ʾ��ȡ����ID3��Ϣ
  Return        : pFileName Ҫ��ȡ���ļ����ļ�����ָ��

  History:     <author>         <time>         <version>
                anzhiguo     2009/06/02         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
_APP_MEDIA_BROWSER_COMMON_
COMMON API void GetMediaItemInfo(UINT16 *pFileName, SORT_INFO_ADDR_STRUCT AddrInfo, UINT16 uiSortId, UINT16 uiCharNum, UINT16 uiCurDeep, UINT16 Flag)
{
    UINT16 i;
    UINT16 temp1;
    UINT8   ucBufTemp[8],ucBufTemp1[2];
    UINT32  AddrOffset;
    UINT8     FileInfoBuf[MEDIA_ID3_SAVE_CHAR_NUM *2];//���ļ�����ID3����ϢҪ������ѡ���ļ��ĳ��ȿ��ռ䲻���������Խ��

   SORTINFO_STRUCT *Subinfo = (SORTINFO_STRUCT *)ucBufTemp;

   if(Flag)
   {
        FW_ReadDataBaseByByte((AddrInfo.ulFileSortInfoSectorAddr<<9)+(unsigned long)(uiSortId*2), ucBufTemp, 2);
        temp1 = (ucBufTemp[0]&0xff)+((ucBufTemp[1]&0xff)<<8); // ��ö�Ӧ���ļ������
   }
   else
   {
        //��ȡһ���������Ϣ�ṹ SORTINFO_STRUCT
        FW_ReadDataBaseByByte((AddrInfo.ulSortSubInfoSectorAddr[uiCurDeep]<<9)+(unsigned long)(uiSortId*sizeof(SORTINFO_STRUCT)), ucBufTemp, sizeof(SORTINFO_STRUCT));
        //ͨ����Ϣ�ṹ�е� BaseID ������ȡ��Ҫ���ļ���
        FW_ReadDataBaseByByte((AddrInfo.ulFileSortInfoSectorAddr<<9)+Subinfo->BaseID*2, ucBufTemp1, 2);
        temp1 = (ucBufTemp1[0]&0xff)+((ucBufTemp1[1]&0xff)<<8); // ��ö�Ӧ���ļ������
   }

    AddrOffset = (UINT32)(temp1)*BYTE_NUM_SAVE_PER_FILE + AddrInfo.uiSortInfoAddrOffset[uiCurDeep]; // ͨ��ƫ��λ����ȷ����ȡ���ǳ��ļ�����Ϣ����ID3Title��Ϣ

    FW_ReadDataBaseByByte((AddrInfo.ulFileFullInfoSectorAddr<<9)+AddrOffset, FileInfoBuf, uiCharNum*2);

    for(i=0;i<uiCharNum;i++)
    {
        *pFileName++ = (UINT16)FileInfoBuf[2*i]+((UINT16)FileInfoBuf[2*i+1]<<8);
    }
}


/*
--------------------------------------------------------------------------------
  Function name : unsigned int GetSummaryInfo(unsigned long ulSumSectorAddr, unsigned int uiSumId, unsigned int uiFindSumType, SortInfoAddrOffset, MusicDirDeep)
{
  Author        : anzhiguo
  Description   : ��ȡID3������Ϣ����Ŀ(ר���µ��ļ�����������ͬһ���������µ��ļ�����)

  Input         : ulSumSectorAddr ID3��Ϣ����洢��ַ
                : uiSumId  -- ��Ŀid
                : uiFindSumType -- ��ȡ��Ŀ��Ϣ�����ͣ�(��Ŀ�������ǣ���ʼ��Ŀ��ţ����Ǿ����ID3��Ϣ)
  Return        :

  History:     <author>         <time>         <version>
                anzhiguo      2009/06/02         Ver1.0
  desc:         ָ����Ӧ��Ϣ��flash�е�sec��ַ
--------------------------------------------------------------------------------
*/
_APP_MEDIA_BROWSER_COMMON_
COMMON API UINT16 GetSummaryInfo(UINT32 ulSumSectorAddr, UINT16 uiSumId, UINT16 uiFindSumType, UINT16 SortInfoAddrOffset, UINT16 MusicDirDeep)
{
    UINT16 uiSumInfo;
    UINT32 uiSumTypeOffset;
    SORTINFO_STRUCT Subinfo;

    FW_ReadDataBaseByByte((ulSumSectorAddr<<9)+(UINT32)uiSumId*8, (UINT8 *)(&Subinfo), 8);


    if(uiFindSumType==FIND_SUM_STARTFILEID)
    {
        uiSumInfo = Subinfo.BaseID;
    }
    else if(uiFindSumType==FIND_SUM_SORTSTART)
    {
        uiSumInfo = Subinfo.ItemBaseID;
    }
    else if(uiFindSumType==FIND_SUM_ITEMNUM)
    {
        uiSumInfo = Subinfo.ItemNum;

        //Rk Aaron.sun
        if(SortInfoAddrOffset == ID3_ALBUM_SAVE_ADDR_OFFSET &&  MusicDirDeep != 0)
        {
            uiSumInfo++;  // for All Album
        }
    }
    else if (uiFindSumType==FIND_SUM_FILENUM)
    {
        uiSumInfo = Subinfo.FileNum;
    }

    return uiSumInfo;
}

#endif

