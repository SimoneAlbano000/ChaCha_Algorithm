// Written by Simone Albano on 04/05/2023

#include "chacha.h"

// Private Prototypes
static void chacha_fill_matrix(chacha*, uint8_t*, uint8_t*, uint32_t);
static void chacha_compute_round(chacha*, uint8_t);
static void* mempcpy(void *, const void *, size_t);

static void chacha_fill_matrix(chacha* chacha, uint8_t* key0, uint8_t* nonce, uint32_t counter) {
    /* ChaCha first block structure

        32bit  32bit  32bit  32bit
        |------|------|------|------|
        |  K0  |  K1  |  K2  |  K3  | 32bit
        |------|------|------|------|
        | key0 | key1 | key2 | key3 | 32bit
        |------|------|------|------|
        | key4 | key5 | key6 | key7 | 32bit
        |------|------|------|------|
        |  bc  |  n0  |  n1  |  n2  | 32bit
        |------|------|------|------|

        where K = constants
            key = input key
            bc = block counter
            n = nonce
    */
    chacha->block0[0] = W0;
    chacha->block0[1] = W1;
    chacha->block0[2] = W2;
    chacha->block0[3] = W3;
    chacha->endptr = mempcpy(chacha->block0+4, key0, sizeof(uint32_t)*key_size_word);
    chacha->endptr = mempcpy(chacha->endptr, &counter, sizeof(uint32_t)*block_counter_size_word);
    memcpy(chacha->endptr, nonce, sizeof(uint32_t)*nonce_size_word);
    return;
}

static void chacha_compute_round(chacha* chacha, uint8_t n_rounds) {
    // Copy the initial block into the ChaCha state block
    memcpy(chacha->state_block, chacha->block0, sizeof(uint32_t)*state_block_size);
    // Compute (n_rounds/2) ChaCha rounds where n_rounds in ChaCha8/12/20
    for(uint8_t i = 0; i < (n_rounds>>1); i++) {
        QUARTERROUND(chacha->state_block[0], chacha->state_block[4], chacha->state_block[8], chacha->state_block[12]);
        QUARTERROUND(chacha->state_block[1], chacha->state_block[5], chacha->state_block[9], chacha->state_block[13]);
        QUARTERROUND(chacha->state_block[2], chacha->state_block[6], chacha->state_block[10], chacha->state_block[14]);
        QUARTERROUND(chacha->state_block[3], chacha->state_block[7], chacha->state_block[11], chacha->state_block[15]);
        QUARTERROUND(chacha->state_block[0], chacha->state_block[5], chacha->state_block[10], chacha->state_block[15]);
        QUARTERROUND(chacha->state_block[1], chacha->state_block[6], chacha->state_block[11], chacha->state_block[12]);
        QUARTERROUND(chacha->state_block[2], chacha->state_block[7], chacha->state_block[8], chacha->state_block[13]);
        QUARTERROUND(chacha->state_block[3], chacha->state_block[4], chacha->state_block[9], chacha->state_block[14]);
    }
    // Add the final state_block to the initial one
    for(uint8_t i = 0; i < state_block_size; i++) {
        chacha->state_block[i] += chacha->block0[i];
    }
    // Serialize in little-endian order the final state block
    chacha->serialized_key_block = (uint8_t*)chacha->state_block;
    return;
}

void chacha_encrypt_decrypt(chacha* chacha, uint8_t* key0, uint8_t* nonce, uint32_t counter, uint8_t* message, uint64_t message_length) {
    // Encryption and Decryption are done in the same way because XOR is simmetrically reversible
    uint32_t n_of_blocks = ((message_length+(state_block_size_byte-(message_length%state_block_size_byte)))/state_block_size_byte)-!(message_length%state_block_size_byte);
    uint8_t last_block_len = message_length%state_block_size_byte;
    // Main ChaCha loop (compute all the blocks except for the last one)
    for(uint32_t t = 0; t < n_of_blocks-1; t++) {
       chacha_fill_matrix(chacha, key0, nonce, counter+t);
       chacha_compute_round(chacha, chacha_cycles);
       for(uint8_t i = 0; i < state_block_size_byte; i++) {
        message[(state_block_size_byte*t)+i] ^= chacha->serialized_key_block[i];
       }
    }
    // Compute the last block
    chacha_fill_matrix(chacha, key0, nonce, counter+n_of_blocks-1);
    chacha_compute_round(chacha, chacha_cycles);
    for(uint8_t i = 0; i < last_block_len; i++) {
        message[(message_length-last_block_len)+i] ^= chacha->serialized_key_block[i];
    }
    return;
}

void* mempcpy(void *dest, const void *src, size_t len) {
    return memcpy (dest, src, len) + len;
}
