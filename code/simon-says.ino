// CONSTANTS FOR PINS
const int RedLEDPin = 8;
const int YellowLEDPin = 5;
const int BlueLEDPin = 3;

const int ReedPin = 21;
const int But1Pin = 19;
const int But2Pin = 20;

const int LED_PINS[] = {BlueLEDPin, RedLEDPin, YellowLEDPin};  // The order of the pins.
const int INPUT_PINS[] = {ReedPin, But1Pin, But2Pin};         // The order of the inputs.


// GLOBAL VARIABLES
// For the pattern, we define 1 as red, 2 as yellow, 3 as blue.
const int MAX_PATTERN_LENGTH = 20;
int currentPattern[MAX_PATTERN_LENGTH]; // Go up to a max of seven.
int patternLength = 3;

// Variables relating to user input and if the game is going.
int currentIndex = 0;
bool inputPhase = true;
bool isAlive = true;


// FORWARD DECLARATIONS
void WaitForFirstInput();
void LightUpLED(int inputPin, int outputPin, bool reverse = false);     // Light up the corresponding button pushed.
void GeneratePattern();                                                 // Generate a pattern of lights based on the var patternLength.
void PlayPattern();                                                     // Flash LEDs based on the pattern var currentPattern.
void RequestInput();                                                    // Handle user input.

void FlashCorrect();    // If they succeeded with their memory.
void FlashIncorrect();  // If they failed.


// CODE
void setup() {  
  pinMode(RedLEDPin, OUTPUT);
  pinMode(YellowLEDPin, OUTPUT);
  pinMode(BlueLEDPin, OUTPUT);

  pinMode(ReedPin, INPUT_PULLUP);
  pinMode(But1Pin, INPUT_PULLUP);
  pinMode(But2Pin, INPUT_PULLUP);

  WaitForFirstInput();
  
  // Seed the random numbers.
  randomSeed(analogRead(0));

  GeneratePattern();
  PlayPattern();
}



void loop() {
  if(inputPhase == true) {
    RequestInput();
  }
  else {
    if(isAlive == false) {  // They died.
      FlashIncorrect();
      while(1);             // Stop execution.
    }
    else {                  // They are alive.
      FlashCorrect();
      patternLength += 1;
      GeneratePattern();
      PlayPattern();
      currentIndex = 0;
      inputPhase = true;
    }
  }
}

// Keep all three lights on until first input.
void WaitForFirstInput() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PINS[i], HIGH);  
  }
  while(true) {
    for(int i = 0; i < 3; i++) {
      if(digitalRead(INPUT_PINS[i]) == LOW) { 
        for(int i = 0; i < 3; i++) {
          digitalWrite(LED_PINS[i], LOW);  
        }
        delay(1000);
        return;
      }
    }
  }
}


void GeneratePattern() {
  if(patternLength > MAX_PATTERN_LENGTH) patternLength = MAX_PATTERN_LENGTH;
  for (int i = 0; i < patternLength; i++) {
    currentPattern[i] = random(3);
  }
}


void PlayPattern() {
  for (int i = 0; i < patternLength; i++) {
    int pinToWrite = LED_PINS[currentPattern[i]];
    digitalWrite(pinToWrite, HIGH);
    delay(1000);
    
    digitalWrite(pinToWrite, LOW);
    delay(600);
  }
}


void RequestInput() {
  // Check each input. If there is an input, determine if it's correct.
  for(int i = 0; i < 3; i++) {
    if(digitalRead(INPUT_PINS[i]) == LOW) {     // They are all INPUT_PULLUP, so LOW means input.
      digitalWrite(LED_PINS[i], HIGH);
      delay(750);
      digitalWrite(LED_PINS[i], LOW);
      
      if(currentPattern[currentIndex] != i) {   // If the input was wrong
        inputPhase = false;
        isAlive = false;
      }
      else {
        currentIndex++;
        if(currentIndex >= patternLength) {      // Great success!
           inputPhase = false;
           isAlive = true;
        }
      }

      return;                                   // Only allow one input per cycle.
    }
  }
  
  
  delay(10);
}

void FlashCorrect() {
  // Blink the LEDs three times.
  for(int i = 0; i < 3; i++) {
    digitalWrite(BlueLEDPin, HIGH);
    digitalWrite(RedLEDPin, HIGH);
    digitalWrite(YellowLEDPin, HIGH);
    delay(750); 

    digitalWrite(BlueLEDPin, LOW);
    digitalWrite(RedLEDPin, LOW);
    digitalWrite(YellowLEDPin, LOW);
    delay(500); 
  }
}

void FlashIncorrect() {
  // Turn all on, turn off one at a time.
  digitalWrite(BlueLEDPin, HIGH);
  digitalWrite(RedLEDPin, HIGH);
  digitalWrite(YellowLEDPin, HIGH);
  delay(750); 

  digitalWrite(BlueLEDPin, LOW);
  delay(1000); 
  
  digitalWrite(RedLEDPin, LOW);
  delay(1000); 
  
  digitalWrite(YellowLEDPin, LOW);
  delay(1000); 
}

// Give a input pin and output pin. Reverse determines if high -> high or high -> low
void LightUpLED(int inputPin, int outputPin, bool reverse) {
  if(digitalRead(inputPin) == HIGH) {
    digitalWrite(outputPin, !reverse);
  }
  else {
    digitalWrite(outputPin, reverse);
  }
}
