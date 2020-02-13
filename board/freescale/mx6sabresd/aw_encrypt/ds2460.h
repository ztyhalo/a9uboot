
#ifndef __DS2460_H
#define __DS2460_H
void ds2460_init(void);
int ds2460_read(uint suba, uchar *buf, char len);
int ds2460_write(uint suba, uchar *buf, char len);

#endif


