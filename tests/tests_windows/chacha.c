
#include <stdio.h>
#include "../../lib/chacha.h"

int main() {

    chacha chacha;
    uint8_t key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
    uint8_t nonce[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x00};
    uint8_t message[114] = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
    uint32_t counter = 1;
    
    chacha_encrypt_decrypt(&chacha, key, nonce, counter, message, sizeof(message));
    for(int i = 0; i < sizeof(message); i++) {
        printf("%0x ", message[i]);
    }
    printf("\n\n");
    chacha_encrypt_decrypt(&chacha, key, nonce, counter, message, sizeof(message));
    for(int i = 0; i < sizeof(message); i++) {
        printf("%c", message[i]);
    }
    return 0;
}
