#include <AmperkaGPRS.h>
//#include <GPRS.h>
//#include <GPRSClient.h>
//#include <GPRSServer.h>

// библиотека для работы с GPRS устройством
#include <SoftwareSerial.h>
// длина сообщения
#define MESSAGE_LENGTH 160

// номер сообщения в памяти сим-карты
int messageIndex = 0;
 
// текст сообщения
char massage[MESSAGE_LENGTH];
// номер, с которого пришло сообщение
char phone[16];
// дата отправки сообщения
char datetime[24];
bool flage = false;

// длина сообщения
#define MESSAGE_LENGTH 160
#define MESSAGE_2ON  "2on"
#define MESSAGE_0ON  "0on"

#define MESSAGE_2OFF  "2off"
#define MESSAGE_0OFF  "0off"

#define MESSAGE_allon  "allon"
#define MESSAGE_alloff  "alloff"
#define MESSAGE_allreset  "allreset"

#define MESSAGE_2reset  "2reset"
#define MESSAGE_0reset  "0reset"

#define MESSAGE_S1  "s1"
#define MESSAGE_S2  "s2"
#define MESSAGE_S3  "s3"
#define MESSAGE_S4  "s4"
#define MESSAGE_S5  "s5"

#define MESSAGE_S5RESET  "s5reset"
#define MESSAGE_S1RESET  "s1reset"
#define MESSAGE_S2RESET  "s2reset"
#define MESSAGE_S3RESET  "s3reset"
#define MESSAGE_S4RESET  "s4reset"

#define MESSAGE_ARDUINO  "arduino"

SoftwareSerial mySerial(10, 11);
//SoftwareSerial mySerial (0, 1);
// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(mySerial);
// можно указать дополнительные параметры — пины PK и ST
// по умолчанию: PK = 2, ST = 3
//GPRS gprs(Serial1, 2, 3);
 
void setup()
{
  pinMode(A3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  digitalWrite(5, LOW);
  // открываем последовательный порт для мониторинга действий в программе
  Serial.begin(9600);
  mySerial.begin(9600);
  // ждём пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  while (!Serial) {
  }
  Serial.print("Serial init OK\r\n");
  // открываем Serial-соединение с GPRS Shield
  Serial.begin(9600);
  // включаем GPRS-шилд
  gprs.powerOn();
  // проверяем, есть ли связь с GPRS-устройством
  while (!gprs.begin()) {
    // если связи нет, ждём 1 секунду
    // и выводим сообщение об ошибке;
    // процесс повторяется в цикле,
    // пока не появится ответ от GPRS-устройства
    delay(1000);
    Serial.print("GPRS Init error\r\n");
  }
  // выводим сообщение об удачной инициализации GPRS Shield
  Serial.println("GPRS init success");

}
double valeee = 0;
void loop()
{
  valeee = analogRead(A3);
 // Serial.println(valeee);
  if(valeee < 100 && flage == false){
      gprs.sendSMS(phone, "!!WARNING!! BLCAKOUT IN OFFICE!!!");
      flage = true;
      Serial.println("---------");
    } else if (valeee > 100 & flage == true){
      gprs.sendSMS(phone, "The light turned on.");
      flage = false;
      Serial.println("+++++++");
      }
 // vale = digitalRead(7);
 /* if(vale == 0 & flag == false){
   // gprs.sendSMS("+79097669661", "!!WARNING!! BLCAKOUT IN OFFICE!!!");
  gprs.sendSMS(phone, "!!WARNING!! BLCAKOUT IN OFFICE!!!");
    flag = true;
    Serial.println("---------");
  } else if(vale == 1 & flag == true){
   // gprs.sendSMS("+79097669661", "The light turned on.");
   gprs.sendSMS(phone, "The light turned on.");
    Serial.println("+++++++");
    flag = false;
   }*/
  if (gprs.incomingSMS()) {
    gprs.readSMS(massage, phone, datetime);
    Serial.print(massage);
    Serial.print('|');
    Serial.println(phone);
    Serial.println("-----");
    if (strcmp(massage, MESSAGE_0ON) == 0) {
        digitalWrite(4, HIGH);
        gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_0OFF) == 0){
        digitalWrite(4, LOW);
        gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_2OFF) == 0){
        digitalWrite(6, LOW);
        gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_2ON) == 0){
        digitalWrite(6, HIGH);
        gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_0reset) == 0){
      digitalWrite(4, LOW);
      delay(3000);
      digitalWrite(4, HIGH);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_2reset) == 0){
      digitalWrite(6, LOW);
      delay(3000);
      digitalWrite(6, HIGH);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_allreset) == 0){
      digitalWrite(4, LOW);
      digitalWrite(6, LOW);
      delay(3000);
      digitalWrite(4, HIGH);
      digitalWrite(6, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_alloff) == 0){
      digitalWrite(4, LOW);
      digitalWrite(6, LOW);
        gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_allon) == 0){
      digitalWrite(4, HIGH);
      digitalWrite(6, HIGH);
        gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S1) == 0){
      digitalWrite(12, HIGH);
      delay(3000);
      digitalWrite(12, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S2) == 0){
      digitalWrite(8, HIGH);
      delay(3000);
      digitalWrite(8, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S3) == 0){
      digitalWrite(9, HIGH);
      delay(3000);
      digitalWrite(9, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S4) == 0){
      digitalWrite(13, HIGH);
      delay(3000);
      digitalWrite(13, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S5) == 0){
      digitalWrite(19, HIGH);
      delay(3000);
      digitalWrite(19, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S1RESET) == 0){
      digitalWrite(14, HIGH);
      delay(3000);
      digitalWrite(14, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S2RESET) == 0){
      digitalWrite(15, HIGH);
      delay(3000);
      digitalWrite(15, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S3RESET) == 0){
      digitalWrite(16, HIGH);
      delay(3000);
      digitalWrite(16, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S4RESET) == 0){
      digitalWrite(7, HIGH);
      delay(3000);
      digitalWrite(7, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_S5RESET) == 0){
      digitalWrite(18, HIGH);
      delay(3000);
      digitalWrite(18, LOW);
      gprs.sendSMS(phone, massage);
      } else if(strcmp(massage, MESSAGE_ARDUINO) == 0){
      gprs.sendSMS(phone, massage);
      digitalWrite(5, HIGH);
      }
    }
  }
