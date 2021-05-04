#include "enigma.h"


Setting::Setting() {
    chosen[0] = 0; chosen[1] = 1; chosen[2] = 2;
    offset = 0;
}

bool Circles::isEqual(const Circle &a, char begin, char *poss, int len) {
    if (a[0] != begin) return false;
    if (a.size() != len + 1) return false;
    for (int i = 0; i < len; i++)
        if (poss[i] != a[i + 1])
            return false;
    return true;
}

bool Circles::insert(char begin, char *poss, int len) {
    for (Circle c: cir) {
        if (isEqual(c, begin, poss, len))
            return false;
    }
    Circle new_cir;
    new_cir.push_back(begin);
    for (int i = 0; i < len; i++)
        new_cir.push_back(poss[i]);
    cir.push_back(new_cir);
    return true;
}

Enigma::Enigma() {
    // plug board
    for (int i = 0; i < N; i++)
        plugBoard[i] = i;
    
    // rotors
    for (int i = 0; i < N; i++) {
        /////  *************  ABCDEFGHIJKLMNOPQRSTUVWXYZ  ***
        rotors[0][i] = digit("EKMFLGDQVZNTOWYHXUSPAIBRCJ"[i]);
		rotors[1][i] = digit("AJDKSIRUXBLHWTMCQGZNPYFVOE"[i]);
		rotors[2][i] = digit("BDFHJLCPRTXVZNYEIWGAKMUSQO"[i]);
		rotors[3][i] = digit("ESOVPZJAYQUIRHXLNFTGKDCMWB"[i]);
		rotors[4][i] = digit("VZBRGITYUPSDNHLXAWMJQOFECK"[i]);
    }

    // rotorsR
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < N; j++) {
            int pos = rotors[i][j];
            rotorsR[i][pos] = j;
        }
    }
    
    // reflect
    for (int i = 0; i < N; i++)
        /////  *************  ABCDEFGHIJKLMNOPQRSTUVWXYZ  ***
        reflector[i] = digit("YRUHQSLDPXNGOKMIEBFZCWVJAT"[i]);
}

char Enigma::encrypt(char plain) {
    updateRotorPosition();
    // plug board
    int a = plugBoard[digit(plain)];
    // rotors
    for (int i = 0; i < 3; i++) {
        a = rotors[chosen[i]][resi(a + offset[i])];
    }
    // reflector
    a = reflector[a];
    // rotors
    for (int i = 2; i >= 0; i--) {
        a = resi(rotorsR[chosen[i]][a] + N - offset[i]);
    }
    // plug board
    a = plugBoard[a];
    
    return letter(a);
}

void Enigma::encrypt(char *secret, char *plain, int length) {
    for (int i = 0; i < length; i++)
        secret[i] = encrypt(plain[i]);
}

char Enigma::constEncrypt(char plain, int step) {
    int oldPos = offset[2] * N * N + offset[1] * N + offset[0];
    for (int _ = 0; _ < step; _++)
        updateRotorPosition();
    char secret = encrypt(plain);
    setOffset(oldPos);
    return secret;
}

void Enigma::set(const Setting &s) {
    this->setRotors(s.chosen[0], s.chosen[1], s.chosen[2]);
    this->setOffset(s.offset);
    for (int i = 0; i < N; i++)
        this->plugBoard[i] = s.plugBoard[i] != 0? 
            (int)digit(s.plugBoard[i]): i;
}

Setting* Enigma::randSetting() {
    Setting *set = new Setting();
    // select rotors
    int sel[5] = {0, 1, 2, 3, 4};
    shuffle(sel, sel + 5, std::default_random_engine(12345));
    for (int i = 0; i < 3; i++)
        set->chosen[i] = sel[i];
    // offset
    set->offset = rand() % (26 * 26 * 26);
    // plugboard
    char pb[N] = { 0 };
    for (int i = 0; i < N; i++) pb[i] = letter(i);
    shuffle(pb, pb + N, std::default_random_engine(5748209)); // 5748209, 8
    for (int i = 0; i < 10; i++) {
        set->plugBoard[digit(pb[2 * i])] = pb[2 * i + 1];
        set->plugBoard[digit(pb[2 * i + 1])] = pb[2 * i];
    }
    this->set(*set);
    return set;
}

void Enigma::unplug() {
    for (int i = 0; i < 26; i++)
        plugBoard[i] = i;
}

void Enigma::updateRotorPosition() {
    bool carry = false;
    int carryFlag[5] = {digit('R'), digit('F'), digit('W'), digit('K'), digit('A')};
    carry = (offset[0] = resi(offset[0] + 1)) == carryFlag[chosen[0]];
    if (!carry) return;
    carry = (offset[1] = resi(offset[1] + 1)) == carryFlag[chosen[1]];
    if (!carry) return;
    offset[2] = resi(offset[2] + 1);
}

void Enigma::setRotors(int r0, int r1, int r2) {
    chosen[0] = r0;
    chosen[1] = r1;
    chosen[2] = r2;
}

void Enigma::setOffset(int pos) {
    pos %= (N * N * N);
    offset[0] = pos % N;
    pos /= N;
    offset[1] = pos % N;
    pos /= N;
    offset[2] = pos % N;
}

void Enigma::setPlugboard(char *pb) {
    for (int i = 0; i < N; i++)
        plugBoard[i] = pb[i]? digit(pb[i]): i;
}