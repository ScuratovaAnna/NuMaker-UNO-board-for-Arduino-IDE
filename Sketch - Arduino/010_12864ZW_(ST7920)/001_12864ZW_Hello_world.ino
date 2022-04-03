#include <SPI.h>//Подключаем библиотеку
  const int Slave_Select_Pin = 10;
  const int RST_Pin = 11;
void setup() {
  pinMode (Slave_Select_Pin, OUTPUT);
  pinMode (RST_Pin, OUTPUT);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(16);
  SPI.begin();
  ZW12864_Init();
  ZW12864_Graphic_mode(0);
  delay(50);
}

void loop() {  
  ZW12864_Send_symbol_text(0, 1, "Hello_world");
  ZW12864_Send_symbol_text(1, 1, "Arduino_IDE");
  ZW12864_Send_symbol_text(2, 1, "NuMaker_UNO");
  ZW12864_Send_symbol_text(3, 1, "NUC131SD2AE");
  delay(1500);
  ZW12864_Send_symbol_text(0, 1, "NUC131SD2AE");
  ZW12864_Send_symbol_text(1, 1, "NuMaker_UNO");
  ZW12864_Send_symbol_text(2, 1, "Arduino_IDE");
  ZW12864_Send_symbol_text(3, 1, "Hello_world");
  delay(1500);
}
/*----------------------The function of sending a command to the display------------------------*/
void ZW12864_Send_command(uint8_t  Data){
  /// Функция отправки команды на дисплей
  /// \param Data - 8 бит данных. DB0 - DB7.
  digitalWrite(Slave_Select_Pin, HIGH);//set
  uint8_t tx_buffer = 0xF8; //Отправка команды. RW = 0/RS = 0
  SPI.transfer (tx_buffer);
  delay(1);
  tx_buffer = Data & 0xF0;  //Разбиваем 8 бит на 2 части. Передаем 7-4 бит.
  SPI.transfer (tx_buffer);
  delay(1);
  tx_buffer = (Data << 4); //Разбиваем 8 бит на 2 части. Передаем оставшиеся 3-0 бит.
  SPI.transfer (tx_buffer);
  delay(1);
  digitalWrite(Slave_Select_Pin, LOW);//reset
}
/*----------------------The function of sending a command to the display------------------------*/
/*----------------------Function of sending data to the display --------------------------------*/
void ZW12864_Send_data(uint8_t Data) {
  /// Функция отправки данных на дисплей
  /// \param Data - 8 бит данных. DB0 - DB7.
  digitalWrite(Slave_Select_Pin, HIGH);
  uint8_t tx_buffer = 0xFA; //Отправка данных. RW = 0/RS = 1
  SPI.transfer (tx_buffer);
  delay(1);
  tx_buffer = Data & 0xF0; //Разбиваем 8 бит на 2 части. Передаем 7-4 бит.
  SPI.transfer (tx_buffer);
  delay(1);
  tx_buffer = (Data << 4); //Разбиваем 8 бит на 2 части. Передаем оставшиеся 3-0 бит.
  SPI.transfer (tx_buffer);
  delay(1);
  digitalWrite(Slave_Select_Pin, LOW);  
}
/*----------------------Function of sending data to the display --------------------------------*/
/*-------------------------Display initialization function--------------------------------------*/
void ZW12864_Init(void) {
/// Функция инициализации дисплея
  digitalWrite(RST_Pin, LOW); //Дернем ножку RST
  delay(10);
  digitalWrite(RST_Pin, HIGH);
  delay(40); //Ждем 40 мс

  //Далее все согласно Datasheet://
  uint8_t tx_buffer = 0x30; //Function set
  ZW12864_Send_command(tx_buffer);
  delay(1);
  ZW12864_Send_command(tx_buffer);
  delay(1);
  tx_buffer = 0x0C; //D = 1, C = 0, B = 0.
  ZW12864_Send_command(tx_buffer);
  delay(1);
  tx_buffer = 0x01;
  ZW12864_Send_command(tx_buffer); //Display Clean
  delay(12);
  tx_buffer = 0x06;
  ZW12864_Send_command(tx_buffer); //Cursor increment right no shift
  delay(1);
}
/*-------------------------Display initialization function--------------------------------------*/
/*----------------Function for displaying symbolic text on the display--------------------------*/
void ZW12864_Send_symbol_text(uint8_t y, uint8_t x, char *message) {
/// Функция вывода символьного текста на дисплей.
/// Дисплей превращается в матрицу 16*4.
/// \param y - координата по y(от 0 до 3).
/// \param x - координата по x(от 0 до 15).

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
