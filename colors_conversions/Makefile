CPP_FLAGS=
OPENCV_LIBS=-lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_text
LD_FLAGS=$(OPENCV_LIBS)
LIB=-L/usr/local/lib/

sources=$(wildcard src/*.cpp)
objects=$(patsubst src/%.cpp,release/%.o,$(sources))

release/%.o: src/%.cpp
	g++ $(CPP_FLAGS) -c $^ -o $@ $(LIB)
default: $(objects)
	g++ $(CPP_FLAGS) -o main $(objects) $(LD_FLAGS) $(LIB) -std=c++11 -g
clean:
	rm -rf release/* main
