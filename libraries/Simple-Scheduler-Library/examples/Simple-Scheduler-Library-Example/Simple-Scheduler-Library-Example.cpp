#include <Arduino.h>
#include "SimpleScheduler.h"
#include "SimplePrint.h"

SimpleScheduler scheduler( 1000);

#define LED_OFF LOW
#define LED_ON HIGH

void print() { 
  Serial.print( "*"); 
}

void blink() { 
  static bool b = false; b = !b;

  if ( b) {
    digitalWrite( LED_BUILTIN, LED_ON);
  } else {
    digitalWrite( LED_BUILTIN, LED_OFF);
  }
}

void setup()
{
  Serial.begin( 9600);
  pinMode( LED_BUILTIN, OUTPUT);

  scheduler.attachHandler( print);
  scheduler.attachHandler( blink);

  Serial.println( "# Ready");
  scheduler.start();
}

void loop()
{
  //yield();
}
