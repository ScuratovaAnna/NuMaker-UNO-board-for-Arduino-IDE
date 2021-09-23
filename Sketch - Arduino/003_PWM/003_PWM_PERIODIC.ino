/*
23.09.2021
*/

#define LED PB13
uint32_t get_clock;
uint32_t GetHXT;
void setup(){
  Serial.begin(115200);//инициализируем и задает скорость передачи данных UART0_RXD, UART0_TXD
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
  uint32_t clock = 12000000; /* 12Mhz */
  Timer1.open(PERIODIC, clock);
  Timer1.setPrescaleFactor(0);
  Timer1.setCompare(12000000);
  Timer1.attachInterrupt(timer_ISR_1);
  Timer1.start();
  Timer2.open(PERIODIC, clock);
  Timer2.setPrescaleFactor(0);
  Timer2.setCompare(12000000);
  Timer2.detachInterrupt();
}

void loop(){}

void timer_ISR_1(uint8_t num){
 Serial.println("1");
 get_clock=TIMER_GetModuleClock(TIMER0);
 Serial.println(get_clock);
 Timer1.detachInterrupt();
 Timer2.attachInterrupt(timer_ISR_2); 
 Timer2.start();
 Timer1.close();
 //TIMER_Stop(TIMER0);
 LED ^= 1;
 get_clock=0; 
}

void timer_ISR_2(uint8_t num)
{
 Serial.println("2");
 GetHXT=CLK_GetHXTFreq();
 Serial.println(GetHXT); 
 Timer1.attachInterrupt(timer_ISR_1);
 Timer2.detachInterrupt();
 Timer2.close();
 //TIMER_Stop(TIMER1);
 LED ^= 1; 
 Timer1.start(); 
 GetHXT=0;
}
