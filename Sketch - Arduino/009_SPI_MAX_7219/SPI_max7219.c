#include <SPI.h>
//---------------------------------------------------------------------------//
/*Массив символов              0     1     2     3     4     5     6     7     8     9    */
const uint8_t massive[10] = { 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};
/*Массив символов                 0.    1.    2.    3.    4.    5.    6.    7.    8.    9. */
const uint8_t massive_point[10]={0xFE, 0xB0, 0xED, 0xF9, 0xB3, 0xDB, 0xDF, 0xF0, 0xFF, 0xFB};
//---------------------------------------------------------------------------//
const int slaveSelectPin = 10;
int16_t f;
uint8_t z;
float x=0.000;
uint8_t symbol[8];
//---------------------------------------------------------------------------//
void setup() {
  pinMode (slaveSelectPin, OUTPUT);
  SPI.begin();
  LCD_Init();
  Led_Display_Test(0x01);//тест дисплея, подадим питание на все сегменты
  Led_Display_Test(0x00);//тест дисплея, снимим питание с сегментов
  LCD_Display_Clean();
}
//---------------------------------------------------------------------------//
void loop() {
for ( z =1;z<9;z++){
  LCD_Send_Data_Transmit( z, massive[z-1]);
  delay(100);   
    }
  for ( z =8;z>0;z--){
  LCD_Send_Data_Transmit( z, 0x00);
  delay(100);   
  }
  for ( z =1;z<9;z++){
  LCD_Send_Data_Transmit( z, massive_point[z-1]);
  delay(100);   
  }
  for ( z =8;z>0;z--){
  LCD_Send_Data_Transmit( z, 0x00);
  delay(100);   
  }
  for ( f=0;f<2000;f++){
  max7219_Send_int32_t(f); 
  delay(3);
  }  
  for ( f=2000;f>-2000;f--){
  max7219_Send_int32_t(f); 
  delay(3);
  }  
  for ( f=0;f<=100;f++){
  x=x+0.001;
  max7219_Send_float(x);
  Serial.println(x);
  if (f==100)x=0.0; 
  delay(1000);
  }
}
//---------------------------------------------------------------------------//
void LCD_Init(void){
  delay(100);
  LCD_Send_Data_Transmit( 0x09, 0x00);//без режима декодирования
  LCD_Send_Data_Transmit( 0x0A, 0x0F);//значение яркости сегментов на максимум
  LCD_Send_Data_Transmit( 0x0B, 0x07);//задействуем все 8 разрядов дисплея
  LCD_Send_Data_Transmit( 0x0C, 0x01);//выводим дисплей из сна    
}

