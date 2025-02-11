#ifndef __GPRS_CLIENT_H__
#define __GPRS_CLIENT_H__

#include "./utility/uartDevice.h"

enum Protocol {
    CLOSED = 0,
    TCP    = 1,
    UDP    = 2,
};

/*enum StateConnectTCP {
    IP_INITIAL  = 0,
    IP_START    = 1,
    IP_CONFIG   = 2,
    IP_GPRSACT  = 3,
    IP_STATUS   = 4,
    CONNECT_OK  = 5,
    TCP_CLOSING = 6,
    TCP_CLOSED  = 7,
};
*/
class GPRSClient : public GPRSNAME::UARTDevice
{
public:
    GPRSClient();
    //GPRSClient(uint8_t sock);
    bool connect(Protocol ptl, const char* host, int port, int timeout = 2 * DEFAULT_TIMEOUT);
    int available(int time = 3000);
    int read();
    void print(const String buff);
    void println(const String buff);
    void print(const int data);
    void println(const int data);
    void println();
    bool connected();
    bool disconnect();
    bool readStateTCP();
    int sendData(const char* str);
    int sendData(const char* str, int len);
    char getClientIPAddress(char* ipClient);
    virtual operator bool();

private:
    bool _socket;
//    StateConnectTCP _stateConnectTCP;
    bool _stateConnectTCP;
};

#endif