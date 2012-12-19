/* Temperature web page

This sketch uses the DallasTemperature library to read several temp sensors,
then serves a webpage with the data

Mike Pitsch
18 Dec 2012

*/

#include <Streaming.h>

// Web includes
#include <SPI.h>
#include <Ethernet.h>
#include "WebServer.h"

// Temp includes
#include <OneWire.h>
#include <DallasTemperature.h>

float temps[4];


void setup() {
  setupEth();
  setupTemp();
}


void loop() {
  static uint32_t timetosend;
  static uint32_t timetoWeb;
    
  loopEth();
  if (millis() - timetosend > 10000)  // every 10 seconds
    {
      timetosend = millis(); 
      // update sensor data
      loopTemp();
    }
 
  GetTemp(&temps[0]);
//  Serial.println(temps[0]);
//  Serial.println(temps[1]);
//  Serial.println(temps[2]);
//  Serial.println(temps[3]);
  
  if (millis() - timetoWeb > 60000) // every 60 seconds
    {
      timetoWeb = millis(); 
      // update google app
      loopPost();
    }
}
