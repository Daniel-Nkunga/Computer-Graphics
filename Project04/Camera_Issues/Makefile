# define libraries
ifeq ($(shell uname),Darwin)
    LIB = -DMAC -framework OPENGL -framework GLUT
else
    LIB = -lGL -lGLU -lglut
endif
LIB2 = ../libim/libim.a ../jpeg/libjpeg.a
CC = g++ -Wall -Wno-deprecated-declarations

.PHONY: all run

all: bad_camera fixed_camera

bad_camera: src/bad_camera.cpp
	$(CC) -o bad_camera -I include src/bad_camera.cpp $(LIB)

fixed_camera: src/fixed_camera.cpp
	$(CC) -o fixed_camera -I include src/fixed_camera.cpp $(LIB)

run: all
	./bad_camera & ./fixed_camera