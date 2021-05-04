import re
import numpy as np

# 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
# A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z

# s = 'KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST'
s = 'BNVSNSIHQCEELSSKKYERIFJKXUMBGYKAMQLJTYAVFBKVTDVBPVVRJYYLAOKYMPQSCGDLFSRLLPROYGESEBUUALRWXMMASAZLGLEDFJBZAVVPXWICGJXASCBYEHOSNMULKCEAHTQOKMFLEBKFXLRRFDTZXCIWBJSICBGAWDVYDHAVFJXZIBKCGJIWEAHTTOEWTUHKRQVVRGZBXYIREMMASCSPBHLHJMBLRFFJELHWEYLWISTFVVYEJCMHYUYRUFSFMGESIGRLWALSWMNUHSIMYYITCCQPZSICEHBCCMZFEGVJYOCDEMMPGHVAAUMELCMOEHVLTIPSUYILVGFLMVWDVYDBTHFRAYISYSGKVSUUHYHGGCKTMBLRX'

def guess_the(piece):
    begin = 0
    pos = []
    while begin != -1:
        begin = s.find(piece, begin + 3)
        if begin != -1:
            pos.append(begin)
    if (pos[1] - pos[0]) % 6 != 0:
        return
    print(f'{piece}')
    secr = 'THE'
    k = [0 for _ in range(6)]
    for i in range(3):
        ps = pos[0] + i
        k[ps % 6] = (ord(secr[i]) + 26 - ord(piece[i])) % 26
    print(k)
    decode(k)


def find_all():
    for i in range(len(s) - 2):
        r = re.findall(s[i:i+3], s)
        if len(r) > 1:
            guess_the(s[i:i+3])
            # print(r)


def find_spec(piece):
    begin = 0
    print(f'{piece}: ', end=' ')
    print(begin, end=' ')
    while begin != -1:
        begin = s.find(piece, begin + 3)
        print(begin, end=' ')
    print()

def decode(key=[24, 9, 2, 11, 7, 12]):
    for i, c in enumerate(s):
        # print(chr(ord(c)))
        print('%c' % chr((ord(c) - ord('A') + key[i % 6]) % 26 + ord('a')), end='')
    print()

def single_stat():
	for c in range(26):
		print(chr(c + ord('A')) + ' %.3f' % (s.count(chr(c + ord('A')))/len(s)))
	print()

def simple_decode():
    simple = 'KQEREJEBCPPCJCRKIEACUZBKRVPKRBCIBQCARBJCVFCUPKRIOFKPACUZQEPBKRXPEIIEABDKPBCPFCDCCAFIEABDKPBCPFEQPKAZBKRHAIBKAPCCIBURCCDKDCCJCIDFUIXPAFFERBICZDFKABICBBENEFCUPJCVABPCYDCCDPKBCOCPERIVKSCPICBRKIJPKABI'
    for c in simple:
        print('%c' % chr(((ord(c) - ord('A')) * 11 + 8) % 26 + ord('a')), end='')
    print()

def analysis(sep):
    for i in range(sep):
        k = np.zeros(26)
        for j in range(i, sep):
            k[ord(s[j]) - ord('A')] += 1
        res = (k * (k - 1)).sum() / ((k.sum() - 1) * k.sum())
        print('%.4f' % res, end=' ')
    print()

if __name__ == '__main__':
    # single_stat()
    decode([7, 19, 22, 12, 9, 2])
    # find_all()
    # find_spec('AVF')
    # find_spec('VVR')
    # find_spec('GES')
    # find_spec('MMA')
    # find_spec('MAS')
    # find_spec('SIC')
    # find_spec('MBL')
    # find_spec('WDV')
    # find_spec('DVY')
    # decode()
    # simple_decode()
