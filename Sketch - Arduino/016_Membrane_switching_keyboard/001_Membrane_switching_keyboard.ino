/* 
 *  Идея и пример взяты отсюда:
 *  https://3d-diy.ru/wiki/arduino-moduli/elastichnaya-matrichnaya-klaviatura-4x4/
 *
 *
 *  date  28.08.2023
 *  Первый пример из статьи "Матричная клавиатура".
 *
 *  authors ScuratovaAnna
 *  сode debugging PivnevNikolay
 */
 
#define ALONE 

const int P[] = { 11, 10, 9, 8 };
const int M[] = { 7, 6, 5, 4 };    
const char k4x4[4][4] = {          
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

void setup() {
  for (int i = 0; i <= 3; i++) { 
    pinMode(P[i], OUTPUT);
    pinMode(M[i], INPUT_PULLUP);
    digitalWrite(P[i], HIGH);
  }
  Serial.begin(9600);
  Serial.println("begin");
}

void loop() {
  char a = GetKey4x4();
  if (a != 0) {
    Serial.print(a);
  }
}

char GetKey4x4() {
  static unsigned long timer;
  static char olda;
  char a = 0;
  if ((timer + 50) > millis()) return 0;
  for (byte p = 0; p <= 3; p++) {
    digitalWrite(P[p], LOW);
    for (byte m = 0; m <= 3; m++) {
      if (!digitalRead(M[m])) {
        a = k4x4[p][m];  
      }
    }
    digitalWrite(P[p], HIGH);
  }
  timer = millis();
#ifdef ALONE  
  if (a == olda) return 0;
  olda = a;
#endif
  return a;
}