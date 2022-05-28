/**-----------------------------------------------------------------------------------------------
  \date  30.04.2022
  \brief
  \authors ScuratovaAnna + PivnevNikolay
  подсмотренно
  https://www.youtube.com/watch?v=RuUFxePFrmo
  https://vk.com/solderingiron.stm32
  Ссылка на Telegram канал: https://t.me/nuvoton_programming
  https://duino.ru/oled-sh1106.html/image-128x64/
*/
#include <SPI.h>
#include "Font.h"
#include "picture.h"
const int Slave_Select_Pin = 10;
const int RST_Pin = 11;
byte Frame_buffer[1024] = { 0, }; //Буфер кадра
uint8_t ZW12864_width = 128; //Ширина дисплея в пикселях
uint8_t ZW12864_height = 64; //Высота дисплея в пикселях
char tx_buffer[128] = { 0, }; //Буфер для отправки текста на дисплей

void setup() {
  pinMode (Slave_Select_Pin, OUTPUT);
  pinMode (RST_Pin, OUTPUT);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(16);
  SPI.begin();
  ZW12864_Init();
  ZW12864_Graphic_mode(1);
  ZW12864_Clean();
  sprintf(tx_buffer, "Привет Мир!");// Вывод строки с инверсией
  ZW12864_Decode_UTF8(1, 0, 1, tx_buffer);
  sprintf(tx_buffer, "Arduino_IDE");
  ZW12864_Decode_UTF8(1, 2, 0, tx_buffer);
  sprintf(tx_buffer, "NuMaker_UNO");
  ZW12864_Decode_UTF8(1, 4, 0, tx_buffer);
  sprintf(tx_buffer, "NUC131SD2AE");
  ZW12864_Decode_UTF8(1, 6, 0, tx_buffer);
  ZW12864_Update();
  delay(1000);
}

