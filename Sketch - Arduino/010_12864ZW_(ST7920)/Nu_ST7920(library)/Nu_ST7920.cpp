#include <Arduino.h>
#include <SPI.h>
#include "Nu_ST7920.h"

ZW12864::ZW12864(uint8_t slave, uint8_t rst)
{
  Slave_Select_Pin = slave;
  RST_Pin = rst;
  pinMode(Slave_Select_Pin, OUTPUT);
  pinMode(RST_Pin, OUTPUT);
}
/*----------------------The function of sending a command to the display------------------------*/
void ZW12864::Send_command(uint8_t  Data) {
  digitalWrite(Slave_Select_Pin, HIGH);
  uint8_t tx_buffer = 0xF8;
  SPI.transfer (tx_buffer);
  delayMicroseconds(20);
  tx_buffer = Data & 0xF0;
  SPI.transfer (tx_buffer);
  delayMicroseconds(20);
  tx_buffer = (Data << 4);
  SPI.transfer (tx_buffer);
  digitalWrite(Slave_Select_Pin, LOW);
}
/*----------------------The function of sending a command to the display------------------------*/
/*----------------------Function of sending data to the display --------------------------------*/
void ZW12864::Send_data(uint8_t Data) {
  digitalWrite(Slave_Select_Pin, HIGH);
  uint8_t tx_buffer = 0xFA;
  SPI.transfer (tx_buffer);
  delayMicroseconds(20);
  tx_buffer = Data & 0xF0;
  SPI.transfer (tx_buffer);
  delayMicroseconds(20);
  tx_buffer = (Data << 4);
  SPI.transfer (tx_buffer);
  digitalWrite(Slave_Select_Pin, LOW);
}
/*----------------------Function of sending data to the display --------------------------------*/
/*-------------------------Display initialization function--------------------------------------*/
void ZW12864::Init(void) {
  digitalWrite(RST_Pin, LOW);
  delay(10);
  digitalWrite(RST_Pin, HIGH);
  delay(40); //Ждем 40 мс
  uint8_t tx_buffer = 0x30;
  Send_command(tx_buffer);
  delay(1);
  Send_command(tx_buffer);
  delay(1);
  tx_buffer = 0x0C;
  Send_command(tx_buffer);
  delay(1);
  tx_buffer = 0x01;
  Send_command(tx_buffer);
  delay(12);
  tx_buffer = 0x06;
  Send_command(tx_buffer);
  delay(1);
}
/*-------------------------Display initialization function--------------------------------------*/
/*----------------Function for displaying symbolic text on the display--------------------------*/
void ZW12864::Send_symbol_text(uint8_t y, uint8_t x, char *message) {
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
  Send_command(x);
  for (int i = 0; i < strlen(message); i++) {
    Send_data(message[i]);
  }
}
/*----------------Function for displaying symbolic text on the display--------------------------*/
/*----------------Graphic mode enable/disable function------------------------------------------*/
void ZW12864::Graphic_mode(bool enable)   // 1-enable, 0-disable
{
  if (enable) {
    Send_command(0x34); 
    delayMicroseconds(15);
    Send_command(0x36);  
    delayMicroseconds(15);
  }
  else if (!enable) {
    Send_command(0x30);  
    delayMicroseconds(15);
  }
}
/*----------------Graphic mode enable/disable function------------------------------------------*/
const char* ZW12864::FloatToChar (float f, signed char width) {
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
        sprintf(Float_buffer_minus, "-%d.%d", (int)f, (int)(f * 0) % 0);
        return Float_buffer;
        break;
      case 1:
        sprintf(Float_buffer_minus, "-%d.%.1d", (int)f, (int)(f * 10) % 10);
        return Float_buffer;
        break;
      case 2:
        sprintf(Float_buffer_minus, "-%d.%.2d", (int)f, (int)(f * 100) % 100);
        return Float_buffer;
        break;
      case 3:
        sprintf(Float_buffer_minus, "-%d.%.3d", (int)f, (int)(f * 1000) % 1000);
        return Float_buffer;
        break;
      case 4:
        sprintf(Float_buffer_minus, "-%d.%.4d", (int)f, (int)(f * 10000) % 10000);
        return Float_buffer;
        break;
      case 5:
        sprintf(Float_buffer_minus, "-%d.%.5d", (int)f, (int)(f * 100000) % 100000);
        return Float_buffer;
        break;
      case 6:
        sprintf(Float_buffer_minus, "-%d.%.6d", (int)f, (int)(f * 1000000) % 1000000);
        return Float_buffer;
        break;
    }
  }
}
void ZW12864::Send_Float_buffer(uint8_t y, uint8_t x) {
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
  Send_command(x);
  for (int i = 0; i < strlen(Float_buffer); i++) {
    Send_data(Float_buffer[i]);
  }
}

