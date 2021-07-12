const word PROGMEM arr_pwm[65] = {0, 127, 139, 152, 164, 176, 187, 198, 208, 217, 225, 233,
                                  239, 244, 249, 252, 253, 254, 253, 252, 249, 244, 239, 233, 225, 217, 208, 198, 187, 176,
                                  164, 152, 139, 127, 115, 102, 90, 78, 67, 56, 46, 37, 29, 21, 15, 10, 5, 2, 1, 1, 1, 2, 5,
                                  10, 15, 21, 29, 37, 46, 56, 67, 78, 90, 102, 115
                                 };
volatile uint8_t  count_A = 1;

void SYS_Init(void) {
  SYS->ALT_MFP = SYS_ALT_MFP_PB3_TM3;
  SYS->ALT_MFP2 = SYS_ALT_MFP2_PB3_TM3 | SYS_ALT_MFP2_PB2_TM2;
  SYS->GPB_MFP = SYS_GPB_MFP_PB3_TM3 | SYS_GPB_MFP_PB2_TM2;
}

/*
  {
  SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB2_Msk | SYS_GPB_MFP_PB3_Msk);
  SYS->GPB_MFP |= (SYS_GPB_MFP_PB2_TM2 | SYS_GPB_MFP_PB3_TM3);
  SYS->ALT_MFP2 &= ~(SYS_ALT_MFP2_PB2_Msk | SYS_ALT_MFP2_PB3_Msk);
  SYS->ALT_MFP2 |= (SYS_ALT_MFP2_PB2_TM2 | SYS_ALT_MFP2_PB3_TM3);
  //}

  //{
  SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB10_Msk | SYS_GPB_MFP_PB11_Msk);
  SYS->GPB_MFP |= (SYS_GPB_MFP_PB10_TM2 | SYS_GPB_MFP_PB11_TM3);
  SYS->ALT_MFP2 &= ~(SYS_ALT_MFP2_PB10_Msk | SYS_ALT_MFP2_PB11_Msk);
  SYS->ALT_MFP2 |= (SYS_ALT_MFP2_PB10_TM2 | SYS_ALT_MFP2_PB11_TM3);
  }*/

void setup() {
  SYS_UnlockReg();
  SYS_Init();
  SYS_LockReg();

  // uint32_t clock = 12000000; // 12Mhz
  /*Поменяем базовые настройки таймера*/
  /*Timer4.open(TOGGLE, 239);
    Timer4.setPrescaleFactor(0);//Установим предделитель 0
    Timer4.setCompare(120);//12000000/128=93750
    //Timer1.attachInterrupt(timer_ISR);
    Timer4.start();*/

  /*Поменяем базовые настройки таймера*/
  Timer4.open(TOGGLE, 23);
  Timer4.setPrescaleFactor(0);//Установим предделитель 0
  Timer4.setCompare(12);
  //Timer1.attachInterrupt(timer_ISR);
  Timer4.start();

  Timer3.open(TOGGLE, 255);
  Timer3.setPrescaleFactor(50);//Установим предделитель 128
  Timer3.setCompare(count_A);//12000000/128=93750
  Timer3.attachInterrupt(timer_ISR_3);
  Timer3.start();
}

void loop() {
}

void timer_ISR_3(uint8_t num)
{
  Timer3.setCompare(arr_pwm[count_A]);
  count_A++;
  if (count_A == 65) {
    count_A = 1;
  }
  Timer3.clearIntFlag();
}
