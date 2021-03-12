#include <AmperkaGPRS.h>
#include <GPRS.h>
#include <GPRSClient.h>

#include <SoftwareSerial.h>
// библиотека для работы с GPRS устройством

 

// текст сообщения о состоянии розетки
#define MESSAGE_STATE  "State"

#define MESSAGE_1STATE  "1state"

#define MESSAGE_2STATE  "2state"

#define MESSAGE_3STATE  "3state"
// длина сообщения
#define MESSAGE_LENGTH 160
// текст сообщения о включении розетки
#define MESSAGE_ON  "On"

#define MESSAGE_1ON  "1on"

#define MESSAGE_2ON  "2on"

#define MESSAGE_3ON  "3on"
// текст сообщения о выключении розетки
#define MESSAGE_OFF  "Off"

#define MESSAGE_1OFF  "1off"

#define MESSAGE_2OFF  "2off"

#define MESSAGE_3OFF  "3off"

#define MESSAGE_allon  "Allon"

#define MESSAGE_alloff  "Alloff"
 
// пин, к которому подключено реле
#define RELAY  7

#define RELAY1 6

#define RELAY2 5 

#define RELAY3 4

 
// текст сообщения
char message[MESSAGE_LENGTH];
// номер, с которого пришло сообщение
char phone[16];
// дата отправки сообщения
char datetime[24];
 
bool stateRelay = false;
bool stateRelay1 = false;
bool stateRelay2 = false;
bool stateRelay3 = false;

// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(Serial1);
 
void setup()
{

  // настраиваем пин реле в режим выхода,
  pinMode(RELAY, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  // подаём на пин реле «низкий уровень» (размыкаем реле)
  digitalWrite(RELAY, HIGH);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);  
  // открываем последовательный порт для мониторинга действий в программе
  Serial.begin(9600);
  // включаем GPRS шилд
  // ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе

  // открываем Serial-соединение с GPRS Shield
  Serial1.begin(9600);
    gprs.powerOn();
    delay(2000);
  Serial1.begin(9600);

  // проверяем есть ли связь с GPRS устройством
  while (!gprs.begin()) {
    // если связи нет, ждём 1 секунду
    // и выводим сообщение об ошибке
    // процесс повторяется в цикле
    // пока не появится ответ от GPRS устройства
    delay(1000);
    Serial.print("GPRS Init error\r\n");
    gprs.powerOn();
    delay(2000);
  }
  // вывод об удачной инициализации GPRS Shield
  Serial.println("GPRS init success");

  //чистим SMS-ки
  Serial.println("SMS deleting... ");
//  if (!gprs.deleteAll())
//  {
//     Serial.println("All SMS delete error...");
//  }else{
 //   Serial.println("All SMS deleted.");
 // }

  delay(2000);
}
 
void loop()
{
  // если пришло новое сообщение
  if (gprs.incomingSMS()) {
    // читаем его
    // если есть хотя бы одно непрочитанное сообщение,
    // читаем его
    gprs.readSMS(message, phone, datetime);
 
    // выводим номер, с которого пришло смс
    Serial.print("From number: ");
    Serial.println(phone);
 
    // выводим дату, когда пришло смс
    Serial.print("Datetime: ");
    Serial.println(datetime);
 
    // выводим текст сообщения
    Serial.print("Recieved Message: ");
    Serial.println(message);
    // вызываем функцию изменения состояния реле
    // в зависимости от текста сообщения

    // если пришло сообщение с текстом «State»,
    // отправляем сообщение с состоянием реле
  
      setRelay(phone, message);
  }

  Serial.println("delay1000");
  delay(1000);
}
 
