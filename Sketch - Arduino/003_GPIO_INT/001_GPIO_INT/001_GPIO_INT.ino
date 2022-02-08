/*
NuMaker UNO board for Arduino IDE.
Изменили Pins_arduino.cpp (NUC131)
добавили в BoardToPin BoardToPinInfo[]; { 22, GPIO_TYPE, 0}, //22: PD7
*/
int ledPin = 13;
volatile boolean flashFast = false;
void setup()
{
//GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
//GPIO_SetMode(PE, BIT5, GPIO_PMD_INPUT);
pinMode(22, INPUT);//PD7-->P22 Button_Pin
pinMode(13, OUTPUT);//Led_Pin
attachInterrupt(22, stuffHapenned, FALLING);
}
void loop()
{
int period = 100;
if (flashFast) period = 1000;
digitalWrite(ledPin, HIGH);
delay(period);
digitalWrite(ledPin, LOW);
delay(period);
}
//interrupt handling 
void stuffHapenned()
{
flashFast = ! flashFast;
}
