const int sleepTime = 1;
const int numLeds = 7;
//const int ledDelay = 80;
const int resultBlinkDelay = 300;
const int ledPins[numLeds] = {12, 11, 10, 9, 8, 7, 6};
int ledDelays[numLeds] = {60, 60, 60, 10, 60, 60, 60};
const int BTN_PIN = 13;

bool buttonState = LOW;
bool lastButtonState = LOW;
bool buttonPulse = LOW;

bool resultBlinkState = HIGH;

int currentIndex = 0;

enum State {
  SPIN,
  RESULT,  
} state = SPIN;

enum Direction {
  NORMAL,
  REVERSE,  
} dir = NORMAL;

class Timer {
  public:
  int state, goal;
  Timer(int goalNum) {
    goal = goalNum;  
    state = 0;
  }

  void tick() {
    state += sleepTime;
    if (state > goal) {
      state = 0;  
    } 
  }

  bool done() {
    return state == goal;
  }
} spinTimer(80), resultTimer(resultBlinkDelay);

void setup() {
  // put your setup code here, to run once
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);  
  }
}

void loop() {
  // Update timers
  spinTimer.tick();
  resultTimer.tick();
  
  // Get Button State
  lastButtonState = buttonState;
  buttonState = digitalRead(BTN_PIN);
  buttonPulse = (buttonState && !lastButtonState);

  if (buttonPulse) {
    if (state == State::SPIN) {
        state = State::RESULT;
      } else {
        currentIndex = 0;
        dir = Direction::NORMAL;
        state = State::SPIN;
        for (int i = 0; i < numLeds; i++) {
          digitalWrite(ledPins[i], LOW);  
        }
      }
  }
  
  if (state == State::SPIN && spinTimer.done()) {
      // Turn off last index
      digitalWrite(ledPins[currentIndex], LOW);

      // Update index
      if (dir == Direction::NORMAL) {
        if (currentIndex != numLeds - 1) {
          currentIndex += 1;  
        } else {
          currentIndex -= 0;
          dir = Direction::REVERSE;  
        }
      }

      if (dir == Direction::REVERSE) {
        if (currentIndex != 0) {
          currentIndex -= 1;  
        } else {
          currentIndex += 1;
          dir = Direction::NORMAL;  
        }
      }

      // Set new timer goal
      spinTimer.goal = ledDelays[currentIndex];
      
      digitalWrite(ledPins[currentIndex], HIGH);
  } else if (state == State::RESULT && resultTimer.done()) {
    
      resultBlinkState = !resultBlinkState;
      digitalWrite(ledPins[currentIndex], resultBlinkState);
  }
  delay(sleepTime);
 
}
