# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
CC = g++ -Wall -Wno-deprecated-declarations

SHARED = src/ray_classes.cpp

ray_trace: src/ray_trace.cpp
	$(CC) -o ray_trace -I include -I . src/ray_trace.cpp $(SHARED) $(LIB)
	./ray_trace

compile: src/ray_trace.cpp
	$(CC) -o ray_trace -I include -I . src/ray_trace.cpp $(SHARED) $(LIB)

run:
	./ray_trace

ray_trace2: src/ray_trace2.cpp
	$(CC) -o ray_trace2 -I include -I . src/ray_trace2.cpp $(SHARED) $(LIB)
	./ray_trace2

ray_trace3: src/ray_trace3.cpp
	$(CC) -o ray_trace3 -I include -I . src/ray_trace3.cpp $(SHARED) $(LIB)
	./ray_trace3

cone: src/cone.cpp
	$(CC) -o cone -I include -I . src/cone.cpp $(SHARED) $(LIB)
	./cone

cube: src/cube.cpp
	$(CC) -o cube -I include -I . src/cube.cpp $(SHARED) $(LIB)
	./cube

cylinder: src/cylinder.cpp
	$(CC) -o cylinder -I include -I . src/cylinder.cpp $(SHARED) $(LIB)
	./cylinder

glass: src/glass.cpp
	$(CC) -o glass -I include -I . src/glass.cpp $(SHARED) $(LIB)
	./glass

mirror: src/mirror.cpp
	$(CC) -o mirror -I include -I . src/mirror.cpp $(SHARED) $(LIB)
	./mirror

all: src/ray_trace.cpp src/ray_trace2.cpp src/ray_trace3.cpp src/cone.cpp src/cube.cpp src/cylinder.cpp
	$(CC) -o ray_trace -I include -I . src/ray_trace.cpp $(SHARED) $(LIB)
	$(CC) -o ray_trace2 -I include -I . src/ray_trace2.cpp $(SHARED) $(LIB)
	$(CC) -o ray_trace3 -I include -I . src/ray_trace3.cpp $(SHARED) $(LIB)
	$(CC) -o cone -I include -I . src/cone.cpp $(SHARED) $(LIB)
	$(CC) -o cube -I include -I . src/cube.cpp $(SHARED) $(LIB)
	$(CC) -o cylinder -I include -I . src/cylinder.cpp $(SHARED) $(LIB)

clean:
	/bin/rm -f ray_trace ray_trace2 ray_trace3 cone cube cylinder