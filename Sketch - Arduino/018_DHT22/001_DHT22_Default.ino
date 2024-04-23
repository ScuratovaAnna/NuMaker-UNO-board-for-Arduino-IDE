
/**-------------------------------------------------------------------
*\ date  23.04.2024
*   NuMaker  UNO             
*   ---------------        
*  |               |       
*  |               |       
*  |     (PE.5) 7~ | ----> DataPin DHT22
*  |               |
*
*\ author           ScuratovaAnna 
*\ сode debugging   PivnevNikolay
*/
#include "SimpleDHT.h"
/* Библиотека для датчика DHT11, DHT22
 * Библиотека дружит с NuMaker UNO 
 * https://github.com/winlinvip/SimpleDHT
 * для примера использовали датчик DHT 22 от RobotDyn 
 * https://aliexpress.ru/item/32797919049.html?sku_id=63893759215&spm=a2g2w.stores.seller_list.8.f3966f0dOD8h2y
 * VCC  DHT22: 5V 
 * GND  DHT22: GND
 * DATA DHT22: pin 7
 */
int pinDHT22 = 7;
SimpleDHT22 dht22(pinDHT22);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT22...");
  
  // read without samples.
  // @remark We use read2 to get a float data, such as 10.1*C
  //    if user doesn't care about the accurate data, use read to get a byte data, such as 10*C.
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(2000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((float)temperature); Serial.print(" *C, ");
  Serial.print((float)humidity); Serial.println(" RH%");
  
  // DHT22 sampling rate is 0.5HZ.
  delay(2500);
}
