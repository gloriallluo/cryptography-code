#ifndef _ENIGMA_H_
#define _ENIGMA_H_

#include <iostream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <set>

#define N 26
#define MAX_LEN 60
#define MAX_CIRCLE_SIZE 15

#define digit(x) (x) - 'A'
#define letter(x) (x) + 'A'
#define resi(x) (x) % 26


/** Setting */

struct Setting {
public:
    int chosen[3] = { 0 };
    int offset = 0;
    char plugBoard[N] = { 0 };
    Setting();
};


/** Circles */

using Circle = std::vector<char>;

struct Circles {
public:
    std::vector<Circle> cir;
    bool isEqual(const Circle &a, char begin, char *poss, int len);
    bool insert(char begin, char *poss, int len);
};


/** Enigma */

struct Enigma {
    int plugBoard[N] = { 0 };
    int rotors[5][N] = {{ 0 }};
    int rotorsR[5][N] = {{ 0 }};
    int offset[3] = { 0 };
    int reflector[N] = { 0 };
    int chosen[3] = { 0 };

    char encrypt(char plain);
    void updateRotorPosition();

public:
    /** plug board */
    void unplug();

    /** rotors */
    void setRotors(int r0, int r1, int r2);
    void setOffset(int pos);
    void setPlugboard(char *pb);

    /** using */
    Enigma();
    void set(const Setting &s);
    Setting *randSetting();
    void encrypt(char *secret, char *plain, int length);
    char constEncrypt(char plain, int step);
};

#endif
