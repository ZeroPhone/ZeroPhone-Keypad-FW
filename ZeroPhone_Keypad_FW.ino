#include <Keypad.h>
#include <Wire.h>

#define address 0x12
#define int_pin A3

const byte ROWS = 6; //four rows
const byte COLS = 5; //three columns
uint8_t keys[ROWS][COLS] = {
  //{'VOL_DW','VOL_UP','F1',   'UP',   'F2'},
    { 27,      26,      18,     2,      19},
  //{'HNG',   'LEFT',  'ENTER','RIGHT','ANS'},
  //{'1',     'PGU',   'DOWN', 'PGD',  '3'},
    { 6,       22,      3,      23,     8},
  //{'4',     '8',     '2',    '5',    '6'},
    { 9,       13,      7,      10,     11},
  //{'P1',    '*',     '7',    '9',    'CAM'},
    { 28,     15,       12,     14,     30},
  //{'F5',    '0',     '#',    'F6',   'P2'}
    { 24,      16,      17,     25,     29}
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
