//https://www.onetransistor.eu/2018/03/code-text-mode-st7920-arduino.html
//http://termin.narod.ru/a/asciicode.htm
#include <SPI.h>
const int Slave_Select_Pin = 10;
const int RST_Pin = 11;
char Int_1_buffer[10] = {0,};
char Int_2_buffer[10] = {0,};
char Float_buffer[10] = {0,};
byte counter_1 = 135;
float f_counter = 12.0001;
int counter_2 = -100;
void setup() {
  //pinMode (Slave_Select_Pin, OUTPUT);
  GPIO_SetMode(PA, BIT13, GPIO_PMD_OUTPUT);
  //pinMode (RST_Pin, OUTPUT);
  GPIO_SetMode(PA, BIT12, GPIO_PMD_OUTPUT);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(16);
  SPI.begin();
  ZW12864_Init();
  ZW12864_Graphic_mode(0);
  delay(10);
  ZW12864_Send_symbol_text(0, 1, "Hello_world");
  ZW12864_Send_symbol_text(1, 1, "Arduino_IDE");
  ZW12864_Send_symbol_text(2, 1, "NuMaker_UNO");
  ZW12864_Send_symbol_text(3, 1, "NUC131SD2AE");
  delay(3500);
  ZW12864_Send_command(0x01);
  delay(10);
}
void loop() {
  if (counter_1 == 140) {
    counter_1 = 0;
    ZW12864_Send_command(0x81);
    ZW12864_Send_data(0x20);
    ZW12864_Send_data(0x20);
    ZW12864_Send_command(0x82);
    ZW12864_Send_data(0x20);
    ZW12864_Send_data(0x20);
  }
  if (counter_2 == -115) {
    counter_2 = 0;
    ZW12864_Send_command(0x89);
    ZW12864_Send_data(0x20);
    ZW12864_Send_data(0x20);
    ZW12864_Send_command(0x8A);
    ZW12864_Send_data(0x20);
    ZW12864_Send_data(0x20);
  }
  itoa(counter_1, Int_1_buffer, DEC);
  itoa(counter_2, Int_2_buffer, DEC);
  FloatToChar (f_counter, 4);
  ZW12864_Send_symbol_text(0, 1, Int_1_buffer);
  ZW12864_Send_symbol_text(1, 1, Float_buffer);
  ZW12864_Send_symbol_text(2, 1, Int_2_buffer);
  counter_1++;
  counter_2--;
  f_counter = f_counter + 0.0001;
  delay(500);
}
/*----------------------The function of sending a command to the display------------------------*/
void ZW12864_Send_command(uint8_t  Data) {
  /// Функция отправки команды на дисплей
  /// \param Data - 8 бит данных. DB0 - DB7.
  digitalWrite(Slave_Select_Pin, HIGH);//set
  uint8_t tx_buffer = 0xF8; //Отправка команды. RW = 0/RS = 0
  SPI.transfer (tx_buffer);
  delayMicroseconds(20);
  tx_buffer = Data & 0xF0;  //Разбиваем 8 бит на 2 части. Передаем 7-4 бит.
  SPI.transfer (tx_buffer);
  delayMicroseconds(20);
  tx_buffer = (Data << 4); //Разбиваем 8 бит на 2 части. Передаем оставшиеся 3-0 бит.
  SPI.transfer (tx_buffer);
  digitalWrite(Slave_Select_Pin, LOW);//reset
}
/*----------------------The function of sending a command to the display------------------------*/
/*----------------------Function of sending data to the display --------------------------------*/
void ZW12864_Send_data(uint8_t Data) {
  /// Функция отправки данных на дисплей
  /// \param Data - 8 бит данных. DB0 - DB7.
  digitalWrite(Slave_Select_Pin, HIGH);
  uint8_t tx_buffer = 0xFA; //Отправка данных. RW = 0/RS = 1
  SPI.transfer (tx_buffer);
  delayMicroseconds(20);
  tx_buffer = Data & 0xF0; //Разбиваем 8 бит на 2 части. Передаем 7-4 бит.
  SPI.transfer (tx_buffer);
  delayMicroseconds(20);
  tx_buffer = (Data << 4); //Разбиваем 8 бит на 2 части. Передаем оставшиеся 3-0 бит.
  SPI.transfer (tx_buffer);
  digitalWrite(Slave_Select_Pin, LOW);
}
/*----------------------Function of sending data to the display --------------------------------*/
/*-------------------------Display initialization function--------------------------------------*/
void ZW12864_Init(void) {
  /// Функция инициализации дисплея
  digitalWrite(RST_Pin, LOW);
  delay(10);
  digitalWrite(RST_Pin, HIGH);
  delay(40); //Ждем 40 мс
  uint8_t tx_buffer = 0x30;
  ZW12864_Send_command(tx_buffer);
  delay(1);
  ZW12864_Send_command(tx_buffer);
  delay(1);
  tx_buffer = 0x0C;
  ZW12864_Send_command(tx_buffer);
  delay(1);
  tx_buffer = 0x01;
  ZW12864_Send_command(tx_buffer);
  delay(12);
  tx_buffer = 0x06;
  ZW12864_Send_command(tx_buffer);
  delay(1);
}
/*-------------------------Display initialization function--------------------------------------*/
/*----------------Function for displaying symbolic text on the display--------------------------*/
void ZW12864_Send_symbol_text(uint8_t y, uint8_t x, char *message) {
  /// Функция вывода символьного текста на дисплей.
  /// Дисплей превращается в матрицу 16*4.
  /// \param y - координата по y(от 0 до 3).
  /// \param x - координата по x(от 0 до 15).

  switch (y) {
    case 0:
      x |= 0x80;
      break;
    case 1:
      x |= 0x90;
      break;
    case 2:
      x |= 0x88;
      break;
    case 3:
      x |= 0x98;
      break;
    default:
      x |= 0x80;
      break;
  }
  ZW12864_Send_command(x);

  for (int i = 0; i < strlen(message); i++) {
    ZW12864_Send_data(message[i]);
  }
}
/*----------------Function for displaying symbolic text on the display--------------------------*/
/*----------------Graphic mode enable/disable function------------------------------------------*/
void ZW12864_Graphic_mode(bool enable)   // 1-enable, 0-disable
{
  if (enable) {
    ZW12864_Send_command(0x34);  // Т.к. работаем в 8мибитном режиме, то выбираем 0x30 + RE = 1. Переходим в extended instruction.
    delayMicroseconds(15);
    ZW12864_Send_command(0x36);  // Включаем графический режим
    delayMicroseconds(15);
  }
  else if (!enable) {
    ZW12864_Send_command(0x30);  // Т.к. работаем в 8мибитном режиме, то выбираем 0x30 + RE = 0. Переходим в basic instruction.
    delayMicroseconds(15);
  }
}
/*----------------Graphic mode enable/disable function------------------------------------------*/
const char* FloatToChar (float f, signed char width) {
  bool positive_value;
  if (f < 0) {
    f = f * (-1);
    positive_value = false;
  } else {
    positive_value = true;
  }
  if (positive_value) {
    switch (width) {
      case 0:
        sprintf(Float_buffer, "%d.%d", (int)f, (int)(f * 0) % 0);
        return Float_buffer;
        break;
      case 1:
        sprintf(Float_buffer, "%d.%.1d", (int)f, (int)(f * 10) % 10);
        return Float_buffer;
        break;
      case 2:
        sprintf(Float_buffer, "%d.%.2d", (uint32_t)f, (uint32_t)(f * 100) % 100);
        return Float_buffer;
        break;
      case 3:
        sprintf(Float_buffer, "%d.%.3d", (int)f, (int)(f * 1000) % 1000);
        return Float_buffer;
        break;
      case 4:
        sprintf(Float_buffer, "%d.%.4d", (int)f, (int)(f * 10000) % 10000);
        return Float_buffer;
        break;
      case 5:
        sprintf(Float_buffer, "%d.%.5d", (int)f, (int)(f * 100000) % 100000);
        return Float_buffer;
        break;
      case 6:
        sprintf(Float_buffer, "%d.%.6d", (int)f, (int)(f * 1000000) % 1000000);
        return Float_buffer;
        break;
    }
  }
  else {
    switch (width) {
      case 0:
        sprintf(Float_buffer, "-%d.%d", (int)f, (int)(f * 0) % 0);
        return Float_buffer;
        break;
      case 1:
        sprintf(Float_buffer, "-%d.%1d", (int)f, (int)(f * 10) % 10);
        return Float_buffer;
        break;
      case 2:
        sprintf(Float_buffer, "-%d.%2d", (int)f, (int)(f * 100) % 100);
        return Float_buffer;
        break;
      case 3:
        sprintf(Float_buffer, "-%d.%3d", (int)f, (int)(f * 1000) % 1000);
        return Float_buffer;
        break;
      case 4:
        sprintf(Float_buffer, "-%d.%4d", (int)f, (int)(f * 10000) % 10000);
        return Float_buffer;
        break;
      case 5:
        sprintf(Float_buffer, "-%d.%5d", (int)f, (int)(f * 100000) % 100000);
        return Float_buffer;
        break;
      case 6:
        sprintf(Float_buffer, "-%d.%6d", (int)f, (int)(f * 1000000) % 1000000);
        return Float_buffer;
        break;
    }
  }
}