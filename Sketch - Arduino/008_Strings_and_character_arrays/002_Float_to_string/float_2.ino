//https://www.youtube.com/watch?v=SRfW3AsPytE
//https://vk.com/solderingiron.stm32
byte sign_number;
int integer_number;
uint32_t  fractional_number;
char Float_buffer[20] = {0,};
float Val = 3.00010202;
/*----------------------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(9600);
}
/*----------------------------------------------------------------------------------------------*/
void loop() {
  Float_transform(Val, 4, &sign_number, &integer_number, &fractional_number);//четыре знака после запятой
  sprintf(Float_buffer, "%d.%.04ld", integer_number, fractional_number);
  Serial.println(Float_buffer);
  delay(1000);
}
/*----------------------------------------------------------------------------------------------*/
void Float_transform(float value, uint8_t width, uint8_t *sign_number, int *integer_number, uint32_t *fractional_number) {
  /// Функция разбития числа float на две части: целую и дробную.
  /// \param float value - число float, которое хотим разбить
  /// \param uint8_t width - сколько знаков после запятой хотим выводить
  /// \param uint8_t *sign_number - знак числа(положительное или отрицательное)
  /// \param int *integer_number - глобальная переменная для вещественной части
  /// \param uint32_t *fractional_number - глобальная переменная для дробной части
  *sign_number = 0;
  float rounding = 0.5f;
  uint32_t rounding_2 = 1;
  if (*((uint32_t*) &value) & 0x80000000) {
    *((uint32_t*) &value) &= 0x7FFFFFFF;
    *sign_number = 1;
  }
  for (uint8_t i = 0; i < width; i++) {
    rounding_2 = rounding_2 * 10;
  }
  rounding = rounding / rounding_2;
  *integer_number = (int) ((float) value + rounding);
  *fractional_number = (((float) value + rounding) * rounding_2) - (*integer_number * rounding_2);
}
/*-----------------------------------------------END--------------------------------------------*/
