/**---------------------------------------------
*\date  29.05.2021
*  http://publicatorbar.ru/2017/12/21/arduino-multi-function-shield/
* В примере используется аппаратный SPI(SPI0).
*
*    NuMaker UNO                  Arduino Shild
*   ------------                ----------------
*  |            |              |
*  |  10 (PA.13)|------------->| Pin 4 (LATCH_DIO)
*  |  27 (PC.1) |------------->| Pin 7 (CLK_DIO)  (NuMaker UNO SPI Interface)
*  |  25 (PC.3) |------------->| Pin 8 (DATA_DIO) (NuMaker UNO SPI Interface)
*  |            |              | 
*  |            |              | 
*  |        GND |<------------>| GND
*  |       +5V  |<------------>| +5V
*
*\ author ScuratovaAnna 
*\ сode debugging PivnevNikolay
*/

#include <SPI.h>
// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
/* -------------------- байты чисел от 0 до 9... 10->пустота, 11->минус --------------------------- */
/*                           0    1   2    3    4    5    6    7   8    9    10   11                */
/*const byte SEGMENT_MAP[] = {0x3F, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0X7F, 0X6F, 0X00, 0X40};*/
/* Байт-карты для выбора разряда дисплея от 1 до 4                                                  */
/* ---------------------------- 0     1   2    3 -------------------------------------------------- */
const byte SEGMENT_SELECT[] = {0xF7, 0xFB, 0xFD, 0xFE};
uint8_t symbol[4];
int i = 0;
float z = 0.0;
boolean variable = false;

void setup () {
  pinMode (slaveSelectPin, OUTPUT);
  SPI.begin();
  //-------------------------------//TIM_1
  int clock = 12000000; /* 12Mhz */
  Timer1.open(PERIODIC, clock);
  Timer1.setPrescaleFactor(0);
  Timer1.setCompare(300000);
  Timer1.attachInterrupt(timer_ISR_1);
  Timer1.start();
  //-------------------------------//TIM_2
  Timer2.open(PERIODIC, clock);
  Timer2.setPrescaleFactor(0);
  Timer2.setCompare(300000);
  Timer2.detachInterrupt();
  Timer2.start();
  //-------------------------------//TIM_3
  Timer3.open(PERIODIC, clock);
  Timer3.setPrescaleFactor(0);
  Timer3.setCompare(300000);
  Timer3.detachInterrupt();
  Timer3.start();
  //-------------------------------//TIM_4
  Timer4.open(PERIODIC, clock);
  Timer4.setPrescaleFactor(0);
  Timer4.setCompare(300000);
  Timer4.detachInterrupt();
  Timer4.start();
}

void loop() {  
  switch (variable) {
    case 0:
      Display_Send_int32_t(i);
      break;
    case 1:
      Display_Send_float(z);
      break;
  }  
}

/* отправляем значение (символ) в один из четырех разрядов дисплея */
void WriteNumberToSegment(byte Segment, byte Value)
{
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(symbol[Value]);
  SPI.transfer(SEGMENT_SELECT[Segment]);//нетрогать выбор сигмента
  digitalWrite(slaveSelectPin, HIGH);
}

