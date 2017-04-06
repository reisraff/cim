CC = gcc -o
INSTALL = install

RM = rm -rf
MKDIR = mkdir

prefix= /usr/local

bindir= $(prefix)/bin

default:
	$(RM) dist
	$(MKDIR) dist
	$(CC) dist/cim src/cim.c

install:
	$(INSTALL) dist/cim $(bindir)/cim

clean:
	$(RM) dist
