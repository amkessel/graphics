all:
	g++ -Wall -O3 -lglut -lGLU -o test test.cpp
	
clean:
	rm test
