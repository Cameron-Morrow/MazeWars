S := $(shell uname)
ifeq ($(OS),Darwin)
CFLAGS = -I/usr/local/lib  -I/usr/include/malloc -g

##LIB    = ./libggfonts.so
##required for Arch linux: /usr/lib/libopenal.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -l/usr/local/lib\
		 libalut.a -L/usr/include/malloc\
		 #-lXrandr
else
CFLAGS = -I ./include -g

##LIB    = ./libggfonts.so
##required for Arch linux: /usr/lib/libopenal.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm\
			#-lXrandr
endif

##CFLAGS = -I ./include -g

LIB    = ./lib/libggfonts.a
CPPFILES = mazewars.cpp textrender.cpp 
##required for Arch linux: /usr/lib/libopenal.so
##LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: clean fonts.o fonttex.o staticLib mazewars

fonts.o: fonts/fonts.cpp fonts/fonts.h defs.h
	g++ -c -Wall -Wextra fonts/fonts.cpp -I/usr/include/malloc -I/opt/X11/include
fonttex.o: fonts/fonttex.cpp
	g++ -c -Wall -Wextra fonts/fonttex.cpp

#makelib: fonts/fonts.o fonts/fonttex.o fonts/fonts.h
#	g++ -shared -o $(MYLIB) fonts/fonts.o fonts/fonttex.o


## Creating static library twice?
## If you build the file while it already exists, it grows larger.
## But, you cannot remove the file if it is not there.
## So, create it, remove it, then create it.
staticLib: fonts.o fonttex.o
	ar -cvq libggfonts.a fonts.o fonttex.o
	rm libggfonts.a
	ar -cvq libggfonts.a fonts.o fonttex.o


mazewars: mazewars.cpp ppm.c log.c defs.h fonts/fonts.h fonts/fonts.o fonts/fonttex.o
	g++ $(CFLAGS) cameronM.cpp matthewG.cpp davidV.cpp ppm.c jobG.cpp roseP.cpp mtime.cpp fonttex.o fonts/fonts.cpp $(CPPFILES) $(LIB) log.c\
			/usr/lib/x86_64-linux-gnu/libopenal.so /usr/lib/libalut.so\
			joystick.cc -Wall -std=c++11 -Wextra -w $(LFLAGS)\
			-o MazeWars

clean:
	rm -f MazeWars
	rm -f x.x
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f fonts/*.o

update:
	git add -A
	git commit -m "autoupdate"
	git push

pullnmake:
	make clean
	git pull
	make
	./MazeWars
