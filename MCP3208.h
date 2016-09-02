
#include "Arduino.h"

// ---------------------------------------- //
// ---------------------------------------- //
// --- 8CH 12BIT SPI ADC CONTROL CLASS ---- //
// ---------------------------------------- //
// ---------------------------------------- //

class MCP3208 {
  
  static MCP3208 *s_instance;

  int _busy;

  int _mcpCLK;
  int _mcpDIN;
  int _mcpDOUT;
  int _mcpCS;

  int _cycleState;
  int _stepState;

  int _selectedADC;

  int _inValues[8] = {0,0,0,0,0,0,0,0};

  /*
  HOW TO USE: MCP3208
  
  CYCLE:
  A. CLOCK DOWN
     SET DATA PINS (PERFORM STEP)
     RESECHEDULE SELF NOW+500ns
  B. CLOCK UP
     RESECHEDULE SELF NOW+500ns
  
  STEPS: (sampling channel 0)
  00. CS INIT HI  - CHIP DISABLE
  01. CS LOW      - CHIP ENABLE
  02. DIN HIGH    - SINGLE (NOT DIFF)
  03. DIN LOW     - CHANNEL SELECT BIT 02 (MSB)
  04. DIN LOW     - CHANNEL SELECT BIT 01
  05. DIN LOW     - CHANNEL SELECT BIT 00 (LSB)
  06. NOTHING     - SAMPLING CYCLE
  07. DOUT NULL   - NULL BIT
  08. DOUT READ   - BIT 11
  09. DOUT READ   - BIT 10
  10. DOUT READ   - BIT 09
  xx. .... ....   - ... ..
  19. DOUT READ   - BIT 00
  20. CS HIGH     - CHIP DISABLE
      RESET STATE - RESET STATE  
  */

  // returns 1 if still running
  int _performStep() {
    switch(_stepState) {
      case 0:   digitalWrite(_mcpCS,  HIGH);                                          break;
      case 1:   digitalWrite(_mcpCS,  LOW);                                           break;
      case 2:   digitalWrite(_mcpDIN, HIGH);                                          break;
      case 3:   digitalWrite(_mcpDIN, _selectedADC & bit(2));                         break;
      case 4:   digitalWrite(_mcpDIN, _selectedADC & bit(1));                         break;
      case 5:   digitalWrite(_mcpDIN, _selectedADC & bit(0));                         break;
      case 6:                                                                         break;
      case 7:                                                                         break;
      case 8:   _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 11;   break;
      case 9:   _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 10;   break;
      case 10:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 9;    break;
      case 11:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 8;    break;
      case 12:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 7;    break;
      case 13:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 6;    break;
      case 14:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 5;    break;
      case 15:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 4;    break;
      case 16:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 3;    break;
      case 17:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 2;    break;
      case 18:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 1;    break;
      case 19:  _inValues[_selectedADC] |= (digitalRead(_mcpDOUT)==HIGH?1:0) << 0;    break;
      case 20:  digitalWrite(_mcpCS, HIGH);                                           break;
      case 21:  _busy = 0; return 2;
    }
    _stepState++;
    return 1;
  }

  // returns 1 if still running
  int _performCycle() {
    switch(_cycleState) {
      case 0:
        digitalWrite(_mcpCLK, LOW);
        _cycleState = _performStep();;
      break;
      case 1:
        digitalWrite(_mcpCLK, HIGH);
        _cycleState = 0;
      break;
      default:
        return 0;
      break;
    }
    return 1;
  }

  public:
  
  MCP3208(int mcpClock, int mcpDataIn, int mcpDataOut, int mcpChipSelect) {
      _busy = 0;
    
      _cycleState = 2;
      _stepState = 21;
      
      _mcpCLK   = mcpClock;
      _mcpDIN   = mcpDataIn;
      _mcpDOUT  = mcpDataOut;
      _mcpCS    = mcpChipSelect;

      _selectedADC = 0;
      
      pinMode(_mcpCLK, OUTPUT);
      pinMode(_mcpDIN, OUTPUT);
      pinMode(_mcpDOUT, INPUT);
      pinMode(_mcpCS, OUTPUT);

      digitalWrite(_mcpCLK, LOW);
      digitalWrite(_mcpDIN, LOW);
      digitalWrite(_mcpCS, HIGH);
  }

  // returns 1 if running, 0 if idle/finished
  int CommunicationsLoop() {
    return _performCycle();
  }

  // reset the state machine to start an analog read operation
  void StartAnalogRead(int analogIn) {
    _busy = 1;
    _selectedADC = analogIn;
    _inValues[analogIn] = 0;
    _stepState = 0;
    _cycleState = 0;
  }

  int IsBusy() {
    return _busy;  
  }

  // get the value stored for ADC -- only call if CommunicationsLoop returns 0 or not busy
  int GetValue(int analogIn) {
    return _inValues[analogIn];
  }
};

