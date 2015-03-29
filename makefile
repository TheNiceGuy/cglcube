ifndef $(CC)
	CC=gcc
endif

ifndef $(LD)
	LD=gcc
endif

ifndef $(DB)
	DB=valgrind
endif

ifndef $(EXECCMD)
	EXECCMD=exec
endif

DOXYGEN=doxygen
DOXYGEN_CONF=./doc/doxygen.conf
CCFLAGS+=-Wall -pedantic -g
LDFLAGS+=-lm -lpthread -lSDL2 -lSDL2_ttf -lGL -lGLU -lGLEW

EXEC=main
BINDIR=/usr/bin
SRCDIR=src

OBJECTS=$(SRCDIR)/main.o $(SRCDIR)/sdl.o $(SRCDIR)/render.o $(SRCDIR)/draw.o \
		$(SRCDIR)/camera.o $(SRCDIR)/text.o $(SRCDIR)/std.o

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LD) -o bin/$(EXEC) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CCFLAGS)

docs:
	$(DOXYGEN) $(DOXYGEN_CONF)

mkdir:
	@mkdir -p bin

clean:
	rm -rf $(OBJECTS)

run:
	$(EXECCMD) ./bin/$(EXEC)

debug:
	$(DB) bin/$(EXEC)

install:
	cp bin/$(EXEC) $(BINDIR)/$(EXEC)

uninstall:
	rm $(BINDIR)/$(EXEC)
