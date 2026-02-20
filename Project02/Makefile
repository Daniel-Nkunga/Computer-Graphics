# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
# LIB2 = ../libim/libim.a ../jpeg/libjpeg.a
CC = g++ -Wall -Wno-deprecated-declarations

pacman: src/main.cpp
	$(CC) -o pacman -I include src/* $(LIB)
	./pacman

compile: src/main.cpp
	$(CC) -o pacman -I include src/* $(LIB)
