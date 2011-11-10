#include "USBHostLLD.h"

/* Handle requests from the application for this interface */
int android_ctrl( streaming chanend c, int reqVal, int timeNow)
{
    /* Handle requests from the application thread via a channel
     *   e.g. To register with the device, send data etc
     */
    
    return 0;
}

int android_rx(streaming chanend c, unsigned char data[], int datalen, int ackstate) 
{
    /* Handle any data that comes in response to an IN, send to application via a channel */
    
    return 0;
}

