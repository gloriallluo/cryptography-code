build: main.cpp enigma.h enigma.cpp
	g++ main.cpp enigma.cpp -o enigma -std=c++14

dbuild: main.cpp enigma.h enigma.cpp
	g++ main.cpp enigma.cpp -o enigma -std=c++14 -D DEBUG

run: build
	./enigma

debug: dbuild
	./enigma
