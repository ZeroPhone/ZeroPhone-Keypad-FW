#include <Keypad.h>
#include <Wire.h>

#define address 0x12
#define int_pin A3

const byte ROWS = 6; //four rows
const byte COLS = 5; //three columns
uint8_t keys[ROWS][COLS] = {
  //{'VOL_DW','VOL_UP','F1',   'UP',   'F2'},
    { 26,      25,      17,     1,      18},
  //{'HNG',   'LEFT',  'ENTER','RIGHT','ANS'},
    { 19,      30,      4,      3,      20},
  //{'1',     'PGU',   'DOWN', 'PGD',  '3'},
    { 5,       21,      2,      22,     7},
  //{'4',     '8',     '2',    '5',    '6'},
    { 8,       12,      6,      9,      10},
  //{'P1',    '*',     '7',    '9',    'CAM'},
    { 27,     14,       11,     13,     29},
  //{'F5',    '0',     '#',    'F6',   'P2'}
    { 23,      15,      16,     24,     28}
};

uint8_t pressed_key;

byte rowPins[ROWS] = {8, 7, 6, 4, 3, 2}; 
byte colPins[COLS] = {12, 13, A0, A1, A2}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void sendKey() {
  Wire.write(pressed_key);
  pressed_key = 0;
  digitalWrite(int_pin, HIGH);
}

void setup(){
  Serial.begin(115200);
  Serial.write('a'); //TXD/RXD testing
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
