/**-----------------------------------------------------------------------
 *    
 * NUC131SD2AE_LQFP64
 *   ------------ 
 *  |            |
 *  |            |
 *  |       PB.13| ---> LED   _|¯|_  Period=300us +Width=300us -Width=600ms
 *  |            |
 *  |            |
 *  |        +5V | <--   +5V
 *  |        GND | <--   GND 
 *
 *  TIMER ONESHOT MODE --> Пример без прерываний (опрос флага).
 *
 * author           ScuratovaAnna 
 * сode debugging   PivnevNikolay
 *
 *
 *************************************************************************/
 
#define TIMER_SET_MODE(timer, u32Mode)   ((timer)->TCSR = ((timer)->TCSR & ~TIMER_TCSR_MODE_Msk) | (u32Mode))

__IO uint32_t get_clock;

void SYS_Init(void);
void delay_us(uint32_t us);

void setup() {
  SYS_Init();
  Serial.begin(9600);
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
}

void loop() {
  PB13=!PB13;
  delay_us(300);
 // get_clock=TIMER_GetModuleClock(TIMER0);
 // Serial.println(get_clock);// get_clock = 48000000 
}

void SYS_Init(void){
  SYS_UnlockReg();
  CLK_EnableModuleClock(TMR0_MODULE);
  CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HCLK, 0);
  SYS_LockReg();
}

void delay_us(uint32_t us) {
  __IO uint32_t u32_ticks = (SystemCoreClock / 1000000);
  TIMER_SET_MODE(TIMER0, TIMER_ONESHOT_MODE);
  TIMER_SET_PRESCALE_VALUE(TIMER0, u32_ticks - 1);
  TIMER_SET_CMP_VALUE(TIMER0, us);
  TIMER_Start(TIMER0);
  while (!TIMER_GetIntFlag(TIMER0)); // Ожидание завершения
  TIMER_ClearIntFlag(TIMER0);        // Сброс флага
}