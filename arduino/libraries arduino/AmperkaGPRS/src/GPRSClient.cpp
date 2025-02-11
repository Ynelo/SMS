#include "GPRS.h"
#include "GPRSClient.h"

using namespace GPRSNAME;

GPRSClient::GPRSClient() {

}

/*GPRSClient::GPRSClient(uint8_t socket) {
    _socket = socket;
}*/

bool GPRSClient::connect(Protocol ptl, const char* host, int port, int timeout) {
    char strPort[4];
    char gprsBuffer[64];

    if (ptl == TCP) {
        uartDeviceSendCMD("AT+CIPSTART=\"TCP\",\"");
        uartDeviceSendCMD(host);
        uartDeviceSendCMD("\",");
        itoa(port, strPort, 10);
        uartDeviceSendCMD(strPort);
        uartDeviceSendCMD("\r\n");
    } else if (ptl == UDP) {
        uartDeviceSendCMD("AT+CIPSTART=\"UDP\",\"");
        uartDeviceSendCMD(host);
        uartDeviceSendCMD("\",");
        itoa(port, strPort, 10);
        uartDeviceSendCMD(strPort);
        uartDeviceSendCMD("\r\n");
    } else {
        return false;
    }

    if (!uartDeviceWaitForResp("OK\r\n", CMD)) {
        return false;
    }

    if (!uartDeviceWaitForResp("\r\nCONNECT OK\r\n", CMD)) {
        return false;
    }

    _stateConnectTCP = true;

    return true;
}

int GPRSClient::available(int time) {
    if (uartDeviceAvailable()) {
        return true;
    }

    if (uartDeviceAvailable(time)) {
        return true;
    }
    _stateConnectTCP = false;
    return false;
}

int GPRSClient::read() {
    return uartDeviceReadSerial();
}

void GPRSClient::print(const String str) {
    int len = str.length() + 2;
    char request[len];
    str.toCharArray(request, len);
	sendData(request);
}

void GPRSClient::println() {
    sendData("\r\n");
}

void GPRSClient::println(const String str) {
    str += "\r\n";
    int len = str.length() + 2;
    char request[len];
    str.toCharArray(request, len);
    sendData(request);
}

void GPRSClient::print(const int data) {
    char request[8];
    itoa(data, request, 10);
    sendData(request);
}

void GPRSClient::println(const int data) {
    char request[8];
    itoa(data, request, 10);
    strcat(request, "\r\n");
    sendData(request);
}

bool GPRSClient::connected() {
    if (!_stateConnectTCP) { 
        if (uartDeviceCheckWithCMD("AT+CIPSTATUS\r\n\r\n","\r\nOK\r\n", CMD)) {   
            if (uartDeviceWaitForResp("\r\nSTATE: CONNECT OK\r\n", CMD)) {
                return true;
            }
        }

        if (uartDeviceCheckWithCMD("AT+CIPSTATUS\r\n\r\n","\r\nOK\r\n", CMD)) {   
            if (uartDeviceWaitForResp("\r\nSTATE: TCP CLOSED\r\n", CMD)) {
                return false;
            }
        }
    } else {
        return true;
    }
}

bool GPRSClient::disconnect() {
    if (!uartDeviceCheckWithCMD("AT+CIPCLOSE\r\n", "CLOSE OK\r\n", CMD)) {
        return false;
    }
    return true;
}

int GPRSClient::sendData(const char* str, int len) {
    char strLen[4];
    if(len > 0){
        uartDeviceSendCMD("AT+CIPSEND=");
        itoa(len, strLen, 10);
        uartDeviceSendCMD(strLen);
        if(!uartDeviceCheckWithCMD("\r\n",">",CMD)) {
            return false;
        }
        uartDeviceSendCMD(str);
        uartDeviceSendEndMark();
        if (!uartDeviceWaitForResp("SEND OK\r\n", DATA, DEFAULT_TIMEOUT * 10, DEFAULT_INTERCHAR_TIMEOUT * 10)) {
            return false;
        }
    }
    return true;
}

int GPRSClient::sendData(const char* str) {
    int len = strlen(str);
    char strLen[4];
    if (len > 0) {
        uartDeviceSendCMD("AT+CIPSEND=");
        itoa(len, strLen, 10);
        uartDeviceSendCMD(strLen);
        if (!uartDeviceCheckWithCMD("\r\n", ">", DATA)) {
              return false;
        }
        uartDeviceSendCMD(str);
        uartDeviceSendEndMark();
        if (!uartDeviceWaitForResp("SEND OK\r\n", DATA, DEFAULT_TIMEOUT * 10, DEFAULT_INTERCHAR_TIMEOUT * 10)) {
            return false;
        }
    }
    return true;
}

GPRSClient::operator bool() {
    return _socket;
}