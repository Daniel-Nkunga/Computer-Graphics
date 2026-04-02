# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
LIB2 = ../libim/libim.a ../jpeg/libjpeg.a
CC = g++ -Wall -Wno-deprecated-declarations

SHARED = src/cube.cpp

dice_scene: src/main.cpp
	$(CC) -o dice_scene -I include src/main.cpp $(SHARED) $(LIB)
	./dice_scene

compile: src/main.cpp
	$(CC) -o dice_scene -I include src/main.cpp $(SHARED) $(LIB)

run:
	./dice_scene

dice_test: src/dice_test.cpp
	$(CC) -o dice_test -I include src/dice_test.cpp $(SHARED) $(LIB)
	./dice_test