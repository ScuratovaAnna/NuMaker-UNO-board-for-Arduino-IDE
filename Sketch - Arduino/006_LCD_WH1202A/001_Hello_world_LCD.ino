/**---------------------------------------------
*\date  13.02.2022
*\brief 
* дисплей куплен:  https://www.chipdip.ru/product/wh1202a-ygh-ct
* вывод RW дисплея по умолчанию притянут к логическому нулю
* 
*\authors ScuratovaAnna + PivnevNikolay
*/
#define e1 PE5 = 1
#define e0 PE5 = 0
#define rs1 PB11 = 1
#define rs0 PB11 = 0

void lcd_com(unsigned char p);
void lcd_dat(unsigned char p);
void lcd_init_8bit(void);
void lcd_SetPos(uint8_t x, uint8_t y);
void lcd_print( const char* s );

void setup()
 {
 GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 ), GPIO_PMD_OUTPUT);/// \brief инициализация шины PA0-PA7 LCD
 pinMode(PE5, GPIO_PMD_OUTPUT);//PE5-->e
 pinMode(PB11, GPIO_PMD_OUTPUT);//PB11-->rs
 lcd_init_8bit(); // Инициализация дисплея
 lcd_SetPos(0,0);
 lcd_print("Hello NUC131");
 lcd_SetPos(2,1);
 lcd_print("world!");
 }
void loop()
 {

 }
//------------------------------------------------------------------------------
// Функция записи команды в LCD
void lcd_com(unsigned char p)
 {
  rs0; /// \brief RS = 0 будет передана команда
  e1; // EN = 1 (начало записи команды в LCD)
  PA->DOUT = p; // Вывод команды на шину PA0-PA7 LCD
  delay(1); // Длительность сигнала EN
  e0; // EN = 0 (конец записи команды в LCD)
  delay(1); // Пауза для выполнения команды
 }
//------------------------------------------------------------------------------
// Функция вывода одного символа (char)
void lcd_write_Char(unsigned char p)
 {
  rs1;// \brief RS = 1 будут переданы пользовательские данные 
  e1; // EN = 1 (начало записи команды в LCD)
  PA->DOUT = p; // Вывод команды на шину PA0-PA7 LCD
  delay(1); // Длительность сигнала EN
  e0; // EN = 0 (конец записи команды в LCD)
  delay(1); // Пауза для выполнения команды
 }
//------------------------------------------------------------------------------
// Функция инициализации LCD
void lcd_init_8bit(void)
 {
  rs0;
  e0;
  PA->DOUT = 0x00; // Лог. нули на выходе
  lcd_com(0x08); // Полное выключение дисплея
  lcd_com(0x38); // 8 бит 2 строки
  delay(2);
  lcd_com(0x01); // Очистка дисплея
  delay(2);
  lcd_com(0x06); // Сдвиг курсора вправо
  delay(2);
  lcd_com(0x0C); // Курсор невидим
 }
//------------------------------------------------------------------------------
//Функция установки позиции курсора 
void lcd_SetPos(uint8_t x, uint8_t y)
 {       
 switch(y)
  {
  case 0:
   lcd_com(x|0x80);
   delay(1);
  break;
  case 1:
   lcd_com((0x40+x)|0x80);
   delay(1);
  break;
  case 2:
    lcd_com((0x14+x)|0x80);
    delay(1);
  break;
  case 3:
   lcd_com((0x54+x)|0x80);
   delay(1);
  break;
  }
 }
//------------------------------------------------------------------------------
//Вывод строки символов
void lcd_print( const char* s )
 {    
  unsigned int len = strlen( s );
  unsigned int i = 0; 
 for( i = 0 ; i < len ; i++ )
 {
  lcd_write_Char( s[i] );
  }
 }