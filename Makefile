SHELL := /bin/bash
CC=gcc
INSTALL=install

RM=rm -rf
MKDIR=mkdir
CD=cd
MV=mv
LN=ln -s

prefix=/usr/local
lib_path=/usr/lib

bindir=$(prefix)/bin
include=include
readline_lib_dir=$(include)/readline
ncurses_lib_dir=$(include)/ncurses
dist=dist
src=src

default: prepare build

prepare: clean
	$(CD) $(ncurses_lib_dir) && ./configure --with-shared && $(MAKE)
	$(RM) $(readline_lib_dir)/shlib/*.so*
	$(CD) $(readline_lib_dir) && ./configure && $(MAKE)
	$(CD) $(readline_lib_dir)/shlib/ && $(LN) libreadline.so.7.0 libreadline.so
	$(CD) $(readline_lib_dir)/shlib/ && $(LN) libreadline.so.7.0 libreadline.so.7

build:
	$(RM) $(dist) $(src)/*.o
	$(MKDIR) $(dist)
	$(CC) $(src)/*.c -o $(dist)/cim -Wall -I$(include) \
		-L$(readline_lib_dir)/shlib -lreadline \
		-L$(ncurses_lib_dir)/lib -lncurses

install:
	if ! test  -f $(lib_path)/libreadline.so.7.0 ; then \
		$(INSTALL) $(readline_lib_dir)/shlib/libreadline.so.7.0 $(lib_path) ; fi
	if ! test  -f $(lib_path)/libreadline.so ; then \
		$(INSTALL) $(readline_lib_dir)/shlib/libreadline.so $(lib_path) ; fi
	if ! test  -f $(lib_path)/libreadline.so.7 ; then \
		$(INSTALL) $(readline_lib_dir)/shlib/libreadline.so.7 $(lib_path) ; fi
	$(INSTALL) $(dist)/cim $(bindir)/cim

clean:
	$(RM) $(dist) $(src)/*.o
