#  List of all programs you want to build
EX=ex10 ex11 ex12

#  Libraries - Linux
LIBS=-lglut -lGLU
#  Libraries - OSX
#LIBS=-framework GLUT -framework OpenGL
#  Libraries - MinGW
#LIBS=-lglut32cu -lglu32 -lopengl32

#  Main target
all: $(EX)

#  Generic compile rules
.c.o:
	gcc -c -O -Wall $<

#  Generic compile and link
%: %.c CSCIx229.a
	gcc -Wall -O3 -o $@ $^ $(LIBS)

#  Delete unwanted files - Linux or OS/X
clean:;rm -f $(EX) *.o *.a
#  Delete unwanted files - MinGW
#clean:;del *.exe *.o *.a

#  Create archive (include glWindowPos here if you need it)
CSCIx229.a:fatal.o loadtexbmp.o print.o project.o errcheck.o object.o
	ar -rcs CSCIx229.a $^

#  Obligatory UNIX inside joke
love:
	@echo "not war?"
