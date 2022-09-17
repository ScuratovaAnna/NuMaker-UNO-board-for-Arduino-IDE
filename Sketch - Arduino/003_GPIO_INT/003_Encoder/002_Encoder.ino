// http://codius.ru/articles/Инкрементальный_энкодер_принцип_действия_схемы_подключения_работа_с_Arduino

volatile long pause    = 50;  // Пауза для борьбы с дребезгом
volatile long lastTurn = 0;   // Переменная для хранения времени последнего изменения
volatile int count = 0;       // Счетчик оборотов
int actualcount    = 0;       // Временная переменная определяющая изменение основного счетчика
volatile int state = 0;       // Статус одного шага - от 0 до 4 в одну сторону, от 0 до -4 - в другую
volatile int pinAValue = 0;   // Переменные хранящие состояние пина, для экономии времени
volatile int pinBValue = 0;   // Переменные хранящие состояние пина, для экономии времени

void setup()
{
 GPIO_SetMode(PA, BIT15, GPIO_PMD_INPUT);// pinMode(6, INPUT);
 GPIO_SetMode(PE, BIT5, GPIO_PMD_INPUT); // pinMode(7, INPUT);
 GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_16);
 GPIO_ENABLE_DEBOUNCE(PE, BIT5);
 GPIO_ENABLE_DEBOUNCE(PA, BIT15);

  attachInterrupt(6, A, CHANGE);  // Настраиваем обработчик прерываний по изменению сигнала
  attachInterrupt(7, B, CHANGE);  // Настраиваем обработчик прерываний по изменению сигнала

  Serial.begin(9600);             // Включаем Serial
}

void loop()
{
  if (actualcount != count) {     // Чтобы не загружать ненужным выводом в Serial, выводим состояние
    actualcount = count;          // счетчика только в момент изменения
    Serial.println(actualcount);
  }
}

void A()
{
  if (micros() - lastTurn < pause) return;  // Если с момента последнего изменения состояния не прошло
  // достаточно времени - выходим из прерывания
  pinAValue = PA15;            // Получаем состояние пинов A и B
  pinBValue = PE5;
  
  if (state == 0  && !pinAValue &&  pinBValue || state == 2  && pinAValue && !pinBValue) {
    state += 1; // Если выполняется условие, наращиваем переменную state
    lastTurn = micros();
  }
  if (state == -1 && !pinAValue && !pinBValue || state == -3 && pinAValue &&  pinBValue) {
    state -= 1; // Если выполняется условие, наращиваем в минус переменную state
    lastTurn = micros();
  }
  setCount(state); // Проверяем не было ли полного шага из 4 изменений сигналов (2 импульсов)
  if (pinAValue && pinBValue && state != 0) state = 0; // Если что-то пошло не так, возвращаем статус в исходное состояние
}

void B()
{
  if (micros() - lastTurn < pause) return;
  pinAValue = PA15;
  pinBValue = PE5;
  
  if (state == 1 && !pinAValue && !pinBValue || state == 3 && pinAValue && pinBValue) {
    state += 1; // Если выполняется условие, наращиваем переменную state
    lastTurn = micros();
  }
  if (state == 0 && pinAValue && !pinBValue || state == -2 && !pinAValue && pinBValue) {
    state -= 1; // Если выполняется условие, наращиваем в минус переменную state
    lastTurn = micros();
  }
  setCount(state); // Проверяем не было ли полного шага из 4 изменений сигналов (2 импульсов)  
  if (pinAValue && pinBValue && state != 0) state = 0; // Если что-то пошло не так, возвращаем статус в исходное состояние
}

void setCount(int state) {          // Устанавливаем значение счетчика
  if (state == 4 || state == -4) {  // Если переменная state приняла заданное значение приращения
    count += (int)(state / 4);      // Увеличиваем/уменьшаем счетчик
    lastTurn = micros();            // Запоминаем последнее изменение
  }
}