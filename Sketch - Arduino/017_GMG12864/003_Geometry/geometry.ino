/**-------------------------------------------------------------------
 *\date  10.10.2023
 *
 * Сcылки на источник :
 * https://github.com/Solderingironspb/Lessons-Stm32/tree/Practice%2311
 * https://www.youtube.com/watch?v=ajEqZN5s5xc
 *
 *************************************************************************
 * 
 *************************************************************************
 *
 *   NuMaker  UNO             GMG12864 
 *   ---------------         ------------
 *  |               |       |
 *  |               |       |
 *  |               |       |
 *  |   (PB.11)  8~ | ----> | CS  (CS)
 *  |   (PA.14)  9~ | ----> | RSE (RST(Reset))
 *  |   (PA.13) 10~ | ----> | RS  (DC(Data/Command) 
 *  |               |       |
 *  |   (PC.1)      | ----> | SCL (SCK)    NuMaker-UNO --> SPI0_CLK   Physical Pin 27
 *  |   (PC.3)      | ----> | SI  (MOSI)   NuMaker-UNO --> SPI0_MOSI  Physical Pin 25
 *  |               |       |
 *  |         +3.3V | <---> | +3.3V
 *  |           GND | <---> | GND
 *  |         +3.3V | <---> | +3.3V 
 *  |           GND | <---> | GND
 *
 *\ author         PivnevNikolay , ScuratovaAnna 
 *\ сode debugging ScuratovaAnna
 */
#include <SPI.h>

const byte Smile_bitmap[] PROGMEM = { 0x0, 0xA, 0x0, 0x0, 0x11, 0xE, 0x0, 0x0 };

#define CS_Pin 8
#define cs_set() digitalWrite(CS_Pin, LOW)       //CS_Pin --> GND
#define cs_reset() digitalWrite(CS_Pin, HIGH)    //CS_Pin --> +3.3v
#define RST_Pin 9
#define RST_set() digitalWrite(RST_Pin, LOW)     //RST_Pin --> GND
#define RST_reset() digitalWrite(RST_Pin, HIGH)  //RST_Pin --> +3.3v
#define DC_Pin 10
#define DC_set() digitalWrite(DC_Pin, LOW)       //DC_Pin --> GND
#define DC_reset() digitalWrite(DC_Pin, HIGH)    //DC_Pin --> +3.3v

uint8_t Frame_buffer[1024] = {
  0,
};                             //Frame Buffer
uint8_t GMG12864_width = 128;  //Display width in pixels
uint8_t GMG12864_height = 64;  //Display height in pixels
uint8_t i;

void setup() {
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(16);
  SPI.begin();
  pinMode(CS_Pin, OUTPUT);
  pinMode(RST_Pin, OUTPUT);
  pinMode(DC_Pin, OUTPUT);
  GMG12864_Init();
  delay(100);  //pause
  GMG12864_Clean_Frame_buffer();
}

void loop() {
  /*
  GMG12864_Clean_Frame_buffer();
  GMG12864_Draw_pixel(1, 1);
  GMG12864_Update();
  delay(1000);  //pause

  GMG12864_Clean_pixel(1,1);
  GMG12864_Update();
  delay(1000);  //pause

  GMG12864_Draw_line(1, 1, 51, 51);
  GMG12864_Update();
  delay(1000);  //pause

  GMG12864_Clean_Frame_buffer();
  GMG12864_ICON_Print(1, 1, Smile_bitmap);
  GMG12864_Update();
  delay(1000);  //pause

 GMG12864_Clean_Frame_buffer();
 uint8_t w2 = GMG12864_width;
 uint8_t h2 = GMG12864_height;
  w2 /= 2;
  h2 /= 2;
  for ( uint8_t y = 0; y < h2; y++ ) {
    for ( uint8_t x = 0; x < w2; x++ ) {
      if ( (x + y) & 1 ) {
        GMG12864_Draw_pixel(x, y);
        GMG12864_Draw_pixel(x, y + h2);
        GMG12864_Draw_pixel(x + w2, y);
        GMG12864_Draw_pixel(x + w2, y + h2);
      }
    }
  }
  GMG12864_Update();
  delay(1000);  //pause
*/

  for (uint8_t h = 0; h < GMG12864_width; h++) {
    GMG12864_Draw_line(0 + i, 10, 127 - i, 50);
    GMG12864_Draw_line(0 + i, 10, 0 + i, 50);
    GMG12864_Draw_line(127 - i, 10, 0 + i, 50);
    GMG12864_Draw_line(127 - i, 10, 127 - i, 50);
    i++;
    GMG12864_Inversion(0, 1024);
    GMG12864_Update();
    GMG12864_Clean_Frame_buffer();
    if (i == 127) i = 0;
  }
}

