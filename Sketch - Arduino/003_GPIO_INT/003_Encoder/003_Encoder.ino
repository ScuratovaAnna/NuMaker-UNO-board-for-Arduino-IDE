// http://codius.ru/articles/Инкрементальный_энкодер_принцип_действия_схемы_подключения_работа_с_Arduino

volatile int count = 0;       // Счетчик оборотов
int actualcount    = 0;       // Временная переменная определяющая изменение основного счетчика
volatile int state = 0;       // Переменная хранящая статус вращения
volatile int pinAValue = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int pinBValue = 0;   // Переменные хранящие состояние пина, для экономии времени

void setup()
{
 GPIO_SetMode(PA, BIT15, GPIO_PMD_INPUT);// pinMode(6, INPUT);
 GPIO_SetMode(PE, BIT5, GPIO_PMD_INPUT); // pinMode(7, INPUT);
 GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_16);
 GPIO_ENABLE_DEBOUNCE(PA, BIT15);
 attachInterrupt(6, A, CHANGE);  // Настраиваем обработчик прерываний по изменению сигнала - в этом примере отслеживаем только 1 пин
 Serial.begin(9600);             // Включаем Serial
}

void loop()
{
 if (actualcount != count) {     // Чтобы не загружать ненужным выводом в Serial, выводим состояние
  actualcount = count;           // счетчика только в момент изменения
  Serial.println(actualcount);
  }
}

void A()
{
 pinAValue = PA15;            // Получаем состояние пинов A и B
 pinBValue = PE5;
  
 if (!pinAValue &&  pinBValue) state = 1;  // Если при спаде линии А на линии B лог. единица, то вращение в одну сторону
 if (!pinAValue && !pinBValue) state = -1; // Если при спаде линии А на линии B лог. ноль, то вращение в другую сторону
 if (pinAValue && state != 0) {
  if (state == 1 && !pinBValue || state == -1 && pinBValue) { // Если на линии А снова единица, фиксируем шаг
      count += state;  
      state = 0;
    }
 }