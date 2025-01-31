#include "menu.h"
#include "Arduino.h"

#define PLUS_PIN 5
#define MINUS_PIN 6

#define DELAY_TIME 2000

void beginMenu(){
    pinMode(PLUS_PIN, INPUT_PULLUP);
    pinMode(MINUS_PIN, INPUT_PULLUP);
}

long timeBuffer=0;
bool busy = false;

bool getLongPress(bool button, bool presState, long delayTime)
{
    if (button == presState )   //pressed button
    {
        if (busy == false)
        {
            timeBuffer= millis() + delayTime; //start timer
            busy = true;        //just once
        }
        if(timeBuffer <= millis()){ // times up
            return true;
        }
    }
    else{
        busy = false;   //reset timer
    }
    return false;  
}

/*
state diagramm
default: showTime;
pressing "+"and "-" for more than 2s enters SETTING MODE;
    pressing "+"and "-" again for more than 2s leaves SETTING MODE;

pressing "+" or "-" circulates between :
    set time hours(dot off),set time minutes(dot off),
    set alarm hours(dot on),set alarm minutes(dot on),
    enable alarm(dot on);
    
    pressing "+"and "-" again for more than 2s leaves SETTING MODE;

pressing "+" or "-" for 1s enters setting of selected parameter;
pressing "+" or "-" set new value;
pressing "+"and "-" for 2s leaves setting and stores new value;

pressing "+" or "-" circulates between parameters of setting mode;
pressing "+"and "-" again for more than 2s leaves SETTING MODE;

*/
bool isTimeShowModeOn = true;
enum clockState {setTimeHH, setTimeMM, setAlarmHH, setAlarmMM, setAlarmEN};
enum timeSelection {hours, minutes};
enum clockState modeSelection = setTimeHH;
enum timeSelection timeSetting = hours;
uint8_t currentSetMinutes, currentSetHours;

void menu(){
    if (isTimeShowModeOn&& 
        getLongPress(digitalRead(PLUS_PIN)&&digitalRead(MINUS_PIN), 
                    false, DELAY_TIME))
    {

        busy = false;   //reset timer
    }
    
}