
void setup(){
  Serial.begin(115200);//инициализируем и задает скорость передачи данных UART0_RXD, UART0_TXD
  int clock = 12000000; /* 12Mhz */
  Timer1.open(PERIODIC, clock);
  Timer1.setPrescaleFactor(0);
  Timer1.setCompare(1200000);
  Timer1.attachInterrupt(timer_ISR_1);
  Timer1.start();
  Timer2.open(PERIODIC, clock);
  Timer2.setPrescaleFactor(0);
  Timer2.setCompare(1200000);
  Timer2.detachInterrupt();
}

void loop(){}

void timer_ISR_1(uint8_t num){
 Serial.println("1");
 Timer1.detachInterrupt();
 Timer2.attachInterrupt(timer_ISR_2); 
 Timer2.start();
 TIMER_Stop(TIMER0); 
}

void timer_ISR_2(uint8_t num)
{
 Serial.println("2");
 Timer1.attachInterrupt(timer_ISR_1);
 Timer2.detachInterrupt();
 TIMER_Stop(TIMER1); 
 Timer1.start();  
}
