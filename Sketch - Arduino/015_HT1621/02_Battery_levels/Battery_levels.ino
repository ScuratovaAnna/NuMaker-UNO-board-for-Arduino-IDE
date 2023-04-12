#include <HT1621.h> 
HT1621 lcd; 
void setup(){
  lcd.begin(13, 12, 8, 10); // (cs, wr, Data, backlight)
  lcd.backlight();
  lcd.clear();
}

void loop(){
  lcd.setBatteryLevel(1);
  delay(500);
  lcd.setBatteryLevel(2);
  delay(500);
  lcd.setBatteryLevel(3);
  delay(500);
}