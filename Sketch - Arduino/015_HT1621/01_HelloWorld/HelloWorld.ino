//https://github.com/valerionew/ht1621-7-seg
#include <HT1621.h>
HT1621 lcd;
long  x=1;

void setup(){
  /* start the lcd:
   cs to pin 13
   wr to pin 12
   Data to pin 8
   backlight to pin 10
   you can chose whichever pin you want*/
  lcd.begin(13, 12, 8, 10); // (cs, wr, Data, backlight)
  lcd.backlight(); // turn on the backlight led
  lcd.clear(); // clear the screen
}

void loop(){
  lcd.print(x++);
  delay(50);
  if(x>=999999)x=0;
}