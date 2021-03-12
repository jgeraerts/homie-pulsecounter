#include <Homie.h>
#include <Bounce2.h>
#include <Arduino.h>
#include <stdint.h>

#include <Homie.h>

const int PIN_BUTTON = 5; // D1
const int SEND_INTERVAL = 30000;

Bounce b = Bounce();

uint32 pulses = 0;
uint32 lastSent = 0;

int buttonState = 0;
bool buttonPressed = false;

HomieNode pulseNode("pulses", "Pulse Counter", "pulsecounter");

void loopHandler() {
  if (b.fell()) {
    pulses++;
  }
  if (millis() - lastSent > SEND_INTERVAL) {
    pulseNode.setProperty("pulses").send(String(pulses));
    lastSent = millis();
  }
}

void setup() {
  lastSent = millis();
  b.attach(PIN_BUTTON,INPUT_PULLUP);
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("pulsecounter", "1.0.0");
  Homie.setLoopFunction(loopHandler);


  pulseNode.advertise("pulses").setName("pulses")
                                .setDatatype("integer")
                                .setRetained(false);

  Homie.setup();
}

void loop() {
  Homie.loop();
  b.update();
}

