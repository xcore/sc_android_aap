#ifndef __AAP_IF_H__
#define __AAP_IF_H__

/**
 * Hook for EP Manager to handle requests from the application layer
 */
#ifdef __XC__
int android_ctrl( streaming chanend c, int reqVal, int timeNow);
#else
int android_ctrl( chanend c, int reqVal, int timeNow);
#endif

/**
 * Hook for EP manager to handle data coming in
 */
#ifdef __XC__
int android_rx(streaming chanend c, unsigned char data[], int datalen, int ackstate);
#else
int android_rx(chanend c, unsigned char data[], int datalen, int ackstate);
#endif

#endif /* __AAP_IF_H__ */
