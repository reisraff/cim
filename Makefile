CC=gcc
INSTALL=install

RM=rm -rf
MKDIR=mkdir
CD=cd
MV=mv
LN=ln -s

prefix=/usr/local

bindir=$(prefix)/bin
include=include
readline_lib_dir=$(include)/readline
ncurses_lib_dir=$(include)/ncurses
dist=dist
src=src

default: clean
	$(CD) $(readline_lib_dir) && ./configure && $(MAKE)
	$(CD) $(ncurses_lib_dir) && ./configure --with-shared && $(MAKE)
	$(CD) $(readline_lib_dir)/shlib/ && $(LN) libreadline.so.7.0 libreadline.so
	$(CD) $(readline_lib_dir)/shlib/ && $(LN) libhistory.so.7.0 libhistory.so
	$(MKDIR) $(dist)
	$(CC) $(src)/*.c -o $(dist)/cim -Wall -I$(include) \
		-L$(ncurses_lib_dir)/lib -lncurses \
		-L$(readline_lib_dir)/shlib -lreadline

install:
	$(INSTALL) $(dist)/cim $(bindir)/cim

clean:
	$(RM) $(readline_lib_dir)/shlib/*.so
	$(RM) $(dist) $(src)/*.o
