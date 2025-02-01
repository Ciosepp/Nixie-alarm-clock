
#include "Timer.h"

#include "nixieDisplay.h"

#include "menu.h"



#define CLOCK_PIN 2
#define DATA_PIN 3
#define SAMPLE_PIN 4

nixieDisplay n(CLOCK_PIN,DATA_PIN,SAMPLE_PIN);

#define REFRESH_DUTYCYCLE 80
#define REFRESH_TIME_uS 2000



void setup(){
  beginMenu();
	n.nixieBegin(REFRESH_TIME_uS,REFRESH_DUTYCYCLE);
  
 
}

void loop(){
  n.displayNixie(37, (millis()/1000)%60, 0);
  //n.displayNixie(rtc.hour(),rtc.minute(), 0);
  
  
}

