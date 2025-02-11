#include "GPRS.h"
#include "GPRSServer.h"

using namespace GPRSNAME;

GPRSServer::GPRSServer(Stream &serial, int port) : UARTDevice(&serial) {
    _port = port;
}

bool GPRSServer::begin() {
    char strPort[4];
    uartDeviceSendCMD("AT+CIPSERVER=1,");
    itoa(_port, strPort, 10);
    uartDeviceSendCMD(strPort);
    uartDeviceSendCMD("\"\r\n\r\n");
    if (!uartDeviceWaitForResp("\r\nOK\r\n\r\nSERVER OK\r\n", CMD)) {
        return false;
    }
    return true;
}

int GPRSServer::available() {
    int i = 0;
    char c;
    if (!uartDeviceWaitForResp("REMOTE IP: ", CMD)){
        return false;
    }
    if (uartDeviceAvailable(50)) {
        c = uartDeviceReadSerial();
        while (uartDeviceAvailable(50) && c != '\n') {
            _clientIP[i++] = c;
            c = uartDeviceReadSerial();
        }
    }
    _clientIP[i] = '\0';
    return true;
}

bool GPRSServer::getClientIP(char* clientIP) {
    strcpy(clientIP, _clientIP);
    return true;
}