/*-------------------- The function of clearing the frame buffer ---------------------*/
void GMG12864_Clean_Frame_buffer(void) {
  memset(Frame_buffer, 0x00, sizeof(Frame_buffer));
}
/*-------------------- The function of clearing the frame buffer ---------------------*/
/*------------------------- Display initialization function --------------------------*/
void GMG12864_Init(void) {
  cs_set();
  RST_set();
  RST_reset();
  GMG12864_Send_command(0xA2);  // Set the duty cycle ( 1/7 or 1/9 ) depending on the physical LCD display
  //** Set the horizontal and vertical orientation to a known state **//
  GMG12864_Send_command(0xA0);        //ADC selection(SEG0->SEG128)
  GMG12864_Send_command(0xC8);        //SHL selection(COM0->COM64)
  GMG12864_Send_command(0x20 | 0x7);  // the divider of the internal resistor is set to 7 (from 0..7)
  GMG12864_Send_command(0x28 | 0x7);  // power management, all indoor units are on (from 0..7)
  //****** enter dynamic contrast mode ******//
  GMG12864_Send_command(0x81);  //Electronic Volume
  GMG12864_Send_command(15);    // Contrast adjustment. Adjust on your display. Maximum 63.
  GMG12864_Send_command(0x40);
  //****** CMD_DISPLAY_ON  CMD_DISPLAY_OFF ******//
  GMG12864_Send_command(0xAF);  //Display on
  //****** Inverting the screen ******//
  GMG12864_Send_command(0xA6);  //0xA6 - nomal, 0xA7 - revers
  cs_reset();
}
/*------------------------- Display initialization function --------------------------*/
/*------------ The function of sending a command to the display ----------------------*/
void GMG12864_Send_command(uint8_t Command) {
  DC_set();
  SPI.transfer(Command);
  DC_reset();
}
/*------------ The function of sending a command to the display ----------------------*/
/*--------------- The function of sending data to the display ------------------------*/
void GMG12864_Send_data(uint8_t Dat) {
  SPI.transfer(Dat);
}
/*--------------- The function of sending data to the display ------------------------*/
/*------------------------------------------------------------------------------------*/
void ST7565_SetX(uint8_t x) {
  GMG12864_Send_command(((x)&0xF0) >> 4 | 0x10);
  GMG12864_Send_command((x)&0x0F);
}
/*------------------------------------------------------------------------------------*/
void ST7565_SetY(uint8_t y) {
  GMG12864_Send_command(((y)&0x07) | 0xB0);
}
/*------------------------------------------------------------------------------------*/
/*------------- The function of output of the frame buffer to the display ------------*/
void GMG12864_Update(void) {
  cs_set();
  for (uint8_t y = 0; y < 8; y++) {
    ST7565_SetX(0);
    ST7565_SetY((int16_t)y);
    for (uint8_t x = 0; x < 128; x++) {
      GMG12864_Send_data(Frame_buffer[x + 128 * y]);
    }
  }
  cs_reset();
}
/*------------- The function of output of the frame buffer to the display ------------*/
/*-------------------- Inversion function of any place in the buffer -----------------*/
void GMG12864_Inversion(uint16_t x_start, uint16_t x_end) {
  // Parameter --> x_start - the starting point for x is from 0 to 1024
  // Parameter --> x_end - y endpoint from 0 to 1024
  for (; x_start < x_end; x_start++) {
    Frame_buffer[x_start] = ~(Frame_buffer[x_start]);
  }
}
/*-------------------- Inversion function of any place in the buffer -----------------*/
/*-------------------------------- Pixel drawing function ----------------------------*/
void GMG12864_Draw_pixel(int16_t x, int16_t y) {
  if (y < GMG12864_height && x < GMG12864_width) {
    Frame_buffer[(x) + ((y / 8) * 128)] |= 0x01 << y % 8;
  }
}
/*-------------------------------- Pixel drawing function ----------------------------*/
/*------------------------------- Pixel cleaning function ----------------------------*/
void GMG12864_Clean_pixel(uint8_t x, uint8_t y) {
  if (y < GMG12864_height && x < GMG12864_width) {
    Frame_buffer[(x) + ((y / 8) * 128)] &= 0xFE << y % 8;
  }
}
/*------------------------------- Pixel cleaning function ----------------------------*/
/*---------------------------- drawing a line on the screen --------------------------*/
void GMG12864_Draw_line(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end) {
  int dx = (x_end >= x_start) ? x_end - x_start : x_start - x_end;
  int dy = (y_end >= y_start) ? y_end - y_start : y_start - y_end;
  int sx = (x_start < x_end) ? 1 : -1;
  int sy = (y_start < y_end) ? 1 : -1;
  int err = dx - dy;
  for (;;) {
    GMG12864_Draw_pixel(x_start, y_start);
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
/*---------------------------- drawing a line on the screen --------------------------*/
/*----------------------------Print ICON(8*8) x=0-15 y=0-7----------------------------*/
void GMG12864_ICON_Print(uint8_t x, uint8_t y, const uint8_t* ICON) {
  int i;
  for (i = 0; i < 8; i++)
    Frame_buffer[i + (x * 8) + (y * 128)] = ICON[i];
}
/*----------------------------Print ICON(8*8) x=0-15 y=0-7----------------------------*/
