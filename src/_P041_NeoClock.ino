#include "_Plugin_Helper.h"


//#ifdef USES_P041
//#######################################################################################################
//#################################### Plugin 041: NeoPixel clock #######################################
//#######################################################################################################
#include <Adafruit_NeoPixel.h>
#include "src/Globals/ESPEasy_time.h"

#define NUM_LEDS      110

byte Plugin_041_red = 0;
byte Plugin_041_green = 0;
byte Plugin_041_blue = 0;

Adafruit_NeoPixel *Plugin_041_pixels;

#define PLUGIN_041
#define PLUGIN_ID_041         41
#define PLUGIN_NAME_041       "Output - NeoPixel (Word Clock)"
#define PLUGIN_VALUENAME1_041 "Clock"
boolean Plugin_041(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_041;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].VType = Sensor_VType::SENSOR_TYPE_NONE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 0;
        Device[deviceCount].SendDataOption = false;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_041);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_041));
        break;
      }

    case PLUGIN_GET_DEVICEGPIONAMES:
      {
        event->String1 = formatGpioName_output(F("Data"));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        addFormNumericBox(F("Red"), F("p041_red"), PCONFIG(0), 0, 255);
        addFormNumericBox(F("Green"), F("p041_green"), PCONFIG(1), 0, 255);
        addFormNumericBox(F("Blue"), F("p041_blue"), PCONFIG(2), 0, 255);
        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        PCONFIG(0) = getFormItemInt(F("p041_red"));
        PCONFIG(1) = getFormItemInt(F("p041_green"));
        PCONFIG(2) = getFormItemInt(F("p041_blue"));
        Plugin_041_red = PCONFIG(0);
        Plugin_041_green = PCONFIG(1);
        Plugin_041_blue = PCONFIG(2);
        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        if (!Plugin_041_pixels)
        {
          Plugin_041_pixels = new Adafruit_NeoPixel(NUM_LEDS, CONFIG_PIN1, NEO_GRB + NEO_KHZ800);
          Plugin_041_pixels->begin(); // This initializes the NeoPixel library.
        }
        Plugin_041_red = PCONFIG(0);
        Plugin_041_green = PCONFIG(1);
        Plugin_041_blue = PCONFIG(2);
        success = true;
        break;
      }

    case PLUGIN_CLOCK_IN:
      {
        Plugin_041_update();
        success = true;
        break;
      }


    case PLUGIN_ONCE_A_SECOND:
      {
        //int ldrVal = map(analogRead(A0), 0, 1023, 15, 245);
        //     Serial.print("LDR value: ");
        //     Serial.println(ldrVal, DEC);
        //Plugin_041_pixels->setBrightness(255 - ldrVal);
        //Plugin_041_pixels->show(); // This sends the updated pixel color to the hardware.
        //Plugin_041_update();
        success = true;
        break;
      }
      

    case PLUGIN_WRITE:
      {
        String cmd = parseString(string, 1);

        if (cmd.equalsIgnoreCase(F("NeoClockColor")))
        {
          Plugin_041_red = event->Par1;
          Plugin_041_green = event->Par2;
          Plugin_041_blue = event->Par3;
          Plugin_041_update();
          success = true;
        }

        if (cmd.equalsIgnoreCase(F("NeoTestAll")))
        {
          for (int i = 0; i < NUM_LEDS; i++)
            Plugin_041_pixels->setPixelColor(i, Plugin_041_pixels->Color(event->Par1, event->Par2, event->Par3));
          Plugin_041_pixels->show(); // This sends the updated pixel color to the hardware.
          success = true;
        }

        if (cmd.equalsIgnoreCase(F("NeoTestLoop")))
        {
          for (int i = 0; i < NUM_LEDS; i++)
          {
            resetAndBlack();
            Plugin_041_pixels->setPixelColor(i, Plugin_041_pixels->Color(event->Par1, event->Par2, event->Par3));
            Plugin_041_pixels->show(); // This sends the updated pixel color to the hardware.
            delay(200);
          }
          success = true;
        }

        break;
      }

  }
  return success;
}

void Plugin_041_update()
{
  byte Hours = node_time.hour();
  byte Minutes = node_time.minute();
  resetAndBlack();
  timeToStrip(Hours, Minutes);
  Plugin_041_pixels->show(); // This sends the updated pixel color to the hardware.
}


void resetAndBlack() {
  for (int i = 0; i < NUM_LEDS; i++) {
    Plugin_041_pixels->setPixelColor(i, Plugin_041_pixels->Color(0, 0, 0));
  }
}

void pushToStrip(int ledId) {
  Plugin_041_pixels->setPixelColor(ledId, Plugin_041_pixels->Color(Plugin_041_red, Plugin_041_green, Plugin_041_blue));
}

void pushToStripArr(int positions[], int sizeArr) {
  for (int i = 0; i < sizeArr; i++) {
    // we count from 1, setPixelColor needs from 0
    pushToStrip(positions[i] - 1);
    //    Serial.print(positions[i]-1);
    //    Serial.print(',');
  }
  //  Serial.println(sizeArr);
}

