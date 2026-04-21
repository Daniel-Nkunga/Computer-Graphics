# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
LIB2 = libim/libim.a jpeg/libjpeg.a

SHARED = src/ray_classes.cpp

# define makefile targets
CC = g++ -Wall -Wno-deprecated-declarations
BIN = ray_trace ray_trace2 ray_trace3


ray_trace: src/ray_trace.cpp
	$(CC) -o ray_trace -i include . src/ray_trace.cpp $(SHARED) $(LIB) $(LIB2)


all: $(BIN)

clean:
	/bin/rm -f $(BIN)

# ray_trace: src/ray_trace.cpp src/ray_classes.cpp
# 	$(CC) -o ray_trace src/ray_trace.cpp src/ray_classes.cpp $(LIB)

ray_trace2: src/ray_trace2.cpp src/ray_classes.cpp
	$(CC) -o ray_trace2 src/ray_trace2.cpp src/ray_classes.cpp $(LIB)

ray_trace3: src/ray_trace3.cpp src/ray_classes.cpp
	$(CC) -o ray_trace3 src/ray_trace3.cpp src/ray_classes.cpp $(LIB)

cone: src/cone.cpp src/ray_classes.cpp
	$(CC) -o cone src/cone.cpp src/ray_classes.cpp $(LIB)