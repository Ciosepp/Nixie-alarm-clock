#ifndef nixieDisplay_h
#define nixieDisplay_h

#include <Arduino.h>

class nixieDisplay
{ 
    public:
        nixieDisplay(int clockPin,
                    int dataPin, 
                    int samplePin);
        ~nixieDisplay();

        void nixieBegin(long refreshTime,
                    int dutyCycle);
        void displayNixie(uint8_t a, uint8_t b ,uint8_t dot);

	 private:
        uint8_t CK_PIN,D_PIN, S_PIN;
        uint8_t currentDigit;
        bool flag;
       
};



#endif