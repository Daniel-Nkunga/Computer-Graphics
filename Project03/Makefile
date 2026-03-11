# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
# LIB2 = ../libim/libim.a ../jpeg/libjpeg.a
CC = g++ -Wall -Wno-deprecated-declarations

terrain: src/main.cpp
	$(CC) -o terrain -I include src/* $(LIB)
	./terrain

compile: src/main.cpp
	$(CC) -o terrain -I include src/* $(LIB)
