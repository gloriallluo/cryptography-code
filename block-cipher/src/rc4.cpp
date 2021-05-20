#include <iostream>
#include <cstdint>

using u8 = uint8_t;
using u32 = uint32_t;
using u128 = __uint128_t;

#define LENGTH 2048
#define KEY_LEN 128
u8 key[KEY_LEN] = { 0 };

void initKey() {
    for (int i = 0; i < KEY_LEN; i++)
        key[i] = rand() & 0xff;
}

/** S-Box */
u8 S[256] = { 0 };

void swap(u8 &x, u8 &y) {
    u8 tmp = x; x = y; y = tmp;
}

/** KSA function 
 * shuffle S box according to key
*/
void KSA() {
    for (int i = 0; i < 256; i++) 
        S[i] = i;
    for (int i = 0, j = 0; i < 256; i++) {
        j = (j + S[i] + key[i & 0x7f]) & 0xff;
        swap(S[i], S[j]);
    }
}

/** PRGA function 
 * Used in key stream generating
 * @param output: output key stream
 * @param outLen: output length in byte
 * @param input: input key
 * @param inLen: input length in byte
*/
void PRGA(u8 *output, int outLen, const u8 *input, int inLen) {
    int i = 0; int j = 0;
    for (int k = 0; k < outLen; k++) {
        i = (i + 1) & 0xff;
        j = (j + S[i]) & 0xff;
        swap(S[i], S[j]);
        output[k] = input[k % inLen] ^ S[(S[i] + S[j]) & 0xff];
    }
}

/** KeyStream Generation */
void genKeyStream(u8 *keyStream) {
    KSA();
    PRGA(keyStream, LENGTH, key, KEY_LEN);
}

/** rc4 encoding and decoding */
void rc4(u8 *encrypt, const u8 *plain, u8 *ks) {
    genKeyStream(ks);
    for (int i = 0; i < LENGTH; i++) 
        encrypt[i] = plain[i] ^ ks[i];
}


u8 plain[LENGTH] = { 0 };
u8 encrypt[LENGTH] = { 0 };
u8 decrypt[LENGTH] = { 0 };
u8 keyStream[LENGTH] = { 0 };

int main() {

    // init plain text
    for (int i = 0; i < LENGTH; i++) 
        plain[i] = rand() & 0xff;
    
    initKey();
    // TODO: start timer
    rc4(encrypt, plain, keyStream);
    rc4(decrypt, encrypt, keyStream);
    // TODO: end timer

    // test
    for (int i = 0; i < LENGTH; i++)
        assert(plain[i] == decrypt[i]);
    return 0;
}