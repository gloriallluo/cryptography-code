#include "aes128.h"
#include <ctime>

#define ROUND 10
#define GROUP 128  // 16 K bits

State plain[GROUP] = { 0 };
State encrypt[GROUP] = { 0 };
State decrypt[GROUP] = { 0 };


/** encode with AES-128 */
void encode(State *state) {
    addRoundKey(state, &W[0]);
    for (int i = 1; i <= ROUND; i++) {
        subBytes(state);
        shiftRows(state);
        if (i < ROUND) mixCols(state);
        addRoundKey(state, &W[i]);
    }
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
}

int main() {
    State vec = 0;
    u8 init[16] = { 0 };

    Key key = *(Key*) "thekeyforaes128";
    keyExpansion(key);
    printKey(key);

#ifdef TEST
    // encrypt
    for (int i = 0, vec = 0; i < 1; i++) {
        for (int i = 0; i < 16; i++)
            init[i] = rand() & 0xff;
        memcpy(&plain[i], init, 16);
        memcpy(&encrypt[i], &plain[i], 16);

        // encrypt
        encrypt[i] ^= vec;
        encode(&encrypt[i]);
        vec = encrypt[i];
    }

    printf("plain:\n");
    printState(plain[0]);
    printf("encode:\n");
    printState(encrypt[0]);

    // decrypt
    for (int i = 0, vec = 0; i < GROUP; i++) {
        memcpy(&decrypt[i], &encrypt[i], 16);
        // decrypt
        decode(&decrypt[i]);
        decrypt[i] ^= vec;
        vec = encrypt[i];
    }
    printf("decode:\n");
    printState(decrypt[0]);

#else
    clock_t start = clock();
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
    
    clock_t end = clock();
    double runtime = (double)(end - start) / (double)CLOCKS_PER_SEC;
    double velocity = (double)(GROUP << 7) / (runtime * 1000000);
    printf("runtime: %.6f sec\n", runtime);
    printf("velocity: %.4f Mbps\n", velocity);

    for (int i = 0; i < GROUP; i++) {
        assert(plain[i] == decrypt[i]);
    }
#endif

    return 0;
}