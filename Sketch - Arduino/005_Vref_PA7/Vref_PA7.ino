 /**
*\date  09.02.2022
*\brief
*Данный код необходим для проверки PA.7
*PA.7 будет необходим для задействования порта A целиком PA.0-PA.15
*\authors ScuratovaAnna + PivnevNikolay
*/
 #define LED1 PA7=1
 #define LED0 PA7=0
 void setup()
  {
  GPIO_SetMode(PA, BIT7, GPIO_PMD_OUTPUT);
  }
 void loop()
  {
  LED1;
  delay(1000);
  LED0;
  delay(1000);
  }