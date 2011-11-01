all:
	g++ -Wall -O3 -g -lglut -lGLU -c -o loadtexbmp.o loadtexbmp.c
	g++ -Wall -O3 -g -lglut -lGLU -c -o fatal.o fatal.c
	g++ -Wall -O3 -g -lglut -lGLU -c -o errcheck.o errcheck.c
	g++ -Wall -O3 -g -lglut -lGLU -c -o kutils.o kutils.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw.o kdraw.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw2d.o kdraw2d.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o kdraw3d.o kdraw3d.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o falcon.o falcon.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o sheet.o sheet.cpp
	g++ -Wall -O3 -g -lglut -lGLU -c -o planets.o planets.cpp
	g++ -Wall -O3 -g -lglut -lGLU errcheck.o fatal.o loadtexbmp.o kutils.o kdraw.o kdraw2d.o kdraw3d.o falcon.o sheet.o planets.o -o kessel_project main.cpp
	
clean:
	rm *.o
	rm kessel_project
