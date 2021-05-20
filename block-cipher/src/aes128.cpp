#include "aes128.h"

#define ROUND 10
#define GROUP 128  // 16 K bits

#ifndef TEST
State plain[GROUP] = { 0 };
State encrypt[GROUP] = { 0 };
State decrypt[GROUP] = { 0 };
#endif


void printState(const State &s) {
    u8 *b_ptr = (u8*)&s;
    for (int i = 0; i < 16; i++) {
        printf("%02x", b_ptr[i]);
        if ((i & 3) == 3) printf(" ");
    }   printf("\n");
}


/** encode with AES-128 */
void encode(State *state) {
    addRoundKey(state, &W[0]);
    for (int i = 1; i <= ROUND; i++) {
        subBytes(state);
        shiftRows(state);
        if (i < ROUND) mixCols(state);
        addRoundKey(state, &W[i]);
    }
    // transpose(state);
}

/** decode with AES-128 */
void decode(State *state) {
    addRoundKey(state, &W[ROUND]);
    for (int i = ROUND - 1; i >= 0; i--) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, &W[i]);
        if (i > 0) invMixCols(state);
    }
    // transpose(state);
}

int main() {
    
    State vec = 0;
    u8 init[16] = { 0 };

    Key key = *(Key*)"\x32\x81\xf1\x9a\x3b\xbc\x82\x1d\xc8\x2c\xff\xe0\x8a\x00\x73\x6c";
    keyExpansion(key);
#ifdef TEST
    for (int i = 0; i < 16; i++)
        init[i] = rand() & 0xff;
    State state;
    memcpy(&state, init, 16);

    // initialization
    printf("plain:  ");
    printState(state);

    // encode
    encode(&state);
    printf("encode: ");
    printState(state);

    // decode
    decode(&state);
    printf("decode: ");
    printState(state);
    printf("\n");
#else
    // TODO: start timer
    
    // encrypt
    for (int i = 0, vec = 0; i < GROUP; i++) {
        for (int i = 0; i < 16; i++)
            init[i] = rand() & 0xff;
        memcpy(&plain[i], init, 16);
        memcpy(&encrypt[i], &plain[i], 16);

        // encrypt
        encrypt[i] ^= vec;
        encode(&encrypt[i]);
        vec = encrypt[i];
    }

    // decrypt
    for (int i = 0, vec = 0; i < GROUP; i++) {
        memcpy(&decrypt[i], &encrypt[i], 16);
        // decrypt
        decode(&decrypt[i]);
        decrypt[i] ^= vec;
        vec = encrypt[i];
    }
    
    // TODO: end timer

    for (int i = 0; i < GROUP; i++) {
        assert(plain[i] == decrypt[i]);
    }
#endif

    return 0;
}