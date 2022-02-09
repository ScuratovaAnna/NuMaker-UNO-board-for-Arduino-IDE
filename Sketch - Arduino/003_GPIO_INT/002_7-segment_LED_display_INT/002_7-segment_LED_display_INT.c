/**---------------------------------------------
*\date  09.02.2022
*\brief 
*      1        1->PA.0->44
*     ---       2->PA.1->45
*  6 | 7 |2     3->PA.2->46
*     ---       4->PA.3->47
*  5 | 4 |3     5->PA.4->48
*     ---       6->PA.5->49
*               7->PA.6->50
*      ++       USR_SW PA.15
*      --       USR_SW PA.11
*\authors ScuratovaAnna + PivnevNikolay
*/
///               0    1   2    3    4    5    6    7   8    9
uint8_t led[10]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
volatile boolean condition = true;
uint8_t Count = 0;
void setup() {
GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 ), GPIO_PMD_OUTPUT);
pinMode(7, OUTPUT);///Вывод для управления точки на сегментном индикаторе
pinMode(6, INPUT);/// \brief  PA15--> Button_Pin [add]
pinMode(5, INPUT);/// \brief  PA11--> Button_Pin [lower]
attachInterrupt(6, append, RISING);
attachInterrupt(5, lower,  RISING);
}

void loop() {
switch(Count){      
 case 0:
 PA->DOUT = *(led+0);///0
 condition = true;
 break;
 case 1:
 PA->DOUT = *(led+1);///1
 condition = true;
 break;
 case 2:
 PA->DOUT = *(led+2);///2
 condition = true;
 break;
 case 3:
 PA->DOUT = *(led+3);///3
 condition = true;
 break;
 case 4:
 PA->DOUT = *(led+4);///4
 condition = true;
 break;
 case 5:
 PA->DOUT = *(led+5);///5
 condition = true;
 break;
 case 6:
 PA->DOUT = *(led+6);///6
 condition = true;
 break;
 case 7:
 PA->DOUT = *(led+7);///7
 condition = true;
 break;
 case 8:
 PA->DOUT = *(led+8);///8
 condition = true;
 break;
 case 9:
 PA->DOUT = *(led+9);///9
 condition = true;
 break;
}/// мограем точкой на 7-сегментном светодиодном индикаторе
  while(condition){
  pinMode(7, HIGH);
  delay(200);
  pinMode(7, LOW);
  delay(200);
  }
}
/// \brief  interrupt handling 1
void append()
{
 condition = FALSE;
 Count++;
 if(Count>=9)Count=9;
}
/// \brief  interrupt handling 2 
void lower()
{
 condition = FALSE;
 if(Count==0)Count=1;
 Count--;
}
