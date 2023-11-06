// библиотека для работы с GPRS устройством
#include <GPRS_Shield_Arduino.h>
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
bool flag = false;
String message;

SoftwareSerial mySerial(10, 11);
//SoftwareSerial mySerial (0, 1);
// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(mySerial);
// можно указать дополнительные параметры — пины PK и ST
// по умолчанию: PK = 2, ST = 3
//GPRS gprs(Serial1, 2, 3);
 
void setup()
{
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(7, INPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
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
  while (!gprs.init()) {
    // если связи нет, ждём 1 секунду
    // и выводим сообщение об ошибке;
    // процесс повторяется в цикле,
    // пока не появится ответ от GPRS-устройства
    delay(1000);
    Serial.print("GPRS Init error\r\n");
    digitalWrite(12, HIGH);
  }
  // выводим сообщение об удачной инициализации GPRS Shield
  Serial.println("GPRS init success");
}
int vale;
void loop()
{
  vale = digitalRead(7);
  if(vale == 0 & flag == false){
   // gprs.sendSMS("+79097669661", "!!WARNING!! BLCAKOUT IN OFFICE!!!");
    gprs.sendSMS(phone, "!!WARNING!! BLCAKOUT IN OFFICE!!!");
    flag = true;
    Serial.println("---------");
  } else if(vale == 1 & flag == true){
   // gprs.sendSMS("+79097669661", "The light turned on.");
    gprs.sendSMS(phone, "The light turned on.");
    Serial.println("+++++++");
    flag = false;
   }
/*  if (!gprs.init()) {
   Serial.print("GPRS Init error\r\n");
   digitalWrite(12, HIGH);
 } else {
   digitalWrite(12, LOW);
Serial.println("GPRS init success"); 
}*/
  if (gprs.ifSMSNow()) {
    gprs.readSMS(massage, phone, datetime);
    message = massage;
    Serial.println(message);
    if(message == "2Off" | message == "2off"){
      digitalWrite(6, LOW);
      gprs.sendSMS(phone, "2Off");
    } else if(message == "2On" | message == "2on"){
      digitalWrite(6, HIGH);
      gprs.sendSMS(phone, "2On");
    } else if(message == "0On" | message == "0on"){
      digitalWrite(4, HIGH);
      gprs.sendSMS(phone, "0On");
    } else if(message == "0Off" | message == "0off"){
      digitalWrite(4, LOW);
      gprs.sendSMS(phone, "0Off");
    } else if(message == "2Reset" | message == "2reset"){
      digitalWrite(6, LOW);
      delay(3000);
      digitalWrite(6, HIGH);
      gprs.sendSMS(phone, "2Reset");
    } else if(message == "0Reset" | message == "0reset"){
      digitalWrite(4, LOW);
      delay(3000);
      digitalWrite(4, HIGH);
      gprs.sendSMS(phone, "0Reset");
      } else if(message == "Allreset" | message == "allreset"){
      digitalWrite(4, LOW);
      digitalWrite(6, LOW);
      delay(3000);
      digitalWrite(4, HIGH);
      digitalWrite(6, LOW);
      gprs.sendSMS(phone, "AllReset");
      } else if(message == "Alloff" | message == "alloff"){
      digitalWrite(4, LOW);
      digitalWrite(6, LOW);
      gprs.sendSMS(phone, "Alloff");
      } else if(message == "Allon" | message == "allon"){
      digitalWrite(4, HIGH);
      digitalWrite(6, HIGH);
      gprs.sendSMS(phone, "Allon");
    } else if(message == "arduino" | message == "Arduino"){
      gprs.sendSMS(phone, "arduino Reset");
      digitalWrite(5, HIGH);
    } else if(message == "S1" | message == "s1"){
      digitalWrite(12, HIGH);
      delay(3000);
      digitalWrite(12, LOW);
      gprs.sendSMS(phone, "s1");
    } else if(message == "S2" | message == "s2"){
      digitalWrite(8, HIGH);
      delay(3000);
      digitalWrite(8, LOW);
      gprs.sendSMS(phone, "s2");
    } else if(message == "S3" | message == "s3"){
      digitalWrite(9, HIGH);
      delay(3000);
      digitalWrite(9, LOW);
      gprs.sendSMS(phone, "s3");
    } else if(message == "S4" | message == "s4"){
      digitalWrite(13, HIGH);
      delay(3000);
      digitalWrite(13, LOW);
      gprs.sendSMS(phone, "s4");
    } else if(message == "S5" | message == "s5"){
      digitalWrite(19, HIGH);
      delay(3000);
      digitalWrite(19, LOW);
      gprs.sendSMS(phone, "s5");
    }  else if(message == "S1reset" | message == "s1reset"){
      digitalWrite(14, HIGH);
      delay(3000);
      digitalWrite(14, LOW);
      gprs.sendSMS(phone, "s1reset");
    }  else if(message == "S2reset" | message == "s2reset"){
      digitalWrite(15, HIGH);
      delay(3000);
      digitalWrite(15, LOW);
      gprs.sendSMS(phone, "s2reset");
    }  else if(message == "S3reset" | message == "s3reset"){
      digitalWrite(16, HIGH);
      delay(3000);
      digitalWrite(16, LOW);
      gprs.sendSMS(phone, "s3reset");
    }  else if(message == "S4reset" | message == "s4reset"){
      digitalWrite(17, HIGH);
      delay(3000);
      digitalWrite(17, LOW);
      gprs.sendSMS(phone, "s4reset");
    }  else if(message == "S5reset" | message == "s5reset"){
      digitalWrite(18, HIGH);
      delay(3000);
      digitalWrite(18, LOW);
      gprs.sendSMS(phone, "s5reset");
    }
  }
}
