s = 'myamraruyiqtenctorahroywdsoyeouarrgdernogw'

def decode(_s, m, n):
    # print(f'*** s: {_s} ***')
    # return
    k = [[0 for j in range(m)] for i in range(n)]
    count = 0
    reach_end = False
    for j in range(m):
        for i in range(n):
            k[i][j] = _s[count]
            count += 1
            if count == len(_s):
                reach_end = True
                break
        if reach_end:
            break
    count = 0
    # reach_end = False
    for i in range(n):
        for j in range(m):
            if k[i][j] == 0:
                continue
            print(k[i][j], end='')
            count += 1
            if count == len(_s):
                return

if __name__ == '__main__':
    for m in range(2, 8):
        for n in range(2, 8):
            begin = 0
            while begin < len(s):
                if begin + m * n <= len(s):
                    decode(s[begin: begin + m * n], m, n)
                else:
                    decode(s[begin:], m, n)
                # print()
                begin += m * n
            print()
