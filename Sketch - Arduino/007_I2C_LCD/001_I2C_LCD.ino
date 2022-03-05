/// https://blog.naver.com/vhf145/221792111544 Огромное спасибо! Thank you very much!
#include <Wire.h>
#define LCD_ADDRESS     0x27
#define E0              0xFB 
#define Back_Light_OFF  0x5 // low  4 bit, E = 1, RW = 0, RS = 1
#define Back_Light_ON   0xD // high 4 bit, E = 1, RW = 0, RS = 1

void setup() {
  Wire.begin();
  delay(500);  
  Init_LCD();
  LCD_send_byte('V');
}

void loop() {  /// https://blog.naver.com/vhf145/221792111544 Огромное спасибо! Thank you very much!
#include <Wire.h>
#define LCD_ADDRESS     0x27
#define E0              0xFB 
#define Back_Light_OFF  0x5 // low  4 bit, E = 1, RW = 0, RS = 1
#define Back_Light_ON   0xD // high 4 bit, E = 1, RW = 0, RS = 1

void setup() {
  Wire.begin();
  delay(500);
  Init_LCD();
  //*******************************************//
  Lcd_SetPos(4,0);
  LCD_string("NUVOTON");
  Lcd_SetPos(0,1);
  LCD_string("MCU  NUC131SD2AE");
}

void loop() {  
}

void Init_LCD(void) {
 // 8-bit mode
 LCD_command_8bit(0x30);
 delay(10);
 LCD_command_8bit(0x30);
 delay(6);
 LCD_command_8bit(0x30);
 LCD_command_8bit(0x20); // changes to 4-bit mode    
 // 4-bit mode
 LCD_command_nibble(0x28); // function set(4-bit, 2 line, 5x7 dot)
 LCD_command_nibble(0x0C); // display control(display ON, cursor OFF)
 LCD_command_nibble(0x06); // entry mode set(increment, not shift)
 LCD_command_nibble(0x01); // clear display
 delay(4);
}

void LCD_command_8bit(byte val)
{
  Wire.beginTransmission( LCD_ADDRESS );  
  val = (val & 0xF0) | 0x0C;
  Wire.write(val);
  val = val & E0;// E = 0;
  Wire.write(val);
  Wire.endTransmission();
}

void LCD_command_nibble(byte val)
{
 byte alfa = val;
 Wire.beginTransmission( LCD_ADDRESS );
 val = (val & 0xF0 ) | 0x0C;
 Wire.write(val);
 val = val & E0;// E = 0;
 Wire.write(val);
 val = (alfa << 4) | 0x0C;
 Wire.write(val);
 val = val & E0;// E = 0; 
 Wire.write(val);       
 Wire.endTransmission();
}

void LCD_send_byte(byte val)
{
 byte zero = val;
 Wire.beginTransmission( LCD_ADDRESS );
 val = (val & 0xF0) | Back_Light_ON;  // high 4 bit, E = 1, RW = 0, RS = 1
 Wire.write(val);
 val = val & E0;  // E = 0;
 Wire.write(val);
 val = (zero << 4 ) | Back_Light_ON;  // high  4 bit, E = 1, RW = 0, RS = 1
 Wire.write(val);
 val = val & E0;  // E = 0;
 Wire.write(val);
 Wire.endTransmission();
}
void LCD_string(const char *s){
 while(*s){
  LCD_send_byte(*s++);
 }
}

void Lcd_SetPos(uint8_t x, uint8_t y)
 {       
 switch(y)
  {
  case 0:
   LCD_command_nibble(x|0x80);
  break;
  case 1:
   LCD_command_nibble((0x40+x)|0x80);
  break;
  case 2:
   LCD_command_nibble((0x14+x)|0x80);
  break;
  case 3:
   LCD_command_nibble((0x54+x)|0x80);   
  break;
  }
 }



}

void Init_LCD(void) {
 // 8-bit mode
 LCD_command_8bit(0x30);
 delay(10);
 LCD_command_8bit(0x30);
 delay(6);
 LCD_command_8bit(0x30);
 LCD_command_8bit(0x20); // changes to 4-bit mode    
 // 4-bit mode
 LCD_command_nibble(0x28); // function set(4-bit, 2 line, 5x7 dot)
 LCD_command_nibble(0x0C); // display control(display ON, cursor OFF)
 LCD_command_nibble(0x06); // entry mode set(increment, not shift)
 LCD_command_nibble(0x01); // clear display
 delay(4);
}

void LCD_command_8bit(byte val)
{
  Wire.beginTransmission( LCD_ADDRESS );  
  val = (val & 0xF0) | 0x0C;
  Wire.write(val);
  val = val & E0;// E = 0;
  Wire.write(val);
  Wire.endTransmission();
}

void LCD_command_nibble(byte val)
{
 byte alfa = val;
 Wire.beginTransmission( LCD_ADDRESS );
 val = (val & 0xF0 ) | 0x0C;
 Wire.write(val);
 val = val & E0;// E = 0;
 Wire.write(val);
 val = (alfa << 4) | 0x0C;
 Wire.write(val);
 val = val & E0;// E = 0; 
 Wire.write(val);       
 Wire.endTransmission();
}

void LCD_send_byte(byte val)
{
 byte zero = val;
 Wire.beginTransmission( LCD_ADDRESS );
 val = (val & 0xF0) | Back_Light_ON;  // high 4 bit, E = 1, RW = 0, RS = 1
 Wire.write(val);
 val = val & E0;  // E = 0;
 Wire.write(val);
 val = (zero << 4 ) | Back_Light_ON;  // high  4 bit, E = 1, RW = 0, RS = 1
 Wire.write(val);
 val = val & E0;  // E = 0;
 Wire.write(val);
 Wire.endTransmission();
}