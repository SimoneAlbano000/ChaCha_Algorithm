// Written by Simone Albano on 04/05/2023

#ifndef CHACHA_H_
#define CHACHA_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Macros
//#define modulo 4294967296 // Used for sum mod 2^32
//#define sum(x, y) (((uint32_t)x + (uint32_t)y)%modulo)
#define to_big_endian(value) (((value & 0xff) << 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24))
#define ROTL(value, bits) ((value << bits) | ((value >> (32 - bits)))) // Circular rotate left
#define QUARTERROUND(x, y, z, w) ({\
	x += y; w ^= x; w = ROTL(w, 16);\
	z += w; y ^= z; y = ROTL(y, 12);\
	x += y; w ^= x; w = ROTL(w, 8);\
	z += w; y ^= z; y = ROTL(y, 7);\
})

// define the number of ChaCha cycles
#define chacha_cycles 20

#define key_size_word 8 // 256-bit key
#define nonce_size_word 3 // 96-bit nonce
#define block_counter_size_word 1 // 32-bit block counter
#define state_block_size_byte 64
#define state_block_size 16

#define W0 0x61707865
#define W1 0x3320646e
#define W2 0x79622d32
#define W3 0x6b206574

typedef struct chacha {
    uint32_t block0[state_block_size];
    uint32_t state_block[state_block_size];
    uint8_t* serialized_key_block; // uint8_t pointer used to serialize state_block vector
    //uint8_t* chacha_digest; // Encrypted or Decrypted message vector
    uint32_t* endptr;
} chacha;

// Public Prototypes
void chacha_encrypt_decrypt(chacha*, uint8_t*, uint8_t*, uint32_t, uint8_t*, uint64_t);

#ifdef __cplusplus
}
#endif
#endif