void ZW12864::Send_Float_buffer_minus(uint8_t y, uint8_t x) {
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
  Send_command(x);
  for (int i = 0; i < strlen(Float_buffer_minus); i++) {
    Send_data(Float_buffer_minus[i]);
  }
}
/*-------------------Функция вывода изображения на экран дисплея--------------------------------*/
void ZW12864::Draw_bitmap( uint8_t *bitmap) {
  uint8_t x, y;
  uint16_t i = 0;
  uint8_t Temp, Db;

  for (y = 0; y < 64; y++) {
    for (x = 0; x < 8; x++) {
      if (y < 32) {
        Send_command(0x80 | y);        
        Send_command(0x80 | x); 
      } else {
        Send_command(0x80 | (y - 32));
        Send_command(0x88 | x);
      }

      i = ((y / 8) * 128) + (x * 16);
      Db = y % 8;

      Temp = (((bitmap[i] >> Db) & 0x01) << 7) | (((bitmap[i + 1] >> Db) & 0x01) << 6) | (((bitmap[i + 2] >> Db) & 0x01) << 5)
             | (((bitmap[i + 3] >> Db) & 0x01) << 4) | (((bitmap[i + 4] >> Db) & 0x01) << 3) | (((bitmap[i + 5] >> Db) & 0x01) << 2)
             | (((bitmap[i + 6] >> Db) & 0x01) << 1) | (((bitmap[i + 7] >> Db) & 0x01) << 0);
      Send_data(Temp);

      Temp = (((bitmap[i + 8] >> Db) & 0x01) << 7) | (((bitmap[i + 9] >> Db) & 0x01) << 6) | (((bitmap[i + 10] >> Db) & 0x01) << 5)
             | (((bitmap[i + 11] >> Db) & 0x01) << 4) | (((bitmap[i + 12] >> Db) & 0x01) << 3) | (((bitmap[i + 13] >> Db) & 0x01) << 2)
             | (((bitmap[i + 14] >> Db) & 0x01) << 1) | (((bitmap[i + 15] >> Db) & 0x01) << 0);

      Send_data(Temp);
    }
  }
}
/*-------------------Функция вывода изображения на экран дисплея--------------------------------*/
/*---------------------Функция вывода буфера кадра на дисплей-----------------------------------*/
void ZW12864::Update(void) {
  Draw_bitmap(Frame_buffer);
}
/*---------------------Функция вывода буфера кадра на дисплей-----------------------------------*/
/*------------------------Функция очистки буфера кадра------------------------------------------*/
void ZW12864::Clean_Frame_buffer(void) {
  /// Функция очистки буфера кадра
  memset(Frame_buffer, 0x00, sizeof(Frame_buffer));
}
/*------------------------Функция очистки буфера кадра------------------------------------------*/
/*---------------------Функция рисования пикселя на экране--------------------------------------*/
void ZW12864::Draw_pixel(uint8_t x, uint8_t y) {
  /// Функция рисования точки.
  /// param\ x - координата по X(от 0 до 127)
  /// paran\ y - координата по Y(от 0 до 63)
  if (y < ZW12864_height && x < ZW12864_width) {
    Frame_buffer[(x) + ((y / 8) * 128)] |= 0x01 << y % 8;
  }
}
/*---------------------Функция рисования пикселя на экране--------------------------------------*/
/*--------------------- Clear Pixel-------------------------------------------------------------*/

void ZW12864::Clean_pixel(uint8_t x, uint8_t y) {
  /// Функция удаления точки.
  /// param\ x - координата по X(от 0 до 127)
  /// paran\ y - координата по Y(от 0 до 63)
  if (y < ZW12864_height && x < ZW12864_width) {
    Frame_buffer[(x) + ((y / 8) * 128)] &= 0xFE << y % 8;
  }
}
/*--------------------- Clear Pixel-------------------------------------------------------------*/