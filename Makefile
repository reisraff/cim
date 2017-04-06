CC = gcc -o
INSTALL = install

RM = rm -rf
MKDIR = mkdir

prefix= /usr/local

bindir= $(prefix)/bin

default: clean
	$(MKDIR) dist
	$(CC) dist/cim -Wall src/*.c

install:
	$(INSTALL) dist/cim $(bindir)/cim

clean:
	$(RM) dist src/*.o
