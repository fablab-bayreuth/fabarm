#include "Servo.h"
#include "Arduino.h"

class ITSHServoMove {
  protected:
    int _start;
    int _current;
    int _target;
  public:
    ITSHServoMove(int start, int target) : _start(start), _target(target), _current(start) {}
    virtual ~ITSHServoMove() {}
    virtual int nextPosition() = 0;
    virtual boolean isDone();
};

class ITSHServoMoveLinear : public ITSHServoMove {
  public:
    ITSHServoMoveLinear(int start, int target) : ITSHServoMove(start, target) {}
    virtual ~ITSHServoMoveLinear() {}
    virtual int nextPosition();
};

class ITSHServoMoveEasing : public ITSHServoMoveLinear {
  protected:
    int _step;
    int sineEaseInOut(int t, int b, int c, int d);
  public:
    ITSHServoMoveEasing(int start, int target) : ITSHServoMoveLinear(start, target), _step(0) {}
    virtual ~ITSHServoMoveEasing() {}
    virtual int nextPosition(); 
};

class ITSHServo {
  private:
    Servo _servo;
    const int _min;
    const int _max;
    const int _home;
    const int _pin;

    int _currentPosition;
    ITSHServoMove* _moveStrategy;

    void updatePosition(int position);
  public:
    ITSHServo(int pin, int min, int max, int home) : _pin(pin), _min(min), _max(max), _home(home), _currentPosition(-1) {}
    ~ITSHServo();
    void moveTo(int end, boolean easing);
    void setup();

    /**
     * Wird in jeder Loop aufgerufen. Liefert true, wenn der Servo eine Bewegung durchgeführt hat.
     */
    boolean loop();
};

