#include "Nu_ST7920.h"
#define SLAVE 10
#define RST 11
ZW12864 NU_disp(SLAVE, RST);
byte counter_1 = 135;
float f_counter = 12.0001;
int counter_2 = -100;
float f_counter_minus = -12.0001;
char Int_1_buffer[10] = {0,};
char Int_2_buffer[10] = {0,};

void setup() {
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(16);
  SPI.begin();
  NU_disp.Init();
  NU_disp.Graphic_mode(0);
  delay(10);
  NU_disp.Send_symbol_text(0, 1, "Hello_world");
  NU_disp.Send_symbol_text(1, 1, "Arduino_IDE");
  NU_disp.Send_symbol_text(2, 1, "NuMaker_UNO");
  NU_disp.Send_symbol_text(3, 1, "NUC131SD2AE");
  delay(3500);
  NU_disp.Send_command(0x01);
  delay(10);
}

void loop() {
  if (counter_1 == 140) {
    counter_1 = 0;
    NU_disp.Send_command(0x81);
    NU_disp.Send_data(0x20);
    NU_disp.Send_data(0x20);
    NU_disp.Send_command(0x82);
    NU_disp.Send_data(0x20);
    NU_disp.Send_data(0x20);
  }
  if (counter_2 == -115) {
    counter_2 = 0;
    NU_disp.Send_command(0x89);
    NU_disp.Send_data(0x20);
    NU_disp.Send_data(0x20);
    NU_disp.Send_command(0x8A);
    NU_disp.Send_data(0x20);
    NU_disp.Send_data(0x20);
  }
  itoa(counter_1, Int_1_buffer, DEC);
  itoa(counter_2, Int_2_buffer, DEC);
  NU_disp.FloatToChar (f_counter, 4);
  NU_disp.FloatToChar (f_counter_minus, 4);
  NU_disp.Send_symbol_text(0, 1, Int_1_buffer);
  NU_disp.Send_Float_buffer(1, 1);
  NU_disp.Send_symbol_text(2, 1, Int_2_buffer);
  NU_disp.Send_Float_buffer_minus(3, 1);
  counter_1 ++;
  counter_2 --;
  f_counter = f_counter + 0.0001;
  f_counter_minus = f_counter_minus - 0.0001;
  delay(500);
}
