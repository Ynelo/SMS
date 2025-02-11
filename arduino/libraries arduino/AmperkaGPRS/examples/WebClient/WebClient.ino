// библиотека для работы с GPRS устройством
#include <AmperkaGPRS.h>

// массив, для хранения IP-адреса
char ipAdress[16];

// создаём объект класса GPRS и передаём в него объект Serial1 
GPRS gprs(Serial1);
// можно указать дополнительные параметры — пины PK и ST
// по умолчанию: PK = 2, ST = 3
// GPRS gprs(Serial1, 2, 3);

// создаём клиента, который будет подключаться
// к необходимому для нас серверу и порту
GPRSClient client;

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
  Serial.println("GPRS begin success");

  // включаем передачу данных через GPRS соеденинение
  while (!gprs.dataTransferOn("internet", "gdata", "gdata")) {
    // если GPRS-соединения нет
    // выводим сообщение об ошибке и ждём 1 секунду
    // процесс повторяется в цикле
    // пока не появится положительный ответ от GPRS устройства
    Serial.println("Data Transfer On is Error");
    delay(1000);
  }
  // выводим сообщение об удачной установке GPRS-соединения
  Serial.println("Data Transfer On is OK");
  // получаем  и выводим локальный IP адрес
  gprs.getIPAddress(ipAdress);
  Serial.print("IP Address is ");
  Serial.println(ipAdress);
  // пытаемся подключиться к серверу
  // указывая тип соединения, адрес сервера и номер порта
  if (client.connect(TCP, "arduino.cc", 80)) {
    // если сооедение успешно
    // выводим сообщение об удачном подключении к серверу
    Serial.println("Connect success");
    // отправляем HTTP-запрос
    client.println("GET /latest.txt HTTP/1.1");
    client.println("Host: arduino.cc");
    client.println("Connection: close");
    client.println("");
  } else {
    // если сервер не отвечает или отвечает ошибкой
    // выводим сообщение об ошибке
    Serial.println("Connect error");
  }
  Serial.println("Header OK");
}

void loop()
{
  // если есть данные с GPRS-шилда
  if (client.available()) {
    // считываем данные
    char c = client.read();
    // печатаем в Serial-порт
    Serial.print(c);
  }
  // если сервер отключился
  if (!client.connected()) {
    // печатаем об этом в Serial-порт
    Serial.println("Disconnecting...");
    // останавливаем работу клиента
    client.disconnect();
    // останавливаем программу в бесконечном цикле
    while (true);
  }
}
