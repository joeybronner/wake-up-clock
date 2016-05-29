#include <DS1302.h>
#include <TM1637Display.h>
#include <stdlib.h> 

/*
 * descr : Movement sensor who wakes up a clock to display
 *         datetime when someone walks in front.
 * 
 * author: Joey Bronner
 * date  : 29.05.2016
 */

// RTC Module documentation DS1302
// http://domoticx.com/arduino-modules-rtc-tijdklok-ds1302/
DS1302 rtc(7, 6, 5);

// 4 Digits Display
// Set the CLK & DIO pin connection to the display and set up the device
const int CLK = 8;
const int DIO = 9;
TM1637Display display(CLK, DIO); 

// Movement sensor
const int movementPin = 2;

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}

char *convert (uint8_t *a)
{
    int buffer1[9];
    char buffer2[9];
    int i;
    char *buffer_pointer;

    buffer1[8]='\0';

    for(i=0; i<=7; i++)
        buffer1[7-i]=( ((*a)>>i)&(0x01) );

    for(i=0; i<=7; i++)
        buffer2[i] = buffer1[i] + '0';

    buffer2[8] = '\0';

    puts(buffer2);

    buffer_pointer = buffer2;

    return buffer_pointer;
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  /*snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);*/

  snprintf(buf, sizeof(buf), "%02d%02d", t.hr, t.min);

  // Print the formatted string to serial so we can see the time.
  int number = atoi(buf);
  display.setColon(true);
  display.showNumberDec(number);
}

void setup(){
  pinMode(movementPin, INPUT);
  Serial.begin(9600);
  
  // Set the diplay to maximum brightness
  display.setBrightness(0x0a);

  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(2016, 5, 29, 13, 51, 40, Time::kSunday);
  rtc.time(t);
}

void loop(){
  int value= digitalRead(movementPin);
  if (value == HIGH)
  {
    display.setBrightness(0x0a);
    printTime();
    Serial.print("HIGH");
    delay(1000);
  } else {
    Serial.print("LOW");
    uint8_t data[] = {0x0, 0x0, 0x0, 0x0};
    display.setSegments(data);
  }
}
