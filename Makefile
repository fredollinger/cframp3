MAIN = cframp3
SDL_INCLUDES = /usr/include/SDL
SDL_LIBS = /usr/lib/
CPP=gcc
#CPP=gcc
#CPP=tcc
#CPP=ccache gcc
STYLE=--style=gnu
SDLFLAGS = `sdl-config --cflags --libs` -lSDL_mixer
SDLCFLAGS = `sdl-config --cflags` 
TFLAGS = -lpthread
PCREFLAGS = `pcre-config  --libs`
CDAUFLAGS = `libcdaudio-config --libs`
GLIBFLAGS=`pkg-config --cflags --libs glib-2.0`
CFLAGS=-lreadline -lhistory  -lmagic -ggdb -Wshadow -O3  $(SDLFLAGS)  \
$(TFLAGS) $(PCREFLAGS) $(CDAUFLAGS) $(GLIBFLAGS)

all:
	make $(MAIN).o
	make $(MAIN)

str: cfram3lib.o 
	$(CPP) $(CFLAGS) -o strtest.o strtest.c
	$(CPP) -o strtest  cfram3lib.o strtest.o 

main:
	make $(MAIN)
cframp3: $(MAIN).o cfram3lib.o framplayer.o cframp3engine.o cframp3getc.o
	$(CPP) -o $(MAIN) $(CFLAGS) cfram3lib.o framplayer.o cframp3engine.o cframp3getc.o $(MAIN).o 
cframp3.o: 
	$(CPP) -c cframp3.c  
cframp3engine.o:
	$(CPP) -c cframp3engine.c $(TCFLAGS) 
framplayer.o:
	$(CPP) -c framplayer.c $(SDLCFLAGS)
lib: 
	rm -f cfram3lib.o
	make cfram3lib.o
cfram3lib.o:
	$(CPP) -c cfram3lib.c $(GLIBFLAGS)
framp3_engine.o:
	$(CPP) -c framp3_engine.c
test:
	gdb ./$(MAIN)
clean:
	rm -rf *.o doc autoscan.log autom4te.cache config.h config.log config.status $(MAIN).o $(MAIN) issong

realclean:
	make clean
	rm -f Makefile
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

issongtest.o:
	 $(CPP) -c issongtest.c

engtest.o: 
	$(CPP) -c engtest.c
engtest: cfram3lib.o framplayer.o engtest.o cframp3engine.o
	$(CPP) -o engtest $(CFLAGS) engtest.o framplayer.o cframp3engine.o cfram3lib.o

cframp3getc.o: 
	$(CPP) -c cframp3getc.c
