#include <Homie.h>

// LoLin NodeMCU pins (https://github.com/esp8266/Arduino/issues/584)
#define D0  16
#define D1  5
#define D2  4
#define D3  0
#define D4  2
#define D5  14
#define D6  12
#define D7  13
#define D8  15
#define D9  3
#define D10 1

#define PIN_LED_R D0
#define PIN_LED_G D1
#define PIN_LED_B D2
#define PIN_SWITCH D3

HomieNode buttonNode("button", "switch");
String buttonValue;

Bounce switchDebouncer = Bounce();
int lastSwitchValue = -1;

bool updateStatus(const String& value) {
    if (value != "on" && value != "off") {
        return false;
    }

    digitalWrite(PIN_LED_R, HIGH);

    if(value == "on") {
        digitalWrite(PIN_LED_G, LOW);
        digitalWrite(PIN_LED_B, HIGH);
    } else if (value == "off") {
        digitalWrite(PIN_LED_G, HIGH);
        digitalWrite(PIN_LED_B, LOW);
    }

    buttonValue = value;
    buttonNode.setProperty("status").send(value);

    Homie.getLogger() << "Button status is " << value << endl;

    return true;
}

bool buttonHandler(const HomieRange& range, const String& value) {
    return updateStatus(value);
}

void loopHandler() {
    int switchValue = switchDebouncer.read();

    if(switchValue != lastSwitchValue) {
        lastSwitchValue = switchValue;
        //Homie.getLogger() << "Switch is " << (switchValue ? "up" : "down") << endl;

        if(!switchValue) {
            updateStatus(buttonValue == "on" ? "off" : "on");
        }
    }
}

void setup() {
    //Homie.setStandalone();
    Serial.begin(9600);

    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);

    digitalWrite(PIN_LED_R, HIGH);
    digitalWrite(PIN_LED_G, HIGH);
    digitalWrite(PIN_LED_B, HIGH);

    pinMode(PIN_SWITCH, INPUT_PULLUP);
    switchDebouncer.attach(PIN_SWITCH);
    switchDebouncer.interval(50);

    Homie_setFirmware("magic-button", "1.0.0"); // The underscore is not a typo! See Magic bytes
    Homie.setLoopFunction(loopHandler);
    Homie.setLedPin(PIN_LED_R, LOW); // before Homie.setup() -- 2nd param is the state of the pin when the LED is o
    Homie.setup();

    buttonNode.advertise("status").settable(buttonHandler);
}

void loop() {
    Homie.loop();
    switchDebouncer.update();
}
