// http://codius.ru/articles/Инкрементальный_энкодер_принцип_действия_схемы_подключения_работа_с_Arduino

long timeButtonPressed = 1500;  // Долгое удержание кнопки после 1,5 секунд

volatile int state = 0;         // Переменная хранящая статус вращения

// Переменные хранящие состояние действия до его выполнения
volatile bool flagCW            = false;     // Было ли вращение по часовой стрелке
volatile bool flagCCW           = false;     // Было ли вращение против часовой стрелки
volatile bool flagButton        = false;     // Было ли нажатие кнопки
volatile bool flagButtonLong    = false;     // Было ли долгое удержание кнопки
volatile long timeButtonDown    = 0;         // Переменная хранящая время нажатия кнопки
volatile bool isButtonDown      = false;     // Переменная хранящая время нажатия кнопки
volatile bool longPressReleased = false;     // Переменная для фиксации срабатывания долгого нажатия

void setup()
{
 GPIO_SetMode(PA, BIT15, GPIO_PMD_INPUT);// pinMode(6, INPUT);
 GPIO_SetMode(PE, BIT5, GPIO_PMD_INPUT); // pinMode(7, INPUT);
 GPIO_SetMode(PB, BIT11, GPIO_PMD_INPUT);// pinMode(8, INPUT);
 GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_16);
 GPIO_ENABLE_DEBOUNCE(PA, BIT15);
 GPIO_ENABLE_DEBOUNCE(PB, BIT11);
 attachInterrupt(6, A, CHANGE);  // Настраиваем обработчик прерываний по изменению сигнала - в этом примере отслеживаем только 1 пин
 attachInterrupt(8, Button, CHANGE);   // Настраиваем обработчик прерываний по изменению сигнала нажатия кнопки
 Serial.begin(9600);             // Включаем Serial
}


void loop()
{
  if (millis() - timeButtonDown > timeButtonPressed && isButtonDown) { // Время длительного удержания наступило
    flagButtonLong = true;
  }
  if (flagCW) {                           // Шаг вращения по часовой стрелке
    // ...
    Serial.println("turn_right");
    flagCW = false;                       // Действие обработано - сбрасываем флаг
  }
  if (flagCCW) {                          // Шаг вращения против часовой стрелки
    // ...
    Serial.println("turn_left");
    flagCCW = false;                      // Действие обработано - сбрасываем флаг
  }
  if (flagButton) {                       // Кнопка нажата
    // ...
    Serial.println("short_press");
    flagButton = false;                   // Действие обработано - сбрасываем флаг
  }
  if (flagButtonLong && isButtonDown) {   // Кнопка удерживается
    if (!PB11 && millis() - timeButtonDown > timeButtonPressed) { // Защита от ложного срабатывания
      // ...
      Serial.println("long_press");
  }
    
    //=========================================== Настраиваем реакцию на долгое удержание кнопки ===============================================
    // Чтобы событие long_press во время удержания срботало только один раз, необходимо раскомментировать блок и закомментировать следующий
    //isButtonDown = false;                 // Программно "отжимаем" кнопку

    // Эти две строки отвечают за то, чтобы при долгом удержании кнопки, событие long_press повторялось каждые 1,5 секунды
    // Для того, чтобы изменить это поведение нужно закомментировать две эти строки и раскомментировать строку из предыдущего блока
    timeButtonDown = millis();            // Сбрасываем таймер
    longPressReleased = true;             // Флаг срабатывания долгого удержания, чтобы отсечь генерацию обычного нажатия при отпускании кнопки
    //==========================================================================================================================================

    flagButtonLong = false;               // Действие обработано - сбрасываем флаг
  }
}

void A()
{
  int pinAValue = PA15;        // Получаем состояние пинов A и B
  int pinBValue = PE5;

  if (!pinAValue &&  pinBValue) state = 1;  // Если при спаде линии А на линии B лог. единица, то вращение в одну сторону
  if (!pinAValue && !pinBValue) state = -1; // Если при спаде линии А на линии B лог. ноль, то вращение в другую сторону
  if (pinAValue && state != 0) {
    if (state == 1 && !pinBValue || state == -1 && pinBValue) { // Если на линии А снова единица, фиксируем шаг
      if (state == 1)   flagCW = true;      // Флаг вращения по часовой стрелке
      if (state == -1) flagCCW = true;      // Флаг вращения против часовой стрелки
      state = 0;
    }
  }
}

void Button()
{
  if (millis() - timeButtonDown < 50) return;
  int pinButValue = PB11;   // Получаем состояние пина кнопки
  timeButtonDown = millis();                  // Запоминаем время нажатия/отжатия
  if (!pinButValue) {                         // При нажатии подается инвертированный сигнал
    isButtonDown = true;                      // Устанавливаем флаг нажатия кнопки
  }
  else if (isButtonDown) {                    // Если кнопка отжата, смотрим не было ли выполнено действие
    if (!longPressReleased) {                 // Если долгое нажатие не было ни разу отработано, то...
      flagButton = true;                      // Если не было удержания, ставим флаг события обычного нажатия
    }
    isButtonDown = false;                     // Сбрасываем флаг нажатия
    longPressReleased = false;                // Сбрасываем флаг длительного удержания
  }
}