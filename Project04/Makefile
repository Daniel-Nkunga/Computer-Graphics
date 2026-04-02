# define libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lGL -lGLU -lglut
endif
LIB2 = ../libim/libim.a ../jpeg/libjpeg.a
CC = g++ -Wall -Wno-deprecated-declarations

camera_issues: src/main.cpp
	$(CC) -o camera_issues -I include src/* $(LIB)
	./camera_issues

compile: src/main.cpp
	$(CC) -o camera_issues -I include src/* $(LIB)

run : src/main.cpp
	./camera_issues
