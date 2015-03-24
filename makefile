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

CCFLAGS+=-Wall -pedantic -g
LDFLAGS+=-lSDL2 -lGL -lGLU -lpthread

EXEC=main
BINDIR=/usr/bin
SRCDIR=src

OBJECTS=$(SRCDIR)/main.o $(SRCDIR)/sdl.o $(SRCDIR)/render.o $(SRCDIR)/draw.o

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LD) -o bin/$(EXEC) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CCFLAGS)

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
