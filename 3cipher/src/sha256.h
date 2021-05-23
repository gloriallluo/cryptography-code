#ifndef SHA_256_H
#define SHA_256_H

#include <iostream>
#include <cstdint>

#define W 64

using u8 = uint8_t;
using u64 = uint64_t;

/**
 * struct State, 1600 bits
 * State[x, y, z]
*/
struct State {
    // state[x, y, z]: s[x][y][63, 62, ..., 0]
    u64 s[5][5] = {{ 0 }};

public:
    /** 
     * default construction function 
    */
    State() {}

    /**
     * construction function
     * convert string to state
    */
    State(char *str) {
        // assert(strlen(str) >= 200);
        u64 *l_ptr = (u64*)str;
        for (int x = 0; x < 5; x++)
            for (int y = 0; y < 5; y++)
                s[x][y] = l_ptr[x + 5 * y];
    }

    /**
     * function get
     * @param xyz: position
     * @return: the bit at the specific position
    */
    int get(int x, int y, int z) {
        u64 tmp = s[x][y];
        return (tmp & (1ULL << z)) >> z;
    }

    /**
     * function x
     * actually it's xor
     * @param d: a u64 array
    */
    void x(const u64 *d) {
        for (int y = 0; y < 5; y++)
            for (int x = 0; x < 5; x++)
                s[x][y] ^= d[x + 5 * y];
    }

    /**
     * function print, for debugging
    */
    void print() {
        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < 5; y++)
                printf("%016llx  ", s[x][y]);
            printf("\n");
        }
    }
};

/// Step Mapping

// shift left for d bits
// when d = 1, [63, ..., 0] -> [62, ..., 0, 63]
#define rot(x, d) (((x) << (d)) | ((x) >> (64 - (d))))

// flip bits
#define flip(x) \
    ((((x) & 0x01) << 7) | (((x) & 0x02) << 5) | (((x) & 0x04) << 3) | (((x) & 0x08) << 1) | \
    (((x) & 0x10) >> 1) | (((x) & 0x20) >> 3) | (((x) & 0x40) >> 5) | (((x) & 0x80) >> 7))

// round constant
const u64 RC[24] = {
    0x0000000000000001UL, 0x0000000000008082UL, 0x800000000000808aUL, 0x8000000080008000UL, 
    0x000000000000808bUL, 0x0000000080000001UL, 0x8000000080008081UL, 0x8000000000008009UL, 
    0x000000000000008aUL, 0x0000000000000088UL, 0x0000000080008009UL, 0x000000008000000aUL,
    0x000000008000808bUL, 0x800000000000008bUL, 0x8000000000008089UL, 0x8000000000008003UL, 
    0x8000000000008002UL, 0x8000000000000080UL, 0x000000000000800aUL, 0x800000008000000aUL, 
    0x8000000080008081UL, 0x8000000000008080UL, 0x0000000080000001UL, 0x8000000080008008UL,
};

/** 
 * function thet
 * @param s: state to be changed
*/
void thet(State &s) {
    u64 C[5] = { 0, 0, 0, 0, 0 };
    u64 D[5] = { 0, 0, 0, 0, 0 };
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            C[x] ^= s.s[x][y];
    for (int x = 0; x < 5; x++)
        D[x] = C[(x + 4) % 5] ^ rot(C[(x + 1) % 5], 1);
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            s.s[x][y] ^= D[x];
}

/**
 * function rho_n_pi
 * in the Keccak mapping steps, it is rho conbined with pi
 * @param s: state to be changed
*/
void rho_n_pi(State &s) {
    int x = 1; int y = 0;
    u64 cur = s.s[x][y]; u64 tmp;
    int nx, ny;
    for (int t = 0; t < 24; x = nx, y = ny, t++) {
        nx = y; ny = (2 * x + 3 * y) % 5;
        tmp = s.s[nx][ny];
        s.s[nx][ny] = rot(cur, (((t + 1) * (t + 2)) >> 1) & 0x3f);
        cur = tmp;
    }
}

/**
 * function chi
 * @param s: state to be changed
*/
void chi(State &s) {
    for (int y = 0; y < 5; y++) {
        u64 tmp[5] = { 0, 0, 0, 0, 0 };
        for (int x = 0; x < 5; x++)
            tmp[x] = s.s[x][y];
        for (int x = 0; x < 5; x++)
            s.s[x][y] ^= (~tmp[(x + 1) % 5]) & tmp[(x + 2) % 5];
    }
}

/**
 * function iota
 * @param s: state to be changed
*/
void iota(State &s, int r) {
    s.s[0][0] ^= RC[r % 24];
}

/// Padding

/**
 * function padding
 * implement pad10*1
 * @param x: padding unit length in bit
 * @param m: padding message
 * @param len: length of m in bit (< x)
*/
void padding(int x, char *m, int len) {
    if (len == x) return;
    int c = (x - len - 2) % x;  // the length of 10*1
    int n = len >> 3; // number of char
    int r = len & 7;  // residue
    m[n] |= (1 << r);
    m[(x >> 3) - 1] |= 0x80;
}


#endif