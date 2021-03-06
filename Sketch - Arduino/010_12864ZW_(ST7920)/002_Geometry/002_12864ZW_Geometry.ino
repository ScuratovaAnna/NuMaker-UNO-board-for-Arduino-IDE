/**-----------------------------------------------------------------------------------------------
*\date  01.04.2022
*\brief 
*\authors ScuratovaAnna + PivnevNikolay
* подсмотренно
* https://www.youtube.com/watch?v=RuUFxePFrmo
* https://vk.com/solderingiron.stm32
* Ссылка на Telegram канал: https://t.me/nuvoton_programming
*/
#include <SPI.h>//Подключаем библиотеку
const byte Rook_bitmap[] PROGMEM = {
  0x55,         // 01010101
  0x55,         // 01010101
  0x7f,         // 01111111
  0x3e,         // 00111110
  0x3e,         // 00111110
  0x3e,         // 00111110
  0x3e,         // 00111110
  0x7f          // 01111111
};
const byte Smile_bitmap[] PROGMEM = {0x0, 0xA, 0x0, 0x0, 0x11, 0xE, 0x0, 0x0};
const int Slave_Select_Pin = 10;
const int RST_Pin = 11;
byte Frame_buffer[1024] = { 0, }; //Буфер кадра
uint8_t ZW12864_width = 128; //Ширина дисплея в пикселях
uint8_t ZW12864_height = 64; //Высота дисплея в пикселях
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
  ZW12864_Draw_rectangle(0, 0, 40, 20);
  ZW12864_Draw_rectangle_filled(10, 10, 40, 20);
  ZW12864_Draw_circle(80, 15, 15);
  ZW12864_Draw_circle_filled(100, 20, 12);
  ZW12864_Draw_triangle(20, 40, 30, 62, 0, 62);
  ZW12864_Draw_triangle_filled(40, 40, 50, 63, 10, 63);
  ZW12864_ICON_Print(11, 6, Rook_bitmap);
  ZW12864_ICON_Print(13, 6, Smile_bitmap);
  ZW12864_Draw_bitmap(Frame_buffer);
  delay(3500);
  ZW12864_Clean_Frame_buffer();
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
  delay(2500);
  for (h = 0; h < ZW12864_width; h++) {
    ZW12864_Draw_line(0 + i, 10, 127 - i, 50);
    ZW12864_Draw_line(0 + i, 10, 0 + i, 50);
    ZW12864_Draw_line(127 - i, 10, 0 + i, 50);
    ZW12864_Draw_line(127 - i, 10, 127 - i, 50);
    i++;
    ZW12864_Inversion(0, 1024);
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
void ZW12864_Draw_line(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end) {
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
/*---------------------Функция инверсии любого места в буфере-----------------------------------*/
void ZW12864_Inversion(uint16_t x_start, uint16_t x_end) {
  /// Функция инверсии любого места в буфере
  /// \param x_start - начальная точка по х от 0 до 1024
  /// \param x_end - конечная точка по y от 0 до 1024
  for (; x_start < x_end; x_start++) {
    Frame_buffer[x_start] = ~(Frame_buffer[x_start]);
  }
}
/*---------------------Функция инверсии любого места в буфере-----------------------------------*/
/*--------------------------------Вывести пустотелый прямоугольник------------------------------*/
void ZW12864_Draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  /// Вывести пустотелый прямоугольник
  /// \param x - начальная точка по оси "x"
  /// \param y - начальная точка по оси "y"
  /// \param width - ширина прямоугольника
  /// \param height - высота прямоугольника

  /*Проверка ширины и высоты*/
  if ((x + width) >= ZW12864_width) {
    width = ZW12864_width - x;
  }
  if ((y + height) >= ZW12864_height) {
    height = ZW12864_height - y;
  }

  /*Рисуем линии*/
  ZW12864_Draw_line(x, y, x + width, y); /*Верх прямоугольника*/
  ZW12864_Draw_line(x, y + height, x + width, y + height); /*Низ прямоугольника*/
  ZW12864_Draw_line(x, y, x, y + height); /*Левая сторона прямоугольника*/
  ZW12864_Draw_line(x + width, y, x + width, y + height); /*Правая сторона прямоугольника*/
}
/*--------------------------------Вывести пустотелый прямоугольник------------------------------*/
/*-------------------------------Вывести закрашенный прямоугольник------------------------------*/
void ZW12864_Draw_rectangle_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  /// Вывести закрашенный прямоугольник
  /// \param x - начальная точка по оси "x"
  /// \param y - начальная точка по оси "y"
  /// \param width - ширина прямоугольника
  /// \param height - высота прямоугольника

  /*Проверка ширины и высоты*/
  if ((x + width) >= ZW12864_width) {
    width = ZW12864_width - x;
  }
  if ((y + height) >= ZW12864_height) {
    height = ZW12864_height - y;
  }

  /*Рисуем линии*/
  for (uint8_t i = 0; i <= height; i++) {
    ZW12864_Draw_line(x, y + i, x + width, y + i);
  }
}
/*-------------------------------Вывести закрашенный прямоугольник---------------------------------*/
/*---------------------------------Вывести пустотелую окружность-----------------------------------*/
void ZW12864_Draw_circle(uint8_t x, uint8_t y, uint8_t radius) {
  /// Вывести пустотелую окружность
  /// \param x - точка центра окружности по оси "x"
  /// \param y - точка центра окружности по оси "y"
  /// \param radius - радиус окружности

  int f = 1 - (int) radius;
  int ddF_x = 1;

  int ddF_y = -2 * (int) radius;
  int x_0 = 0;

  ZW12864_Draw_pixel(x, y + radius);
  ZW12864_Draw_pixel(x, y - radius);
  ZW12864_Draw_pixel(x + radius, y);
  ZW12864_Draw_pixel(x - radius, y);

  int y_0 = radius;
  while (x_0 < y_0) {
    if (f >= 0) {
      y_0--;
      ddF_y += 2;
      f += ddF_y;
    }
    x_0++;
    ddF_x += 2;
    f += ddF_x;
    ZW12864_Draw_pixel(x + x_0, y + y_0);
    ZW12864_Draw_pixel(x - x_0, y + y_0);
    ZW12864_Draw_pixel(x + x_0, y - y_0);
    ZW12864_Draw_pixel(x - x_0, y - y_0);
    ZW12864_Draw_pixel(x + y_0, y + x_0);
    ZW12864_Draw_pixel(x - y_0, y + x_0);
    ZW12864_Draw_pixel(x + y_0, y - x_0);
    ZW12864_Draw_pixel(x - y_0, y - x_0);
  }
}
/*---------------------------------Вывести пустотелую окружность-----------------------------------*/
/*--------------------------------Вывести закрашенную окружность-----------------------------------*/
void ZW12864_Draw_circle_filled(int16_t x, int16_t y, int16_t radius) {
  /// Вывести закрашенную окружность
  /// \param x - точка центра окружности по оси "x"
  /// \param y - точка центра окружности по оси "y"
  /// \param radius - радиус окружности

  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x_0 = 0;
  int16_t y_0 = radius;

  ZW12864_Draw_pixel(x, y + radius);
  ZW12864_Draw_pixel(x, y - radius);
  ZW12864_Draw_pixel(x + radius, y);
  ZW12864_Draw_pixel(x - radius, y);
  ZW12864_Draw_line(x - radius, y, x + radius, y);

  while (x_0 < y_0) {
    if (f >= 0) {
      y_0--;
      ddF_y += 2;
      f += ddF_y;
    }
    x_0++;
    ddF_x += 2;
    f += ddF_x;

    ZW12864_Draw_line(x - x_0, y + y_0, x + x_0, y + y_0);
    ZW12864_Draw_line(x + x_0, y - y_0, x - x_0, y - y_0);
    ZW12864_Draw_line(x + y_0, y + x_0, x - y_0, y + x_0);
    ZW12864_Draw_line(x + y_0, y - x_0, x - y_0, y - x_0);
  }
}
/*--------------------------------Вывести закрашенную окружность-----------------------------------*/
/*-----------------------------------Вывести пустотелый треугольник--------------------------------*/
void ZW12864_Draw_triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3) {
  /// Вывести пустотелый треугольник
  /// \param x_1 - первая точка треугольника. Координата по оси "x"
  /// \param y_1 - первая точка треугольника. Координата по оси "y"
  /// \param x_2 - вторая точка треугольника. Координата по оси "x"
  /// \param y_2 - вторая точка треугольника. Координата по оси "y"
  /// \param x_3 - третья точка треугольника. Координата по оси "x"
  /// \param y_3 - третья точка треугольника. Координата по оси "y"

  ZW12864_Draw_line(x1, y1, x2, y2);
  ZW12864_Draw_line(x2, y2, x3, y3);
  ZW12864_Draw_line(x3, y3, x1, y1);
}
/*-----------------------------------Вывести пустотелый треугольник--------------------------------*/
void ZW12864_Draw_triangle_filled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3) {
  /// Вывести закрашенный треугольник
  /// \param x_1 - первая точка треугольника. Координата по оси "x"
  /// \param y_1 - первая точка треугольника. Координата по оси "y"
  /// \param x_2 - вторая точка треугольника. Координата по оси "x"
  /// \param y_2 - вторая точка треугольника. Координата по оси "y"
  /// \param x_3 - третья точка треугольника. Координата по оси "x"
  /// \param y_3 - третья точка треугольника. Координата по оси "y"

#define ABS(x)   ((x) > 0 ? (x) : -(x))
  int16_t deltax = 0;
  int16_t deltay = 0;
  int16_t x = 0;
  int16_t y = 0;
  int16_t xinc1 = 0;
  int16_t xinc2 = 0;
  int16_t yinc1 = 0;
  int16_t yinc2 = 0;
  int16_t den = 0;
  int16_t num = 0;
  int16_t numadd = 0;
  int16_t numpixels = 0;
  int16_t curpixel = 0;

  deltax = ABS(x2 - x1);
  deltay = ABS(y2 - y1);
  x = x1;
  y = y1;

  if (x2 >= x1) {
    xinc1 = 1;
    xinc2 = 1;
  } else {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (y2 >= y1) {
    yinc1 = 1;
    yinc2 = 1;
  } else {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay) {
    xinc1 = 0;
    yinc2 = 0;
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;
  } else {
    xinc2 = 0;
    yinc1 = 0;
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++) {
    ZW12864_Draw_line(x, y, x3, y3);

    num += numadd;
    if (num >= den) {
      num -= den;
      x += xinc1;
      y += yinc1;
    }
    x += xinc2;
    y += yinc2;
  }
}
/*----------------------------------Вывести закрашенный треугольник--------------------------------*/
/*----------------------------------Print ICON(8*8) x=0-15 y=0-7-----------------------------------*/
void ZW12864_ICON_Print(uint8_t x, uint8_t y, const uint8_t * ICON)
{
  int i;
  for (i = 0; i < 8; i++)
    Frame_buffer[i + (x * 8) + (y * 128)] = ICON[i];
}
/*----------------------------------Print ICON(8*8) x=0-15 y=0-7-----------------------------------*/