void loop() {
  ZW12864_Clean();
  for (int i = 0; i < 1024; i++) {
    Frame_buffer[i] = picture_1[i];
  }
  ZW12864_Update();
  delay(1500);
  ZW12864_Clean();
  for (int i = 0; i < 1024; i++) {
    Frame_buffer[i] = picture_2[i];
  }
  ZW12864_Update();
  delay(1500);
  ZW12864_Clean();
  for (int i = 0; i < 1024; i++) {
    Frame_buffer[i] = picture_3[i];
  }
  ZW12864_Update();
  delay(1500);
    ZW12864_Clean();
  for (int i = 0; i < 1024; i++) {
    Frame_buffer[i] = picture_4[i];
  }
  ZW12864_Update();
  delay(1500);
}
/*----------------------The function of sending a command to the display------------------------*/
void ZW12864_Send_command(uint8_t  Data) {
  digitalWrite(Slave_Select_Pin, HIGH);//set
  uint8_t tx_buffer = 0xF8;
  SPI.transfer (tx_buffer);
  delayMicroseconds(15);
  tx_buffer = Data & 0xF0;
  SPI.transfer (tx_buffer);
  tx_buffer = (Data << 4);
  SPI.transfer (tx_buffer);
  digitalWrite(Slave_Select_Pin, LOW);
}
/*----------------------The function of sending a command to the display------------------------*/
/*----------------------Function of sending data to the display --------------------------------*/
void ZW12864_Send_data(uint8_t Data) {
  digitalWrite(Slave_Select_Pin, HIGH);
  uint8_t tx_buffer = 0xFA;
  SPI.transfer (tx_buffer);
  delayMicroseconds(15);
  tx_buffer = Data & 0xF0;
  SPI.transfer (tx_buffer);
  tx_buffer = (Data << 4);
  SPI.transfer (tx_buffer);
  digitalWrite(Slave_Select_Pin, LOW);
}
/*----------------------Function of sending data to the display --------------------------------*/
/*-------------------------Display initialization function--------------------------------------*/
void ZW12864_Init(void) {
  digitalWrite(RST_Pin, LOW);
  delay(10);
  digitalWrite(RST_Pin, HIGH);
  delay(40);
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
/*----------------Graphic mode enable/disable function------------------------------------------*/
void ZW12864_Graphic_mode(bool enable)   // 1-enable, 0-disable
{
  if (enable) {
    ZW12864_Send_command(0x34);  // Т.к. работаем в 8мибитном режиме, то выбираем 0x30 + RE = 1. Переходим в extended instruction.
    delay(1);
    ZW12864_Send_command(0x36);  // Включаем графический режим
    delay(1);
  }
  else if (!enable) {
    ZW12864_Send_command(0x30);  // Т.к. работаем в 8мибитном режиме, то выбираем 0x30 + RE = 0. Переходим в basic instruction.
    delay(1);
  }
}
/*----------------Graphic mode enable/disable function------------------------------------------*/
/*-------------------Функция вывода изображения на экран дисплея--------------------------------*/
void ZW12864_Draw_bitmap( uint8_t *bitmap) {
  //void ZW12864_Draw_bitmap(const unsigned char *bitmap) {
  /// Функция вывода изображения на дисплей
  /// Работает с памятью ST7920.
  /// \param *bitmap - изображение 128*64. т.е. Буфер из 1024 элементов.
  uint8_t x, y;
  uint16_t i = 0;
  uint8_t Temp, Db;

  for (y = 0; y < 64; y++) {
    for (x = 0; x < 8; x++) {
      if (y < 32) {
        ZW12864_Send_command(0x80 | y);        //y(0-31)
        ZW12864_Send_command(0x80 | x);        //x(0-8)
      } else {
        ZW12864_Send_command(0x80 | (y - 32));   //y(0-31)
        ZW12864_Send_command(0x88 | x);        //x(0-8)
      }

      i = ((y / 8) * 128) + (x * 16);
      Db = y % 8;

      Temp = (((bitmap[i] >> Db) & 0x01) << 7) | (((bitmap[i + 1] >> Db) & 0x01) << 6) | (((bitmap[i + 2] >> Db) & 0x01) << 5)
             | (((bitmap[i + 3] >> Db) & 0x01) << 4) | (((bitmap[i + 4] >> Db) & 0x01) << 3) | (((bitmap[i + 5] >> Db) & 0x01) << 2)
             | (((bitmap[i + 6] >> Db) & 0x01) << 1) | (((bitmap[i + 7] >> Db) & 0x01) << 0);
      ZW12864_Send_data(Temp);

      Temp = (((bitmap[i + 8] >> Db) & 0x01) << 7) | (((bitmap[i + 9] >> Db) & 0x01) << 6) | (((bitmap[i + 10] >> Db) & 0x01) << 5)
             | (((bitmap[i + 11] >> Db) & 0x01) << 4) | (((bitmap[i + 12] >> Db) & 0x01) << 3) | (((bitmap[i + 13] >> Db) & 0x01) << 2)
             | (((bitmap[i + 14] >> Db) & 0x01) << 1) | (((bitmap[i + 15] >> Db) & 0x01) << 0);

      ZW12864_Send_data(Temp);
    }
  }
}
/*-------------------Функция вывода изображения на экран дисплея--------------------------------*/
/*---------------------Функция вывода буфера кадра на дисплей-----------------------------------*/
void ZW12864_Update(void) {
  /// Функция вывода буфера кадра на дисплей
  /// Подготовьте буфер кадра, перед тем, как обновить изображение
  ZW12864_Draw_bitmap(Frame_buffer);
}
/*---------------------Функция вывода буфера кадра на дисплей-----------------------------------*/
/*------------------------Функция очистки буфера кадра------------------------------------------*/
void ZW12864_Clean_Frame_buffer(void) {
  /// Функция очистки буфера кадра
  memset(Frame_buffer, 0x00, sizeof(Frame_buffer));
}
/*------------------------Функция очистки буфера кадра------------------------------------------*/
/*---------------------Функция очистки дисплея в графическом режиме-----------------------------*/
void ZW12864_Clean() {
  uint8_t x, y;
  for (y = 0; y < 64; y++) {
    if (y < 32) {
      ZW12864_Send_command(0x80 | y);
      ZW12864_Send_command(0x80);
    } else {
      ZW12864_Send_command(0x80 | (y - 32));
      ZW12864_Send_command(0x88);
    }
    for (x = 0; x < 8; x++) {
      ZW12864_Send_data(0x00);
      ZW12864_Send_data(0x00);
    }
  }
  ZW12864_Clean_Frame_buffer();
}
/*---------------------Функция очистки дисплея в графическом режиме-----------------------------*/
/*---------------------Функция декодирования UTF-8 в набор символов-----------------------------*/

void ZW12864_Decode_UTF8(uint16_t x, uint8_t y, bool inversion, char *tx_buffer) {
  /// Функция декодирования UTF-8 в набор символов и последующее занесение в буфер кадра
  /// \param x - координата по х. От 0 до 127
  /// \param y - координата по y. от 0 до 7
  /// Дисплей поделен по страницам(строка из 8 пикселей)
  ///  1 строка: x = 0;
  ///  2 строка: x = 128;
  ///  3 строка: x = 256;
  ///  4 строка: x = 384;
  ///  5 строка: x = 512;
  ///  6 строка: x = 640;
  ///  7 строка: x = 786;
  ///  8 строка: x = 896;
  x = x + y * 128;
  uint16_t symbol = 0;
  bool flag_block = 0;
  for (int i = 0; i < strlen(tx_buffer); i++) {
    if (tx_buffer[i] < 0xC0) { //Английский текст и символы. Если до русского текста, то [i] <0xD0. Но в font добавлен знак "°"
      if (flag_block) {
        flag_block = 0;
      } else {
        symbol = tx_buffer[i];
        if (inversion) {
          print_symbol(x, symbol - 32, 1); //Таблица UTF-8. Basic Latin. С "пробел" до "z". Инверсия вкл.
        } else {
          print_symbol(x, symbol - 32, 0); //Таблица UTF-8. Basic Latin. С "пробел" до "z". Инверсия выкл.
        }
        x = x + 6;
      }
    }

    else { //Русский текст
      symbol = tx_buffer[i] << 8 | tx_buffer[i + 1];
      if (symbol < 0xD180 && symbol > 0xD081) {
        if (inversion) {
          print_symbol(x, symbol - 53297, 1); //Таблица UTF-8. Кириллица. С буквы "А" до "п". Инверсия вкл.
        } else {
          print_symbol(x, symbol - 53297, 0); //Таблица UTF-8. Кириллица. С буквы "А" до "п". Инверсия выкл.
        }
        x = x + 6;
      } else if (symbol == 0xD081) {
        if (inversion) {
          print_symbol(x, 159, 1); ////Таблица UTF-8. Кириллица. Буква "Ё". Инверсия вкл.
        } else {
          print_symbol(x, 159, 0); ////Таблица UTF-8. Кириллица. Буква "Ё". Инверсия выкл.
        }
        x = x + 6;
      } else if (symbol == 0xD191) {
        if (inversion) {
          print_symbol(x, 160, 1); ////Таблица UTF-8. Кириллица. Буква "ё". Инверсия вкл.
        } else {
          print_symbol(x, 160, 0); ////Таблица UTF-8. Кириллица. Буква "ё". Инверсия выкл.
        }
        x = x + 6;
      } else if (symbol == 0xC2B0) {
        if (inversion) {
          print_symbol(x, 161, 1); ////Таблица UTF-8. Basic Latin. Символ "°". Инверсия вкл.
        } else {
          print_symbol(x, 161, 0); ////Таблица UTF-8. Basic Latin. Символ "°". Инверсия выкл.
        }
        x = x + 6;
      }

      else {
        if (inversion) {
          print_symbol(x, symbol - 53489, 1); //Таблица UTF-8. Кириллица. С буквы "р", начинается сдвиг. Инверсия вкл.
        } else {
          print_symbol(x, symbol - 53489, 0); //Таблица UTF-8. Кириллица. С буквы "р", начинается сдвиг. Инверсия выкл.
        }
        x = x + 6;
      }
      flag_block = 1;
    }
  }
}
/*---------------------Функция декодирования UTF-8 в набор символов-----------------------------*/
/*---------------------Функция вывода символа на дисплей----------------------------------------*/
void print_symbol(uint16_t x, uint16_t symbol, bool inversion) {
  /// Функция вывода символа на дисплей
  /// \param x положение по х (от 0 до 1023)
  /// Дисплей поделен по страницам(строка из 8 пикселей)
  ///  1 строка: x = 0;
  ///  2 строка: x = 128;
  ///  3 строка: x = 256;
  ///  4 строка: x = 384;
  ///  5 строка: x = 512;
  ///  6 строка: x = 640;
  ///  7 строка: x = 786;
  ///  8 строка: x = 896;
  ///  \param symbol - код символа
  ///  \param inversion - инверсия. 1 - вкл, 0 - выкл.
  for (int i = 0; i <= 6; i++) {
    if (inversion) {
      Frame_buffer[i + x - 1] = ~Font[(symbol * 6) + i];
    } else {
      Frame_buffer[i + x - 1] = Font[(symbol * 6) + i];
    }
  }
}
/*---------------------Функция вывода символа на дисплей----------------------------------------*/
