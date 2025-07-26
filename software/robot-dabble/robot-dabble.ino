/*
   Docs: https://thestempedia.com/docs/dabble/game-pad-module/
   Android app: https://play.google.com/store/apps/details?id=io.dabbleapp
   iOS app: https://apps.apple.com/us/app/dabble-bluetooth-controller/id1472734455?ls=1

   Arduino Libraries (install using Library Manager):
   * DabbleESP32
   * ESP32Servo
   Board (install ESP32 from espressif using Board Manager):
   * ESP32 Dev Module
*/

#define ROBOT_NAME "Bangolaužis"

// Uncomment your variant.
#define PINS_KMS_2024 1 // Kaunas Makerspace Antweight Workshop October 2024
// #define PINS_KMS_2023 1 // Kaunas Makerspace Antweight Workshop September 2023
// #define PINS_NTA_2023 1 // No Trolls Allowed Antweight Workshop Summer 2023

// Uncomment this if the red motor wire is on the left when looking from the back
#define MOTOR_PINS_INVERT 1

#ifdef PINS_KMS_2024
  #ifndef MOTOR_PINS_INVERT
    // MR & ML connected correctly (plus on PCB is red wire)
    #define PIN_MOTOR_IN1 27
    #define PIN_MOTOR_IN2 13
    #define PIN_MOTOR_IN3 26
    #define PIN_MOTOR_IN4 25
  #else
    // MR & ML connected incorrectly (plus on PCB is black wire)
    #define PIN_MOTOR_IN1 13
    #define PIN_MOTOR_IN2 27
    #define PIN_MOTOR_IN3 25
    #define PIN_MOTOR_IN4 26
  #endif 

    #define PIN_SERVO_1_SIG 32
    #define PIN_SERVO_2_SIG 23
#elif PINS_KMS_2023
  #ifndef MOTOR_PINS_INVERT
    // MR & ML connected correctly (plus on PCB is red wire)
    #define PIN_MOTOR_IN1 26
    #define PIN_MOTOR_IN2 25
    #define PIN_MOTOR_IN3 13
    #define PIN_MOTOR_IN4 27
  #else
    // MR & ML connected incorrectly (plus on PCB is black wire)
    #define PIN_MOTOR_IN1 25
    #define PIN_MOTOR_IN2 26
    #define PIN_MOTOR_IN3 27
    #define PIN_MOTOR_IN4 13
  #endif
  #define PIN_SERVO_1_SIG 32
#elif PINS_NTA_2023
  #define PIN_MOTOR_IN1 19
  #define PIN_MOTOR_IN2 18
  #define PIN_MOTOR_IN3 17
  #define PIN_MOTOR_IN4 16
#endif

// PWM settings
#define MOTOR_PWM_FREQ     5000 // 1 - 50000 Hz
#define MOTOR_PWM_BIT_RES  8 // pwm bit resolution

#ifdef PIN_SERVO_1_SIG || PIN_SERVO_2_SIG
#include <ESP32Servo.h>
#endif

#include "esp32-hal-ledc.h"

int ms = 0;

// speed variables
int spd_left = 0;
int spd_right = 0;

// servo variables
int servo1_deg = 0;
int servo2_deg = 0;
int servo1_deg_last = 0;
int servo2_deg_last = 0;

#ifdef PIN_SERVO_1_SIG || PIN_SERVO_2_SIG
Servo servo1;
Servo servo2;
#endif 

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

