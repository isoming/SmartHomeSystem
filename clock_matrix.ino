#include <NS_Rainbow.h>
#include <VirtualWire.h>
#include <SPI.h>
#include <Time.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

// matrix clock const variables
const int clockDisplayStartColumn = 0;
const int clockDisplayStartRow = 1;
const int ampmStartColumn = 4;
const int ampmStartRow = 7;

unsigned long currentMillis;
unsigned long previousMillis = 0; // LED의 상태가 업데이트된 시간을 기록할 변수  
const long interval = 500;    //LED 상태 변경할 시간 지정(ms단위) 
int miliFlag = 0; 

// declaration of parts
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();


void setup() {
    Serial.begin(115200);
    
    setSyncProvider (RTC.get);
    setTime (19, 59, 50, 24, 2, 15);


    // clock display
    matrix.begin(0x71);  // pass in the address
}


// number display for clock
static const uint8_t PROGMEM
  hour_1[] =
  { B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00000000,
    B00000000,
    B00000000 },
  hour_2[] =
  { B00111000,
    B00001000,
    B00111000,
    B00100000,
    B00111000,
    B00000000,
    B00000000,
    B00000000 },
  hour_3[] =
  { B00111000,
    B00001000,
    B00111000,
    B00001000,
    B00111000,
    B00000000,
    B00000000,
    B00000000 },
  hour_4[] =
  { B00101000,
    B00101000,
    B00101000,
    B00111000,
    B00001000,
    B00000000,
    B00000000,
    B00000000 },
  hour_5[] =
  { B00111000,
    B00100000,
    B00111000,
    B00001000,
    B00111000,
    B00000000,
    B00000000,
    B00000000 },
  hour_6[] =
  { B00111000,
    B00100000,
    B00111000,
    B00101000,
    B00111000,
    B00000000,
    B00000000,
    B00000000 },
  hour_7[] =
  { B00111000,
    B00001000,
    B00001000,
    B00001000,
    B00001000,
    B00000000,
    B00000000,
    B00000000 },
  hour_8[] =
  { B00111000,
    B00101000,
    B00111000,
    B00101000,
    B00111000,
    B00000000,
    B00000000,
    B00000000 },
  hour_9[] =
  { B00111000,
    B00101000,
    B00111000,
    B00001000,
    B00111000,
    B00000000,
    B00000000,
    B00000000 },
  hour_10[] =
  { B10111000,
    B10101000,
    B10101000,
    B10101000,
    B10111000,
    B00000000,
    B00000000,
    B00000000 },
  hour_11[] =
  { B10001000,
    B10001000,
    B10001000,
    B10001000,
    B10001000,
    B00000000,
    B00000000,
    B00000000 },
  hour_12[] =
  { B10111000,
    B10001000,
    B10111000,
    B10100000,
    B10111000,
    B00000000,
    B00000000,
    B00000000 },

  minute_0[] =
  { B111,
    B101,
    B101,
    B101,
    B111,
    B000,
    B000,
    B000 },
  minute_1[] =
  { B010,
    B010,
    B010,
    B010,
    B010,
    B000,
    B000,
    B000 },
  minute_2[] =
  { B111,
    B001,
    B111,
    B100,
    B111,
    B000,
    B000,
    B000 },
  minute_3[] =
  { B111,
    B001,
    B111,
    B001,
    B111,
    B000,
    B000,
    B000 },
  minute_4[] =
  { B101,
    B101,
    B101,
    B111,
    B001,
    B000,
    B000,
    B000 },
  minute_5[] =
  { B111,
    B100,
    B111,
    B001,
    B111,
    B000,
    B000,
    B000 },
  minute_6[] =
  { B111,
    B100,
    B111,
    B101,
    B111,
    B000,
    B000,
    B000 },
  minute_7[] =
  { B111,
    B001,
    B001,
    B001,
    B001,
    B000,
    B000,
    B000 },
  minute_8[] =
  { B111,
    B101,
    B111,
    B101,
    B111,
    B000,
    B000,
    B000 },
  minute_9[] =
  { B111,
    B101,
    B111,
    B001,
    B111,
    B000,
    B000,
    B000 },

  seconds[] =
  { B000,
    B100,
    B000,
    B100,
    B000,
    B000,
    B000,
    B000 };


