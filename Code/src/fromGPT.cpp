#include <Wire.h>
#include <RTClib.h>

#define BTN_PLUS  2   // Pin per il pulsante +
#define BTN_MINUS 3   // Pin per il pulsante -

RTC_DS1307 rtc;

// Definizione stati del menu
enum MenuState { SHOW_TIME, CHOOSE_PARAMETER, SET_PARAMETER };
MenuState menuState = SHOW_TIME;

// Definizione parametri modificabili
enum Parameter { SET_HH, SET_MM, SET_ALARM_HH, SET_ALARM_MM, ENABLE_ALARM };
Parameter currentParameter = SET_HH;

// Variabili di gestione pulsanti
unsigned long pressStartTime = 0;
bool btnPlusState = false, btnMinusState = false;
bool btnPlusLongPress = false, btnMinusLongPress = false;

// Variabili per i parametri dell'orologio
int hour, minute, alarmHour, alarmMinute;
bool alarmEnabled = false;

// Tempo di pressione prolungata
const unsigned long LONG_PRESS_TIME = 2000;

void setup() {
    pinMode(BTN_PLUS, INPUT_PULLUP);
    pinMode(BTN_MINUS, INPUT_PULLUP);
    Serial.begin(9600);

    if (!rtc.begin()) {
        Serial.println("RTC non trovato!");
        while (1);
    }

    // Legge l'orario dall'RTC
    DateTime now = rtc.now();
    hour = now.hour();
    minute = now.minute();
}

void loop() {
    handleButtons();  // Gestisce la lettura dei pulsanti

    switch (menuState) {
        case SHOW_TIME:
            showTime();
            if (btnPlusLongPress && btnMinusLongPress) {
                menuState = CHOOSE_PARAMETER;
                resetButtonStates();
            }
            break;

        case CHOOSE_PARAMETER:
            showParameters();
            if (btnPlusState) changeParameter(1);
            if (btnMinusState) changeParameter(-1);
            if (btnPlusLongPress && btnMinusLongPress) {
                menuState = SHOW_TIME;
                resetButtonStates();
            }
            if (btnPlusLongPress || btnMinusLongPress) {
                menuState = SET_PARAMETER;
                resetButtonStates();
            }
            break;

        case SET_PARAMETER:
            modifyParameter();
            if (btnPlusLongPress || btnMinusLongPress) {
                saveToRTC();
                menuState = CHOOSE_PARAMETER;
                resetButtonStates();
            }
            break;
    }
}

// Funzione per gestire i pulsanti con millis()
void handleButtons() {
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck < 50) return;  // Debounce
    lastCheck = millis();

    bool btnPlus = digitalRead(BTN_PLUS) == LOW;
    bool btnMinus = digitalRead(BTN_MINUS) == LOW;

    // Gestione pressione pulsante +
    if (btnPlus && !btnPlusState) pressStartTime = millis();
    if (!btnPlus && btnPlusState) btnPlusLongPress = false;
    if (btnPlus && millis() - pressStartTime > LONG_PRESS_TIME) btnPlusLongPress = true;

    // Gestione pressione pulsante -
    if (btnMinus && !btnMinusState) pressStartTime = millis();
    if (!btnMinus && btnMinusState) btnMinusLongPress = false;
    if (btnMinus && millis() - pressStartTime > LONG_PRESS_TIME) btnMinusLongPress = true;

    btnPlusState = btnPlus;
    btnMinusState = btnMinus;
}

void resetButtonStates() {
    btnPlusState = btnMinusState = false;
    btnPlusLongPress = btnMinusLongPress = false;
}

void showTime() {
    Serial.print("Orario: ");
    Serial.print(hour);
    Serial.print(":");
    Serial.println(minute);
}

void showParameters() {
    const char* paramNames[] = { "setHH", "setMM", "setAlarmHH", "setAlarmMM", "enableAlarm" };
    Serial.print("Selezionato: ");
    Serial.println(paramNames[currentParameter]);
}

void changeParameter(int dir) {
    currentParameter = static_cast<Parameter>((currentParameter + dir + 5) % 5);
}

void modifyParameter() {
    Serial.print("Modifica: ");
    switch (currentParameter) {
        case SET_HH: Serial.print("Ore "); hour += (btnPlusState ? 1 : -1); break;
        case SET_MM: Serial.print("Minuti "); minute += (btnPlusState ? 1 : -1); break;
        case SET_ALARM_HH: Serial.print("Ore Allarme "); alarmHour += (btnPlusState ? 1 : -1); break;
        case SET_ALARM_MM: Serial.print("Minuti Allarme "); alarmMinute += (btnPlusState ? 1 : -1); break;
        case ENABLE_ALARM: Serial.print("Abilita Allarme "); alarmEnabled = !alarmEnabled; break;
    }
    Serial.println();
    delay(200);  // Per evitare cambi troppo rapidi
}

void saveToRTC() {
    rtc.adjust(DateTime(2024, 1, 1, hour, minute, 0));
    Serial.println("Dati salvati su RTC");
}
