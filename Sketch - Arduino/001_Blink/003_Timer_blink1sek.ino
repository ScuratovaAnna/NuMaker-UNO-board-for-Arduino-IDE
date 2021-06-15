/*
   Герц — Обозначается Гц или Hz — единица измерения частоты периодических процессов(например колебаний).
   1 Гц означает одно исполнение такого процесса за одну секунду:
   1 Гц = 1/с.
   Если мы имеем 10 Гц, то это означает, что мы имеет десять исполнений такого процесса за одну секунду.
   Базовые настройки таймера на генерацию прерывания в 1 секунду ->
   int clock = 12000000;
   Timer1.open(PERIODIC, clock);
   Timer1.setPrescaleFactor(0);
   Timer1.setCompare(clock);
   https://istarik.ru/blog/arduino/105.html#perepolnenie
*/
#define LED PB13
void setup() {
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
  uint32_t clock = 12000000; /* 12Mhz */
  /*Поменяем базовые настройки таймера*/
  Timer1.open(PERIODIC, clock);
  Timer1.setPrescaleFactor(128);//Установим предделитель 128
  Timer1.setCompare(93750);//12000000/128=93750
  Timer1.attachInterrupt(timer_ISR);
  Timer1.start();
}

void loop() {
  /*
     При использованием синтаксиса Arduino, нам доступно четыре таймера
     Timer1
     Timer2
     Timer3
     Timer4
  */
}
/* Прерывание срабатывает один раз в секунду */
void timer_ISR(uint8_t num)
{
  LED ^= 1;
}
