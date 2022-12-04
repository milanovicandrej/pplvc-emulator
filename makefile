debug:
	echo "Building the version with debug symbols"
	g++ src/*.cpp -o bin/emulator-debug -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

release:
	g++ /src/*.cpp -o /bin/emulator-release -DNDEBUG -O -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17