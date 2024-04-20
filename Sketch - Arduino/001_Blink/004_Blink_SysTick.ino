/**-------------------------------------------------------------------
*\ date  20.04.2024
*   NuMaker  UNO             
*   ---------------        
*  |               |       
*  |               |       
*  |  (PB.13)  13~ | ----> LED I\O
*  |               |
*
*\ author           ScuratovaAnna 
*\ сode debugging   PivnevNikolay
*/

/****************** Example 1 ***************************/
void setup() {
 GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
}

void loop() {
 PB13=!PB13;
/*Процессор имеет 24 разрядный системный таймер SysTick, который считает вниз от загруженного в нeго значения до нуля. 
 *   --    --    --
 *  |  |  |  |  |  |
 *  |  |__|  |__|  |__ period 10ms Freq 10Hz
 *
 *
 *  The Max value is 2^24 / CPU Clock(MHz).50MHz => 335544us, 48MHz => 349525us, 28MHz => 699050us ...
 */
 CLK_SysTickDelay(50000);
}
/****************** Example 2 ***************************/
void setup() {
 GPIO_SetMode(PB, BIT13, GPIO_PMD_OUTPUT);
}

void loop() {
 PB13=!PB13;
 /*  --    --    --
 *  |  |  |  |  |  |
 *  |  |__|  |__|  |__ period 1s Freq 1Hz
 */
 CLK_SysTickLongDelay(500000);
}
/********************************************************/