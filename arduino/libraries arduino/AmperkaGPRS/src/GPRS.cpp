#include "GPRS.h"
using namespace GPRSNAME;

GPRS::GPRS(Stream &serial, uint8_t pkPin, uint8_t stPin) : UARTDevice(&serial) {
    _stPin = stPin;
    _pkPin = pkPin;
    pinMode(_pkPin, OUTPUT);
    pinMode(_stPin, INPUT);
}

bool GPRS::begin() {
    DEBUG_PRINTLN("I am debuger");
    uartDeviceFlushSerial();
    // reset GPRS
    uartDeviceSendCMD("AT\r\n");
    delay(1000);
    uartDeviceSendCMD("AT&F\r\n");
    delay(1000);
    uartDeviceFlushSerial();


    if (!uartDeviceCheckWithCMD("AT\r\n", "OK\r\n", CMD)) {
        return false;
    }
    DEBUG_PRINTLN("1. AT test is OK");
    
    if (!uartDeviceCheckWithCMD("AT+CFUN=1\r\n", "OK\r\n", CMD)) {
        return false;
    }
    DEBUG_PRINTLN("2. Full functionality turn On is OK");

    if (!checkStatusSIM()) {
        return false;
    }
    DEBUG_PRINTLN("3. SIM card is OK");

    if (!uartDeviceCheckWithCMD("AT+CNMI=2,2,0,0,0\r\n", "OK\r\n", CMD)) {
        return false;
    }
    DEBUG_PRINTLN("4. Setings fos SMS indicators is OK");
  
    if (!uartDeviceCheckWithCMD("AT+CMGF=1\r\n", "OK\r\n", CMD)) {
        return false;
    }
    DEBUG_PRINTLN("5. Text mode SMS turn On is OK");
  
    if (!uartDeviceCheckWithCMD("AT+CLIP=1\r\n", "OK\r\n", CMD)) {
        return false;
    }
    DEBUG_PRINTLN("6. Calling Line Identification turn On is OK");

    if (!uartDeviceCheckWithCMD("AT+COLP=1\r\n", "OK\r\n", CMD)) {
        return false;
    }
    DEBUG_PRINTLN("7. Connected Line Identification Presentation turn On is OK");

    delay(2000);
    return true;
}

// SIM card check
bool GPRS::checkStatusSIM() {
    return uartDeviceCheckWithCMD("AT+CPIN?\r\n","+CPIN: READY\r\n\r\nOK\r\n", CMD);
}

// power Up GPRS Shield
void GPRS::powerOn() {
    if (!digitalRead(_stPin)) {
        digitalWrite(_pkPin, HIGH);
        delay(3000);
        digitalWrite(_pkPin, LOW);
    } else {
        digitalWrite(_pkPin, LOW);
    }
}

// power Off GPRS Shield
void GPRS::powerOff() {
    if (digitalRead(_stPin)) {
        digitalWrite(_pkPin, HIGH);
        delay(3000);
        digitalWrite(_pkPin, LOW);
    } else {
        digitalWrite(_pkPin, LOW);
    }
}  

bool GPRS::testCommandAT(void) {
    return uartDeviceCheckWithCMD("AT\r\n","OK\r\n", CMD);
}

// getSignalStrength from SIM900
// returns 0 — 113 dBm or less
//         1 — 111 dBm
//    2...30 — 109... 53 dBm
//        31 — 51 dBm or greater
//        99 — not known or not detectable
int GPRS::getSignal() {
    int i = 0;
    int signal = 0;
    char gprsBuffer[12];
    if (!uartDeviceCheckWithCMD("AT+CSQ\r\n", "+CSQ: ", CMD)) {
        return ERROR;
    }
    uartDeviceCleanBuffer(gprsBuffer, sizeof(gprsBuffer));
    uartDeviceReadBuffer(gprsBuffer, sizeof(gprsBuffer), DEFAULT_TIMEOUT);
    while (gprsBuffer[i]!= ',') {
        i++;
    }
    gprsBuffer[i] = '\0';
    signal = atoi(gprsBuffer);
    if (signal == 0) {
        return -113;
    } else if (signal == 1) {
        return -111;
    } else if (signal >= 2 && signal <= 30) {
        return map(signal, 2 , 30 , -109, -53);
    } else if (signal == 31) {
        return -51;
    } else if (signal == 99) {
        return ERROR;
    }
}

