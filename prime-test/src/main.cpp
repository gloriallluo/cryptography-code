#include "num.h"

#define TEST_ROUND 10

const std::string nums[6] = {
    "\xCA\xA9\x2D\xDE\x51\x80\x7F\x14\xF6\xC8\x23\xFD\xE4\x27\x1A\xE2\xA7\xF8\x4A\x3C\x07\xEE\x58\xB8\x4B\xE8\xFF\x05\x6D\xE2\x56\x7A\xD2\xAD\xF4\x15\x1F\xF2\xCC\x2D\x93\x02\xED\x4C\xBD\x85\x5F\x71\xF0\x23\x97\x7F\xB8\xC4\xF7\xE1\xD5\x36\x7E\x73\xBD\x05\x36\xCD",
    "\xDA\x77\xD0\xB4\x03\xA5\x43\x8C\xFB\x9D\x4C\x89\xF8\x54\x8B\x66\x4F\xBD\x85\x54\x9E\xB1\xE1\xD7\x2A\x1C\x04\xE7\x76\x8C\x9B\xE2\xAE\x80\xFC\x6D\x66\x08\xC8\x69\x70\x21\xE3\xCD\x0A\xD4\x82\xD2\x0D\x94\xDC\xDE\x1E\x89\x2E\x60\x95\xD7\x1A\x52\xC6\x3A\xB9\x75",
    "\x82\xCF\x4B\x66\xB6\x65\x20\x9D\xAB\x0F\x8D\x2C\x0C\xD8\xB2\x80\x7D\xC2\x9C\xBF\xD6\x70\x72\x37\xE4\xCD\x4F\x4C\xBB\xE3\xC1\x39\xA4\xF9\x0E\x5D\xA9\xB4\x9B\x59\x13\xEA\xC4\x6E\x8E\x4F\x37\xD1\xF6\xAD\x9D\x4C\x70\x4C\x1E\xC4\x72\x8F\x6E\x75\x69\xDE\x5D\xE7",
    "\xAA\x34\xCE\x9E\x44\x3A\xFD\x46\x32\x9E\xB4\xB8\x76\xCB\xCC\x7D\x72\x14\x8D\xB8\x36\xB9\xE4\x57\x92\xAE\xC8\x9F\x79\xCB\x04\xFD\x45\x7B\xA3\xFF\xA0\x21\xCC\xE6\x2E\x11\xD0\x39\xE7\x1A\x5D\xFB\x5F\x8A\xD6\x66\x7D\x4A\xE9\x68\x16\x2F\x1F\xDA\x89\xBE\x6C\x03",
    "\xAC\xE5\xB7\x9B\xF9\xD8\x88\x89\x38\x3C\x71\xC8\x1C\xE0\x4C\x2D\xD2\x03\xE0\x5B\xDD\x9E\x1E\xD3\x6C\x1B\xB9\xCA\x7D\x65\xAB\x67\x95\x63\x11\x15\x3D\x30\x80\xF0\x84\x11\x64\xE5\xD2\xB9\x1F\xA8\xF1\xB3\xF7\x32\x2B\x08\x3D\x14\xE4\x83\x2A\xDC\xC4\x6A\x34\xA5",
    "\x96\x04\xA3\x60\x89\x63\xA5\x16\xE6\x9E\x3B\xFF\x70\xB5\x62\xB6\xB3\xA3\x35\x58\xC7\x74\x08\x69\x67\xF8\xB6\xE6\xE9\xF1\xE1\x36\x62\xA0\xE8\xA1\x4C\xF2\x64\xC1\x52\xE7\xB6\x31\x57\xBD\x37\xDE\xDB\x55\x52\xAB\x6E\x60\xAD\x05\x75\x82\x69\xD6\x73\x55\x31\xBF",
};


/**
 * mod multiply
 * a * b (mod m)
*/
u512 mul(u512 a, u512 b, u512 mod) {
    u512 res = 0;
    a = a % mod;
    for (; b != 0; b >>= 1) {
        if (b & 1) res = (res + a) % mod;
        a = (a + a) % mod;
    }
    return res;
}


/**
 * mod pow
 * a ^ b (mod m)
*/
u512 pow(u512 a, u512 b, u512 mod) {
    u512 res = 1;
    a = a % mod;
    for (; b != 0; b >>= 1) {
        if (b & 1) res = mul(res, a, mod);
        a = mul(a, a, mod);
    }
    return res;
}

bool isPrime(u512 n) {
    if (n == 2) return true;
    if (n == 3) return true;
    if ((n & 1) == 0) return false;
    
    // n - 1 = m * 2^k
    u512 m; int k;
    for (m = n - 1, k = 0; (m & 1) == 0; ++k) m >>= 1;

    for (int i = 0; i < TEST_ROUND; ++i) {
        u512 a = u512::rand() % (n - 3) + 2;
        u512 y = pow(a, m, n); // y = a^m
        // probably prime number
        if (y == 1 || y == n - 1) continue;
        u512 x;
        bool prob = false;
        for (int j = 1; j <= k; ++j, y = x) {
            x = mul(y, y, n); // x = a^(m * 2^j)
            if (x == 1 && y != 1 && y != n - 1) {
                return false;
            } else if (x == 1 || x == n - 1) {
                prob = true;
                break;
            }
        }
        if (prob) continue;
        // x = a ^ (n - 1)
        if (x != 1) return false;
    }
    return true;
}


int main() {
    assert(isPrime(u512(nums[0])) == 1);
    assert(isPrime(u512(nums[1])) == 1);
    assert(isPrime(u512(nums[2])) == 1);
    assert(isPrime(u512(nums[3])) == 1);
    assert(isPrime(u512(nums[4])) == 0);
    assert(isPrime(u512(nums[5])) == 0);
    printf("Test Passed!\n");
    return 0;
}