#include <Servo.h>
#include "ITSHServo.h"
#include "positions.h"

const int LED_PIN = 13;
const int BUTTON_PIN = 2;

const int POSITION_A_PIN = 0;
const int POSITION_B_PIN = 1;
const int POSITION_C_PIN = 2;

ITSHServo middle(7, 12, 160, 80);
ITSHServo claw(8, 0, 97, 97);
ITSHServo right(9, 45, 180, 90);
ITSHServo left(10, 30, 90, 30);

const boolean useEasing = true;

int _delay = 20;

const int* program;
int programStep;
int programLength;

const int TOTAL_PLACES = 3;
int places = 0;
int filledPlaces = 0;

boolean loaded = false;
int position = 0;

void setup()
{
  Serial.begin(19200);

  randomSeed(analogRead(4));

  pinMode(LED_PIN, OUTPUT);  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  claw.setup();
  middle.setup();
  right.setup();
  left.setup();
}

int target_value(int target, int min, int max) {
  if (target > max) {
    return max;
  } else if (target < min) {
    return min;
  } else {
    return target;
  }
}

void loop()
{
  boolean active = middle.loop() || left.loop() || right.loop() || claw.loop();

  if(!active && !program) {
    int button = digitalRead(BUTTON_PIN);
    if(button == LOW) {
      startNextProgram();
    }
  }

  if(!active && program) {
    if(programStep < programLength) {
      int servo = program[programStep++];
      int position = program[programStep++];

      switch(servo) {
        case middle_id:
          middle.moveTo(position, useEasing);
          break;
         case left_id:
         left.moveTo(position, useEasing);
         break;
         case right_id:
         right.moveTo(position, useEasing);
         break;
         case claw_id:
         claw.moveTo(position, useEasing);
         break;
      }
      Serial.print(servo);
      Serial.print(" ");
      Serial.println(position);
    }
    else {
      program = 0;
      programLength = 0;
      programStep = 0;
    }
  }
  
  digitalWrite(LED_PIN, active || program);
  
  delay(_delay);
}

void readFilledPlaces() {
  filledPlaces = 0;
  places = 0;
  if(analogRead(POSITION_A_PIN) < 50) {
    places |= 1;
    ++filledPlaces;
  }
  if(analogRead(POSITION_B_PIN) < 50) {
    places |= 2;
    ++filledPlaces;
  }
  if(analogRead(POSITION_C_PIN) < 50) {
    places |= 4;
    ++filledPlaces;
  }

  Serial.print("filledPlaces: ");
  Serial.print(filledPlaces);
  Serial.print(" places: ");
  Serial.println(places);
}

int randomLoadedPlace() {
  if(filledPlaces == 0) {
    return 0;
  }

  int selection = (random(0,1000) % (filledPlaces)) + 1;
  int currentItem = 0;
  
  for(int i = 0; i <= TOTAL_PLACES; i++) {
    if((places >> i & 1)) {
      if(++currentItem == selection) {
        return i + 1;
      }
    }
  }

  return 0;
}

int randomFreePlace() {
  if(TOTAL_PLACES <= filledPlaces) {
    return 0;
  }

  int selection = (random(0,1000) % (TOTAL_PLACES - filledPlaces)) + 1;
  int currentItem = 0;
  
  for(int i = 0; i <= TOTAL_PLACES; i++) {
    if((places >> i & 1) == 0) {
      if(++currentItem == selection) {
        return i + 1;
      }
    }
  }

  return 0;
}

void startNextProgram() {
  readFilledPlaces();
  
  if(loaded) {
    // TODO: Freie Position ermitteln!
    int position = randomFreePlace();
    
    // Suche zufällig eine neue Position raus!
    //position = (random(0,1000) % 3) + 1;
    Serial.print("Nächste Position: ");
    Serial.println(position);

    switch(position) {
      case 0:
        program = nothing_found[0];
        programLength = sizeof(nothing_found) / sizeof(*nothing_found) * 2;
        break;
      case 2:
        program = b_drop[0];
        programLength = sizeof(b_drop) / sizeof(*b_drop) * 2;
      break;
      case 1:
        program = a_drop[0];
        programLength = sizeof(a_drop) / sizeof(*a_drop) * 2;
      break;
      case 3:
        program = c_drop[0];
        programLength = sizeof(c_drop) / sizeof(*c_drop) * 2;
      break;
    }

    loaded = position < 1;
  } else {
    int position = randomLoadedPlace();

    Serial.print("Wuerfel gefunden auf Position: ");
    Serial.println(position);

    switch(position) {
      case 0:
        program = nothing_found[0];
        programLength = sizeof(nothing_found) / sizeof(*nothing_found) * 2;
        break;
      case 2:
        program = b_pickup[0];
        programLength = sizeof(b_pickup) / sizeof(*b_pickup) * 2;
      break;
      case 1:
        program = a_pickup[0];
        programLength = sizeof(a_pickup) / sizeof(*a_pickup) * 2;
      break;
      case 3:
        program = c_pickup[0];
        programLength = sizeof(c_pickup) / sizeof(*c_pickup) * 2;
      break;
    }

    loaded = position > 0;
  }

  programStep = 0;  
}


void serialEvent() {
  if (Serial.available() > 0) {
    String x = Serial.readString();
    int target = x.substring(1).toInt();
    Serial.println("--------");
    switch (x[0]) {
      case 'm':
        middle.moveTo(target, useEasing);
        Serial.println("--------");
        break;
      case 'l':
        left.moveTo(target, useEasing);
        break;
      case 'r':
        right.moveTo(target, useEasing);
        break;
      case 'c':
        claw.moveTo(target, useEasing);
        break;
      case 'd':
        _delay = target;
        break;
      case 'q':
        if(loaded) {
          program = a_drop[0];
          programLength = sizeof(a_drop) / sizeof(*a_drop) * 2;
          programStep = 0;
          loaded = false;
        } else {
          program = a_pickup[0];
          programLength = sizeof(a_pickup) / sizeof(*a_pickup) * 2;
          programStep = 0;
          loaded = true;
        }
        
        break;
      case 'w':
        if(loaded) {
          program = b_drop[0];
          programLength = sizeof(b_drop) / sizeof(*b_drop) * 2;
          programStep = 0;
          loaded = false;
        } else {
          program = b_pickup[0];
          programLength = sizeof(b_pickup) / sizeof(*b_pickup) * 2;
          programStep = 0;
          loaded = true;
        }
        
        break;
        
      case 'e':
        if(loaded) {
          program = c_drop[0];
          programLength = sizeof(c_drop) / sizeof(*c_drop) * 2;
          programStep = 0;
          loaded = false;
        } else {
          program = c_pickup[0];
          programLength = sizeof(c_pickup) / sizeof(*c_pickup) * 2;
          programStep = 0;
          loaded = true;
        }
        
        break;
      default:
        Serial.println("Verstehe ich nicht... Ich kann:");
        Serial.println("m(iddle), l(eft), r(ight), c(law) oder d(elay) gefolgt von einer Zahl :-)");
        Serial.println("q, w oder e, um eine der 3 Ablageschalen direkt anzusteuern.");
    }
  }
}

