/**
 * Title: ControlsTest
 * Author: D Cooper Dalrymple
 * Created: 11/10/2020
 * Updated: 11/10/2020
 * Description: Simple controls test with serial data output for Mozzi Mono Synth.
 * https://dcooperdalrymple.com/
 */

#define LED_1 11
#define LED_2 6

#define BUTTON_1 12
#define BUTTON_2 8

#define KNOB_1 A5
#define KNOB_2 A4
#define KNOB_3 A0
#define KNOB_4 A1
#define KNOB_5 A2
#define KNOB_6 A3

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  pinMode(KNOB_1, INPUT);
  pinMode(KNOB_2, INPUT);
  pinMode(KNOB_3, INPUT);
  pinMode(KNOB_4, INPUT);
  pinMode(KNOB_5, INPUT);
  pinMode(KNOB_6, INPUT);
}

byte ledState = true;
void loop() {
  if (!!ledState) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
  } else {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
  }
  ledState = !ledState;
  
  Serial.print("Button 1: ");
  if (!!digitalRead(BUTTON_1)) {
    Serial.println("Off");
  } else {
    Serial.println("On");
  }
  Serial.print("Button 2: ");
  if (!!digitalRead(BUTTON_2)) {
    Serial.println("Off");
  } else {
    Serial.println("On");
  }

  Serial.print("Knob 1: ");
  Serial.println(analogRead(KNOB_1));
  Serial.print("Knob 2: ");
  Serial.println(analogRead(KNOB_2));
  Serial.print("Knob 3: ");
  Serial.println(analogRead(KNOB_3));
  Serial.print("Knob 4: ");
  Serial.println(analogRead(KNOB_4));
  Serial.print("Knob 5: ");
  Serial.println(analogRead(KNOB_5));
  Serial.print("Knob 6: ");
  Serial.println(analogRead(KNOB_6));
  
  delay(1000);
}
