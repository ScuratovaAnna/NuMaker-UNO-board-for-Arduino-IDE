/**---------------------------------------------
*\date  04.03.2022
*\brief 
* https://blog.naver.com/vhf145/221792111544 Огромное спасибо! Thank you very much!
* дисплей куплен:  https://www.chipdip.ru/product/wh1202a-ygh-ct
* Сommand codes for LCD
* -------------------------------------------------------------------|
* Clear display screen | (очистить экран дисплея )  | Hex Code 0x01  |
* -------------------------------------------------------------------|
* Return home          | (вернуться в начало)       | Hex Code 0x02  |
* -------------------------------------------------------------------|
* Shift cursor to left | (сдвинуть курсор влево)    | Hex Code 0x04  |???
* -------------------------------------------------------------------|
* Shift cursor to right| (сдвинуть курсор вправо)   | Hex Code 0x06  |???
* -------------------------------------------------------------------|
* Shift display right  | (сдвиг дисплея вправо)     | Hex Code 0x05  |
* -------------------------------------------------------------------|
* Shift display left   | (сдвиг дисплея влево)      | Hex Code 0x07  |
* -------------------------------------------------------------------|
*           Display off, cursor off                 | Hex Code 0x08  |
* -------------------------------------------------------------------|
*           Display off, cursor on                  | Hex Code 0x0A  |
* -------------------------------------------------------------------|
*           Display on, cursor off                  | Hex Code 0x0C  |
* -------------------------------------------------------------------|
*           Display on, cursor blinking             | Hex Code 0x0E  |
* -------------------------------------------------------------------|
*           Display on, cursor blinking             | Hex Code 0x0F  |
* -------------------------------------------------------------------|
*           Shift cursor position to left           | Hex Code 0x10  | (Сдвинуть курсор влево)
* -------------------------------------------------------------------|
*           Shift cursor position to right          | Hex Code 0x14  | (Сдвинуть курсор вправо)
* -------------------------------------------------------------------|
*           Shift the entire display to the left    | Hex Code 0x18  | (Сдвиг всего экрана влево)
* -------------------------------------------------------------------|
*           Shift the entire display to the right   | Hex Code 0x1C  | (Сдвиг всего экрана вправо)
* -------------------------------------------------------------------|
*           Force cursor to beginning ( 1st line)   | Hex Code 0x80  | (Переместить курсор в начало 1-я строка)
* -------------------------------------------------------------------|
*           Force cursor to beginning ( 2nd line)   | Hex Code 0xC0  | (Переместить курсор в начало 2-я строка)
* -------------------------------------------------------------------|
*           2 lines and 5×7 matrix                  | Hex Code 0x38  | (2 строки и матрица 5×7 )
* -------------------------------------------------------------------|
* 
*\authors ScuratovaAnna + PivnevNikolay
*/
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
  Lcd_SetPos(5,0);
  LCD_string("NUVOTON");
  Lcd_SetPos(0,1);
  LCD_string("MCU  NUC131SD2AE");
  delay(2000); 
  Shift_the_entire_display_to_the_left(5);
  delay(1000);
  Shift_the_entire_display_to_the_right(5);
  delay(1000);
  Turn_display_off();
  Turn_on_the_screen_LCD_cursor_off();
  delay(1000);
  Cleaning_LCD();
  LCD_string_output_with_coordinates("HI_BABY!!!",2,0);
  LCD_string_output_with_coordinates("LCD_OUT",3,1);
}

void loop() {  
}
/******************************************************************************
*                  Функция инициализации LCD                                  *
*******************************************************************************/
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
/******************************************************************************
*                  Функция записи команды в LCD                               *
*******************************************************************************/
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
/******************************************************************************
*                   Функция вывода строки на экран LCD                        *
*******************************************************************************/
void LCD_string(const char *s){
 while(*s){
  LCD_send_byte(*s++);
 }
}
/******************************************************************************
*                   Функция установки позиции курсора                         *
*******************************************************************************/
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
/******************************************************************************
*                   Функция смещения экрана LCD влево на n символов           *
*******************************************************************************/
void Shift_the_entire_display_to_the_left(uint8_t n)
{
  for (uint8_t i=0;i<n;i++)
  {                
    LCD_command_nibble(0x18);
    delay(1000);              
  }
}
/******************************************************************************
*                  Функция смещения экрана LCD вправо на n символов           *
*******************************************************************************/
void Shift_the_entire_display_to_the_right(uint8_t n)
{
  for (uint8_t i=0;i<n;i++)
  {
    LCD_command_nibble(0x1C);
    delay(1000);
  }
}
/******************************************************************************
*                  Функция выключения экрана LCD                              *
*******************************************************************************/
void Turn_display_off(void)
{  
  LCD_command_nibble(0x8);
  delay(1000);  
}
/******************************************************************************
*                  Функция включения экрана LCD, выключение курсора           *
*******************************************************************************/
void Turn_on_the_screen_LCD_cursor_off(void)
{  
  LCD_command_nibble(0xE);
  delay(1000);  
}
/******************************************************************************
*                  Функция очистки экрана LCD                                 *
*******************************************************************************/
void Cleaning_LCD(void){
  LCD_command_nibble(0x01);
  delay(4);
}
/******************************************************************************
*         Функция вывода строка на экран LCD с заданными координатами         *
*******************************************************************************/
//Выводим строку на экран LCD с заданными координатами
void LCD_string_output_with_coordinates(char *p,uint8_t x,uint8_t y) //Вывести строку на дисплей X,Y
///lcd_string("HI BABY",4,1) ; выведется сторка по конкретным координатам
{
 Lcd_SetPos(x, y);
  while( *p )
  {
    LCD_send_byte(*p++);
  }
}
//************************************END**************************************/
