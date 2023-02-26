#include "HID-Project.h"
#include "NameMochiBitmap.h"
#include "LogoMochiBitmap.h"
#include "DonKaBitmap.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

#define bitmap_height 128
#define bitmap_width 32

//PINS DEFINITIONS
const int pinLed = LED_BUILTIN;
const int REDL = 16;
const int REDR = 14;
const int BLUEL = 15;
const int BLUER = 10;

//MISC DEFINITIONS
byte REDRoldState = HIGH;
byte REDLoldState = HIGH;
byte BLUERoldState = HIGH;
byte BLUELoldState = HIGH;

const unsigned long debounceTime = 40;  // milliseconds
unsigned long task_time_ms = 0;
unsigned long timeOfLastHit = 0;
unsigned long REDRswitchPressTime;  // when the switch last changed state
unsigned long REDLswitchPressTime;  // when the switch last changed state
unsigned long BLUERswitchPressTime;  // when the switch last changed state
unsigned long BLUELswitchPressTime;  // when the switch last changed state

bool isUp = false;


//*******************************************************************************
//*******************************************************************************
//*******************************************************************************

void setup()
{
  //Pinmodes Setup
  pinMode(REDL, INPUT_PULLUP);
  pinMode(REDR, INPUT_PULLUP);
  pinMode(BLUEL, INPUT_PULLUP);
  pinMode(BLUER, INPUT_PULLUP);

  Wire.setClock(400000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    for (;;)
      ; // Don't proceed, loop forever
  }



  // Displays the startup logo.
  for(int i=-36; i<0; i++)
  {
    display.clearDisplay();
    display.drawBitmap(i, 0, LogoMochi, 33, 27, WHITE);
    display.display();
  }

  for(int i=-21; i<3; i++)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0, LogoMochi, 33, 27, WHITE);
    display.drawBitmap(35,i, NameOled, 91, 23, WHITE);
    display.display();
  }

  delay(2000);

  display.clearDisplay();
  display.drawBitmap(0, 0, kaDown, bitmap_height, bitmap_width, WHITE);
  display.display();
  
  Keyboard.begin();
}


//*******************************************************************************
//*******************************************************************************
//*******************************************************************************

void loop()
{

  // Reads any button.
  byte REDRState = digitalRead(REDR);
  byte REDLState = digitalRead(REDL);
  byte BLUERState = digitalRead(BLUER);
  byte BLUELState = digitalRead(BLUEL);


  if((millis()-timeOfLastHit) >= 5000)
  {
      if((millis()-task_time_ms) >= 500)
      {
      display.clearDisplay();
      if (isUp)
        display.drawBitmap(0, 0, kaDown, bitmap_height, bitmap_width, WHITE);
      else
        display.drawBitmap(0, 0, donUp, bitmap_height, bitmap_width, WHITE);
        
      isUp = !isUp;
      display.display();
      task_time_ms = millis();
    }
  }


  if (REDRState != REDRoldState)
  {
    // debounce
    if (millis() - REDRswitchPressTime >= debounceTime)
    {
      if(REDRState == LOW)
      {
        Keyboard.write('j');
        isUp = !isUp;
        timeOfLastHit = millis();
      }

      REDRswitchPressTime = millis();
      REDRoldState = REDRState;
    }
  }


  if (REDLState != REDLoldState)
  {
    // debounce
    if (millis() - REDLswitchPressTime >= debounceTime)
    {
      if(REDLState == LOW)
      {
        Keyboard.write('f');
        isUp = !isUp;
        timeOfLastHit = millis();
      }

      REDLswitchPressTime = millis();
      REDLoldState = REDLState;
    }
  }

  if (BLUERState != BLUERoldState)
  {
    // debounce
    if (millis() - BLUERswitchPressTime >= debounceTime)
    {
      if(BLUERState == LOW)
      {
        Keyboard.write('k');
        isUp = !isUp;
        timeOfLastHit = millis();
      }

      BLUERswitchPressTime = millis();
      BLUERoldState = BLUERState;
    }
  }


  if (BLUELState != BLUELoldState)
  {
    // debounce
    if (millis() - BLUELswitchPressTime >= debounceTime)
    {
      if(BLUELState == LOW)
      {
        Keyboard.write('d');
        isUp = !isUp;
        timeOfLastHit = millis();
      }

      BLUELswitchPressTime = millis();
      BLUELoldState = BLUELState;
    }
  }
}