void loop() {
  currentMillis = millis();  

  
    if(currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;     
        if (miliFlag == 1) {
            miliFlag = 0;
        } else {
            miliFlag = 1;
        }
    }

    timeDisplayOnMatrix (miliFlag, hour(), minute(), second());
    
    delay(5);

}


// clock related functions
void timeDisplayOnMatrix (int flag, int hour, int minute, int second) {
    matrix.setRotation(1);

    if (flag == 1) {
        matrix.clear();
        drawHour (hour);
        matrix.drawBitmap(clockDisplayStartColumn + 1, 
                          clockDisplayStartRow, 
                          seconds, 8, 8, LED_ON);
        drawMinute (minute);
        matrix.writeDisplay();
    } else {
        matrix.clear();
        drawHour (hour);
        drawMinute (minute);
        matrix.writeDisplay();
    }
}

void drawHour (int hour) {
    /*if (hour >= 0 && hour < 12) {
        matrix.drawLine(ampmStartColumn, ampmStartRow, 
                        ampmStartColumn + 1, ampmStartRow, 
                        LED_ON);
    } else {
        matrix.drawLine(ampmStartColumn + 3, ampmStartRow, 
                        ampmStartColumn + 4, ampmStartRow, 
                        LED_ON);
    }*/
    
    switch (hour) {
      case 1:
      case 13:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_1, 8, 8, LED_ON);
        break;
      case 2:
      case 14:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_2, 8, 8, LED_ON);
        break;
      case 3:
      case 15:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_3, 8, 8, LED_ON);
        break;
      case 4:
      case 16:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_4, 8, 8, LED_ON);
        break;
      case 5:
      case 17:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_5, 8, 8, LED_ON);
        break;
      case 6:
      case 18:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_6, 8, 8, LED_ON);
        break;
      case 7:
      case 19:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_7, 8, 8, LED_ON);
        break;
      case 8:
      case 20:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_8, 8, 8, LED_ON);
        break;
      case 9:
      case 21:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_9, 8, 8, LED_ON);
        break;
      case 10:
      case 22:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_10, 8, 8, LED_ON);
        break;
      case 11:
      case 23:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_11, 8, 8, LED_ON);
        break;
      case 12:
      case 24:
      case 00:
        matrix.drawBitmap(clockDisplayStartColumn, 
                          clockDisplayStartRow, 
                          hour_12, 8, 8, LED_ON);
        break;
      default:
        break;
    }
}

void drawMinute (int minute) {
    int firstNum = minute / 10;
    int secondNum = minute % 10;
  
    switch (firstNum) {
        case 0:
            matrix.drawBitmap(clockDisplayStartColumn + 3, 
                              clockDisplayStartRow, 
                              minute_0, 8, 8, LED_ON);
            break;
        case 1:
            matrix.drawBitmap(clockDisplayStartColumn + 3, 
                              clockDisplayStartRow, 
                              minute_1, 8, 8, LED_ON);
            break;
        case 2:
            matrix.drawBitmap(clockDisplayStartColumn + 3, 
                              clockDisplayStartRow, 
                              minute_2, 8, 8, LED_ON);
            break;
        case 3:
            matrix.drawBitmap(clockDisplayStartColumn + 3, 
                              clockDisplayStartRow, 
                              minute_3, 8, 8, LED_ON);
            break;
        case 4:
            matrix.drawBitmap(clockDisplayStartColumn + 3, 
                              clockDisplayStartRow, 
                              minute_4, 8, 8, LED_ON);
            break;
        case 5:
            matrix.drawBitmap(clockDisplayStartColumn + 3, 
                              clockDisplayStartRow, 
                              minute_5, 8, 8, LED_ON);
            break;
        default:
            break;
    }
  
    switch (secondNum) {
        case 0:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_0, 8, 8, LED_ON);
            break;
        case 1:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_1, 8, 8, LED_ON);
            break;
        case 2:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_2, 8, 8, LED_ON);
            break;
        case 3:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_3, 8, 8, LED_ON);
            break;
        case 4:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_4, 8, 8, LED_ON);
            break;
        case 5:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_5, 8, 8, LED_ON);
            break;
        case 6:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_6, 8, 8, LED_ON);
            break;
        case 7:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_7, 8, 8, LED_ON);
            break;
        case 8:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_8, 8, 8, LED_ON);
            break;
        case 9:
            matrix.drawBitmap(clockDisplayStartColumn + 7, 
                              clockDisplayStartRow, 
                              minute_9, 8, 8, LED_ON);
            break;
        default:
            break;
    }
}
