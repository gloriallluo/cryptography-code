aes: aes128.h aes128.cpp
	@g++ aes128.cpp -o main -std=c++11 -O1 && ./main

t-aes: aes128.h aes128.cpp
	@g++ aes128.cpp -o main -std=c++11 -D TEST && ./main

rc4: rc4.cpp
	@g++ rc4.cpp -o main -std=c++11 -O1 && ./main

t-rc4: rc4.cpp
	@g++ rc4.cpp -o main -std=c++11 -D TEST && ./main

sha3: sha256.h sha256.cpp
	@g++ sha256.cpp -o main -std=c++11 -O1 && ./main

t-sha3: sha256.h sha256.cpp
	@g++ sha256.cpp -o main -std=c++11 -D TEST && ./main

cr: main
	@rm main