float scale(float x, float in_min, float in_max, float out_min, float out_max) {
  if (x > in_max) return out_max;
  else if (x <= in_min) return out_min;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup_pwm() {
  ledcAttach(PIN_MOTOR_IN1, MOTOR_PWM_FREQ, MOTOR_PWM_BIT_RES);
  ledcAttach(PIN_MOTOR_IN2, MOTOR_PWM_FREQ, MOTOR_PWM_BIT_RES);
  ledcAttach(PIN_MOTOR_IN3, MOTOR_PWM_FREQ, MOTOR_PWM_BIT_RES);
  ledcAttach(PIN_MOTOR_IN4, MOTOR_PWM_FREQ, MOTOR_PWM_BIT_RES);
}

void setup_servo() {
#ifdef PIN_SERVO_1_SIG
  servo1.setPeriodHertz(50);// Standard 50Hz servo
  servo1.attach(PIN_SERVO_1_SIG, 500, 2400);
#endif
#ifdef PIN_SERVO_2_SIG
  servo2.setPeriodHertz(50);// Standard 50Hz servo
  servo2.attach(PIN_SERVO_2_SIG, 500, 2400);
#endif
}
 
void set_speed_left(int spd) {
  int pin1 = PIN_MOTOR_IN3;
  int pin2 = PIN_MOTOR_IN4;
  if (spd < 0) {
    pin1 = PIN_MOTOR_IN4;
    pin2 = PIN_MOTOR_IN3;
    spd *= -1;
  }
  ledcWrite(pin1, 255);
  ledcWrite(pin2, 255 - spd);
}

void set_speed_right(int spd) {
  int pin1 = PIN_MOTOR_IN1;
  int pin2 = PIN_MOTOR_IN2;
  if (spd < 0) {
    pin1 = PIN_MOTOR_IN2;
    pin2 = PIN_MOTOR_IN1;
    spd *= -1;
  }
  ledcWrite(pin1, 255);
  ledcWrite(pin2, 255 - spd);
}

void setup() {
  Serial.begin(115200);
  Serial.println("PWM setup...");
  setup_pwm();
  Serial.println("Servo setup...");
  setup_servo();
  Dabble.begin(ROBOT_NAME);  // set bluetooth name of your device
  Serial.println("Ready!"); 
}

void loop() {
  ms = millis();

  // Refresh data obtained from smartphone.
  Dabble.processInput();

  // joystick x/y value is -7 to 7, 0 is center
  float x = GamePad.getXaxisData();
  float y = GamePad.getYaxisData();

  // The distance from the center of the joystick.
  // This value varies from 0 to 7. At the center, the value is 0 and on the perimeter of the joystick, the value is 7.
  float mag0 = GamePad.getRadius();
  float mag = scale(mag0, 0.0, 6.0, 0.0, 1.0); 

  // float scale(float x, float in_min, float in_max, float out_min, float out_max)
  x = scale(x, -7, 7, -255, 255);
  y = scale(y, -7, 7, -255, 255);
  spd_left = (y + x) * mag;
  spd_right = (y - x) * mag;

  set_speed_left(spd_left);
  set_speed_right(spd_right);
 
#ifdef PIN_SERVO_1_SIG
  // servo control
  if (GamePad.isTrianglePressed()) {
    servo1_deg = 180;
  } else if (GamePad.isCirclePressed()) {
    servo1_deg = 90;
  }
  else if (GamePad.isCrossPressed()) {
    servo1_deg = 45;
  }
  else if (GamePad.isSquarePressed()) {
    servo1_deg = 0;
  } 

  if (servo1_deg_last != servo1_deg) {
    servo1.write(constrain(servo1_deg, 0, 180));
    servo1_deg_last = servo1_deg;
  }
#endif 
  
  if (ms % 100 == 0) {
    Serial.print("mag=");
    Serial.print(mag0);
    Serial.print(" mag_scaled=");
    Serial.print(mag);
    Serial.print(" mleft=");
    Serial.print(spd_left);
    Serial.print(" mright=");
    Serial.print(spd_right);
    Serial.print(" servo1=");
    Serial.print(servo1_deg);
    Serial.println();
  }

  // All modes - buttons
  //if (GamePad.isTrianglePressed())
  //if (GamePad.isCrossPressed())
  //if (GamePad.isCirclePressed())
  //if (GamePad.isSquarePressed())
  //if (GamePad.isStartPressed())
  //if (GamePad.isSelectPressed())
}
