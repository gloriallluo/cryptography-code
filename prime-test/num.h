#ifndef NUM_H
#define NUM_H

#include <cstdint>
#include <iostream>

using u8 = uint8_t;
using u16 = uint16_t;
using u64 = uint64_t;



struct u512 {
public:
    u8 data[64] = { 0 };

    u512() {}

    u512(int n) {
        u8 *ptr = (u8*)&n;
        for (int i = 0; i < 4; ++i) 
            data[i] = ptr[i];
    }

    u512(std::string n) {
        u8 *ptr = (u8*)n.data();
        for (int i = 0; i < 4; ++i) 
            data[i] = ptr[i];
    }

    u512(const u512 &n) {
        for (int i = 0; i < 64; i++)
            data[i] = n.data[i];
    }

    u512 operator+(const u512 &n) const {
        u512 res;
        u8 carry = 0;
        for (int i = 0; i < 64; i++) {
            u16 v = (u16)data[i] + (u16)n.data[i] + (u16)carry;
            res.data[i] = (u8)(v & 0xff);
            carry = (u8)((v & 0xff00) >> 8);
        }
        return res;
    }

    // TODO
    u512 operator-(const u512 &n) const {
        u512 res;
        u8 carry = 0;
        for (int i = 0; i < 64; i++) {
            u16 t = (u16)n.data[i] + (u16)carry;
            carry = (u16)data[i] >= t? 0: 1;
            u16 v = carry * 0x0100 + data[i] - t;
            res.data[i] = (u8)(v & 0xff);
        }
        return res;
    }

    // TODO
    u512 operator%(const u512 &n) {
        u512 q, r;
        divide(*this, n, q, r);
        return r;
    }

    void operator=(const u512 &n) {
        for (int i = 0; i < 64; i++)
            data[i] = n.data[i];
    }

    bool operator==(const u512 &n) const {
        for (int i = 0; i < 64; ++i)
            if (data[i] != n.data[i]) 
                return false;
        return true;
    }

    bool operator!=(const u512 &n) const {
        for (int i = 0; i < 64; ++i)
            if (data[i] != n.data[i]) 
                return true;
        return false;
    }

    bool operator<(const u512 &n) const {
        for (int i = 63; i >= 0; --i) {
            if (data[i] < n.data[i]) return true;
            else if (data[i] > n.data[i]) return false;
        }
        return false;
    }

    bool operator>(const u512 &n) const {
        for (int i = 63; i >= 0; --i) {
            if (data[i] > n.data[i]) return true;
            else if (data[i] < n.data[i]) return false;
        }
        return false;
    }

    bool operator<=(const u512 &n) const {
        for (int i = 63; i >= 0; --i) {
            if (data[i] < n.data[i]) return true;
            else if (data[i] > n.data[i]) return false;
        }
        return true;
    }

    bool operator>=(const u512 &n) const {
        for (int i = 63; i >= 0; --i) {
            if (data[i] > n.data[i]) return true;
            else if (data[i] < n.data[i]) return false;
        }
        return true;
    }

    int operator&(int n) const {
        int res;
        u8 *pr = (u8*)&res;
        u8 *pn = (u8*)&n;
        for (int i = 0; i < 4; ++i)
            pr[i] = pn[i] & data[i];
        return res;
    }

    void operator&=(int n) {
        u8 *pn = (u8*)&n;
        for (int i = 0; i < 4; ++i) {
            data[i] &= pn[i];
        }
    }

    void operator|=(int n) {
        u8 *pn = (u8*)&n;
        for (int i = 0; i < 4; ++i) {
            data[i] |= pn[i];
        }
    }

    void operator>>=(int n) {
        int q = (n >> 3) & 0x3f;
        int r = n & 0x7;
        u8 mask = ((1 << r) - 1) & 0xff;
        u8 p, t;
        for (int i = 63, p = 0; i >= 0; --i, p = t << (8 - r)) {
            t = data[i] & mask; data[i] >>= r; data[i] |= p;
        }
        for (int i = 0; i < 64; ++i) {
            data[i] = i + q < 64? data[i + q]: 0;
        }
    }

    void operator<<=(int n) {
        int q = (n >> 3) & 0x3f;
        int r = n & 0x7;
        u8 mask = (((1 << r) - 1) << (8 - r)) & 0xff;
        u8 p, t;
        for (int i = 0, p = 0; i < 64; ++i, p = t >> (8 - r)) {
            t = data[i] & mask; data[i] <<= r; data[i] |= p;
        }
        for (int i = 63; i >= 0; --i) {
            data[i] = i - q >= 0? data[i - q]: 0;
        }
    }

    int clz() const {
        int count = 0;
        for (int i = 63; i >= 0; --i) {
            if (data[i] == 0) { 
                count += 8;
            } else {
                u8 v = data[i];
                while ((v & 0x80) == 0) { ++count; v <<= 1; }
                break;
            }
        }
        return count;
    }

    void shift(int clz) {
        int c = this->clz();
        if (c == clz) return;
        if (clz < c) {
            (*this)<<=(c - clz);
        } else {
            (*this)>>=(clz - c);
        }
    }

    void print() const {
        for (int i = 63; i >= 0; --i)
            printf("%02x", data[i]); printf("\n");
    }

    static u512 rand() {
        u512 res;
        for (int i = 0; i < 60; ++i)
            res.data[i] = std::rand() & 0xff;
        return res;
    }

    /**
     * divide
     * a / b = quot ... res
    */
    static void divide(u512 a, u512 b, u512 &quot, u512 &res) {
        // a.print(); b.print(); printf("\n");
        quot = 0; res = 0;
        if (a < b) { quot = 0; res = a; return; }
        int step = b.clz() - a.clz();
        // printf("step = %d\n", step);
        b <<= step;
        for (int _ = 0; _ <= step; _++) {
            if (a >= b) { quot |= 1; a = a - b; }
            quot <<= 1; b >>= 1;
        }   quot >>= 1;
        res = a;
    }
};


#endif
