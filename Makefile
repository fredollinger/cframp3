MAIN = cframp3
SDL_INCLUDES = /usr/include/SDL
SDL_LIBS = /usr/lib/
CPP=gcc
#CPP=gcc
#CPP=tcc
#CPP=ccache gcc
STYLE=--style=gnu
SDLLFLAGS=`sdl-config --libs` -lSDL_mixer -lSDL
SDLCFLAGS=`sdl-config --cflags` 
TFLAGS=-lpthread
PCREFLAGS=`pcre-config --libs`
CDAUFLAGS=`libcdaudio-config --cflags --libs`
GLIBFLAGS=`pkg-config --cflags --libs glib-2.0`
CFLAGS=-lreadline -lpthread -lhistory  -lmagic -ggdb -Wshadow -O3 -Werror $(TFLAGS) $(PCREFLAGS) $(CDAUFLAGS) $(GLIBFLAGS) $(SDLCFLAGS)  $(SDLLFLAGS)
OBJS=cfram3lib.o framplayer.o cframp3engine.o cframp3getc.o

cframp3: cfram3lib.o framplayer.o cframp3engine.o cframp3getc.o
	$(CPP) -o $(MAIN) $(MAIN).c $(OBJS) $(CFLAGS) 

str: cfram3lib.o 
	$(CPP) $(CFLAGS) -o strtest.o strtest.c
	$(CPP) -o strtest  cfram3lib.o strtest.o 

%.o: %.c
	        $(CC) $(CFLAGS) -c $<

test:
	gdb ./$(MAIN)
clean:
	rm -rf *.o doc autoscan.log autom4te.cache config.h config.log config.status $(MAIN).o $(MAIN) issong

edit:
	vim $(MAIN).c 
style:
	astyle $(STYLE) $(MAIN).cpp
	astyle $(MAIN).h
documentation:
	doxygen $(MAIN).cfg
kill:
	ps aux | grep cframp3
	killall cframp3
	rm -fv /tmp/file*
	killall mplayer
install:
	cp -v cframp3 ~/bin/

issong: issongtest.o cfram3lib.o 
	$(CPP) -o issong $(CFLAGS) cfram3lib.o issongtest.o 

hello:
				$(CC) -o hello -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include  -lglib-2.0 hello.c
