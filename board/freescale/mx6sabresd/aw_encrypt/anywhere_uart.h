/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**                                      
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               Anywhere_Uart.h
** Latest modified Date:    2009-9-11
** Latest Version:          1.0
** Descriptions:            �ڴ���ͨѶ��ʵ��AnywhereЭ�� 
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Liujinjin
** Created date:            2009-9-11
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/


#ifndef __ANYWHERE_UART_H
#define __ANYWHERE_UART_H

#include "typedef.h"


/*
 * �˿ڿ��
 */
#define     AW_BYTE_MODE          0x01                                   /*  �ֽ�ģʽ                   */
#define     AW_DBYTE_MODE         0x02                                   /*  ˫�ֽ�ģʽ                 */
#define     AW_WORD_MODE          0x04                                   /*  ��ģʽ                     */
#define     AW_BIT_MODE           0x08                                   /*  λģʽ                     */

/*
 * ��дģʽ
 */
#define     AW_IO_MODE            0x80                                   /*  IOģʽ						*/
#define     AW_MEMORY_MODE        0x40                                   /*  �ڴ�ģʽ					*/

/*
 * ����״̬
 */
#define     AW_ZY_OK              0x00                                   /*  ��ȷ						*/
#define     AW_ZY_NO_PORT         0x01                                   /*  �����ڵĶ˿�				*/
#define     AW_ZY_NO_MEMARY       0x02                                   /*  ���ڴ�						*/
#define     AW_ZY_READING         0x03                                   /*  ���ڶ�						*/
#define     AW_ZY_READED          0x04                                   /*  �����						*/
#define     AW_ZY_WRITEING        0x05                                   /*  ����д						*/
#define     AW_ZY_WRITED          0x06                                   /*  д���						*/

/* 
 * ֡����
 */
#define     AW_BROADCAST_FRAME    0xFF                                   /*  �㲥֡                     */
#define     AW_ARP_FRAME          0x00                                   /*  ARP֡                      */
#define     AW_ARP_BACKFRAME      0x80                                   /*  ARP�ظ�֡                  */ 
#define     AW_READ_FRAME         0x01                                   /*  ��֡                       */
#define     AW_READ_BACKFRAME     0x81                                   /*  ���ظ�֡                   */
#define     AW_WRITE_FRAME        0x02                                   /*  д֡                       */
#define     AW_WRITE_BACKFRAME    0x82                                   /*  д�ظ�֡                   */
#define     AW_READPORT_FRAME     0x03                                   /*  ���˿�֡                   */
#define     AW_READPORT_BACKFRAME 0x83                                   /*  ���˿ڻظ�֡               */

#define     AWFrameDstAddr        0x1000                                 /*  ֡Ŀ���ַ���˴���PC����ַ */
#define     AWFrameSrcAddr        0x2000                                 /*  ֡��Դ��ַ����DS2460��ַ   */ 



/*
 * Anywhere֡ͷ
 */

typedef struct tag_awFrameHead
{
	UINT8  ucProtocolType;                                               /* Э������         1�ֽ�      */
	UINT8  ucProtocolVers;                                               /* Э��汾         1�ֽ�      */
	UINT16 usFrameLen;                                                   /* ֡����           2�ֽ�      */
	UINT8  ucFrameID;                                                    /* ֡ID             1�ֽ�      */
	UINT8  ucFrameCode;                                                  /* ֡����           1�ֽ�      */
	UINT8  ucRWmode;                                                     /* ��дģʽ         1�ֽ�      */
	UINT8  ucBackState;                                                  /* ����״̬         1�ֽ�      */
	UINT32 ulSrcAddr;                                                    /* Դ��ַ           4�ֽ�      */
	UINT32 ulDstAddr;                                                    /* Ŀ�ĵ�ַ         4�ֽ�      */
}awFrameHead;

/*
 * Anywhere֡��������֡������֡������֡����ʱ����Ҫ��Щ����
 */
typedef struct tag_awFrameParame
{
    awFrameHead FrameHead;                                               /* Anywhere֡ͷ                */   
	UINT32      ulReturnVal;                                             /* ����ֵ                      */ 
	UINT32      ulPortAddr;                                              /* �˿ڵ�ַ                    */ 
	UINT8       *pucDataAddr;                                            /* Ҫ���������׵�ַ            */
	UINT16      usDataLen;                                               /* Ҫ�������ݸ���              */
	UINT8       *pucFrameAddr;                                           /* ֡���׵�ַ                  */
	UINT16      usFrameSendLen;                                          /* ֡�ķ��ͳ���                */
}awFrameParame;


void UINT32toUINT8(UINT32 ul,UINT8 *puc);
UINT32 UINT8toUINT32(UINT8 *puc);
void UINT16toUINT8(UINT16 us,UINT8 *puc);
UINT16 UINT8toUINT16(UINT8 *puc);
UINT16 crc16Cal(UINT32 iFcs, UINT8 *pucData, UINT32 iLen);

void awFrameHeadSet(
					awFrameHead *FrameHead,                              /* Ҫ���õ�֡ͷ�׵�ַ          */ 
					UINT8  ucProtocolType,                               /* Э������         1�ֽ�      */
					UINT8  ucProtocolVers,                               /* Э��汾         1�ֽ�      */
					UINT16 usFrameLen,                                   /* ֡����           2�ֽ�      */
					UINT8  ucFrameID,                                    /* ֡ID             1�ֽ�      */
					UINT8  ucFrameCode,                                  /* ֡����           1�ֽ�      */
					UINT8  ucRWmode,                                     /* ��дģʽ         1�ֽ�      */
					UINT8  ucBackState,                                  /* ����״̬         1�ֽ�      */
					UINT32 ulSrcAddr,                                    /* Դ��ַ           4�ֽ�      */
					UINT32 ulDstAddr                                     /* Ŀ�ĵ�ַ         4�ֽ�      */
					);
awFrameHead awFrameHeadGet(UINT8 *awFrameBuff);
void awFrameParameSet(
					  awFrameParame *FrameParame,                        /* Ҫ���õ�֡�����׵�ַ        */ 
					  awFrameHead   FrameHead,                           /* Anywhere֡ͷ                */
					  UINT32        ulReturnVal,                         /* ����ֵ                      */ 
					  UINT32        ulPortAddr,                          /* �˿ڵ�ַ                    */
					  UINT8         *pucDataAddr,                        /* Ҫ���������׵�ַ            */
					  UINT16        usDataLen,                           /* Ҫ�������ݸ���              */
					  UINT8         *pucFrameAddr,                       /* ֡���׵�ַ                  */
					  UINT16        usFrameSendLen                       /* ֡�ķ��ͳ���                */
                      ); 
awFrameParame awFrameParameGet(UINT8 *awFrameBuff);
void awFrameBuild(awFrameParame *pFrameParame);
void awFrameFilter(awFrameParame *pFrameParame);
UINT32 awFrameRecv(UINT8 *pucAWFrameAddr,UINT8 *pucInData,UINT32 iInLen);
UINT32 awFrameCheck(UINT8 *pucAWFrameAddr);


#endif
