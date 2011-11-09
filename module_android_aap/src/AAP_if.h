#ifndef __AAP_IF_H__
#define __AAP_IF_H__

/**
 * Hook for EP Manager to handle requests from the application layer
 */
int android_ctrl( streaming chanend c, int reqVal, int timeNow);

/**
 * Hook for EP manager to handle data coming in
 */
int android_rx(streaming chanend c, unsigned char data[], int datalen, int ackstate);

#endif /* __AAP_IF_H__ */
