/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**                                      
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               ds2460.c
** Latest modified Date:    2009-09-7
** Latest Version:          1.0
** Descriptions:            
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Liujinjin
** Created date:            2009-09-7
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

#ifndef __DS2460_ENCRYPT_H
#define __DS2460_ENCRYPT_H
#include "typedef.h"

#define  DS2460_PAGE_SIZE				8                                              /* DS2460的页大小             */ 

#define  DS2460_IIC_ADDR				0x80	
#define  DS2460_SPACE_SIZE				256

/*
* DS2460中的: 0xa8~0xab(40):DNS IP:  在EPC-8000系列工控板上未用, 用于下面的作用
*
* 加密软件中: DNS0=1.49.128.165, 表示:
*
* address(0xA8) = 0x01			   
* address(0xA9) = 0x31
*	
* address(0xAA) = 128 (即0x80)  
* address(0xAB) = 165 (即0xA5)
*
*/

/*
*  两字节(地址为: 0xA8, 0xA9), 保存EPC-8000工控板硬件版本
*/
#define  DS2460_USER_ADDR				0x80
#define  DS2460_USER_SIZE				(0xEF - 0x80)   // 112B

// System MAC Address for 1st Ethernet Adapter
#define  DS2460_MAC1_ADDR				0x80
#define  DS2460_MAC1_OFFSET				(0x80 - DS2460_USER_ADDR)
#define  DS2460_MAC1_LEN				6

// System MAC Address for 2nd Ethernet Adapter
#define  DS2460_MAC2_ADDR				0x90
#define  DS2460_MAC2_OFFSET				(0x90 - DS2460_USER_ADDR)
#define  DS2460_MAC2_LEN				6

// System Hardware Version
#define  DS2460_EPC_HARDVER_ADDR		0xA8
#define  DS2460_EPC_HARDVER_OFFSET		(0xA8 - DS2460_USER_ADDR)

/*
*  EPC信息有效标志, 两字节(地址为: 0xAA, 0xAB), 保存有效标志
*/		
#define  DS2460_EPC_INFOVALID_ADDR	    0xAA					
#define  DS2460_EPC_INFOVALID_OFFSET	(0xAA - DS2460_USER_ADDR)
#define  DS2460_EPC_INFOVALID_FLAG		0x80A5		

/*
*  EPC产品的SN号(地址为: 0xB0, 0xB1, 0xB2, 0xB3), 保存有效标志
*/
#define  DS2460_EPC_SN_ADDR			    0xB0 	
#define  DS2460_EPC_SN_OFFSET			(0xB0 - DS2460_USER_ADDR)		


void ds2460DevicAddrSet(awFrameParame awRevcFramParam);
awFrameParame ds2460Operate(awFrameParame awRevcFramParam);
void awFrameBuffPrintf(UINT8 *FrameBuff);
VOID awFrameMsgPrintf(awFrameParame awFramParam);

#define DS2460_DEBUGMSG_EN 1

#if DS2460_DEBUGMSG_EN
#define DS2460_DEBUGMSG  printf
#else
#define DS2460_DEBUGMSG(printf_exp,...)
#endif

#endif