void SEND_SYMBOL(void) {
  WriteNumberToSegment(0 , 0);//записываем в 0 разряд
  delay(1);
  WriteNumberToSegment(1 , 1);//записываем в 1 разряд
  delay(1);
  WriteNumberToSegment(2 , 2);//записываем в 2 разряд
  delay(1);
  WriteNumberToSegment(3 , 3);//записываем в 3 разряд
  delay(1);
}
//------МАССИВ_1_вывод_значения_без_точки---------//
void Display_the_value (uint8_t rg, uint32_t value) {
  switch (value) {
    case 0:
      symbol[rg] =  0x3F; // 0
      break;
    case 1:
      symbol[rg] =  0x6;  // 1
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
      symbol[rg] =  0x7;  // 7
      break;
    case 8:
      symbol[rg] =  0x7F; // 8
      break;
    case 9:
      symbol[rg] =  0X6F; // 9
      break;
    case 254:
      symbol[rg] =  0X40; // - минус
      break;
    case 255:
      symbol[rg] =  0x00; // - пустота
      break;
    case 256:
      symbol[rg] =  0X8; //- подчеркивание
      break;
  }
}
//------МАССИВ_2_вывод_значения_с_точкой----------//
void Display_the_Float_value (uint8_t rg, uint32_t value) {
  switch (value) {
    case 0:
      symbol[rg] =  0xBF; // 0.
      break;
    case 1:
      symbol[rg] =  0x86; // 1.
      break;
    case 2:
      symbol[rg] =  0xDB; // 2.
      break;
    case 3:
      symbol[rg] =  0xCF; // 3.
      break;
    case 4:
      symbol[rg] =  0xE6; // 4.
      break;
    case 5:
      symbol[rg] =  0xED; // 5.
      break;
    case 6:
      symbol[rg] =  0xFD; // 6.
      break;
    case 7:
      symbol[rg] =  0x87; // 7.
      break;
    case 8:
      symbol[rg] =  0xFF; // 8.
      break;
    case 9:
      symbol[rg] =  0XEF; // 9.
      break;
    case 254:
      symbol[rg] =  0X40;// - минус
      break;
    case 255:
      symbol[rg] =  0x00;// - пустота
      break;
    case 256:
      symbol[rg] =  0X8;// - подчеркивание
      break;
  }
}
//-------Функция_отправки_значений_int------------//
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
//--------Функция_отправки_значений_float---------//
void Display_Send_float(float value) {
  if (value > 999.9f || value < -99.9f) {
    Display_the_Float_value(0, 256);
    Display_the_Float_value(1, 256);
    Display_the_Float_value(2, 256);
    Display_the_Float_value(3, 256);
    SEND_SYMBOL();
  } else {
    //умножаем на 10 т.к количество знаков после запятой один!
    int32_t value_conv = value * 10;
    value = value * 10;
    //знак -------------------
    bool positive_value;
    if (value_conv < 0) {
      positive_value = false;
      value_conv = value_conv * (-1);
    } else {
      positive_value = true;
    }
    //массив ------------------
    uint8_t BCD_Arr[4] = { 0, };
    int32_t lenght;
    lenght = value_conv;
    while (value_conv >= 1000) {
      value_conv -= 1000;
      BCD_Arr[3]++;
    }
    while (value_conv >= 100) {
      value_conv -= 100;
      BCD_Arr[2]++;
    }
    while (value_conv >= 10) {
      value_conv -= 10;
      BCD_Arr[1]++;
    }
    BCD_Arr[0] = (uint8_t) (value_conv);

    if (lenght < 10) {
      Display_the_value(3, 255);

      if (positive_value == true) {
        Display_the_value(2, 255);
      } else {
        Display_the_value(2, 254);
      }
      Display_the_Float_value(1, BCD_Arr[1]);
      Display_the_value(0, BCD_Arr[0]);
      SEND_SYMBOL();
    }
    else if (lenght < 100) {
      Display_the_value(3, 255);
      if (positive_value == true) {
        Display_the_value(2, 255);
      } else {
        Display_the_value(2, 254);
      }
      Display_the_Float_value(1, BCD_Arr[1]);
      Display_the_value(0, BCD_Arr[0]);
      SEND_SYMBOL();
    }
    else if (lenght < 1000) {
      if (positive_value == true) {
        Display_the_value(3, 255);
      } else {
        Display_the_value(3, 254);
      }
      Display_the_value(2, BCD_Arr[2]);
      Display_the_Float_value(1, BCD_Arr[1]);
      Display_the_value(0, BCD_Arr[0]);
      SEND_SYMBOL();
    }
    else if (lenght < 10000) {
      if (positive_value == false) {
        Display_the_value(0, 256);
        Display_the_value(1, 256);
        Display_the_value(2, 256);
        Display_the_value(3, 256);
      }
      Display_the_value(3, BCD_Arr[3]);
      Display_the_value(2, BCD_Arr[2]);
      Display_the_Float_value(1, BCD_Arr[1]);
      Display_the_value(0, BCD_Arr[0]);
      SEND_SYMBOL();
    }
  }
}
//--------обработчик_прерывания_TIM_1-------------//
void timer_ISR_1(uint8_t num)
{
  i++;
  if (i == 999) {
    Timer1.detachInterrupt();
    Timer2.attachInterrupt(timer_ISR_2);
  }
}
//--------обработчик_прерывания_TIM_2-------------//
void timer_ISR_2(uint8_t num)
{
  i--;
  if (i == -999) {    
    Timer3.attachInterrupt(timer_ISR_3);
    Timer2.detachInterrupt();
	variable = true;
	i=0;
  }
}
//--------обработчик_прерывания_TIM_3-------------//
void timer_ISR_3(uint8_t num)
{
	z+=0.1;
	if(z > 99.9f){
  Timer4.attachInterrupt(timer_ISR_4);
  Timer3.detachInterrupt();
	}
}
//--------обработчик_прерывания_TIM_4-------------//
void timer_ISR_4(uint8_t num)
{
	z-=0.1;
	if(z < -99.9f){
  Timer1.attachInterrupt(timer_ISR_1);
  Timer4.detachInterrupt();
  variable = false;
  z=0.0;
	}
}
