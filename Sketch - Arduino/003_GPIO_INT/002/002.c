//
//-----------------------------------------------------------------------//
/*
*      1        1->P2.0->17
*     ---       2->P2.1->16
*  6 | 7 |2     3->P2.2->15
*     ---       4->P2.3->14
*  5 | 4 |3     5->P2.4->13
*     ---       6->P2.5->12
*               7->P0.0->23
*      ++       USR_SW P1.6
*      --       USR_SW P1.7
*  Autor: ScuratovaAnna + PivnevNikolay
*/
uint8_t led[10]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};
volatile boolean condition = true;
uint8_t Count = 0;
void setup() {
GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 ), GPIO_PMD_OUTPUT);
pinMode(7, OUTPUT);
pinMode(6, INPUT);//PA15--> Button_Pin [add]
pinMode(5, INPUT);//PA11--> Button_Pin [lower]
attachInterrupt(6, append, RISING);
attachInterrupt(5, lower,  RISING);
//PA->DOUT=led[9];
PA->DOUT = *(led+0);
}

void loop() {
switch(Count){      
 case 0:
 PA->DOUT = *(led+0);
 condition = true;
 break;
 case 1:
 PA->DOUT = *(led+1);
 condition = true;
 break;
 case 2:
 PA->DOUT = *(led+2);
 condition = true;
 break;
 case 3:
 PA->DOUT = *(led+3);
 condition = true;
 break;
 case 4:
 PA->DOUT = *(led+4);
 condition = true;
 break;
 case 5:
 PA->DOUT = *(led+5);
 condition = true;
 break;
 case 6:
 PA->DOUT = *(led+6);
 condition = true;
 break;
 case 7:
 PA->DOUT = *(led+7);
 condition = true;
 break;
 case 8:
 PA->DOUT = *(led+8);
 condition = true;
 break;
 case 9:
 PA->DOUT = *(led+9);
 condition = true;
 break;
}
  while(condition){
  pinMode(7, HIGH);
  delay(1000);
  pinMode(7, LOW);
  delay(1000);
  }
}

//interrupt handling 1
void append()
{

}

//interrupt handling 2 
void lower()
{

}
