volatile int encoder_Counter = 50;
volatile boolean flag;
volatile boolean resetFlag = 1;
volatile byte curState;
volatile byte prevState;
void setup() {
  Serial.begin(9600);
  Serial.println(encoder_Counter);
  GPIO_SetMode(PA, BIT15, GPIO_PMD_INPUT);// pinMode(6, INPUT);
  GPIO_SetMode(PE, BIT5, GPIO_PMD_INPUT); // pinMode(7, INPUT);
  attachInterrupt(6, int0, CHANGE);
  attachInterrupt(7, int0, CHANGE);
}
void int0() {
  encoder_Tick();
}
void encoder_Tick() {
  curState = PA15 | PE5 << 1;
  if (resetFlag && curState == 0b11) {
    if (prevState == 0b10) encoder_Counter++;
    if (prevState == 0b01) encoder_Counter--;
    resetFlag = 0;
    flag = true;
  }
  if (curState == 0b00) resetFlag = 1;
  prevState = curState;
}
void loop() {
  if (flag) {
    Serial.println(encoder_Counter);
    flag = false;
  }
}