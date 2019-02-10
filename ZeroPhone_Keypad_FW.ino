#include <Keypad.h>
#include <Wire.h>

#define version 0x04
#define address 0x12
#define int_pin A3
#define vibromotor_pin 11
#define tftbl_pin 10
#define kpbl_pin 9
#define usrgp1_pin 5

//#define debug

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

const byte BUFFER_SIZE = 255;
uint8_t key_buffer[BUFFER_SIZE] = {};
uint8_t key_pointer = 1;
uint8_t i2c_pointer = 1;

uint8_t pressed_key;

byte rowPins[ROWS] = {8, 7, 6, 4, 3, 2}; 
byte colPins[COLS] = {12, 13, A0, A1, A2}; 

Keypad keypad = Keypad( makeKeymap(keys), colPins, rowPins, COLS, ROWS );

void sendKey() {
  if (i2c_pointer == key_pointer) {
    Wire.write(0);
    digitalWrite(int_pin, HIGH);
    return;
  }
  Wire.write(key_buffer[i2c_pointer]);
  i2c_pointer++;
  if (i2c_pointer == key_pointer) {
    digitalWrite(int_pin, HIGH);
  }
}

bool buffer_overflow() {
  if (i2c_pointer == 0){
    return key_pointer == BUFFER_SIZE;
  }
  return key_pointer == i2c_pointer - 1;
}

void keypad_ev_listener_i2c(char key) {
  uint8_t key_value = (uint8_t) key;
  byte state = keypad.getState();
  if (state == 0)
    return;
  state--;
  uint8_t i2c_value = state << 5 | key_value;
  if (buffer_overflow()) {    
    return;
  }
  key_buffer[key_pointer] = i2c_value;
  key_pointer++;
  digitalWrite(int_pin, LOW);
  #ifdef debug
    Serial.print(key_pointer);
    Serial.print(" ");
    Serial.println(i2c_value);
  #endif
}

void processWrite(int len){
    // res-res-res-res-gpo-kpbl-tftbl-vib
    for (int i=0; i<len; i++)
    {
      uint8_t data = Wire.read();
      digitalWrite(vibromotor_pin, bitRead( data, 0 ));
      analogWrite(tftbl_pin, bitRead( data, 1 ) ? 0 : 500);
      analogWrite(kpbl_pin, bitRead( data, 2 ) ? 0 : 500);
      digitalWrite(usrgp1_pin, bitRead( data, 3 ));
    }
}

void setup(){
  #ifdef debug
    Serial.begin(115200);
    Serial.write('a'); //TXD testing
  #endif
  // Interrupt pin
  pinMode(int_pin, OUTPUT);
  digitalWrite(int_pin, HIGH);
  // Vibromotor pin
  pinMode(vibromotor_pin, OUTPUT);
  digitalWrite(vibromotor_pin, LOW);
  // TFT backlight pin
  pinMode(tftbl_pin, OUTPUT);
  digitalWrite(tftbl_pin, LOW);
  // Keypad backlight pin
  pinMode(kpbl_pin, OUTPUT);
  digitalWrite(kpbl_pin, LOW);
  // User-controlled GPIO (in this FW version, it's OUT-only)
  pinMode(usrgp1_pin, OUTPUT);
  digitalWrite(usrgp1_pin, LOW);
  // I2C setup
  Wire.begin(address); 
  Wire.onRequest(sendKey);
  Wire.onReceive(processWrite);
  // Enable the vibromotor for a short while on bootup
  digitalWrite(vibromotor_pin, HIGH);
  delay(300);
  digitalWrite(vibromotor_pin, LOW);
  keypad.addEventListener(keypad_ev_listener_i2c);
  keypad.setHoldTime(500);
}

void loop(){
  keypad.getKey();
}
