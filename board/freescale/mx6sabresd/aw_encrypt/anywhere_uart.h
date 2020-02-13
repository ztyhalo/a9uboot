/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**                                      
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               Anywhere_Uart.h
** Latest modified Date:    2009-9-11
** Latest Version:          1.0
** Descriptions:            在串口通讯中实现Anywhere协议 
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
 * 端口宽度
 */
#define     AW_BYTE_MODE          0x01                                   /*  字节模式                   */
#define     AW_DBYTE_MODE         0x02                                   /*  双字节模式                 */
#define     AW_WORD_MODE          0x04                                   /*  字模式                     */
#define     AW_BIT_MODE           0x08                                   /*  位模式                     */

/*
 * 读写模式
 */
#define     AW_IO_MODE            0x80                                   /*  IO模式						*/
#define     AW_MEMORY_MODE        0x40                                   /*  内存模式					*/

/*
 * 返回状态
 */
#define     AW_ZY_OK              0x00                                   /*  正确						*/
#define     AW_ZY_NO_PORT         0x01                                   /*  不存在的端口				*/
#define     AW_ZY_NO_MEMARY       0x02                                   /*  无内存						*/
#define     AW_ZY_READING         0x03                                   /*  正在读						*/
#define     AW_ZY_READED          0x04                                   /*  读完成						*/
#define     AW_ZY_WRITEING        0x05                                   /*  正在写						*/
#define     AW_ZY_WRITED          0x06                                   /*  写完成						*/

/* 
 * 帧类型
 */
#define     AW_BROADCAST_FRAME    0xFF                                   /*  广播帧                     */
#define     AW_ARP_FRAME          0x00                                   /*  ARP帧                      */
#define     AW_ARP_BACKFRAME      0x80                                   /*  ARP回复帧                  */ 
#define     AW_READ_FRAME         0x01                                   /*  读帧                       */
#define     AW_READ_BACKFRAME     0x81                                   /*  读回复帧                   */
#define     AW_WRITE_FRAME        0x02                                   /*  写帧                       */
#define     AW_WRITE_BACKFRAME    0x82                                   /*  写回复帧                   */
#define     AW_READPORT_FRAME     0x03                                   /*  读端口帧                   */
#define     AW_READPORT_BACKFRAME 0x83                                   /*  读端口回复帧               */

#define     AWFrameDstAddr        0x1000                                 /*  帧目标地址，此处是PC机地址 */
#define     AWFrameSrcAddr        0x2000                                 /*  帧的源地址，是DS2460地址   */ 



/*
 * Anywhere帧头
 */

typedef struct tag_awFrameHead
{
	UINT8  ucProtocolType;                                               /* 协议类型         1字节      */
	UINT8  ucProtocolVers;                                               /* 协议版本         1字节      */
	UINT16 usFrameLen;                                                   /* 帧长度           2字节      */
	UINT8  ucFrameID;                                                    /* 帧ID             1字节      */
	UINT8  ucFrameCode;                                                  /* 帧代码           1字节      */
	UINT8  ucRWmode;                                                     /* 读写模式         1字节      */
	UINT8  ucBackState;                                                  /* 返回状态         1字节      */
	UINT32 ulSrcAddr;                                                    /* 源地址           4字节      */
	UINT32 ulDstAddr;                                                    /* 目的地址         4字节      */
}awFrameHead;

/*
 * Anywhere帧参数，在帧构建、帧操作、帧发送时都需要这些参数
 */
typedef struct tag_awFrameParame
{
    awFrameHead FrameHead;                                               /* Anywhere帧头                */   
	UINT32      ulReturnVal;                                             /* 返回值                      */ 
	UINT32      ulPortAddr;                                              /* 端口地址                    */ 
	UINT8       *pucDataAddr;                                            /* 要传输数据首地址            */
	UINT16      usDataLen;                                               /* 要传输数据个数              */
	UINT8       *pucFrameAddr;                                           /* 帧的首地址                  */
	UINT16      usFrameSendLen;                                          /* 帧的发送长度                */
}awFrameParame;


void UINT32toUINT8(UINT32 ul,UINT8 *puc);
UINT32 UINT8toUINT32(UINT8 *puc);
void UINT16toUINT8(UINT16 us,UINT8 *puc);
UINT16 UINT8toUINT16(UINT8 *puc);
UINT16 crc16Cal(UINT32 iFcs, UINT8 *pucData, UINT32 iLen);

void awFrameHeadSet(
					awFrameHead *FrameHead,                              /* 要设置的帧头首地址          */ 
					UINT8  ucProtocolType,                               /* 协议类型         1字节      */
					UINT8  ucProtocolVers,                               /* 协议版本         1字节      */
					UINT16 usFrameLen,                                   /* 帧长度           2字节      */
					UINT8  ucFrameID,                                    /* 帧ID             1字节      */
					UINT8  ucFrameCode,                                  /* 帧代码           1字节      */
					UINT8  ucRWmode,                                     /* 读写模式         1字节      */
					UINT8  ucBackState,                                  /* 返回状态         1字节      */
					UINT32 ulSrcAddr,                                    /* 源地址           4字节      */
					UINT32 ulDstAddr                                     /* 目的地址         4字节      */
					);
awFrameHead awFrameHeadGet(UINT8 *awFrameBuff);
void awFrameParameSet(
					  awFrameParame *FrameParame,                        /* 要设置的帧参数首地址        */ 
					  awFrameHead   FrameHead,                           /* Anywhere帧头                */
					  UINT32        ulReturnVal,                         /* 返回值                      */ 
					  UINT32        ulPortAddr,                          /* 端口地址                    */
					  UINT8         *pucDataAddr,                        /* 要传输数据首地址            */
					  UINT16        usDataLen,                           /* 要传输数据个数              */
					  UINT8         *pucFrameAddr,                       /* 帧的首地址                  */
					  UINT16        usFrameSendLen                       /* 帧的发送长度                */
                      ); 
awFrameParame awFrameParameGet(UINT8 *awFrameBuff);
void awFrameBuild(awFrameParame *pFrameParame);
void awFrameFilter(awFrameParame *pFrameParame);
UINT32 awFrameRecv(UINT8 *pucAWFrameAddr,UINT8 *pucInData,UINT32 iInLen);
UINT32 awFrameCheck(UINT8 *pucAWFrameAddr);


#endif
