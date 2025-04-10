#include "Wire.h" // This library allows you to communicate with I2C devices.
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data

#define DO_PIN 13
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

#define BUZZER 4
int MIC = A0;
int soundVal = 0; // Initialize soundVal
int gyroVal = 0; // Initialize gyroVal
int lightState; // Declare lightState

Servo myservo1;  // create servo object to control the first servo
int servoPin1 = 5;
Servo myservo2;  // create servo object to control the second servo
int servoPin2 = 6; // Define a different pin for the second servo

int pos1 = 0;    // variable to store the servo position
int pos2 = 50;

char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  Serial.begin(9600);

  // estos si
  pinMode(BUZZER, OUTPUT);
  pinMode(MIC, INPUT);
  pinMode(DO_PIN, INPUT);

  // initialize LCD
  lcd.init();
  lcd.backlight();

  myservo1.attach(servoPin1);  // attaches the first servo on specified pin to the servo object
  myservo2.attach(servoPin2);  // attaches the second servo on specified pin to the servo object

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void loop() {
  lightState = digitalRead(DO_PIN); // Read the light sensor state

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  gyro_x = Wire.read() << 8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read() << 8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read() << 8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  if (gyro_x >= 32000 || gyro_x <= -32000 ||
      gyro_y >= 32000 || gyro_y <= -32000 ||
      gyro_z >= 32000 || gyro_z <= -32000 ||
      analogRead(MIC) >= 200) {
    analogWrite(BUZZER, 200);
    gyroVal = 1;
    soundVal = 1;
    for (int i = 0; i < 3; i++) {
      myservo1.write(pos1);
      myservo2.write(pos1); 
      delay(200);
      myservo1.write(pos2);
      myservo2.write(pos2); 
      delay(200);
    }
  } else {
    analogWrite(BUZZER, 0);
    gyroVal = 0;
    soundVal = 0;
  }

  if (soundVal == 1) {
    lcd.clear();
    lcd.print("Bua bua");
    lcd.setCursor(0, 1);
    lcd.print(" (T_T) ");
  } else if (lightState == HIGH) {
    lcd.clear();
    lcd.print("ZZZZZZZZ");
    lcd.setCursor(0, 1);
    lcd.print(" (-_-) ");
  } else {
    lcd.clear();
    lcd.print("It is Happy");
    lcd.setCursor(0, 1);
    lcd.print(" (^_^) ");
  }

  Serial.print(lightState);
  Serial.print(",");
  Serial.print(soundVal);
  Serial.print(",");
  Serial.println(gyroVal);

  //delay
  delay(250);
}
