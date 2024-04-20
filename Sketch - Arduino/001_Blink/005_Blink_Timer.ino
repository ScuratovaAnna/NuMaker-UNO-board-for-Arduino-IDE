/**-------------------------------------------------------------------
*\ date  20.04.2024
*   NuMaker  UNO             
*   ---------------        
*  |               |       
*  |               |       
*  |  (PB.13)  13~ | ----> LED I\O
*  |               |
*
*\ author           ScuratovaAnna 
*\ сode debugging   PivnevNikolay
*/

/****************** Example 1 ***************************/
uint32_t get_clock;
void SYS_Init(void);
void setup() {
  SYS_Init();
  Serial.begin(9600);
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
}

void loop() {
  PB13=!PB13;
  /*
  *   --    --    --
  *  |  |  |  |  |  |
  *  |  |__|  |__|  |__ period 1s Freq 1Hz
  */
  TIMER_Delay(TIMER0, 500000);
  get_clock=TIMER_GetModuleClock(TIMER0);
  Serial.println(get_clock);// get_clock = 48000000 
}

void SYS_Init(void){
  SYS_UnlockReg();
  CLK_EnableModuleClock(TMR0_MODULE);
  CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HCLK, 0);
  SYS_LockReg();
}
/****************** Example 2 ***************************/
volatile uint16_t count;
void setup() {
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
}

void loop() {
  PB13=!PB13;
  delay_ms(1);
   /*
  *   --    --    --
  *  |  |  |  |  |  |
  *  |  |__|  |__|  |__ period 2s Freq 500mHz
  */
  }
/* Прерывание срабатывает один раз в секунду */
void timer_ISR(uint8_t num)
{
     if (count)
        count--;
}

void delay_ms(uint16_t ms)
{
  uint32_t clock = 12000000; /* 12Mhz */
  Timer1.open(PERIODIC, clock);
  Timer1.setPrescaleFactor(128);//Установим предделитель 128
  Timer1.setCompare(93750);//12000000/128=93750
  Timer1.attachInterrupt(timer_ISR);
  Timer1.start();
  count = ms;
  while(count);
}
/********************************************************/