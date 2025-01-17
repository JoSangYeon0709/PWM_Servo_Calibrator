#include <Wire.h>
#include "ServoEasing.hpp"

#define servo_driver_bits 4096

const int servo_pin = 0;       // 서보 핀 번호
ServoEasing servos_calibration(0x40, &Wire);

int servo_min = 520;           // 서보의 최소 펄스 폭
int servo_max = 2400;          // 서보의 최대 펄스 폭
int servo_min_last = servo_min; // 서보의 최소 펄스 마지막 값
int servo_max_last = servo_max; // 서보의 최대 펄스 마지막 값
int servo_moving_time = 1000;  // 서보 이동 시간(ms)
int servo_angle = 90;          // 서보 이동 목표 각도

String inString;

void servo_setup() {
  servos_calibration.attach(servo_pin, servo_pin, servo_min, servo_max, 180, 0);
  servos_calibration.setEasingType(EASE_CUBIC_IN_OUT);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  servo_setup();
  servos_calibration.write(90);
}

void loop() {
  delay(100);

  if (Serial.available() > 0) {
    inString = Serial.readStringUntil('\n');
    char cmd = inString[0];

    if (cmd == '1') {
      if (inString.indexOf('a') >= 0 && inString.indexOf('b') >= 0 && inString.indexOf('c') >= 0) {
        servo_min = inString.substring(inString.indexOf('a') + 1, inString.indexOf('b')).toInt();
        servo_max = inString.substring(inString.indexOf('b') + 1, inString.indexOf('c')).toInt();
        servo_angle = inString.substring(inString.indexOf('c') + 1, inString.indexOf('d')).toInt();

        if ((servo_min != servo_min_last) || (servo_max != servo_max_last)) {
          servo_setup();
          servo_min_last = servo_min;
          servo_max_last = servo_max;
          Serial.println("Servo settings updated");
          servos_calibration.startEaseToD(90, servo_moving_time);
          delay(servo_moving_time);
        }

        Serial.print("min: ");
        Serial.print(servo_min);
        Serial.print(" max: ");
        Serial.print(servo_max);
        Serial.print(" angle: ");
        Serial.println(servo_angle);

        servos_calibration.startEaseToD(servo_angle, servo_moving_time);
      }
      
    }else if(cmd == '2') {
      Serial.println("");
      Serial.println("check servo calibration");
      Serial.println("");

      Serial.print("min: ");
      Serial.print(servo_min);
      Serial.print(" max: ");
      Serial.println(servo_max);

      servos_calibration.startEaseToD(0, servo_moving_time);
      delay(servo_moving_time+1500);
      servos_calibration.startEaseToD(90, servo_moving_time);
      delay(servo_moving_time+1500);
      servos_calibration.startEaseToD(180, servo_moving_time);
      Serial.println("finish");
    }
  }
}
