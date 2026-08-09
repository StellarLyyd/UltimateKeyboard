#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>

constexpr uint8_t PIN_MP3_TX1 = 5;
constexpr uint8_t PIN_MP3_RX1 = 4;

constexpr uint8_t PIN_MP3_TX2 = 16; 
constexpr uint8_t PIN_MP3_RX2 = 15;

const uint8_t buttonPins[12] = {
  1, 2, 42, 41, 40, 39, 38, 47, 9, 10, 11, 12
};

constexpr uint8_t SLIDER_PIN = 6;

HardwareSerial dfSerial1(1);
HardwareSerial dfSerial2(2);

DFRobotDFPlayerMini player1;
DFRobotDFPlayerMini player2;

bool player1Ready = false;
bool player2Ready = false;

// 0 means that player is free
int player1Track = 0;
int player2Track = 0;

// Previous button states
bool previousButtonState[12] = {
  LOW, LOW, LOW, LOW,
  LOW, LOW, LOW, LOW,
  LOW, LOW, LOW, LOW
};


int readVolume() {
  int raw = analogRead(SLIDER_PIN);

  int volume = map(raw, 0, 4095, 5, 25);

  return constrain(volume, 5, 25);
}

void startNote(int noteNumber) {
  int volume = readVolume();

  if (player1Track == noteNumber ||
      player2Track == noteNumber) {
    return;
  }

  if (player1Ready && player1Track == 0) {
    player1.volume(volume);
    delay(50);

    player1.play(noteNumber);

    player1Track = noteNumber;

    Serial.print("Player 1 playing track ");
    Serial.print(noteNumber);
    Serial.print(" at volume ");
    Serial.println(volume);

    return;
  }

  if (player2Ready && player2Track == 0) {
    player2.volume(volume);
    delay(50);

    player2.play(noteNumber);

    player2Track = noteNumber;

    Serial.print("Player 2 playing track ");
    Serial.print(noteNumber);
    Serial.print(" at volume ");
    Serial.println(volume);

    return;
  }

  Serial.print("Both players busy. Ignoring track ");
  Serial.println(noteNumber);
}

void stopNote(int noteNumber) {
  if (player1Track == noteNumber) {
    player1.stop();
    player1Track = 0;

    Serial.print("Player 1 stopped track ");
    Serial.println(noteNumber);
  }

  if (player2Track == noteNumber) {
    player2.stop();
    player2Track = 0;

    Serial.print("Player 2 stopped track ");
    Serial.println(noteNumber);
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("Starting two-DFPlayer keyboard...");

  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT_PULLDOWN);
    previousButtonState[i] = digitalRead(buttonPins[i]);
  }

  pinMode(SLIDER_PIN, INPUT);
  analogReadResolution(12);

  dfSerial1.begin(
    9600,
    SERIAL_8N1,
    PIN_MP3_RX1,
    PIN_MP3_TX1
  );

  dfSerial2.begin(
    9600,
    SERIAL_8N1,
    PIN_MP3_RX2,
    PIN_MP3_TX2
  );

  delay(3000);

  Serial.println("Starting DFPlayer 1...");

  player1Ready = player1.begin(
    dfSerial1,
    false, 
    false 
  );

  if (player1Ready) {
    Serial.println("DFPlayer 1 ready.");
    player1.volume(10);
  } else {
    Serial.println("DFPlayer 1 failed.");
  }

  delay(1000);

  Serial.println("Starting DFPlayer 2...");

  player2Ready = player2.begin(
    dfSerial2,
    false,
    false
  );

  if (player2Ready) {
    Serial.println("DFPlayer 2 ready.");
    player2.volume(10);
  } else {
    Serial.println("DFPlayer 2 failed.");
  }

  delay(500);

  Serial.println();
  Serial.print("Player 1: ");
  Serial.println(player1Ready ? "ready" : "failed");

  Serial.print("Player 2: ");
  Serial.println(player2Ready ? "ready" : "failed");

  Serial.println("Keyboard ready.");
}

void loop() {
  for (int i = 0; i < 12; i++) {
    bool currentState = digitalRead(buttonPins[i]);
    int noteNumber = i + 1;

    if (currentState == HIGH &&
        previousButtonState[i] == LOW) {
      delay(20);

      currentState = digitalRead(buttonPins[i]);

      if (currentState == HIGH) {
        startNote(noteNumber);
      }
    }

    if (currentState == LOW &&
        previousButtonState[i] == HIGH) {
      delay(20);

      currentState = digitalRead(buttonPins[i]);

      if (currentState == LOW) {
        stopNote(noteNumber);
      }
    }

    previousButtonState[i] = currentState;
  }

  delay(5);
}
