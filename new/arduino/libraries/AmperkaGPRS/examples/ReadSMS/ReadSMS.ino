// библиотека для работы с GPRS устройством
#include <AmperkaGPRS.h>

// длина сообщения
#define MESSAGE_LENGTH 160

// текст сообщения
char message[MESSAGE_LENGTH];
// номер, с которого пришло сообщение
char number[16];
// дата отправки сообщения
char datetime[24];
 
// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(Serial1);
// можно указать дополнительные параметры — пины PK и ST
// по умолчанию: PK = 2, ST = 3
// GPRS gprs(Serial1, 2, 3);
 
void setup()
{
  // открываем последовательный порт для мониторинга действий в программе
  Serial.begin(9600);
  // ждём пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  while (!Serial) {
  }
  Serial.print("Serial init OK\r\n");
  // открываем Serial-соединение с GPRS Shield
  Serial1.begin(9600);
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
  Serial.println("Please send SMS message to me!");
}
 
void loop()
{
  // если пришло новое сообщение
  if (gprs.incomingSMS()) {
    // читаем его
    gprs.readSMS(message, number, datetime);

    // выводим номер, с которого пришло смс
    Serial.print("From number: ");
    Serial.println(number);
 
    // выводим дату, когда пришло смс
    Serial.print("Datetime: ");
    Serial.println(datetime);
 
    // выводим текст сообщения
    Serial.print("Recieved Message: ");
    Serial.println(message);
  }
}
