CC=gcc
INSTALL=install

RM=rm -rf
MKDIR=mkdir
CD=cd
MV=mv

prefix=/usr/local

bindir=$(prefix)/bin
readline_lib_dir=include/readline
lib_dir=./.libs
dist=dist
src=src

default: clean
	$(CD) $(readline_lib_dir) && ./configure && $(MAKE)
	$(MKDIR) $(lib_dir)
	$(MV) $(readline_lib_dir)/shlib/*.so* $(lib_dir)
	$(MKDIR) $(dist)
	$(CC) $(src)/*.c -o $(dist)/cim -Wall -I./include -L./$(lib_dir) -lreadline
	$(CD) $(readline_lib_dir) && $(MAKE) clean

install:
	$(INSTALL) $(dist)/cim $(bindir)/cim

clean:
	$(RM) $(lib_dir)
	$(RM) $(dist) $(src)/*.o
	$(CD) $(readline_lib_dir) && $(MAKE) clean
