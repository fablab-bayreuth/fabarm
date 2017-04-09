#include "ITSHServo.h"
#include "math.h"

ITSHServo::~ITSHServo() {
  if(_moveStrategy) {
    delete _moveStrategy;
  }
}

void ITSHServo::setup() {
  _servo.attach(_pin);
}

void ITSHServo::moveTo(int end, boolean easing) {
  if(end < _min) {
    end = _min;
  }

  if(end > _max) {
    end = _max;
  }

  if(end == _currentPosition) {
    return;
  }

  if(_moveStrategy) {
    delete _moveStrategy;
  }
  
  if(easing) {
    _moveStrategy = new ITSHServoMoveEasing(_currentPosition, end);
  } else {
    _moveStrategy = new ITSHServoMoveLinear(_currentPosition, end);
  }
}

void ITSHServo::updatePosition(int position) {
  _currentPosition = position;
  _servo.write(_currentPosition);
}

boolean ITSHServo::loop() {
  if(_currentPosition == -1) {
    updatePosition(_home);
    return true;
  }
  
  if(_moveStrategy && !_moveStrategy->isDone()) {
    updatePosition(_moveStrategy->nextPosition());
    return true;
  }

  return false;
}

boolean ITSHServoMove::isDone() {
  return _target == _current;
}

int ITSHServoMoveLinear::nextPosition() {
  if (_target > _start) {
    _current++;
  } else {
    _current--;
  }

  return _current;
}

int ITSHServoMoveEasing::nextPosition() {
  int relativeChange = _target - _start;
  int totalTime = floor(abs(relativeChange) / 2);

  _current = ceil(sineEaseInOut(_step, _start, relativeChange, totalTime));

  if (abs(_target - _current) <= 1) {
    _current = _target;
  }
  _step++;
    
  return _current;
}

int ITSHServoMoveEasing::sineEaseInOut(int t, int b, int c, int d)
{
  if (d == 0) {
    return b + c;
  }

  return -c / 2 * (cos(M_PI * t / d) - 1) + b;
}
