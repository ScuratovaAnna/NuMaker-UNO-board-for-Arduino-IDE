// пример использования AT24Cxx для любых типов данных включая массивы и структуры
// проверено на AT24C256
//https://www.instructables.com/How-to-Store-Float-and-Negative-Values-on-External/

#include <Wire.h>

//AT24C256
#define AT24_page 64
#define AT24_max_Address 32768

#define disk 0x50  //адрес чипа AT24C

struct vvv {
  byte a;
  int b;
  long c;
};
struct vvv abc;
struct vvv cab;

union Data{
  float Temp;
  //int   number;
  unsigned char x[4];
}MyUni;

byte data_buff[AT24_page];

void setup(void) {
  Wire.begin();
  Serial.begin(115200);
  GPIO_SetMode(PE, BIT5, GPIO_PMD_OUTPUT);
  AT24C_erase();  //форматируем память микросхемы EEPROM
  //форматирование занимает достаточно долго времени
  PE5 = 0;  // Снимаем блокировку записи  WP = 0
  // производим запись данных
  byte au = 128;
  AT24C_write(0, &au, sizeof(au));  // адрес ячейки 0
  unsigned long cl = 12345678;
  AT24C_write(65, &cl, sizeof(cl));  // адрес ячейки 65
  int ei = 512;
  AT24C_write(129, &ei, sizeof(cl));  // адрес ячейки 129
  abc.a = 8;
  abc.b = 64;
  abc.c = 128;
  AT24C_write(200, &abc, sizeof(abc));  // адрес ячейки 200
  PE5 = 1;  // Устанавливаем блокировку записи  WP = 1*/

  // читаем данные
  byte bu;
  AT24C_read(0, &bu, sizeof(bu));  //читаем данные из ячейки адресс 0
  Serial.println(bu);
  unsigned long dl;
  AT24C_read(65, &dl, sizeof(dl));  //читаем данные из ячейки адресс 65
  Serial.println(dl);
  int fl;
  AT24C_read(129, &fl, sizeof(fl));  //читаем данные из ячейки адресс 129
  Serial.println(fl);
  AT24C_read(200, &cab, sizeof(cab));  //читаем данные из ячейки с начальным адрессом 200
  Serial.println(cab.a);
  Serial.println(cab.b);
  Serial.println(cab.c);
  //проверка форматироваеия микросхемы EEPROM значение 0xFF(255)
  byte ze;
  AT24C_read(30, &ze, sizeof(ze));  //читаем данные из ячейки адресс 30
  Serial.println(ze);
  //проверка форматироваеия микросхемы EEPROM значение 0xFF(255)
  byte zi;
  AT24C_read(94, &zi, sizeof(zi));  //читаем данные из ячейки адресс 94
  Serial.println(zi);
  //проверка форматироваеия микросхемы EEPROM значение 0xFF(255)
  byte zx;
  AT24C_read(158, &zx, sizeof(zx));  //читаем данные из ячейки адресс 158
  Serial.println(zx);
  //проверка форматироваеия микросхемы EEPROM значение 0xFF(255)
  byte zy;
  AT24C_read(2974, &zy, sizeof(zy));  //читаем данные из ячейки адресс 2974
  Serial.println(zy);

  PE5 = 0; // Снимаем блокировку записи  WP = 0
  // записываем данные
  char buffer_in[] = "Nuvoton, NUC131SD2AE";        //
  AT24C_write(257, &buffer_in, sizeof(buffer_in));  // записываем по адресу ячейки 257
  PE5 = 1; // Устанавливаем блокировку записи  WP = 1*/
  // читаем данные
  char buffer_out[21];
  AT24C_read(257, &buffer_out, sizeof(buffer_out));  // считываем данные начиная с ячейки 257
  Serial.print(buffer_out);

  //записываем в память микросхемы EEPROM дробное значение
  MyUni.Temp = -33.58;

  PE5 = 0;  // Снимаем блокировку записи  WP = 0
  // производим запись данных
  AT24C_write(321 , &MyUni.x[0],sizeof(MyUni.x[0]));
  AT24C_write(322 , &MyUni.x[1],sizeof(MyUni.x[1]));
  AT24C_write(323 , &MyUni.x[2],sizeof(MyUni.x[2]));
  AT24C_write(324 , &MyUni.x[3],sizeof(MyUni.x[3]));
  PE5 = 1; // Устанавливаем блокировку записи  WP = 1*/

  // читаем данные
  MyUni.x[0];
  AT24C_read(321, &MyUni.x[0],sizeof(MyUni.x[0]));
  MyUni.x[1];
  AT24C_read(322, &MyUni.x[1],sizeof(MyUni.x[1]));
  MyUni.x[2];
  AT24C_read(323, &MyUni.x[2],sizeof(MyUni.x[2]));
  MyUni.x[3];
  AT24C_read(324, &MyUni.x[3],sizeof(MyUni.x[3]));

  Serial.println();
  Serial.print("Float Number:  ");
  Serial.print(MyUni.Temp);
}

void loop() {}

// --------------------- функции чтение-запись ----------------------

void AT24C_write(unsigned int startAddress, void *data, unsigned int len) {  // адрес, указатель на первый байт, длина в байтах
  Wire.beginTransmission(disk);
  Wire.write((byte *)&startAddress, 2);
  Wire.write((byte *)data, len);
  Wire.endTransmission(true);
  delay(1);
}

int AT24C_read(unsigned int startAddress, void *data, unsigned int len) {  // возвращаем кол-во считанных байт
  byte rdata;
  byte *p;
  Wire.beginTransmission(disk);
  Wire.write((byte *)&startAddress, 2);
  Wire.endTransmission();
  //Wire.beginTransmission(disk);
  Wire.requestFrom(disk, len);
  for (rdata = 0, p = (byte *)data; Wire.available() && rdata < len; rdata++, p++) {
    *p = Wire.read();
  }
  return (rdata);
}
// --------------------- функции чтение-запись ----------------------
// ----- Функция форматирования памяти микросхемы EEPROM ------------
void AT24C_erase(void) {
  //Функция форматирования памяти микросхемы EEPROM
  PE5 = 0;  // Снимаем блокировку записи  WP = 0
  for (uint16_t i = 0; i < AT24_max_Address; i += 63) {
    memset(data_buff, 0XFF, AT24_page);
    AT24C_write(i, &data_buff, AT24_page);
  }
  PE5 = 1;  // Устанавливаем блокировку записи  WP = 1
}