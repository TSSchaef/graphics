#Makefile
BINARY=graphics
DEV-BINARY=graphics-dev
CODEDIRS=src
INCDIRS=src ext 

CC=gcc
#use DEV options for development
#PROD for production
DEV-OPT=-O0
PROD-OPT=-O3
LIBFLAGS=-lm -lpthread -lSDL2 -lSDL2_ttf -lSDL2_image
DEPFLAGS=-MP -MD
DEV-CFLAGS=-Wall -Werror -g $(foreach D, $(INCDIRS), -I$(D)) $(DEV-OPT) $(DEPFLAGS)
PROD-CFLAGS=$(foreach D, $(INCDIRS), -I$(D)) $(PROD-OPT) $(DEPFLAGS)

CFILES=	$(foreach D,$(CODEDIRS), $(wildcard $(D)/*.c))
OBJECTS= $(patsubst %.c, %.o, $(CFILES)) 
DEV-OBJECTS= $(patsubst %.c, %.dev.o, $(CFILES)) 
DEPFILES= $(patsubst %.c, %.d, $(CFILES)) 
DEV-DEPFILES= $(patsubst %.c, %.dev.d, $(CFILES)) 

# Default used for production
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

%.o: %.c
	$(CC) $(PROD-CFLAGS) -c -o $@ $<



# Used for development builds, has debugging enabled
dev: $(DEV-BINARY)

$(DEV-BINARY): $(DEV-OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

%.dev.o: %.c
	$(CC) $(DEV-CFLAGS) -c -o $@ $<




.PHONY: clean

clean:
	@rm -f $(OBJECTS) $(DEPFILES) $(BINARY) $(DEV-OBJECTS) $(DEV-DEPFILES) $(DEV-BINARY) perf.data*

diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

-include $(DEPFILES)
