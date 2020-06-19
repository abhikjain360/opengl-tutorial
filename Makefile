CXX 	= g++
CFLAGS 	= -I -g -pthread -Wpedantic
LIBS 	= -lGLEW -lGL -lglfw


app: app.cpp
	$(CXX) $(CFLAGS) $@.cpp $(LIBS) -o $@ && ./app && make clean

clean:
	rm -f app