void setRelay(char f_phone[], char* f_message){
    Serial.print("setRelay:");
    Serial.println(f_message);
  
    if (strcmp(f_message, MESSAGE_1ON) == 0) {
      // если сообщение — с текстом «On»,
      // выводим сообщение в Serial
      // и подаём на замыкаем реле
      Serial.println("OK! Realy Power is On");
      digitalWrite(RELAY1, HIGH);
      stateRelay1 = true;
  
      // на номер, с которого пришёл запрос,
      // отправляем смс с текстом о включении питания
      gprs.sendSMS(f_phone, " Realy1 Power is On");
    } else

    if (strcmp(f_message, MESSAGE_1OFF) == 0) {
      // если пришло сообщение с текстом «Off»,
      // выводим сообщение в Serial
      // и размыкаем реле
      Serial.println("OK! Realy1 Power is Off");
      digitalWrite(RELAY1, LOW);
     
      stateRelay1 = false;
      // на номер, с которого пришёл запрос
      // отправляем смс с текстом о выключении питания
      gprs.sendSMS(f_phone, " Relay1 Power is Off");
     } else
      
     if (strcmp(f_message, MESSAGE_2ON) == 0) {
       // если сообщение — с текстом «On»,
       // выводим сообщение в Serial
       // и подаём на замыкаем реле
       Serial.println("OK! Power is On");
       digitalWrite(RELAY2, HIGH);
   
       stateRelay2 = true;
       // на номер, с которого пришёл запрос,
       // отправляем смс с текстом о включении питания
        gprs.sendSMS(f_phone, " Relay2 Power is On");
      } else

      if (strcmp(f_message, MESSAGE_2OFF) == 0) {
        // если пришло сообщение с текстом «Off»,
        // выводим сообщение в Serial
        // и размыкаем реле
        Serial.println("OK! Realy Power is Off");
        digitalWrite(RELAY2, LOW);
        stateRelay2 = false;
        // на номер, с которого пришёл запрос
        // отправляем смс с текстом о выключении питания
        gprs.sendSMS(f_phone, " Relay2 Power is Off");
      } else
  
 if (strcmp(f_message, MESSAGE_3OFF) == 0) {
    // если пришло сообщение с текстом «Off»,
    // выводим сообщение в Serial
    // и размыкаем реле
    Serial.println("OK! Realy Power is Off");
    digitalWrite(RELAY3, LOW);
    
   
    stateRelay3 = false;

    
    // на номер, с которого пришёл запрос
    // отправляем смс с текстом о выключении питания
  gprs.sendSMS(f_phone, " Relay3 Power is Off");
  } else if (strcmp(f_message, MESSAGE_1STATE) == 0) {
    // если пришло сообщение с текстом «State»,
    // отправляем сообщение с состоянием реле
      if (stateRelay1) {
      Serial.println("State: Power is On");
      gprs.sendSMS(f_phone, "Relay1 Power is On");

    } else {
      Serial.println("State: Power is Off");
      gprs.sendSMS(f_phone, "Relay1 Power is Off");

    }
         
  }
    else if (strcmp(f_message, MESSAGE_ON) == 0) {
    // если сообщение — с текстом «On»,
    // выводим сообщение в Serial
    // и подаём на замыкаем реле
    Serial.println("OK! Power is On");
    digitalWrite(RELAY, HIGH);

   
    stateRelay = true;
    // на номер, с которого пришёл запрос,
    // отправляем смс с текстом о включении питания
    gprs.sendSMS(f_phone, " Relay Power is On");
    }  
      else if (strcmp(f_message, MESSAGE_3ON) == 0) {
    // если сообщение — с текстом «On»,
    // выводим сообщение в Serial
    // и подаём на замыкаем реле
    Serial.println("OK! Power is On");
    digitalWrite(RELAY3, HIGH);

   
    stateRelay3 = true;
    // на номер, с которого пришёл запрос,
    // отправляем смс с текстом о включении питания
    gprs.sendSMS(f_phone, " Relay3 Power is On");
  } else if (strcmp(f_message, MESSAGE_OFF) == 0) {
    // если пришло сообщение с текстом «Off»,
    // выводим сообщение в Serial
    // и размыкаем реле
    Serial.println("OK! Relay Power is Off");
    digitalWrite(RELAY, LOW);

   
    stateRelay = false;
    // на номер, с которого пришёл запрос
    // отправляем смс с текстом о выключении питания
    gprs.sendSMS(f_phone, " Relay Power is Off");
  } else if (strcmp(f_message, MESSAGE_STATE) == 0) {
    // если пришло сообщение с текстом «State»,
    // отправляем сообщение с состоянием реле
    if (stateRelay) {
      Serial.println("State: Power is On");
      gprs.sendSMS(f_phone, "Relay Power is On");

    } else {
      Serial.println("State: Power is Off");
      gprs.sendSMS(f_phone, "Relay Power is Off");
  
  }
  
 
   } else if (strcmp(f_message, MESSAGE_2STATE) == 0) {
    // если пришло сообщение с текстом «State»,
    // отправляем сообщение с состоянием реле
    if (stateRelay2) {
      Serial.println("State: Power is On");
      gprs.sendSMS(f_phone, "Relay2 Power is On");

    } else {
      Serial.println("State: Power is Off");
      gprs.sendSMS(f_phone, "Relay2 Power is Off");
  
  }
  
 }
    else if (strcmp(f_message, MESSAGE_3STATE) == 0) {
    // если пришло сообщение с текстом «State»,
    // отправляем сообщение с состоянием реле
    if (stateRelay3) {
      Serial.println("State: Power is On");
      gprs.sendSMS(f_phone, "Relay3 Power is On");

    } else {
      Serial.println("State: Power is Off");
      gprs.sendSMS(f_phone, "Relay3 Power is Off");
  
  }
  
 }
    else if (strcmp(f_message, MESSAGE_allon) == 0) {
          Serial.println("OK! AllRelay Power is On");
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY3, HIGH);
    
    stateRelay = true;
    stateRelay1 = true;
    
    gprs.sendSMS(f_phone, " AllRelay Power is On");
    }
        else if (strcmp(f_message, MESSAGE_alloff) == 0) {
          Serial.println("OK! AllRelay Power is Off");
    digitalWrite(RELAY, LOW);
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
    digitalWrite(RELAY3, LOW);
    
    stateRelay = false;
    stateRelay1 = false;



    gprs.sendSMS(f_phone, " AllRelay Power is Off");
    

        }  else { 
    // если сообщение содержит неизвестный текст,
    // отправляем сообщение с текстом об ошибке
    Serial.println("Error... unknown command!");
    gprs.sendSMS(f_phone, "Error...unknown command!");
  }
}
  

  
  
