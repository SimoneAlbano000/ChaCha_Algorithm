/*
Computed digest: (with -Os flag)
RAM:   [          ]   4.6% (used 378 bytes from 8192 bytes)
Flash: [          ]   3.0% (used 7626 bytes from 253952 bytes)
Message: "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it."
162.76 H/s, in 0.012288 s

Computed digest: (with -O3 flag)
RAM:   [          ]   4.6% (used 378 bytes from 8192 bytes)
Flash: [=         ]   5.6% (used 14308 bytes from 253952 bytes)
Message: "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it."
164.74 H/s, in 0.012140 s
*/

#include <Arduino.h>
#include <chacha.h>

void setup() {
  Serial.begin(115200);

  chacha chacha;
  uint8_t key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
  uint8_t nonce[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00};
  uint8_t message[115] = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
  uint32_t counter = 1;

  int iterations = 2;
  double tic, toc;

  tic = micros();
  for(int i = 0; i < iterations; i++) {
    chacha_encrypt_decrypt(&chacha, key, nonce, counter, message, sizeof(message)-1);
  }
  toc = micros();

  Serial.println("\nComputed digest:");
  for(uint8_t i = 0; i < sizeof(message)-1; i++) {
    Serial.print((char)message[i]);
  }
  Serial.print("\n");
  Serial.print(iterations/((toc-tic)/1E6));
  Serial.print(" H/s, in ");
  Serial.print((toc-tic)/1E6, 6);
  Serial.print(" s");
}

void loop() {
}