/* call someone
    Arguments:
        number: number the phone number which you want to call
    Returns:
        1: NO DIALTONE
        2: BUSY
        3: NO CARRIER
        4: NO ANSWER
        5: OK
*/
int GPRS::callUp(const char* number) {
    char gprsBuffer[64];
    uartDeviceCleanBuffer(gprsBuffer, sizeof(gprsBuffer));
    strcat(gprsBuffer, "ATD");
    strcat(gprsBuffer, number);
    strcat(gprsBuffer, ";\r\n");
    uartDeviceSendCMD(gprsBuffer);
    delay(3000);
    //uartDeviceCleanBuffer(gprsBuffer, sizeof(gprsBuffer));
    //uartDeviceReadBuffer(gprsBuffer, sizeof(gprsBuffer), 5000, 3000, 25);
    //Serial.write(gprsBuffer);
    //while(1);
    /*
    if (strstr(gprsBuffer, "NO DIALTONE\r\n")) {
        return 1;
    } else if (strstr(gprsBuffer, "BUSY\r\n")) {
        return 2;
    } else if (strstr(gprsBuffer, "NO CARRIER\r\n")) {
        return 3;
    } else if (strstr(gprsBuffer, "NO ANSWER\r\n")) {
        return 4;
    } else {
        return 5;
    }*/
}

// subscriber answer the call
bool GPRS::subAnswerCall() {
    return uartDeviceWaitForResp("+COLP: ", CMD);
}

// if an incoming call
// param  phone, phone number
// returns true on success, false on error
bool GPRS::incomingCall(char* number) {
    // response is like:
    // +CLIP: "+7XXXXXXXXXX",145,"",,"",0
    if (!uartDeviceWaitForResp("+CLIP: ", CMD))
        return false;

    int i = 0;
    int j = 0;
    char gprsBuffer[80];
    uartDeviceCleanBuffer(gprsBuffer,sizeof(gprsBuffer));
    uartDeviceReadBuffer(gprsBuffer,sizeof(gprsBuffer));

    if (gprsBuffer[i] == '\"') {
        i++;
        while(gprsBuffer[i]!='\"') {
        number[j++] = gprsBuffer[i++];
        }
        number[j] = '\0';
        i++;
    }
    return true;
}

// answer if coming a call   
void GPRS::answerCall() {
    uartDeviceSendCMD("ATA\r\n");
}

// end a call   
void GPRS::endCall() {
    uartDeviceSendCMD("ATH0\r\n");
}

// state call
int GPRS::getStateCall() {
    /*  result code:
        0: ready
        1: unknown
        2: unknown
        3: ringing
        4: call in progress
    */
    char gprsBuffer[10];
    if (!uartDeviceCheckWithCMD("AT+CPAS\r\n","+CPAS: ", CMD)) {
        return ERROR;
    }
    uartDeviceCleanBuffer(gprsBuffer, 10);
    uartDeviceReadBuffer(gprsBuffer, 10);
    int state = gprsBuffer[0] - 48;
    return state;
}



// subscriber hung up
bool GPRS::subEndCall() {
    return uartDeviceWaitForResp("NO CARRIER\r\n", CMD);
}

// send text SMS 
// param *number phone number which SMS will be send to
// param  *data   message that will be send to
// returns false on success, true on error
bool GPRS::sendSMS(const char* number, const char* data) {
    char gprsBuffer[64];
    // set message mode to ASCII
    if (!uartDeviceCheckWithCMD("AT+CMGF=1\r\n", "OK\r\n", CMD)) {
        return false;
    }

    uartDeviceSendCMD("AT+CMGS=\"");
    uartDeviceSendCMD(number);

    if (!uartDeviceCheckWithCMD("\"\r\n",">", CMD)) {
        return false;
    }

    uartDeviceSendCMD(data);
    delay(500);
    uartDeviceSendEndMark();
    uartDeviceFlushSerial();
    uartDeviceCleanBuffer(gprsBuffer, sizeof(gprsBuffer));
    uartDeviceReadBuffer(gprsBuffer, sizeof(gprsBuffer), DEFAULT_TIMEOUT);
    if (strstr(gprsBuffer, "OK\r\n")) {
        return true;
    }
    return false;
}

// if an incoming SMS
bool GPRS::incomingSMS() {
    return uartDeviceWaitForResp("+CMT: ", CMD, 50);
}

// read SMS on buffer
void GPRS::readSMS() {
    char gprsBuffer[80 + 160];
    uartDeviceCleanBuffer(gprsBuffer, sizeof(gprsBuffer));
    uartDeviceReadBuffer(gprsBuffer, sizeof(gprsBuffer), DEFAULT_TIMEOUT);
    Serial.write(gprsBuffer);
}

// read SMS, phone and date if getting a SMS message. It changes SMS status to READ 
// param  message  buffer used to get SMS message
// param  phone  buffer used to get SMS's sender phone number
// param  dataTime  buffer used to get SMS's send dataTime
void GPRS::readSMS(char* message, char* phone, char* dataTime) {
    /* Response is like:
    +CMT: "+79772941911","","15/12/15,01:51:24+12"
    SMS text here
    */

    int i = 0;
    int j = 0;

    char gprsBuffer[80 + 160];

    uartDeviceCleanBuffer(gprsBuffer, sizeof(gprsBuffer));
    uartDeviceReadBuffer(gprsBuffer, sizeof(gprsBuffer), 100);
  
    int len = strlen(gprsBuffer);

    if (gprsBuffer[i]=='\"') {
        i++;
        j = 0;
        while (gprsBuffer[i]!='\"') {
            phone[j++] = gprsBuffer[i++];
        }
        phone[j] = '\0';
        i++;
    }
    if (gprsBuffer[i] == ',')
        i++;
    if (gprsBuffer[i]=='\"') {
        i++;
        while (gprsBuffer[i]!='\"') {
            i++;
        }
        i++;
    }
    if (gprsBuffer[i]==',')
        i++;
    if (gprsBuffer[i]=='\"') {
        i++;
        j = 0;
        while (gprsBuffer[i]!='\"') {
            dataTime[j++] = gprsBuffer[i++];
        }
        dataTime[j] = '\0';
        i++;
    }
    if (gprsBuffer[i]=='\r')
        i++;
    if (gprsBuffer[i]=='\n')
        i++;
    j = 0;
    while (i < len - 2) {
        message[j++] = gprsBuffer[i++];
    }
    message[j] = '\0';
}

