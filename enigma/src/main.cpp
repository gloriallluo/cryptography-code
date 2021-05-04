#include "enigma.h"


Enigma enigma;
Setting *setting = NULL;
Circles circles;

int length = 37;
int circleNum = 0;
////                   0    5   10   15   20   25   30   35
char plain[MAX_LEN] = "THYHUNTRESSNAMETHATMYFULLLIFEDOTHSWAY";
char secret[MAX_LEN] = { 0 };
char decode[MAX_LEN] = { 0 };


/** Check Functions */

void checkRotorSetting(int r0, int r1, int r2, int pos) {
    if (r0 != setting->chosen[0] || 
        r1 != setting->chosen[1] || 
        r2 != setting->chosen[2] || 
        pos != setting->offset) {
        printf("\nCheck Failed!\n");
        return;
    }
    printf("\nCheck passed!\n");
    int carryFlag[5] = {digit('R'), digit('F'), digit('W'), digit('K'), digit('A')};
    int rotorPos[3] = { 0 };
    for (int i = 0; i < 3; i++) {
        rotorPos[i] = pos % N;
        pos /= N;
    }
    printf("First Rotor %d, Pos %c.\n", r0, letter(resi(rotorPos[0] + carryFlag[r0])));
    printf("Second Rotor %d, Pos %c.\n", r1, letter(resi(rotorPos[1] + carryFlag[r1])));
    printf("Third Rotor %d, Pos %c.\n", r2, letter(resi(rotorPos[2] + carryFlag[r2])));
    printf("\n");
}

void checkPlugboard(char *pb) {
    printf("\nDetected:\t");
    for (int i = 0; i < N; i++)
        printf("%c ", pb[i]? pb[i]: '?');
    printf("\nOrigin: \t");
    for (int i = 0; i < N; i++)
        printf("%c ", setting->plugBoard[i]? setting->plugBoard[i]: letter(i));
    printf("\n\n");
}


/** Find Circle */

#define SEARCH_LIMIT 20
char begin = 0;
char posStack[SEARCH_LIMIT] = { 0 };

/** 分别记录明密文对中的后继与对应位置
 * 描述一个加密变化：(a, p) -> b
 * 表示第 p 位的字母 a 经过加密变成 b */
// next[a][i] = b
std::vector<int> next[N];
// pos[a][i] = p
std::vector<int> pos[N];
// incl[a][i] = "(a, p) -> b" included
std::vector<int> incl[N];

/** 在明密文中搜索圈
 * @return 是否已经找到一个圈
 * @param now 现在搜索的字母
 * @param idx 本字母所在的 idx
 * @param depth 搜索深度 */
int dfsCircle(int now, int position, int depth) {
    if (depth >= SEARCH_LIMIT) return false;
    if (depth == 0) begin = letter(now);
    else posStack[depth - 1] = position;
    
    if (depth > 0 && letter(now) == begin) {
        // Success!
        bool inserted = circles.insert(begin, posStack, depth);
#ifdef DEBUG
        if (inserted) {
            printf("New Circle: %c: ", begin);
            for (int i = 0; i < depth; i++)
                printf("%d ", (int)posStack[i]); 
            printf("\n");
        }
#endif
        return 1;
    }
    int find = 0;
    int len = next[now].size();
    for (int i = 0; i < len; i++) { 
        // 搜索后继
        if (incl[now][i]) continue;
        incl[now][i] = true;
        find |= dfsCircle(next[now][i], pos[now][i], depth + 1);
        incl[now][i] = false;
    }
    return find & 1;
}

void findCircles() {
    for (int i = 0; i < length; i++) {
        next[digit(plain[i])].push_back(digit(secret[i]));
        pos[digit(plain[i])].push_back(i);
        incl[digit(plain[i])].push_back(0);
    }
    for (int i = 0; i < N; i++) {
        int len = next[i].size();
        dfsCircle(i, -1, 0); // 从此处开始寻找一个圈
    }
    circleNum = circles.cir.size();
}

char plug[N] = { 0 }; // plug board setting

/** 针对现在的 setting 搜索 plugboard 的设置
 * @return 当前 setting 满足所有的循环圈，plugboard 数量也与实际一致
 * @param depth 当前搜索深度，与一个 circle 对应
 * @param count 当前已确定的交换数量 */
int dfsPlug(int depth) {
    if (depth == circleNum) { 
        // Success!
#ifdef DEBUG
        checkPlugboard(plug);
#endif
        enigma.setPlugboard(plug);

        std::memset(decode, 0, MAX_LEN * sizeof(char));
        enigma.encrypt(decode, secret, length);
        printf("decode:\t%s\n", decode);
        printf("plain:\t%s\n", plain);
        return 1;
    }
    Circle c = circles.cir[depth];
    char head = c[0];
    int len = c.size();
    int find = 0;

    // `head` already swapped
    if (plug[digit(head)] != 0) {
        char now = plug[digit(head)];
        for (int i = 1; i < len; i++) 
            now = enigma.constEncrypt(now, c[i]); 
        if (now != plug[digit(head)]) return 0;
        return dfsPlug(depth + 1);
    }

    for (char initial = 'A'; initial <= 'Z'; initial++) {
        char now = initial;
        for (int i = 1; i < len; i++) 
            now = enigma.constEncrypt(now, c[i]); 
        if (now != initial) 
            continue; // try the next plugging

        // swap `initial` and `head`
        if (plug[digit(initial)] == 0) {
            plug[digit(initial)] = head;
            plug[digit(head)] = initial;
            find |= dfsPlug(depth + 1);
            plug[digit(initial)] = 0;
            plug[digit(head)] = 0;
        }
    }
    return find & 1;
}


int main() {
    setting = enigma.randSetting(); // new alloc memory
    enigma.encrypt(secret, plain, length);
    printf("secret:\t%s\n", secret);
    
    findCircles();
    enigma.unplug();

    // crack
    for (int pos = 0; pos < 26 * 26 * 26; pos++) {
        enigma.setOffset(pos);

        for (int r0 = 0; r0 < 5; r0++) {
            for (int r1 = 0; r1 < 5; r1++) {
                if (r1 == r0) continue;
                for (int r2 = 0; r2 < 5; r2++) {
                    if (r2 == r0 || r2 == r1) continue;

                    enigma.setRotors(r0, r1, r2);
                    std::memset(plug, 0, N * sizeof(char));
                    int res = dfsPlug(0);
#ifdef DEBUG
                    if (res) checkRotorSetting(r0, r1, r2, pos);
#endif
                }
            }
        }
    }
    if (setting) delete setting; 
    return 0;
}
