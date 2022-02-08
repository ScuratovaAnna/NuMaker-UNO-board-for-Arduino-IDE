/*
22.09.2021
*/

#define LED PB13
void setup() {
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
  uint32_t period  = 12000000;
  Timer1.open(PERIODIC, period );
  Timer1.attachInterrupt(timer_ISR);
  Timer1.start();
}

void loop() {
  
}
/* Прерывание срабатывает один раз в секунду */
void timer_ISR(uint8_t num)
{
  LED ^= 1;
}