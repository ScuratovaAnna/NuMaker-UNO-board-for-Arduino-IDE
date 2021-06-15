//15.06.2021
//https://danchouzhou.blogspot.com/2017/08/arduino-idenuvoton-nuc131-series-bsp.html
/*
  //----------------1---------------------//
  void setup()
  {
  pinMode(LED, OUTPUT);
  }
  void loop()
  {
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
  }
  //----------------2---------------------//
  #define LED PB13
  void setup()
  {
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
  }
  void loop()
  {
  LED=1;
  delay(1000);
  LED=0;
  delay(1000);
  }
  //----------------3---------------------//
  void setup()
  {
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
  }
  void loop()
  {
  GPIO_TOGGLE(PB13);
  delay(1000);
  }
  //----------------end---------------------//
*/
//#include <stdio.h>
//#include "NUC131.h"
#define PLL_CLOCK   50000000

uint8_t text_LED_ON[] = "Led ON\n\r";
uint8_t text_LED_OFF[] = "Led OFF\n\r";

void SYS_Init(void)
{
  /*---------------------------------------------------------------------------------------------------------*/
  /* Init System Clock                                                                                       */
  /*---------------------------------------------------------------------------------------------------------*/
  /* Enable Internal RC 22.1184MHz clock */
  CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);
  /* Waiting for Internal RC clock ready */
  CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);
  /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
  CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));
  /* Enable external XTAL 12MHz clock */
  CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
  /* Waiting for external XTAL clock ready */
  CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);
  /* Set core clock as PLL_CLOCK from PLL */
  CLK_SetCoreClock(PLL_CLOCK);
  /* Enable UART module clock */
  CLK_EnableModuleClock(UART0_MODULE);
  /* Select UART module clock source */
  CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));
  /*---------------------------------------------------------------------------------------------------------*/
  /* Init I/O Multi-function                                                                                 */
  /*---------------------------------------------------------------------------------------------------------*/
  /* Set GPB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
  SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
  SYS->GPB_MFP |= (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);
}
void UART0_Init()
{
  /* Reset UART0 module */
  SYS_ResetModule(UART0_RST);
  /* Configure UART0 and set UART0 Baudrate */
  UART_Open(UART0, 115200);
}
void setup()
{
  /* Unlock protected registers */
  SYS_UnlockReg();
  /* Init System, peripheral clock and multi-function I/O */
  SYS_Init();
  /* Lock protected registers */
  SYS_LockReg();
  /* Init UART0 for printf */
  UART0_Init();
  GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
}
void loop() {
  GPIO_TOGGLE(PB13);
  CLK_SysTickDelay(349525);
  CLK_SysTickDelay(349525);
  UART_Write(UART0, &text_LED_ON[0], 8);
  GPIO_TOGGLE(PB13);
  CLK_SysTickDelay(349525);
  CLK_SysTickDelay(349525);
  UART_Write(UART0, &text_LED_OFF[0], 9);
}
