
//#ifndef "Arduino.h"
    #include <Arduino.h>
//#endif

//#ifndef <Adafruit_MCP23008.h>
    #include "Adafruit_MCP23008.h"
//#endif

#include "ACE_Encoder.h"

void ACE_Encoder::begin(Adafruit_MCP23008 & mcpx) {

    mcp = mcpx;

}

// private
// read the encoder pins
// the char array enc.binVal is built in reverse 
byte ACE_Encoder::getCurrentEncoderVal() {

  enc.binVal[0] = {0};
  bool p;
  byte readEnc = 0;
  byte i = 7;

  for (byte n = 0; n < 8; n++) {
    p = mcp.digitalRead(n);
    readEnc |= (byte)p << n;
    enc.binVal[i] = (p == true ? '1' : '0');    
    i--;
  }

  return readEnc;
}


// private
// used to prevent duplicate entries when setting up
boolean ACE_Encoder::inEncoderArray(byte values[], byte val) {
  for (byte i = 0; i < encset.count; i++) {
    if (values[i] == val) {
      return true;
    }
  }
  return false;
}

// public
// read the current encoder value
byte ACE_Encoder::readEncoder() {
  
  byte value = getCurrentEncoderVal();

  if (value == 255) {
    return 0;
  }
  
  return value;
}

//public
void ACE_Encoder::encoderDirection(byte val) {

  // finds the current index by value in enc.values
  byte index = 0;

  while (index < encset.count) {
    if (encset.values[index] == val) {
      break;
    }
    index++;
  }

  // exit if no change
  if (index == encset.lastIndex) {
    return;
  }

  // the first two are a boundary checks, turning over the 360 degree limit to start again;
 
  // gotten to the start of the enc.values array while turning counter-clockwise
  if (encset.lastIndex == 0 && index == (encset.count - 1)) {
    enc.rotation = false;
  }
  // gotten to the end of the enc.values array while turning clockwise
  else if (encset.lastIndex == (encset.count - 1) && index == 0) {
    enc.rotation = true;
  }
  // turning counter-clockwise
  else if (index < encset.lastIndex) {
    enc.rotation = false;
  }
  // turning clockwise
  else {
    enc.rotation = true;
  }

  encset.lastIndex = index;

}

// public
// converts the byte vlaue into a char array in binary format for showing on the display
// the result os placed in enc.binVal
void ACE_Encoder::showEncoderVal(byte val) {

  char dout[9] = {0};
  byte i = 0;
  byte c = 7;
  
  while (i <= 7) {
    if ((val >> i) & 1) {
      dout[c] = '1';
    }
    else {
      dout[c] = '0';
    }
    i++;
    c--;
  }

  strcpy(enc.binVal, dout);  
}

// public
// used when setting up the encoder, rotate the encoder to get all 128 values on the
// output - it will then show an array to copy and paste to replace the ENCODER.values array in settings.h
// carefully rotate the encoder so it doesn't skip positions.
void ACE_Encoder::setupEncoderValues() {

  pinMode(LED_BUILTIN, OUTPUT);
  char iout[40] = {0};
  int c = 0;
  byte values[128] = {0};
  char valString[600] = {0};

  for (byte i = 0; i < encset.count; i++) {
    encset.values[i] = 0;
  }

  Serial.println("Setup: Steadily rotate encoder clockwise until .:STOP:. appears");

  while (true) {
 
    digitalWrite(LED_BUILTIN, HIGH);

    byte val = readEncoder();
    if (val == 0) {
      continue;
    }

    if (c >= encset.count) {
      Serial.println(".:STOP:.");
      break;
    }

    if (inEncoderArray(values, val)) {
      continue;
    }
    
    values[c] = val;

    sprintf(iout, " %3i, %3i", c, val);
    Serial.println(iout);

    c++;
    digitalWrite(LED_BUILTIN, LOW);
  }

  char sVal[4];
  for (byte i = 0; i < encset.count; i++) {
    //itoa(values[i],sVal,10);    
    sprintf(sVal, "%d", values[i]);  //  same as itoa 

    strcat(valString, sVal);
    strcat(valString,", ");    
    encset.values[i] = values[i];
  }
  int lastChar = strlen(valString);
  valString[lastChar-2] = '\0';  // removes the last ', ' from the end

  Serial.print("byte values[128] = {");
  Serial.print(valString);
  Serial.println("};");
  Serial.println(" ");

  digitalWrite(LED_BUILTIN, LOW);
}