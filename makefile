default :
	g++ src/*.cpp -o bin/emulator-release -DNDEBUG -O -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17 --verbose