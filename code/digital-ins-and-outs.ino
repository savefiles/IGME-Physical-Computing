const int BUTTON_PIN = 2;
const int YELLOW_LED_PIN = 3;
const int RED_LED_PIN = 4;
const int REED_PIN = 5;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(REED_PIN, INPUT_PULLUP);  // Use built in resistor
}

void loop() {
   // read the pushbutton input:
   if (digitalRead(BUTTON_PIN) == HIGH) {
     // if the pushbutton is closed:
     digitalWrite(YELLOW_LED_PIN, HIGH);    // turn on the yellow LED
   }
   else {
     // if the switch is open:
     digitalWrite(YELLOW_LED_PIN, LOW);     // turn off the yellow LED
   }
   // read the reed input:
   if (digitalRead(REED_PIN) == HIGH) {   // if the magnet is not there
     digitalWrite(RED_LED_PIN, LOW);     // turn OFF the red LED
   }
   else {
     digitalWrite(RED_LED_PIN, HIGH);     // turn off the yellow LED
   }
}
