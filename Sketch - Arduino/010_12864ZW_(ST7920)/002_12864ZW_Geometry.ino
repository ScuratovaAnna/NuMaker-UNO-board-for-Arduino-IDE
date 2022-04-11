/**-----------------------------------------------------------------------------------------------
*\date  01.04.2022
*\brief 
*\authors ScuratovaAnna + PivnevNikolay
* подсмотренно
* https://www.youtube.com/watch?v=RuUFxePFrmo
* https://vk.com/solderingiron.stm32
* Ссылка на Telegram канал: https://t.me/nuvoton_programming
*/
#include <SPI.h>
const int Slave_Select_Pin = 10;
const int RST_Pin = 11;
byte Frame_buffer[1024] = { 0, };
uint8_t ZW12864_width = 128; 
uint8_t ZW12864_height = 64;
int16_t f;
byte i = 0;
byte h;
byte x, y, w2, h2;

void setup() {
  pinMode (Slave_Select_Pin, OUTPUT);
  pinMode (RST_Pin, OUTPUT);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(16);
  SPI.begin();
  ZW12864_Init();
  ZW12864_Graphic_mode(1);
  ZW12864_Clean();
}
void loop() {
  w2 = ZW12864_width;
  h2 = ZW12864_height;
  w2 /= 2;
  h2 /= 2;
  for ( y = 0; y < h2; y++ ) {
    for ( x = 0; x < w2; x++ ) {
      if ( (x + y) & 1 ) {
        ZW12864_Draw_pixel(x, y);
        ZW12864_Draw_pixel(x, y + h2);
        ZW12864_Draw_pixel(x + w2, y);
        ZW12864_Draw_pixel(x + w2, y + h2);
      }
    }
  }
  ZW12864_Draw_bitmap(Frame_buffer);
  delay(2000);
  for (h = 0; h < ZW12864_width; h++) {
    ZW1286_Draw_line(0 + i, 10, 127 - i, 50);
    ZW1286_Draw_line(0 + i, 10, 0 + i, 50);
    ZW1286_Draw_line(127 - i, 10, 0 + i, 50);
    ZW1286_Draw_line(127 - i, 10, 127 - i, 50);
    i++;
    ZW12864_Draw_bitmap(Frame_buffer);
    ZW12864_Clean_Frame_buffer();
    if (i == 127)i = 0;
  }
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
/*----------------Function for displaying symbolic text on the display--------------------------*/
void ZW12864_Send_symbol_text(uint8_t y, uint8_t x, char *message) {
  /// Функция вывода символьного текста на дисплей. Дисплей превращается в матрицу 16*4.
  /// y - координата по y(от 0 до 3) x - координата по x(от 0 до 15).
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
/*---------------------Функция рисования пикселя на экране--------------------------------------*/
void ZW12864_Draw_pixel(uint8_t x, uint8_t y) {
  /// Функция рисования точки.
  /// param\ x - координата по X(от 0 до 127)
  /// paran\ y - координата по Y(от 0 до 63)
  if (y < ZW12864_height && x < ZW12864_width) {
    Frame_buffer[(x) + ((y / 8) * 128)] |= 0x01 << y % 8;
  }
}
/*---------------------Функция рисования пикселя на экране--------------------------------------*/
/*--------------------- Clear Pixel-------------------------------------------------------------*/

void ZW12864_Clean_pixel(uint8_t x, uint8_t y) {
  /// Функция удаления точки.
  /// param\ x - координата по X(от 0 до 127)
  /// paran\ y - координата по Y(от 0 до 63)
  if (y < ZW12864_height && x < ZW12864_width) {
    Frame_buffer[(x) + ((y / 8) * 128)] &= 0xFE << y % 8;
  }
}
/*--------------------- Clear Pixel-------------------------------------------------------------*/

void ZW12864_Toggle_pixel(uint8_t x, uint8_t y)
{
  if (y < ZW12864_height && x < ZW12864_width)
  {
    if ((Frame_buffer[(x) + ((y / 8) * 128)] >> y % 8) & 0x01)
      ZW12864_Clean_pixel(x, y);
    else
      ZW12864_Draw_pixel(x, y);
  }
}

/*---------------------Функция рисования линии на экране----------------------------------------*/
void ZW1286_Draw_line(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end) {
  int dx = (x_end >= x_start) ? x_end - x_start : x_start - x_end;
  int dy = (y_end >= y_start) ? y_end - y_start : y_start - y_end;
  int sx = (x_start < x_end) ? 1 : -1;
  int sy = (y_start < y_end) ? 1 : -1;
  int err = dx - dy;

  for (;;) {
    ZW12864_Draw_pixel(x_start, y_start);
    if (x_start == x_end && y_start == y_end)
      break;
    int e2 = err + err;
    if (e2 > -dy) {
      err -= dy;
      x_start += sx;
    }
    if (e2 < dx) {
      err += dx;
      y_start += sy;
    }
  }
}
/*---------------------Функция рисования линии на экране----------------------------------------*/
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
