EXEC=cglcube

EXECCMD=exec
DBUGCMD=valgrind

RM=/usr/bin/rm -rf
CP=/usr/bin/cp
ECHO=/usr/bin/echo
MKDIR=/usr/bin/mkdir -p

DOXYGEN=doxygen
DOXYGEN_CONF=./doc/doxygen.conf

BINDIR=./bin
SRCDIR=./src

OBJECTS=$(SRCDIR)/main.o $(SRCDIR)/sdl.o $(SRCDIR)/render.o $(SRCDIR)/draw.o \
		$(SRCDIR)/camera.o $(SRCDIR)/text.o $(SRCDIR)/std.o

LINUX_CC=gcc
LINUX_LD=gcc
LINUX_CCFLAGS=-I/usr/include/SDL2 -I/usr/include -D_REENTRANT \
              -Wall -pedantic -g
LINUX_LDFLAGS=-L/usr/lib -lm -lpthread -lSDL2 -lSDL2_ttf -lGL -lGLU -lGLEW

WIN32_CC=i686-w64-mingw32-gcc
WIN32_LD=i686-w64-mingw32-gcc
WIN32_LIBS_LOC=/usr/i686-w64-mingw32
WIN32_LIBS=SDL2.dll,SDL2_ttf.dll,libwinpthread-1.dll,libfreetype-6.dll,glew32.dll
WIN32_CCFLAGS=-I$(WIN32_LIBS_LOC)/include/SDL2 -Dmain=SDL_main \
              -Wall -pedantic -g -static
WIN32_LDFLAGS=-L$(WIN32_LIBS_LOC)/lib -lmingw32 -mwindows -lwinpthread \
              -lSDL2main -lSDL2 -lSDL2_ttf -lopengl32 -lglu32 -lglew32 -lm

ifndef $(TARGET)
	ifeq ($(TARGET), linux)
		CC=$(LINUX_CC)
		LD=$(LINUX_LD)
		CCFLAGS=$(LINUX_CCFLAGS)
		LDFLAGS=$(LINUX_LDFLAGS)
	else ifeq ($(TARGET), win32)
		CC=$(WIN32_CC)
		LD=$(WIN32_LD)
		CCFLAGS=$(WIN32_CCFLAGS)
		LDFLAGS=$(WIN32_LDFLAGS)
	else
		CC=$(LINUX_CC)
		LD=$(LINUX_LD)
		CCFLAGS=$(LINUX_CCFLAGS)
		LDFLAGS=$(LINUX_LDFLAGS)
	endif
endif


all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LD) -o bin/$(EXEC) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CCFLAGS)

linux:
	make TARGET=linux

win32:
	make TARGET=win32
	@$(CP) $(WIN32_LIBS_LOC)/bin/{$(WIN32_LIBS)} $(BINDIR)/

docs:
	$(DOXYGEN) $(DOXYGEN_CONF)

mkdir:
	@$(MKDIR) $(BINDIR)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(BINDIR)/*.dll
	$(RM) $(BINDIR)/$(EXEC)

run:
	$(EXECCMD) $(BINDIR)/$(EXEC)

debug:
	$(DBUGCMD) $(BINDIR)/$(EXEC)
