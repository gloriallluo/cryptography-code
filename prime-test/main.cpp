#include "num.h"

#define TEST_ROUND 20


/**
 * mod multiply
 * a * b (mod m)
*/
u512 mul(u512 a, u512 b, u512 mod) {
    u512 res = 0;
    while (b != 0) {
        if (b & 1) res = (res + a) % mod;
        a = (a + a) % mod;
        b >>= 1;
    }
    return res;
}


/**
 * mod pow
 * a ^ b (mod m)
*/
u512 pow(u512 a, u512 b, u512 mod) {
    u512 res = 1;
    while (b != 0) {
        if (b & 1) res = mul(res, a, mod);
        a = mul(a, a, mod);
        b >>= 1;
    }
    return res;
}

bool isPrime(u512 n) {
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    
    // n - 1 = m * 2^k
    u512 m; int k;
    for (m = n - 1, k = 0; (m & 1) == 0; ++k) m >>= 1;

    for (int i = 0; i < TEST_ROUND; ++i) {
        // printf("in round %d\n", i);
        u512 a = u512::rand() % (n - 1) + 1;
        // a.print();
        u512 y = pow(a, m, n); // y = a^m
        // y.print();
        u512 x;
        for (int j = 1; j <= k; ++j, y = x) {
            x = mul(y, y, n); // x = a^(m * 2^j)
            // printf("in subround %d\n", j);
            // x.print();
            if (x == 1 && y != 1 && y != n - 1)
                return false;
        }
        // x = a ^ (n - 1)
        if (x != 1) return false;
    }
    return true;
}


int main() {
    // u512 a(18310);
    // u512 b(1831);
    // u512 q, r;
    // u512::divide(a, b, q, r);
    // q.print(); r.print();
    // printf("isPrime(%d) = %d\n", 2, isPrime(2));
    // printf("isPrime(%d) = %d\n", 88, isPrime(88));
    printf("%d\n", isPrime(u512(3187)));
    // printf("isPrime(%d) = %d\n", 11, isPrime(11));
    // printf("isPrime(%d) = %d\n", 13, isPrime(13));
    // printf("isPrime(%d) = %d\n", 17, isPrime(17));
    // printf("isPrime(%d) = %d\n", 129, isPrime(129));
    // printf("isPrime(%d) = %d\n", 71, isPrime(71));
    // printf("isPrime(%d) = %d\n", 91, isPrime(91));
    // printf("isPrime(%d) = %d\n", 25, isPrime(25));
    return 0;
}