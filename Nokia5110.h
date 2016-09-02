
#include "Arduino.h"

// ----------------------------- //
// ----------------------------- //
// --- DRIVER FOR NOKIA 5110 --- //
// ----------------------------- //
// ----------------------------- //

/*
This Code has extra features 
including a XY positioning function on Display
and a Line Draw function on Nokia 3310 LCD 
It is modded from the original 
http://playground.arduino.cc/Code/PCD8544
*/
// Mods by Jim Park 
// jim(^dOt^)buzz(^aT^)gmail(^dOt^)com
// hope it works for you
#define PIN_SCE_PIN   7  // LCD CS  .... Pin 3
#define PIN_RESET_PIN 6  // LCD RST .... Pin 1
#define PIN_DC_PIN    5  // LCD Dat/Com. Pin 5
#define PIN_SDIN_PIN  4  // LCD SPIDat . Pin 6
#define PIN_SCLK_PIN  3  // LCD SPIClk . Pin 4
                     // LCD Gnd .... Pin 2
                     // LCD Vcc .... Pin 8
                     // LCD Vlcd ... Pin 7

#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     48
#define LCD_CMD   0

int a = 0;

static const byte ASCII[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f →
};

class Nokia5110 {
  
  private:
  
  int _clkState = 2;
  int _shiftState = 8;

  int _shiftOutBusy = 0;
  int _busy = 0;

  int _SCE_PIN    = 7;
  int _RESET_PIN  = 6;
  int _DC_PIN     = 5;
  int _SDIN_PIN   = 4;
  int _SCLK_PIN   = 3;

  int _CMD_BUF[LCD_X*LCD_Y];
  int _CMD_BUF_MAX_LENGTH = LCD_X*LCD_Y;
  
  int _CMD_LEN   = 0;
  int _CMD_INDEX = 0;

  void _startShiftOutCommand() {
    _shiftState = 0;
    _clkState = 0;
    _shiftOutBusy = 1;
    LcdInitialise();
  }

  // returns 1 if running, 0 if finished
  int _shiftOutCommandLoop(uint8_t dc, uint8_t data)
  {
    if(_clkState==0&&_shiftState==0) {
      digitalWrite(_DC_PIN, dc);
      digitalWrite(_SCE_PIN, LOW);
    }
    
    switch(_clkState) {  
      case 0:
        digitalWrite(_SCLK_PIN, LOW);
        digitalWrite(_SDIN_PIN, !!(data & (1 << (7 - _shiftState))));
        _shiftState++;
        _clkState = 1;
        return 1;
      break;
      case 1:
        digitalWrite(_SCLK_PIN, HIGH);
        if(_shiftState>7) {
          _clkState = 2;
          _shiftOutBusy = 0;
          return 0;
        }
        _clkState = 0;
        return 1;
      break;
      case 2:
        return 0;
      break;
    }
  }

  public:
  
  // returns 1 if running, 0 if finished
  int CommunicationsLoop() {
    if(_CMD_INDEX>=0 && _CMD_INDEX<_CMD_LEN) {
      _shiftOutCommandLoop( !!(_CMD_BUF[_CMD_INDEX]&bit(9)),
                            _CMD_BUF[_CMD_INDEX]&0xFF);
    }
    if(!_shiftOutBusy) _CMD_INDEX++;
    if(_CMD_INDEX >= _CMD_LEN) {
      _CMD_INDEX = -1;
      _CMD_LEN = 0;
      _busy = 0;
    }
    return _CMD_INDEX==-1?0:1;
  }

  // returns 1 if running, 0 if finished
  int QueueLcdCommand(byte dc, byte data)
  {
    _busy = 1;
    _CMD_BUF[_CMD_LEN] = (data&0xFF)|(dc>0?bit(9):0);
    if(_CMD_INDEX==-1) _CMD_INDEX = 0;
    _CMD_LEN++;
  }

  int IsBusy() {
    return _busy;  
  }
  
  void LcdCharacter(char character)
  {
    QueueLcdCommand(LCD_D, 0x00);
    for (int index = 0; index < 5; index++)
    {
      QueueLcdCommand(LCD_D, ASCII[character - 0x20][index]);
    }
    QueueLcdCommand(LCD_D, 0x00);
  }
  
  void LcdClear()
  {
    for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
    {
      QueueLcdCommand(LCD_D, 0x00);
    }
  }
  
  void LcdInitialise()
  {
    pinMode(_SCE_PIN,   OUTPUT);
    pinMode(_RESET_PIN, OUTPUT);
    pinMode(_DC_PIN,    OUTPUT);
    pinMode(_SDIN_PIN,  OUTPUT);
    pinMode(_SCLK_PIN,  OUTPUT);
  
    digitalWrite(_RESET_PIN, LOW);
    delay(1);
    digitalWrite(_RESET_PIN, HIGH);
  
    QueueLcdCommand( LCD_CMD,  0x21 );  // LCD Extended Commands.
    QueueLcdCommand( LCD_CMD,  0xBf );  // Set LCD Vop (Contrast). //B1
    QueueLcdCommand( LCD_CMD,  0x04 );  // Set Temp coefficent. //0x04
    QueueLcdCommand( LCD_CMD,  0x14 );  // LCD bias mode 1:48. //0x13
    QueueLcdCommand( LCD_CMD,  0x0C );  // LCD in normal mode. 0x0d for inverse
    QueueLcdCommand( LCD_C,    0x20);
    QueueLcdCommand( LCD_C,    0x0C);
  }
  
  void LcdString(char *characters)
  {
    while (*characters)
    {
      LcdCharacter(*characters++);
    }
  }
  
  // gotoXY routine to position cursor 
  // x - range: 0 to 84
  // y - range: 0 to 5
  
  void gotoXY(int x, int y)
  {
    QueueLcdCommand( 0, 0x80 | x);  // Column.
    QueueLcdCommand( 0, 0x40 | y);  // Row.  
  }

  Nokia5110(int sclkPin,
            int sdinPin,
            int dcPin,
            int scePin,
            int resetPin) {
    _SCE_PIN    = scePin;
    _RESET_PIN  = resetPin;
    _DC_PIN     = dcPin;
    _SDIN_PIN   = sdinPin;
    _SCLK_PIN   = sclkPin;
    LcdInitialise();
  }
};




// -------------------- //
// -------------------- //
// --- EXAMPLE CODE --- //
// -------------------- //
// -------------------- //

/*
void setup(void)
{
  LcdInitialise();
  LcdClear();
}

void loop(void)
{
  // Display some simple character animation
  //
  int a,b;
  char Str[15];
  // Draw a Box
  for(b=1000; b>0; b--){
    drawLine();
    for(a=0; a<=5 ; a++){
      gotoXY(4,1);
      // Put text in Box
      LcdString ("TestDisplay");
      gotoXY(24,3);
      LcdCharacter('H');
      LcdCharacter('E');
      LcdCharacter('L');
      LcdCharacter('L');
      LcdCharacter('O');
      LcdCharacter(' ');
      LcdCharacter('=');
      // Draw + at this position
      gotoXY(10,3);
      LcdCharacter('=');
      delay(500);
      gotoXY(24,3);
      LcdCharacter('h');
      LcdCharacter('e');
      LcdCharacter('l');
      LcdCharacter('l');
      LcdCharacter('o');
      LcdCharacter(' ');
      LcdCharacter('-');
      // Draw - at this position
      gotoXY(10,3);
      LcdCharacter('-');
      delay(500);
    }
  }
}*/