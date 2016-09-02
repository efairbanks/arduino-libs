
#include "Arduino.h"

// ---------------------------------------- //
// ---------------------------------------- //
// --- 2CH 16BIT SPI DAC CONTROL CLASS ---- //
// ---------------------------------------- //
// ---------------------------------------- //

class TDA1543 {
  
  static TDA1543 *s_instance;

  int _busy;

  int _tdaBCK;
  int _tdaDATA;
  int _tdaWS;

  int _cycleState;
  int _stepState;

  int _leftChannelValue;
  int _rightChannelValue;

  // returns 1 if still running
  int _performStep() {
    switch(_stepState) {
      case 0:   digitalWrite(_tdaDATA, _leftChannelValue&bit(16)?HIGH:LOW);     break;
      case 1:   digitalWrite(_tdaDATA, _leftChannelValue&bit(15)?HIGH:LOW);     break;
      case 2:   digitalWrite(_tdaDATA, _leftChannelValue&bit(14)?HIGH:LOW);     break;
      case 3:   digitalWrite(_tdaDATA, _leftChannelValue&bit(13)?HIGH:LOW);     break;
      case 4:   digitalWrite(_tdaDATA, _leftChannelValue&bit(12)?HIGH:LOW);     break;
      case 5:   digitalWrite(_tdaDATA, _leftChannelValue&bit(11)?HIGH:LOW);     break;
      case 6:   digitalWrite(_tdaDATA, _leftChannelValue&bit(10)?HIGH:LOW);     break;
      case 7:   digitalWrite(_tdaDATA, _leftChannelValue&bit(9)?HIGH:LOW);      break;
      case 8:   digitalWrite(_tdaDATA, _leftChannelValue&bit(8)?HIGH:LOW);      break;
      case 9:   digitalWrite(_tdaDATA, _leftChannelValue&bit(7)?HIGH:LOW);      break;
      case 10:  digitalWrite(_tdaDATA, _leftChannelValue&bit(6)?HIGH:LOW);      break;
      case 11:  digitalWrite(_tdaDATA, _leftChannelValue&bit(5)?HIGH:LOW);      break;
      case 12:  digitalWrite(_tdaDATA, _leftChannelValue&bit(4)?HIGH:LOW);      break;
      case 13:  digitalWrite(_tdaDATA, _leftChannelValue&bit(3)?HIGH:LOW);      break;
      case 14:  digitalWrite(_tdaDATA, _leftChannelValue&bit(2)?HIGH:LOW);      break;
      case 15:  digitalWrite(_tdaDATA, _leftChannelValue&bit(1)?HIGH:LOW);
                digitalWrite(_tdaWS, HIGH);                                     break;               
      case 16:  digitalWrite(_tdaDATA, _rightChannelValue&bit(16)?HIGH:LOW);    break;
      case 17:  digitalWrite(_tdaDATA, _rightChannelValue&bit(15)?HIGH:LOW);    break;
      case 18:  digitalWrite(_tdaDATA, _rightChannelValue&bit(14)?HIGH:LOW);    break;
      case 19:  digitalWrite(_tdaDATA, _rightChannelValue&bit(13)?HIGH:LOW);    break;
      case 20:  digitalWrite(_tdaDATA, _rightChannelValue&bit(12)?HIGH:LOW);    break;
      case 21:  digitalWrite(_tdaDATA, _rightChannelValue&bit(11)?HIGH:LOW);    break;
      case 22:  digitalWrite(_tdaDATA, _rightChannelValue&bit(10)?HIGH:LOW);    break;
      case 23:  digitalWrite(_tdaDATA, _rightChannelValue&bit(9)?HIGH:LOW);     break;
      case 24:  digitalWrite(_tdaDATA, _rightChannelValue&bit(8)?HIGH:LOW);     break;
      case 25:  digitalWrite(_tdaDATA, _rightChannelValue&bit(7)?HIGH:LOW);     break;
      case 26:  digitalWrite(_tdaDATA, _rightChannelValue&bit(6)?HIGH:LOW);     break;
      case 27:  digitalWrite(_tdaDATA, _rightChannelValue&bit(5)?HIGH:LOW);     break;
      case 28:  digitalWrite(_tdaDATA, _rightChannelValue&bit(4)?HIGH:LOW);     break;
      case 29:  digitalWrite(_tdaDATA, _rightChannelValue&bit(3)?HIGH:LOW);     break;
      case 30:  digitalWrite(_tdaDATA, _rightChannelValue&bit(2)?HIGH:LOW);     break;
      case 31:  digitalWrite(_tdaDATA, _rightChannelValue&bit(1)?HIGH:LOW);
                digitalWrite(_tdaWS, LOW);                                      break;
      default:  _busy = 0; return 2;
    }
    _stepState++;
    return 1;
  }

  // returns 1 if still running
  int _performCycle() {
    switch(_cycleState) {
      case 0:
        digitalWrite(_tdaBCK, LOW);
        _cycleState = _performStep();;
      break;
      case 1:
        digitalWrite(_tdaBCK, HIGH);
        _cycleState = 0;
      break;
      default:
        return 0;
      break;
    }
    return 1;
  }

  public:
  
  TDA1543(int tdaClock, int tdaData, int tdaWordSelect) {
      _busy = 0;
    
      _cycleState = 2;
      _stepState = 100;
      
      _tdaBCK = tdaClock;
      _tdaDATA = tdaData;
      _tdaWS = tdaWordSelect;
      
      pinMode(_tdaBCK, OUTPUT);
      pinMode(_tdaDATA, OUTPUT);
      pinMode(_tdaWS, OUTPUT);

      digitalWrite(_tdaBCK, LOW);
      digitalWrite(_tdaDATA, LOW);
      digitalWrite(_tdaWS, LOW);
  }

  // returns 1 if running, 0 if idle/finished
  int CommunicationsLoop() {
    return _performCycle();
  }

  // reset the state machine to start an analog read operation
  void StartAnalogWrite(int leftChannelValue, int rightChannelValue) {
    _busy = 1;
    _leftChannelValue = leftChannelValue;
    _rightChannelValue = rightChannelValue;
    _stepState = 0;
    _cycleState = 0;
  }

  int IsBusy() {
    return _busy;  
  }
};

