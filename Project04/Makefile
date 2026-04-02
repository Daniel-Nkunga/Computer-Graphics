# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
LIB2 = ../libim/libim.a ../jpeg/libjpeg.a
CC = g++ -Wall -Wno-deprecated-declarations

dice_scene: src/main.cpp
	$(CC) -o dice_scene -I include src/* $(LIB)
	./dice_scene

compile: src/main.cpp
	$(CC) -o dice_scene -I include src/* $(LIB)

run : src/main.cpp
	./dice_scene
