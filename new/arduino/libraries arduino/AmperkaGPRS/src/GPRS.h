#ifndef __GPRS_H__
#define __GPRS_H__

#include "./utility/uartDevice.h"
#include "GPRSClient.h"

#define     ERROR   -1

// define where debug output will be printed
#define DEBUG_PRINTER Serial

// setup debug printing macros
#ifdef DEBUG_PRINTER
    #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
    #define DEBUG_PRINT(...) {}
    #define DEBUG_PRINTLN(...) {}
#endif

class GPRS : public GPRSNAME::UARTDevice
{
public:
    GPRS(Stream &serial, uint8_t pkPin = 2, uint8_t stPin = 3); 
    bool begin();
    void powerOn();
    void powerOff();
    bool testCommandAT();
    int getSignal();
    int callUp(const char* number);
    bool incomingCall(char* number);
    void answerCall();
    void endCall();
    int getStateCall();
    bool subAnswerCall();
    bool subEndCall();
    bool sendSMS(const char* number, const char* data);
    bool incomingSMS();
    void readSMS();
    void readSMS(char* message, char* number, char* dateTime);        
    bool setDateTime(const char* dateTime);
    bool getDateTime(char* dateTime);
    bool dataTransferOn(char* apn = 0, char* username = 0, char* password = 0, int timeout = DEFAULT_TIMEOUT);
    bool dataTransferOff();
    char getIPAddress(char* ip);

private:
    bool checkStatusSIM();
    uint8_t _stPin = 3;
    uint8_t _pkPin = 2;
};

#endif // __GPRS_H__
