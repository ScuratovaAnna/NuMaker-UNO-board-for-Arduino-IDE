/*
22.09.2021
*/

#define LED PB13
uint8_t count=0;
uint32_t TDR;
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
void loop() {}
/*---------обработчик прерывания----------*/
void timer_ISR(uint8_t num)
{
  LED ^= 1;
  count++;
  TDR = (count <= 10) ? 93750 : 23000;
  Timer1.setCompare(TDR); 
  if(count>=20)count=0;
}
