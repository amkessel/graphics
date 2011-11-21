all:
	g++ -Wall -O3 -g -lglut -lGLU -c -o kutils.o ./project/kutils.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw.o ./project/kdraw.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw2d.o ./project/kdraw2d.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw3d.o ./project/kdraw3d.cpp
	g++ -Wall -O3 -lglut -lGLU kutils.o kdraw.o kdraw2d.o kdraw3d.o -o test test.cpp
	
clean:
	rm test
