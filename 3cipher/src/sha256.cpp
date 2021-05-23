#include "sha256.h"
#include <ctime>


/**
 * implement KECCAK-p[1600, 24]
 * @param s: state
*/
void keccak(State &s) {
    for (int r = 0; r < 24; r++) {
        thet(s);
        rho_n_pi(s);
        chi(s);
        iota(s, r);
    }
}


#define PLAIN_LEN 2050
#define R 1088
u8 digest[32] = { 0 };
char plain[PLAIN_LEN] = { 0 };


/**
 * implement SHA3-256, denoted by SPONGE[
 *      f: KECCAK-p[1600,24], 
 *      pad: pad10*1, 
 *      rate: 1600 - 512
 * ] (M||01, 256), where M represents the message.
 * @param message: message string
 * @param length: length of the message in byte
 * @return: digest[], a 256-bits array
*/
void sponge(char *message, int length) {
    int step = R >> 3;  // N_byte for a padding unit

    // add 01 at the tail
    message[length] = 2;
    int n = ((length << 3) + 2 + R - 1) / R;  // n * R bits
    padding(R, &message[(n - 1) * step], ((length << 3) + 2) % R);
    
    State state;
    u8 tmp[200] = { 0 };
    for (int i = 0; i < n; i++) {
        memcpy(tmp, &message[i * step], step);
        memset(&tmp[step], 0, 64);
        state.x((u64*)tmp);
        keccak(state);
    }

    u64 *l_ptr = (u64*)digest;
    for (int i = 0; i < 4; i++)
        l_ptr[i] = state.s[i][0];
}

/** 
 * pass string in, just for test
*/
void hash(std::string message) {
    memset(plain, 0, PLAIN_LEN);
    memcpy(plain, message.data(), message.length());
    sponge(plain, message.length());
}

void printDigest() {
    for (int i = 0; i < 32; i++) {
        printf("%02x", digest[i]);
        if (i % 4 == 3) printf(" ");
    }   printf("\n");
}


int main() {
#ifdef TEST
    // case 1 "": a7ffc6f8 bf1ed766 51c14756 a061d662 f580ff4d e43b49fa 82d80a4b 80f8434a
    hash(""); printDigest();

    // case 2 "abc": 3a985da7 4fe225b2 045c172d 6bd390bd 855f086e 3e9d525b 46bfe245 11431532
    hash("abc"); printDigest();

    // case 3 " ": 60e893e6 d54d8526 e55a81f9 8bfac5da 236bb203 e84ed596 7a8f527d 5bf3d4a4
    hash(" "); printDigest();

    // case 4 "hello world": 644bcc7e 56437304 0999aac8 9e7622f3 ca71fba1 d972fd94 a31c3bfb f24e3938
    hash("hello world"); printDigest();
#else
    for (int i = 0; i < PLAIN_LEN; i++)
        plain[i] = rand() & 0xff;
    clock_t start = clock();
    sponge(plain, PLAIN_LEN);
    clock_t end = clock();

    double runtime = (double)(end - start) / (double)CLOCKS_PER_SEC;
    double velocity = (double)(PLAIN_LEN << 3) / (runtime * 1000000);
    printf("runtime: %.6f sec\n", runtime);
    printf("velocity: %.4f Mbps\n", velocity);
#endif
    return 0;
}