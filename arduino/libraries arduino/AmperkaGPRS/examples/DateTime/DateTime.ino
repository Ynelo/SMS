// библиотека для работы с GPRS устройством
#include <AmperkaGPRS.h>

// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(Serial1);
// можно указать дополнительные параметры — пины PK и ST
// по умолчанию: PK = 2, ST = 3
// GPRS gprs(Serial1, 2, 3);

// время, которое установим в RTC GPRS Shiled’a
char dateTime[32]="24/08/16,15:41:52+03";
// массив для хранения текущего времени и даты
char nowDateTime[32];

void setup()
{
  // открываем последовательный порт для мониторинга действий в программе
  Serial.begin(9600);
  // ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  while (!Serial) {
  }
  Serial.print("Serial init OK\r\n");
  // открываем Serial-соединение с GPRS Shield
  Serial1.begin(9600);
  // включаем GPRS шилд
  gprs.powerOn();
  // проверяем есть ли связь с GPRS устройством
  while (!gprs.begin()) {
    // если связи нет, ждём 1 секунду
    // и выводим сообщение об ошибке
    // процесс повторяется в цикле
    // пока не появится ответ от GPRS устройства
    delay(1000);
    Serial.print("GPRS Init error\r\n");
  }
  // вывод об удачной инициализации GPRS Shield
  Serial.println("GPRS init success");
  while(!gprs.setDateTime(dateTime)) {
    Serial.println("DateTime is not set");
  }
  Serial.println("DateTime set OK");
}

void loop()
{
  // получаем текущее время
  gprs.getDateTime(nowDateTime);
  Serial.print("DateTime: ");
  Serial.println(nowDateTime);
}
