#include <iostream>
#include <cstdint>
#include <ctime>

using u8 = uint8_t;
using u128 = __uint128_t;

#define LENGTH 2048
#define KEY_LEN 128
u8 key[KEY_LEN] = { 0 };

void initKey() {
#ifdef TEST
    u8 *buf = (u8*) "\x12\x34\x56\x78\x9a\xbc\xde\xff";
    memcpy(key, buf, 8);
#else
    for (int i = 0; i < KEY_LEN; i++)
        key[i] = rand() & 0xff;
    printf("key: ");
    for (int i = 0; i < KEY_LEN; i++)
        printf("%02x ", key[i]); printf("\n");
#endif
}

/** S-Box */
u8 S[256] = { 0 };

void swap(u8 &x, u8 &y) {
    u8 tmp = x; x = y; y = tmp;
}

/** 
 * KSA function, i.e. key-scheduling algorithm
 * shuffle S box according to key
 * @param keyLen: length of key in byte, should be less than 128
*/
void KSA(int keyLen) {
    for (int i = 0; i < 256; i++) 
        S[i] = i;
    for (int i = 0, j = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keyLen]) & 0xff;
        swap(S[i], S[j]);
    }
}

/** 
 * PRGA function 
 * Used in key stream generating
 * @param output: output encode text
 * @param input: input plain text
 * @param length: input & output length in byte
*/
void PRGA(u8 *output, const u8 *input, int length) {
    int i = 0; int j = 0;
    for (int k = 0; k < length; k++) {
        i = (i + 1) & 0xff;
        j = (j + S[i]) & 0xff;
        swap(S[i], S[j]);
        output[k] = input[k] ^ S[(S[i] + S[j]) & 0xff];
    }
}

u8 plain[LENGTH] = { 0 };
u8 encrypt[LENGTH] = { 0 };
u8 decrypt[LENGTH] = { 0 };

int main() {

#ifdef TEST
    u8 *test = (u8*) "some plain text to encrypt";
    
    initKey();
    KSA(8); PRGA(encrypt, test, 26);
    KSA(8); PRGA(decrypt, encrypt, 26);

    for (int i = 0; i < 26; i++) assert(test[i] == decrypt[i]);
    for (int i = 0; i < 26; i++) printf("%02x ", test[i]); printf("\n");
    for (int i = 0; i < 26; i++) printf("%02x ", encrypt[i]); printf("\n");
    for (int i = 0; i < 26; i++) printf("%02x ", decrypt[i]); printf("\n");
#else
    // init plain text
    for (int i = 0; i < LENGTH; i++) 
        plain[i] = rand() & 0xff;
    
    initKey();
    KSA(KEY_LEN);
    clock_t start = clock();
    PRGA(encrypt, plain, LENGTH);
    PRGA(decrypt, encrypt, LENGTH);
    clock_t end = clock();

    double runtime = (double)(end - start) / (double)CLOCKS_PER_SEC;
    double velocity = (double)(LENGTH << 3) / (runtime * 1000000);
    printf("runtime: %.6f sec\n", runtime);
    printf("velocity: %.4f Mbps\n", velocity);
#endif
    return 0;
}