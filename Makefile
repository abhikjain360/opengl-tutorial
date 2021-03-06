CXX 	= g++
CFLAGS 	= -I . -g -pthread -Wpedantic -std=c++17
LIBS 	= -lGLEW -lGL -lglfw

SRC 	= $(wildcard *.cpp)


app: $(SRC)
	$(CXX) $(CFLAGS) $^ $(LIBS) -o app

run:
	./app

debug:
	gdb ./app

clean:
	rm -f app