void timeToStrip(uint8_t hours, uint8_t minutes)
{
  pushIT_IS();
  //show minutes
  if (minutes >= 5 && minutes < 10) {
    pushFIVE1();
    pushAFTER();
  } else if (minutes >= 10 && minutes < 15) {
    pushTEN1();
    pushAFTER();
  } else if (minutes >= 15 && minutes < 20) {
    pushQUATER();
    pushAFTER();
  } else if (minutes >= 20 && minutes < 25) {
    pushTWENTY();
    pushAFTER();
  } else if (minutes >= 25 && minutes < 30) {
    pushFIVE1();
    pushFOR();
    pushHALF();
  } else if (minutes >= 30 && minutes < 35) {
    pushHALF();
  } else if (minutes >= 35 && minutes < 40) {
    pushFIVE1();
    pushAFTER();
    pushHALF();
  } else if (minutes >= 40 && minutes < 45) {
    pushTWENTY();
    pushFOR();
  } else if (minutes >= 45 && minutes < 50) {
    pushQUATER();
    pushFOR();
  } else if (minutes >= 50 && minutes < 55) {
    pushTEN1();
    pushFOR();
  } else if (minutes >= 55 && minutes < 60) {
    pushFIVE1();
    pushFOR();
  }

  if (hours >= 12) {
    hours -= 12;
  }
  if (hours == 12) {
    hours = 0;
  }
  if (minutes >= 25) {
    hours++;
  }

  //show hours
  switch (hours) {
    case 0:
      pushTWELVE();
      break;
    case 1:
      if (minutes < 5) {
        pushONE();
      } else {
        pushONEs();
      }
      break;
    case 2:
      pushTWO();
      break;
    case 3:
      pushTHREE();
      break;
    case 4:
      pushFOUR();
      break;
    case 5:
      pushFIVE2();
      break;
    case 6:
      pushSIX();
      break;
    case 7:
      pushSEVEN();
      break;
    case 8:
      pushEIGHT();
      break;
    case 9:
      pushNINE();
      break;
    case 10:
      pushTEN();
      break;
    case 11:
      pushELEVEN();
      break;
    case 12:
      pushTWELVE();
      break;
  }
  //show HOUR
  if (minutes < 5) {
    pushHOURE();
  }
}

void pushIT_IS()  {
  int positions[] = {100, 101, 103, 104, 105};

  pushToStripArr(positions, 5);
}
void pushAFTER() {
  int positions[] = {72, 73, 74, 75};
  pushToStripArr(positions, 4);
}
void pushQUATER() {
  int positions[] = {82, 83, 84, 85, 86, 87, 88};
  pushToStripArr(positions, 7);
}
void pushFOR() {
  int positions[] = {71, 70, 69};
  pushToStripArr(positions, 3);
}
void pushHALF() {
  int positions[] = {56, 57, 58, 59};
  pushToStripArr(positions, 4);
}
void pushONE()  {
  int positions[] = {53, 52, 51};
  pushToStripArr(positions, 3);
}
void pushONEs()  {
  int positions[] = {53, 52, 51, 50};
  pushToStripArr(positions, 4);
}
void pushTWO() {
  int positions[] = {55, 54, 53, 52};
  pushToStripArr(positions, 4);
}
void pushTHREE() {
  int positions[] = {35, 36, 37, 38};
  pushToStripArr(positions, 4);
}
void pushFOUR() {
  int positions[] = {26, 25, 24, 23};
  pushToStripArr(positions, 4);
}
void pushFIVE1() {
  int positions[] = {107, 108, 109, 110};
  pushToStripArr(positions, 4);
}
void pushFIVE2() {
  int positions[] = {41, 42, 43, 44};
  pushToStripArr(positions, 4);
}
void pushSIX() {
  int positions[] = {10, 9, 8, 7, 6};
  pushToStripArr(positions, 5);
}
void pushSEVEN() {
  int positions[] = {50, 49, 48, 47, 46, 45};
  pushToStripArr(positions, 6);
}
void pushEIGHT() {
  int positions[] = {13, 14, 15, 16};
  pushToStripArr(positions, 4);
}
void pushNINE() {
  int positions[] = {30, 29, 28, 27};
  pushToStripArr(positions, 4);
}
void pushTEN() {
  int positions[] = {17, 18, 19, 20};
  pushToStripArr(positions, 4);
}
void pushTEN1() {
  int positions[] = {99, 98, 97, 96};
  pushToStripArr(positions, 4);
}
void pushELEVEN() {
  int positions[] = {33, 32, 31};
  pushToStripArr(positions, 3);
}
void pushTWELVE() {
  int positions[] = {61, 62, 63, 64, 65};
  pushToStripArr(positions, 5);
}
void pushTWENTY() {
  int positions[] = {95, 94, 93, 92, 91, 90, 89};
  pushToStripArr(positions, 7);
}
void pushHOURE() {
  int positions[] = {3, 2, 1};
  pushToStripArr(positions, 3);
}
// #endif // USES_P041
