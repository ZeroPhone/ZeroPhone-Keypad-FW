#include <Keypad.h>
#include <Wire.h>

#define address 0x12
#define int_pin 13

const byte ROWS = 6; //four rows
const byte COLS = 5; //three columns
uint8_t keys[ROWS][COLS] = {
  //{'1','2','3','A','M'},
  {18,1,17,25,26},
  //{'4','5','6','B','N'},
  {20,3,4,30,19},
  //{'7','8','9','C','O'},
  {7,22,2,21,5},
  //{'*','0','#','D','P'},
  {10,9,6,12,8},
  //{'E','I','F','J','Q'},
  {29,13,11,14,27},
  //{'G','K','H','L','R'}
  {28,24,16,15,23}
};

uint8_t pressed_key;

byte rowPins[ROWS] = {A0, A1, A2, A3, 11, 12}; 
byte colPins[COLS] = {2, 3, 4, 5, 7}; 

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
    //Serial.println(key);
    digitalWrite(int_pin, LOW);
  }
}
