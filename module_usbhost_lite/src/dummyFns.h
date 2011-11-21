#ifndef __DUMMY_FNS_H__
#define __DUMMY_FNS_H__


int dummyrx(streaming_chanend c, unsigned char data[], int datalen, int ackstate);
int dummyfn(unsigned char data[], int datalen);
int dummyctrl(streaming_chanend c, int v, int timenow);
int dummytimeout(streaming_chanend c, int timenow);
int dummyflush(streaming_chanend c);
void dummyinit();

#endif