//---------------------------------------------------------------------------//
void LCD_Send_Data_Transmit( uint8_t  addre_ss, uint8_t  data){
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer (addre_ss);
  SPI.transfer(data); 
  digitalWrite(slaveSelectPin, HIGH);
}
//---------------------------------------------------------------------------//
void LCD_Display_Clean(void){
  LCD_Send_Data_Transmit( 0x01, 0x00);//обнулим первый сегмент дисплея
  delay(5);
  LCD_Send_Data_Transmit( 0x02, 0x00);//обнулим второй сегмент дисплея
  delay(5);
  LCD_Send_Data_Transmit( 0x03, 0x00);//обнулим третий сегмент дисплея
  delay(5);
  LCD_Send_Data_Transmit( 0x04, 0x00);//обнулим четвертый сегмент дисплея
  delay(5);
  LCD_Send_Data_Transmit( 0x05, 0x00);//обнулим пятый сегмент дисплея
  delay(5);
  LCD_Send_Data_Transmit( 0x06, 0x00);//обнулим шестой сегмент дисплея
  delay(5);
  LCD_Send_Data_Transmit( 0x07, 0x00);//обнулим седьмой сегмент дисплея
  delay(5);
  LCD_Send_Data_Transmit( 0x08, 0x00);//обнулим восьмой сегмент дисплея
  delay(5);        
}
//---------------------------------------------------------------------------//
void Led_Display_Test(uint8_t  data){
  LCD_Send_Data_Transmit(0x0F, data);
  delay(1000);
}
//---------------------------------------------------------------------------//
void LCD_SEND_SYMBOL(void){
   LCD_Send_Data_Transmit( 8,symbol[0]);
   LCD_Send_Data_Transmit( 7,symbol[1]);
   LCD_Send_Data_Transmit( 6,symbol[2]);
   LCD_Send_Data_Transmit( 5,symbol[3]);
   LCD_Send_Data_Transmit( 4,symbol[4]);
   LCD_Send_Data_Transmit( 3,symbol[5]);
   LCD_Send_Data_Transmit( 2,symbol[6]);
   LCD_Send_Data_Transmit( 1,symbol[7]);
}
//---------------------------------------------------------------------------//
void LCD_num(uint8_t rg, uint32_t value) {
  switch (value) {
  case 0:
    symbol[rg] =  0x7E; // 0
    break;
  case 1:
    symbol[rg] =  0x30; // 1
    break;
  case 2:
    symbol[rg] =  0x6D; // 2
    break;
  case 3:
    symbol[rg] =  0x79; // 3
    break;
  case 4:
    symbol[rg] =  0x33; // 4
    break;
  case 5:
    symbol[rg] =  0x5B; // 5
    break;
  case 6:
    symbol[rg] = 0x5F;  // 6
    break;
  case 7:
    symbol[rg] =  0x70; // 7
    break;
  case 8:
    symbol[rg] =  0x7F; // 8
    break;
  case 9:
    symbol[rg] =  0x7B; // 9
    break;
  case 254:
    symbol[rg] =  0x01; //- подчеркивание
    break;
  case 255:
    symbol[rg] =  0x00; //пустота
    break;
  }
}
//---------------------------------------------------------------------------//
//код подсмотрен здесь.)))
//https://www.youtube.com/watch?v=BonEEaQX8vg
void max7219_Send_int32_t(int32_t value) {    
  if (value > 99999999 || value < -9999999) {
    LCD_num(0, 254);
    LCD_num(1, 254);
    LCD_num(2, 254);
    LCD_num(3, 254);
    LCD_num(4, 254);
    LCD_num(5, 254);
    LCD_num(6, 254);
    LCD_num(7, 254);
    LCD_SEND_SYMBOL();
  } else {
    bool positive_value;
    if (value < 0) {
      positive_value = false;
      value = value * (-1);
    } else {
      positive_value = true;
    }
    uint8_t BCD_Arr[8] = { 0, };
    uint32_t lenght;
    lenght = value;
    while (value >= 10000000) {
      value -= 10000000;
      BCD_Arr[7]++;
    }

    while (value >= 1000000) {
      value -= 1000000;
      BCD_Arr[6]++;
    }
    while (value >= 100000) {
      value -= 100000;
      BCD_Arr[5]++;
    }
    while (value >= 10000) {
      value -= 10000;
      BCD_Arr[4]++;

    }
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
      LCD_num(0, 255);
      LCD_num(1, 255);
      LCD_num(2, 255);
      LCD_num(3, 255);
      LCD_num(4, 255);
      LCD_num(5, 255);
      if (positive_value == true) {
        LCD_num(6, 255);
      } else {
        LCD_num(6, 254);
      }
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
    } else if (lenght < 100) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      LCD_num(2, 255);
      LCD_num(3, 255);
      LCD_num(4, 255);
      if (positive_value == true) {
        LCD_num(5, 255);
      } else {
        LCD_num(5, 254);
      }
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();

    } else if (lenght < 1000) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      LCD_num(2, 255);
      LCD_num(3, 255);
      if (positive_value == true) {
        LCD_num(4, 255);
      } else {
        LCD_num(4, 254);
      }
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
    } else if (lenght < 10000) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      LCD_num(2, 255);
      if (positive_value == true) {
        LCD_num(3, 255);
      } else {
        LCD_num(3, 254);
      }
      LCD_num(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
    } else if (lenght < 100000) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      if (positive_value == true) {
        LCD_num(2, 255);
      } else {
        LCD_num(2, 254);
      }
      LCD_num(3, BCD_Arr[4]);
      LCD_num(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
    } else if (lenght < 1000000) {
      LCD_num(0, 255);
      if (positive_value == true) {
        LCD_num(1, 255);
      } else {
        LCD_num(1, 254);
      }
      LCD_num(2, BCD_Arr[5]);
      LCD_num(3, BCD_Arr[4]);
      LCD_num(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();

    } else if (lenght < 10000000) {
      if (positive_value == true) {
        LCD_num(0, 255);
      } else {
        LCD_num(0, 254);
      }
      LCD_num(1, BCD_Arr[6]);
      LCD_num(2, BCD_Arr[5]);
      LCD_num(3, BCD_Arr[4]);
      LCD_num(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
    } else if (lenght < 100000000) {
      LCD_num(0, BCD_Arr[7]);
      LCD_num(1, BCD_Arr[6]);
      LCD_num(2, BCD_Arr[5]);
      LCD_num(3, BCD_Arr[4]);
      LCD_num(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
    }
  }
}
//---------------------------------------------------------------------------//
void LCD_num_point(uint8_t rg, uint32_t value) {
  switch (value) {
  case 0:
    symbol[rg] = 0xFE; //0.
    break;
  case 1:
    symbol[rg] = 0xB0; //1.
    break;
  case 2:
    symbol[rg] = 0xED; //2.
    break;
  case 3:
    symbol[rg] = 0xF9; //3.
    break;
  case 4:
    symbol[rg] = 0xB3; //4.
    break;
  case 5:
    symbol[rg] = 0xDB; //5.
    break;
  case 6:
    symbol[rg] = 0xDF; //6.
    break;
  case 7:
    symbol[rg] = 0xF0; //7.
    break;
  case 8:
    symbol[rg] = 0xFF; //8.
    break;
  case 9:
    symbol[rg] = 0xFB;//9.
    break;
  }
}
//код подсмотрен здесь.)))
//https://www.youtube.com/watch?v=BonEEaQX8vg
//---------------------------------------------------------------------------//
void max7219_Send_float(float value) {
  if (value > 99999.999f || value < -9999.999f) {
    LCD_num(0, 254);
    LCD_num(1, 254);
    LCD_num(2, 254);
    LCD_num(3, 254);
    LCD_num(4, 254);
    LCD_num(5, 254);
    LCD_num(6, 254);
    LCD_num(7, 254);
    LCD_SEND_SYMBOL();
  } else {
    int value_conv = value * 1000;
    value = value * 1000;
    bool positive_value;
    if (value_conv < 0) {
      positive_value = false;
      value_conv = value_conv * (-1);
    } else {
      positive_value = true;
    }
    uint8_t BCD_Arr[8] = {0,0,0,0,0,0,0,0};
    int lenght;
    lenght = value_conv;
    while (value_conv >= 10000000) {
      value_conv -= 10000000;
      BCD_Arr[7]++;
    }

    while (value_conv >= 1000000) {
      value_conv -= 1000000;
      BCD_Arr[6]++;
    }
    while (value_conv >= 100000) {
      value_conv -= 100000;
      BCD_Arr[5]++;
    }
    while (value_conv >= 10000) {
      value_conv -= 10000;
      BCD_Arr[4]++;

    }
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
   // BCD_Arr[0] = (uint8_t) (value_conv);
      BCD_Arr[0] = value_conv;
if (lenght < 10) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      LCD_num(2, 255);
      if (positive_value == true) {
        LCD_num(3, 255);
      } else {
        LCD_num(3, 254);
      }
      LCD_num_point(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();

      
    } else if (lenght < 100) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      LCD_num(2, 255);
      if (positive_value == true) {
        LCD_num(3, 255);
      } else {
        LCD_num(3, 254);
      }
      LCD_num_point(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();

    } else if (lenght < 1000) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      LCD_num(2, 255);
      if (positive_value == true) {
        LCD_num(3, 255);
      } else {
        LCD_num(3, 254);
      }
      LCD_num_point(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();

      
    } else if (lenght < 10000) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      LCD_num(2, 255);
      if (positive_value == true) {
        LCD_num(3, 255);
      } else {
        LCD_num(3, 254);
      }
      LCD_num_point(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();


      
    } else if (lenght < 100000) {
      LCD_num(0, 255);
      LCD_num(1, 255);
      if (positive_value == true) {
        LCD_num(2, 255);
      } else {
        LCD_num(2, 254);
      }
      LCD_num(3, BCD_Arr[4]);
      LCD_num_point(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
      
    } else if (lenght < 1000000) {
      LCD_num(0, 255);
      if (positive_value == true) {
        LCD_num(1, 255);
      } else {
        LCD_num(1, 254);
      }
      LCD_num(2, BCD_Arr[5]);
      LCD_num(3, BCD_Arr[4]);
      LCD_num_point(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();

    } else if (lenght < 10000000) {
      if (positive_value == true) {
        LCD_num(0, 255);
      } else {
        LCD_num(0, 254);
      }
      LCD_num(1, BCD_Arr[6]);
      LCD_num(2, BCD_Arr[5]);
      LCD_num(3, BCD_Arr[4]);
      LCD_num_point(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
    } else if (lenght < 100000000) {
      LCD_num(0, BCD_Arr[7]);
      LCD_num(1, BCD_Arr[6]);
      LCD_num(2, BCD_Arr[5]);
      LCD_num(3, BCD_Arr[4]);
      LCD_num_point(4, BCD_Arr[3]);
      LCD_num(5, BCD_Arr[2]);
      LCD_num(6, BCD_Arr[1]);
      LCD_num(7, BCD_Arr[0]);
      LCD_SEND_SYMBOL();
    }
  }
}
//------------------- END OF FILE---------------------------------------------/
