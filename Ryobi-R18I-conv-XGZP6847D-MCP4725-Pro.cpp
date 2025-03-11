/*
   K value for XGZP6897D. It depend on the pressure range of the sensor.
   Table found in the data sheet from CFSensor.com
    https://cfsensor.com/product-category/i2c-sensor/https://cfsensor.com/product-category/i2c-sensor/
  the K value is selected according to the positive pressure value only,
*/
#include <Wire.h>
#include <XGZP6897D.h>
#include <Adafruit_MCP4725.h>  // MCP4725 library from adafruit
#include "math.h"
#define adrMCP4725 0x60            //устанавливаем для MCP4725 адрес 0x61, для своего АЦП вы можете использовать свой адрес
#define K 176                      // see table above for the correct value for your sensor, according to the sensitivity.
XGZP6897D mysensor(K);             //
#define P_kPa_max 1030             // Макс входное давление в кПа на R18I (datasheet)
#define VPCB_max 0.45              // Макс напряжение на PCB Ryobi при значении на экране "0"
#define IP_VPCB_max 390            //  Аналоговый уровень макс напряжения на PCB Ryobi при значении на экране "0" для 12бит (4096) пределе
float tC, P_kPa_IN, Inv_P_kPa_IN;  // XGZP6897D: темп., давление (кПа) и инвертированное давление 1030-входное (кПа)
float P_V_Ryobi, Round_P_V_Ryobi;  //наряжение
float ADJ = 0.020;
Adafruit_MCP4725 MCP4725;

void setup() {
  Serial.begin(9600);
  MCP4725.begin(adrMCP4725);  // Default I2C Address of MCP4725 breakout board (sparkfun)
}

void loop() {
  mysensor.readSensor(tC, P_kPa_IN);    // read the temperature (in °Celsius), and the pressure (in Pa)
  constrain(P_kPa_IN, 0, P_kPa_max);    //ограничение давления
  Inv_P_kPa_IN = P_kPa_max - P_kPa_IN;  //инверсия давления для работы PCB Ryobi
  P_V_Ryobi = ((Inv_P_kPa_IN * 422 )/ P_kPa_max);  //перевод в уровни аналогового сигнала ращрядностью 12 бит
 Round_P_V_Ryobi = P_V_Ryobi;
  constrain(P_V_Ryobi, 0, IP_VPCB_max);        //ограничение уровня аналогового сигнала для PCB Ryobi
  MCP4725.setVoltage(Round_P_V_Ryobi, false);  // запись уровня в MCP4725
}
