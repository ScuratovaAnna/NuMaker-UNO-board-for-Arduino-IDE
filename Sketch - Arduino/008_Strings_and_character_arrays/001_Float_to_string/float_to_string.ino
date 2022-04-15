/*
* Функция dtostrf(float_data, width, dec, str) – которая записывает переменную типа float 
* в строку str с количеством символов width и знаков после запятой dec, не хочет работать 
* с  NuMaker Uno.
*/
char buffer[15]= {0,};
float fractional_number_1 = -3.123456;
float fractional_number_2 = 3.654321;
//---------------------------------------------------------------------------//
void setup(){
  Serial.begin(9600);
  for(uint8_t i=0;i<7;i++){
  FloatToChar (fractional_number_1,i);
  Serial.println(buffer);
  delay(100);
  }
  for(uint8_t i=0;i<7;i++){
  FloatToChar (fractional_number_2,i);
  Serial.println(buffer);
  delay(100);
}
}
//---------------------------------------------------------------------------//
void loop() {

}
/******************************************************************************
* Для спецификаторов чисел с плавающей точкой (a, A, e, E, f, F) число знаков,
* которые можно вывести после десятичной точки по умолчанию 6.
******************************************************************************/
const char* FloatToChar (float f,signed char width){
bool positive_value;
if (f < 0) {
  f = f * (-1);
  positive_value = false;
  } else {
  positive_value = true;
  }
if(positive_value){
  switch(width){      
    case 0:
  sprintf(buffer, "%d.%d", (int)f, (int)(f * 0) % 0);
  return buffer;
    break;
    case 1:
  sprintf(buffer, "%d.%.1d", (int)f, (int)(f * 10) % 10);
  return buffer;  
    break;
    case 2:
  sprintf(buffer, "%d.%.2d", (int)f, (int)(f * 100) % 100);
  return buffer;
    break;
    case 3:
  sprintf(buffer, "%d.%.3d", (int)f, (int)(f * 1000) % 1000);
  return buffer;
    break;
    case 4:
  sprintf(buffer, "%d.%.4d", (int)f, (int)(f * 10000) % 10000);
  return buffer;
    break;
    case 5:
  sprintf(buffer, "%d.%.5d", (int)f, (int)(f * 100000) % 100000);
  return buffer;
    break;
    case 6:
  sprintf(buffer, "%d.%.6d", (int)f, (int)(f * 1000000) % 1000000);
  return buffer;
    break;
  }
}
else{
    switch(width){      
    case 0:
  sprintf(buffer, "-%d.%d", (int)f, (int)(f * 0) % 0);
  return buffer;
    break;
    case 1:
  sprintf(buffer, "-%d.%.1d", (int)f, (int)(f * 10) % 10);
  return buffer;  
    break;
    case 2:
  sprintf(buffer, "-%d.%.2d", (int)f, (int)(f * 100) % 100);
  return buffer;
    break;
    case 3:
  sprintf(buffer, "-%d.%.3d", (int)f, (int)(f * 1000) % 1000);
  return buffer;
    break;
    case 4:
  sprintf(buffer, "-%d.%.4d", (int)f, (int)(f * 10000) % 10000);
  return buffer;
    break;
    case 5:
  sprintf(buffer, "-%d.%.5d", (int)f, (int)(f * 100000) % 100000);
  return buffer;
    break;
    case 6:
  sprintf(buffer, "-%d.%.6d", (int)f, (int)(f * 1000000) % 1000000);
  return buffer;
    break;
   }
  }
 }
//------------------- END OF FILE---------------------------------------------/