# Makefile to build the SDL tests

srcdir  = .

CC      = gcc
EXE	= .exe
CFLAGS  = -g -O2 -I/usr/local/include/SDL -I/usr/local/include -D_GNU_SOURCE=1 -D_THREAD_SAFE -DHAVE_OPENGL
LIBS	= -L/usr/local/lib -L/usr/local/lib/x86 -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer 
#LIBS    = -lz -L/usr/local/lib /usr/local/lib/libSDLmain.a /usr/local/lib/libSDL.a /usr/local/lib/libSDL_image.a /usr/local/lib/libSDL_ttf.a /usr/local/lib/libfreetype.a /usr/local/lib/libpng.a /usr/local/lib/libbz2.a -Wl,-framework,OpenGL -Wl,-framework,Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit

main: $(srcdir)/main.c
	$(CC) -o $@ $? $(CFLAGS) $(LIBS) -Wl,-framework,Cocoa

winmain: $(srcdir)/main.c
	$(CC) -o $@$(EXE) $? $(CFLAGS) -lmingw32 $(LIBS)

SpaceFighter: $(srcdir)/main.c
	$(CC) -o $@ $? $(CFLAGS) $(LIBS) -Wl,-framework,Cocoa

winSpaceFighter: $(srcdir)/main.c
	$(CC) -o SpaceFighter.exe $? $(CFLAGS) -lmingw32 $(LIBS)

clean:
	rm -f main
	rm -f SpaceFighter
	rm -f winmain.exe
	rm -f SpaceFighter.exe

winclean:
	rm main
	rm SpaceFighter
	rm winmain.exe
	rm SpaceFighter.exe


