CXX 	= g++
CFLAGS 	= -I -g -pthread -Wpedantic
LIBS 	= -lGLEW -lGL -lglfw


app: app.cpp
	$(CXX) $(CFLAGS) $@.cpp $(LIBS) -o $@ && MESA_DEBUG=1 ./app
clean:
	rm -f app
