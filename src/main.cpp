#include <Arduino.h>

#include <Wire.h>
#include "Adafruit_MCP23008.h"

#include <ACE_Encoder.h>

#define MONITOR_LED LED_BUILTIN

Adafruit_MCP23008 mcp;
ACE_Encoder encoder;

char pout[52] = {0};

void setup() {

  delay(1000);
  Serial.begin(9600);
  Serial.println("ready");

  pinMode(MONITOR_LED, OUTPUT);
  mcp.begin(0x0);      // use default address 0

  // setup the input pins
  for (byte n = 0; n <= 7; n++) {
    mcp.pinMode(n, INPUT);
    mcp.pullUp(n, HIGH); // turn on a 100K pullup internally
  }

  encoder.begin(mcp);
  
  // uncomment the following to setup the enc.values array
  // once complete, copy and paste to replace the ENCODER.values array in settings.h
  //encoder.setupEncoderValues();
  digitalWrite(MONITOR_LED, LOW);
}

void loop() {

  encoder.enc.value = encoder.readEncoder();

  if (encoder.enc.value > 0 && (encoder.enc.value != encoder.enc.lastValue))
  {
    digitalWrite(MONITOR_LED, HIGH);
    encoder.encoderDirection(encoder.enc.value); // sets enc.rotation

    sprintf(pout,"%s, 0x%02X, %3i, %s", encoder.enc.binVal, encoder.enc.value, 
                                encoder.enc.value, (encoder.enc.rotation ? " CW" : "CCW" ));
    Serial.println(pout);

    delay(200);
    encoder.enc.lastValue = encoder.enc.value;
    digitalWrite(MONITOR_LED, LOW);
  }

  delay(10);
}

