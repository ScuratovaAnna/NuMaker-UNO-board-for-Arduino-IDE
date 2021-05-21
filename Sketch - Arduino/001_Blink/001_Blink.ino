
#include <nvtSoftwareSerial.h>// подключаем библиотеку SoftwareSerial
SoftwareSerial mySerial(10, 11); // назначим выводы RX, TX (PA.13 PA.12)

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
  Serial.begin(9600);//Задает скорость передачи данных
  //Задает скорость передачи данных для последовательного порта, созданного библиотекой
  mySerial.begin(115200);
  mySerial.println("Hello, world?");
  Serial.println("NuMaker UNO board for Arduino IDE.");
}


void loop() {
  //digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  PB13=1;
  mySerial.println("Led ON");
  Serial.println("1");
  delay(1000);                       // wait for a second
  //digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  PB13=0;
  mySerial.println("Led Off");
  Serial.println("0");
  delay(1000);                       // wait for a second
}
