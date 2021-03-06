#include <Arduino.h>

const uint8_t SER = 8;
const uint8_t RCLK = 10;
const uint8_t SRCLK = 13;

void ser_set() { digitalWrite(SER, 1); }

void ser_clear() { digitalWrite(SER, 0); }

void srclk_pulse() {
  digitalWrite(SRCLK, 0);
  digitalWrite(SRCLK, 1);
}

void rclk_pulse() {
  digitalWrite(RCLK, 0);
  digitalWrite(RCLK, 1);
}

void rtol(bool b, int del);
void ltor(bool b, int del);

void setup() {
  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
}

void loop() {
  int ms_del = 100;

  for (uint8_t j = 0; j < 4; j++) {
    ser_set();
    for (uint8_t i = 0; i < 4; i++) {
      srclk_pulse();
      rclk_pulse();
      delay(50);
    }

    ser_clear();
    for (uint8_t i = 0; i < 4; i++) {
      srclk_pulse();
      rclk_pulse();
      delay(50);
    }
  }

  for (uint8_t j = 0; j < 8; j++) {
    ser_set();
    for (uint8_t i = 0; i < 2; i++) {
      srclk_pulse();
      rclk_pulse();
      delay(50);
    }

    ser_clear();
    for (uint8_t i = 0; i < 2; i++) {
      srclk_pulse();
      rclk_pulse();
      delay(50);
    }
  }

  for (uint8_t j = 0; j < 16; j++) {
    ser_set();
    srclk_pulse();
    rclk_pulse();
    delay(50);

    ser_clear();
    srclk_pulse();
    rclk_pulse();
    delay(50);
  }

  for (uint8_t j = 0; j < 32; j++) {
    ser_set();
    for (uint8_t i = 0; i < 8; i++) {
      srclk_pulse();
    }
    rclk_pulse();
    delay(20);

    ser_clear();
    for (uint8_t i = 0; i < 8; i++) {
      srclk_pulse();
    }
    rclk_pulse();
    delay(20);
  }

  rtol(1, 20);
  delay(ms_del);
  rtol(0, 20);
  delay(ms_del);

  ltor(1, 20);
  delay(ms_del);
  ltor(0, 20);
  delay(ms_del);

  rtol(1, 20);
  delay(ms_del);
  ltor(0, 20);
  delay(ms_del);

  ltor(1, 20);
  delay(ms_del);
  rtol(0, 20);
  delay(ms_del);

  // no se que es esto pero esta bueno
  for (uint8_t i = 0; i < 16; i++) {
    ser_set();
    srclk_pulse();
    for (uint8_t j = 0; j < i - 1; j++) {
      ser_clear();
      srclk_pulse();
    }
    rclk_pulse();
    delay(100);
  }
  // tampoco se que es esto
  for (uint8_t i = 0; i < 16; i++) {
    ser_clear();
    for (uint8_t j = 15 - i; j < 16; j++) {
      srclk_pulse();
    }

    ser_set();
    for (uint8_t k = 0; k < 15 - i; k++) {
      srclk_pulse();
    }
    rclk_pulse();
    delay(100);
  }

  int microdel = 0;
  int milidelay = 16;

  while (1) {
    ser_set();
    srclk_pulse();
    rclk_pulse();
    if (microdel <= 16383) {
      delayMicroseconds(microdel);
      microdel += 1000;
    } else {
      delay(milidelay);
      milidelay = (milidelay + 10) * 1.41;
    }
    for (uint8_t i = 0; i < 15; i++) {
      ser_clear();
      srclk_pulse();

      rclk_pulse();
      if (microdel <= 16383)
        delayMicroseconds(microdel);
      else
        delay(milidelay);
    }
    for (uint8_t i = 1; i < 16; i++) {
      ser_clear();
      for (uint8_t j = 0; j < i + 1; j++) {
        srclk_pulse();
      }

      if (i == 15) break;

      ser_set();
      srclk_pulse();
      ser_clear();
      for (uint8_t k = 0; k < 15 - i; k++) {
        srclk_pulse();
      }
      rclk_pulse();
      if (microdel <= 16383)
        delayMicroseconds(microdel);
      else
        delay(milidelay);
    }

    if (milidelay >= 50) {
      milidelay = 16;
      microdel = 0;
    }
  }
}

void rtol(bool b, int del) {
  if (b) {
    ser_set();
    for (uint8_t i = 0; i < 16; i++) {
      srclk_pulse();
      rclk_pulse();
      delay(del);
    }
  } else {
    ser_clear();
    for (uint8_t i = 0; i < 16; i++) {
      srclk_pulse();
      rclk_pulse();
      delay(del);
    }
  }
}

void ltor(bool b, int del) {
  if (b) {
    for (uint8_t i = 0; i < 16; i++) {
      ser_set();
      for (uint8_t j = 0; j < i + 1; j++) {
        srclk_pulse();
      }

      ser_clear();
      for (uint8_t k = 0; k < 15 - i; k++) {
        srclk_pulse();
      }
      rclk_pulse();

      delay(del);
    }
  } else {
    for (uint8_t i = 0; i < 16; i++) {
      ser_clear();
      for (uint8_t j = 0; j < i + 1; j++) {
        srclk_pulse();
      }

      ser_set();
      for (uint8_t k = 0; k < 15 - i; k++) {
        srclk_pulse();
      }
      rclk_pulse();
      delay(del);
    }
  }
}
