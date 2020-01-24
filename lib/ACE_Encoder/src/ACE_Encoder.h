//#ifndef <Arduino.h>
#include <Arduino.h>
//#endif

//#ifndef <Adafruit_MCP23008.h>
//   #include "Adafruit_MCP23008.h"
//#endif

class ACE_Encoder
{

public:
  void begin(Adafruit_MCP23008 &mcpx);
  byte readEncoder();
  void encoderDirection(byte val);
  void setupEncoderValues();
  void showEncoderVal(byte val);

  struct ENCODER
  {
    uint8_t value = 0;
    bool rotation = false; // true = clockwise, false = anticlockwise
    char binVal[9] = {0};
    uint8_t lastValue = 0;
  } enc;

private:
  struct ENCVALS {
    // see setupEncoderValues() in ACE_Encoder.cpp to see how this is set.
    uint8_t values[128] = {140, 172, 44, 12, 4, 68, 84, 86, 87, 23, 151, 183, 191, 63, 31, 30, 14, 
                            78, 70, 6, 2, 130, 162, 163, 179, 51, 59, 123, 127, 119, 55, 39, 7, 135, 
                            131, 3, 1, 9, 73, 89, 121, 113, 117, 245, 247, 243, 115, 83, 19, 27, 25, 
                            17, 16, 20, 148, 180, 244, 240, 242, 250, 251, 249, 241, 177, 49, 53, 52, 
                            48, 32, 34, 42, 106, 234, 232, 233, 237, 253, 252, 248, 120, 112, 114, 98, 
                            96, 64, 65, 69, 197, 205, 204, 220, 222, 254, 238, 236, 228, 224, 225, 
                            193, 192, 128, 144, 146, 154, 158, 142, 174, 175, 239, 207, 206, 202, 200, 
                            216, 152, 136, 8, 40, 41, 45, 47, 15, 79, 95, 223, 159, 143, 141};
    const uint8_t count = 128;
    uint8_t lastIndex = 0;
  } encset;

  Adafruit_MCP23008 mcp;
  byte getCurrentEncoderVal();
  bool inEncoderArray(byte values[], byte val);
};
