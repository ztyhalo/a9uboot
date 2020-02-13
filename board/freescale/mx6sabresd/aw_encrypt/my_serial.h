#ifndef __MYSERIAL_H
#define __MYSERIAL_H

#include "typedef.h"

typedef BOOL (*AW_UART_INIT) (void);    
typedef UINT8 (*AW_UART_BYTEREAD) (void);
typedef VOID (*AW_UART_BYTEWRITE) (UINT8 ch);
typedef VOID (*AW_UART_STRWRITE) (UINT8 *ch,int iLen);
typedef BOOL (*AW_UART_SETBAUDRATE) (int iBaudrate);

int aw_uart1_init(void);
unsigned char aw_uart1_read_byte(void);
void aw_uart1_write_byte(unsigned char c);
void aw_uart1_write_nbyte(unsigned char *buf, int len);
void aw_uart1_puts (const char *s);
int aw_uart1_set_baudrate(int baudrate);

#if 0
int uart5_init(void);
unsigned char uart5_read_byte(void);
void uart5_write_byte(unsigned char c);
void uart5_write_nbyte(unsigned char *buf, int len);
void uart5_puts (const char *s);
int uart5_set_baudrate(int baudrate);
void uart5_printf(unsigned char *buf, int len);
#endif

#endif
