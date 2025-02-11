#ifndef __GPRS_SERVER_H__
#define __GPRS_SERVER_H__

#include "./utility/uartDevice.h"

class GPRSClient;

class GPRSServer : public GPRSNAME::UARTDevice
{
public:
	GPRSServer(Stream &serial, int port);
    bool begin();
    
    int available();
    bool getClientIP(char* clientIP);

private:
    int _port;
    char _clientIP[16];
};

#endif  // __GPRS_SERVER_H__