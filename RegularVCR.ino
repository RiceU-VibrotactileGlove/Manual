#include <Wire.h>
#include "Adafruit_DRV2605.h"
#include <esp_now.h>
#include <WiFi.h>

 
Adafruit_DRV2605 drv0;
Adafruit_DRV2605 drv1;
Adafruit_DRV2605 drv2;
Adafruit_DRV2605 drv3;


const int PWM_CHANNEL0 = 0;
const int PWM_CHANNEL1 = 1;
const int PWM_CHANNEL2 = 2;
const int PWM_CHANNEL3 = 3;      // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 32000;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8;
const int JITTER = 23.5;        // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 
int dutyCycle = 180; // corresponds to voltage delivered by PWM

const int DRV1 = 27;
const int DRV2 = 14;
const int DRV3 = 12;
const int DRV4 = 13;

int perms[24][4] = {
  {0, 1, 2, 3},
  {1, 0, 2, 3},
  {2, 0, 1, 3},
  {0, 2, 1, 3},
  {1, 2, 0, 3},
  {2, 1, 0, 3},
  {2, 1, 3, 0},
  {1, 2, 3, 0},
  {3, 2, 1, 0},
  {2, 3, 1, 0},
  {1, 3, 2, 0},
  {3, 1, 2, 0},
  {3, 0, 2, 1},
  {0, 3, 2, 1},
  {2, 3, 0, 1},
  {3, 2, 0, 1},
  {0, 2, 3, 1},
  {2, 0, 3, 1},
  {1, 0, 3, 2},
  {0, 1, 3, 2},
  {3, 1, 0, 2},
  {1, 3, 0, 2},
  {0, 3, 1, 2},
  {3, 0, 1, 2}
};

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

void setup() {

  Serial.begin(115200);
  Wire.begin();
  TCA9548A(0);
  delay(50);

  Serial.println("Adafruit DRV2605 Basic test");
  while (! drv0.begin()) {
    Serial.println("Could not find DRV2605 0");
    delay(5000);

  }
  drv0.useLRA();
  drv0.setMode(3);
  drv0.writeRegister8(0x1D, 161);

  TCA9548A(1);
  while (! drv1.begin()) {
    Serial.println("Could not find DRV2605 1");
    delay(5000);
  }
  drv1.useLRA();
  drv1.setMode(3);
  drv1.writeRegister8(0x1D, 161);

  TCA9548A(2);
  while (! drv2.begin()) {
    Serial.println("Could not find DRV2605 2");
    delay(5000);
  }
  drv2.useLRA();
  drv2.setMode(3);
  drv2.writeRegister8(0x1D, 161);

  TCA9548A(3);
  while (! drv3.begin()) {
    Serial.println("Could not find DRV2605 3");
    delay(5000);
  }
  drv3.useLRA();
  drv3.setMode(3);
  drv3.writeRegister8(0x1D, 161);
  // Sets up a channel (0-15), a PWM duty cycle frequency, and a PWM resolution (1 - 16 bits) 
  //ledcSetup(uint8_t channel, double freq, uint8_t resolution_bits);
  ledcSetup(PWM_CHANNEL1, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL0, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL2, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL3, PWM_FREQ, PWM_RESOLUTION);


  // ledcAttachPin(uint8_t pin, uint8_t channel);
  ledcAttachPin(DRV1, PWM_CHANNEL0);
  ledcAttachPin(DRV2, PWM_CHANNEL1);
  ledcAttachPin(DRV3, PWM_CHANNEL2);
  ledcAttachPin(DRV4, PWM_CHANNEL3);

 //sets LRA_OPEN_LOOP bit to 1 for off resonance driving
  Serial.println(drv0.readRegister8(0x1D), BIN);
  Serial.println(drv1.readRegister8(0x1D), BIN);
  Serial.println(drv2.readRegister8(0x1D), BIN);
  Serial.println(drv3.readRegister8(0x1D), BIN);


void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);

  //Regular VCR
  for(int period = 1; period < 4; period++){
    int pattern = random(25); // picks random perumtation for fingers in current period T_CR
    //Serial.println(pattern);
    for(int j = 0; j < 4; j++){
      ledcWrite(perms[pattern][j], dutyCycle); //on for 100ms
      delay(100);
      ledcWrite(perms[pattern][j], 0); //off dor 66 ms
      delay(66);
    }
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  delay(666);



 




}
