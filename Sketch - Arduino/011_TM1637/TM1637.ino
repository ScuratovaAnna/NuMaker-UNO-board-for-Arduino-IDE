//https://alexgyver.ru/tm1637_display/
#define CLK 6
#define DIO 7
#include "GyverTM1637.h"
byte welcome_banner[] = {_H, _E, _L, _L, _O, _empty, _empty,
                         _1, _6, _3, _7, _empty, _D, _i, _S, _P, _l, _a, _y, _empty, _empty,
                         _1, _2, _3, _4, _5, _6, _7, _8, _9, _0
                        };
GyverTM1637 disp(CLK, DIO);
uint32_t Now, clocktimer;
boolean flag;
void setup() {
  Serial.begin(9600);
  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)
  disp.clear();
  disp.displayByte(_1, _2, _3, _4);
  delay(2500);
}
void loop() {
  disp.runningString(welcome_banner, sizeof(welcome_banner), 200);  // выводим массив
}