#include <Keypad.h>
#include <Wire.h>

#define version 0x03
#define address 0x12
#define int_pin A3

const byte ROWS = 6; 
const byte COLS = 5; 
uint8_t keys[COLS][ROWS] = {
  //{'VOL_DW','ANSWER', '1',    '4',   'PROG1',  'F5'},
    { 27,      20,      6,      9,     28,       24},
  //{'VOL_UP','LEFT',   'F3',   '8',   '*',      '0'},
    { 26,      1,       22,     13,    15,       16},
  //{'F1',    'ENTER',  'DOWN', '2',   '7'       '#'},
    { 18,      5,       3,      7,     12,       17 },
  //{'UP',    'RIGHT',  'F4',   '5',   '9'       'F6'},
    { 2,       4,       23,     10,    14,        25},
  //{'F2',    'HANGUP', '3',    '6',   'CAMERA', 'PROG2'},
    { 19,     21,       8,      11,    30,        29}
};

uint8_t pressed_key;

byte rowPins[ROWS] = {8, 7, 6, 4, 3, 2}; 
byte colPins[COLS] = {12, 13, A0, A1, A2}; 

Keypad keypad = Keypad( makeKeymap(keys), colPins, rowPins, COLS, ROWS );

void sendKey() {
  Wire.write(pressed_key);
  pressed_key = 0;
  digitalWrite(int_pin, HIGH);
}

void setup(){
  Serial.begin(115200);
  Serial.write('a'); //TXD testing
  pinMode(int_pin, OUTPUT);
  digitalWrite(int_pin, HIGH);
  Wire.begin(address); 
  Wire.onRequest(sendKey);
  //Wire.onReceive(sendKey);
}
void loop(){
  uint8_t key = keypad.getKey();
  if (key){
    pressed_key = key;
    Serial.println(key);
    digitalWrite(int_pin, LOW);
  }
}
