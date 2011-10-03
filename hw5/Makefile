all:
	g++ -Wall -O3 -g -lglut -lGLU -c -o kutils.o kutils.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw.o kdraw.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw2d.o kdraw2d.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw3d.o kdraw3d.cpp
	g++ -Wall -O3 -g -lglut -lGLU kutils.o kdraw.o kdraw2d.o kdraw3d.o -o kessel_hw5 main.cpp
	
clean:
	rm kessel_hw5
	rm *.o