// set DateTime for SIM900
// returns false on success, true on error
// format is "yy/MM/dd,hh:mm:ss±zz", where characters
// indicate year (two last digits),month, day, hour, minutes, seconds and time zone 
// E.g. 6th of May 2010, 00:01:52 GMT+2 hours equals to "23/08/16,15:41:52+03"
bool GPRS::setDateTime(const char* dateTime) {
    uartDeviceSendCMD("AT+CCLK=\"");
    uartDeviceSendCMD(dateTime);
    if (!uartDeviceCheckWithCMD("\"\r\n","OK", CMD)) {
        return false;
    }
    return true;
}

// get DateTime from SIM900
// returns false on success, true on error
bool GPRS::getDateTime(char* dateTime) {
    // +CCLK: "14/11/13,21:14:41+04"
    //
    // OK

    int i = 0;
    int j = 0;
    char gprsBuffer[32];
    if (!uartDeviceCheckWithCMD("AT+CCLK?\r\n", "+CCLK: ", CMD)) {
        return false;
    }
    uartDeviceCleanBuffer(gprsBuffer, sizeof(gprsBuffer));
    uartDeviceReadBuffer(gprsBuffer, sizeof(gprsBuffer), DEFAULT_TIMEOUT);
    if (gprsBuffer[i] == '\"') {
        i++;
        while(gprsBuffer[i] != '\"') {
            dateTime[j++] = gprsBuffer[i++];
        }
    }
    dateTime[j] = '\0';
    return true;
}

/*------------------------------GPRS Inet----------------------------------*/
bool GPRS::dataTransferOn(char* apn, char* username, char* password, int timeout) {
    if (!uartDeviceCheckWithCMD("AT+CIPSHUT\r\n","SHUT OK\r\n", CMD)) {
        return false;
    }

    if (!uartDeviceCheckWithCMD("AT+CGATT?\r\n","+CGATT: 1\r\n\r\nOK\r\n", CMD)) {
        if (!uartDeviceCheckWithCMD("AT+CGATT=1\r\n","OK\r\n", CMD)) {
            return false;
        }
        return false;
    }

    if (!uartDeviceCheckWithCMD("AT+CIPMUX=0\r\n","OK\r\n", CMD)) {
        return false;
    }

    uartDeviceSendCMD("AT+CSTT=\"");
    uartDeviceSendCMD(apn);
    uartDeviceSendCMD("\",\"");
    uartDeviceSendCMD(username);
    uartDeviceSendCMD("\",\"");
    uartDeviceSendCMD(password);
    uartDeviceSendCMD("\"\r\n");
    if (!uartDeviceWaitForResp("OK\r\n", CMD)) {
        return false;
    }

    delay(2000);

    if (!uartDeviceCheckWithCMD("AT+CIICR\r\n", "OK\r\n", CMD,  2 * DEFAULT_TIMEOUT)) {
        return false;
    }

    return true;
}

bool GPRS::dataTransferOff() {
    if (!uartDeviceCheckWithCMD("AT+CIPSHUT\r\n","SHUT OK\r\n", CMD)) {
        return false;
    }
    return true;
}

char GPRS::getIPAddress(char* ip) {
    // get local IP address
    // response:
    // AT+CIFSR\r\n
    // \r\n
    // 10.160.57.120\r\n
    // Response error:
    // AT+CIFSR\r\n       
    // \r\n               
    // ERROR\r\n
    char gprsBuffer[32];
    uartDeviceSendCMD("AT+CIFSR");
    uartDeviceSendCMD("\r\n");
    uartDeviceFlushSerial();
    uartDeviceCleanBuffer(gprsBuffer, sizeof(gprsBuffer));
    uartDeviceReadBuffer(gprsBuffer, sizeof(gprsBuffer), DEFAULT_TIMEOUT);
    if (strstr(gprsBuffer, "ERROR")) {
        return false;
    }
    int i = 0;
    char *p;
    p = strstr(gprsBuffer, "\r\n");
    p += 4;
    while (strlen(p) - 2 > 0) {
        ip[i++] = *(p++);
    }
    ip[i] = '\0';

    return true;
}
