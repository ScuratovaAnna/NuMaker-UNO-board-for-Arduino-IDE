//http://publicatorbar.ru/2017/12/21/arduino-multi-function-shield/
#include <SPI.h>
// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
/*                      байты чисел от 0 до 9... 10->пустота, 11->минус              */
/*                           0    1   2    3    4    5    6    7   8    9    10   11 */
const byte SEGMENT_MAP[] = {0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0X7F,0X6F,0X00,0X40};
/* Байт-карты для выбора разряда дисплея от 1 до 4                                   */
/*                              0     1   2    3                                     */
const byte SEGMENT_SELECT[] = {0xF7,0xFB,0xFD,0xFE};
uint8_t symbol[4];
int8_t znachenia_D[19]={-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9};
int8_t znachenia_S[19]={-99,-88,-77,-66,-55,-44,-33,-22,-11,0,19,28,37,46,55,64,73,82,91};
int16_t znachenia_T[19]={-990,-881,-772,-663,-554,-445,-336,-227,-118,0,199,288,377,466,555,644,733,822,911};
int i =0;

 
void setup (){
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin(); 
  //-------------------------------// 
  int clock =12000000; /* 12Mhz */
  Timer1.open(PERIODIC,clock);
  Timer1.setPrescaleFactor(0);
  Timer1.setCompare(100000);  
  Timer1.attachInterrupt(timer_ISR_1);
  Timer1.start();
  //-------------------------------//
  Timer2.open(PERIODIC,clock);
  Timer2.setPrescaleFactor(0);
  Timer2.setCompare(100000);  
  Timer2.detachInterrupt();
  Timer2.start();
  }
 
void loop(){ 

//Display_Send_int32_t(znachenia_T[i]);
Display_Send_int32_t(i);

}
 
/* отправляем десятичное число от 0 до 9 в один из четырех разрядов дисплея */
void WriteNumberToSegment(byte Segment, byte Value)
{
digitalWrite(slaveSelectPin,LOW);
SPI.transfer(symbol[Value]);
SPI.transfer(SEGMENT_SELECT[Segment]);//нетрогать выбор сигмента
digitalWrite(slaveSelectPin,HIGH); 
}

void SEND_SYMBOL(void){
WriteNumberToSegment(0 , 0);//записываем в нулевой (старший) разряд 
delay(1);
WriteNumberToSegment(1 , 1);//записываем в 1 разряд 
delay(1);
WriteNumberToSegment(2 , 2);//записываем в 2 разряд 
delay(1);
WriteNumberToSegment(3 , 3);//записываем в 3 разряд 
delay(1);  
}

void Display_the_value (uint8_t rg, uint32_t value) {
  switch (value) {
  case 0:
    symbol[rg] =  0x3F; // 0
    break;
  case 1:
    symbol[rg] =  0x6; // 1
    break;
  case 2:
    symbol[rg] =  0x5B; // 2
    break;
  case 3:
    symbol[rg] =  0x4F; // 3
    break;
  case 4:
    symbol[rg] =  0x66; // 4
    break;
  case 5:
    symbol[rg] =  0x6D; // 5
    break;
  case 6:
    symbol[rg] =  0x7D; // 6
    break;
  case 7:
    symbol[rg] =  0x7; // 7
    break;
  case 8:
    symbol[rg] =  0x7F; // 8
    break;
  case 9:
    symbol[rg] =  0X6F; // 9
    break;
  case 254:
    symbol[rg] =  0X40; //  0X40 минус
    break;
  case 255:
    symbol[rg] =  0x00; //  пустота
    break;
  case 256:
    symbol[rg] =  0X8;  //_ подчеркивание
    break;
  }
}
void Display_Send_int32_t(int32_t value) {    
  if (value > 9999 || value < -999) {
    Display_the_value(0, 256);
    Display_the_value(1, 256);
    Display_the_value(2, 256);
    Display_the_value(3, 256);
    SEND_SYMBOL();
    } else {
    bool positive_value;
    if (value < 0) {
      positive_value = false;
      value = value * (-1);
    } else {
      positive_value = true;
    }
    uint8_t BCD_Arr[4] = { 0, };
    uint32_t lenght;
    lenght = value;
    while (value >= 1000) {
      value -= 1000;
      BCD_Arr[3]++;

    }
    while (value >= 100) {
      value -= 100;
      BCD_Arr[2]++;

    }
    while (value >= 10) {
      value -= 10;
      BCD_Arr[1]++;

    }
    BCD_Arr[0] = (uint8_t) (value);
      //-------------------------------//
      if (lenght < 10) {
      Display_the_value(3, 255);
      Display_the_value(2, 255);
      Display_the_value(1, 255);

      if (positive_value == true) {
        Display_the_value(1, 255);
      } else {
        Display_the_value(1, 254);
      }
      Display_the_value(0, BCD_Arr[0]);
      SEND_SYMBOL();
      }
      //-------------------------------//
      else if (lenght < 100) {
      Display_the_value(3, 255);
      Display_the_value(2, 255);
      if (positive_value == true) {
        Display_the_value(2, 255);
      } else {
        Display_the_value(2, 254);
      }
      Display_the_value(1, BCD_Arr[1]);
      Display_the_value(0, BCD_Arr[0]);
      SEND_SYMBOL();
      }
      //-------------------------------//
      else if (lenght < 1000) { 
      Display_the_value(3, 255);
      if (positive_value == true) {
       Display_the_value(3, 255);
      } else {
       Display_the_value(3, 254);
      }
      Display_the_value(2, BCD_Arr[2]);
      Display_the_value(1, BCD_Arr[1]);
      Display_the_value(0, BCD_Arr[0]);
      SEND_SYMBOL();
      }
      //-------------------------------//
      else if (lenght < 10000) { 
      if (positive_value == false) {
      Display_the_value(0, 256);
      Display_the_value(1, 256);
      Display_the_value(2, 256);
      Display_the_value(3, 256);
      }       
      Display_the_value(3, BCD_Arr[3]);
      Display_the_value(2, BCD_Arr[2]);
      Display_the_value(1, BCD_Arr[1]);
      Display_the_value(0, BCD_Arr[0]);
      SEND_SYMBOL();
      }
}
}
//обработчик прерывания Timer1
void timer_ISR_1(uint8_t num)
{ 
i++;
if(i==999){
Timer1.detachInterrupt();
Timer2.attachInterrupt(timer_ISR_2);

}
}
//обработчик прерывания Timer1
void timer_ISR_2(uint8_t num)
{
  i--;
  if(i==-999){
Timer1.attachInterrupt(timer_ISR_1);
Timer2.detachInterrupt();
  }  
}
