all: main

# which compiler
CC = gcc

# where to install
INSTDIR = .

# where are include files kept
INCLUDE = .

# compier options for development
CFLAGS = -g -Wall -ansi

# compier options for release
# CFLAGS = -O -Wall -ansi

# local libraries
MYLIB = code.a

main: main.o $(MYLIB)
	$(CC) -o main main.o $(MYLIB)
$(MYLIB): $(MYLIB)(code.o)
main.o: main.c code.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c main.c
code.o: code.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c code.c

clean:
	-rm main.o code.o

install: main
	@if [ -d $(INSTDIR) ]; \
	then \
	cp main $(INSTDIR); \
	chmod a+x $(INSTDIR)/main; \
	chmod og-w $(INSTDIR)/main; \
	echo "Install in $(INSTDIR)"; \
	else \
	echo "Sorry, $(INSTDIR) does not exist"; \
	fi
