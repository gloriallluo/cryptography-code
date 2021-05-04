secret = 'BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD'


for K in range(26):
    plain = ''.join([chr((ord(c) - ord('A') + K) % 26 + ord('A')) for c in secret])
    print(plain)
