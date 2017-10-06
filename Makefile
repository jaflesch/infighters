CC=g++
CFLAGS=-I./include -I./include/freetype -g -I./src -std=c++11 -m64 -w -fpermissive
SRCDIR=./src
BINDIR=./bin/gcc
LIBDIR=./lib
LINKEDLIBS=-lX11 -lGL -pthread
SRCFILES=$(SRCDIR)/os.cpp $(SRCDIR)/util.cpp $(SRCDIR)/Main.cpp $(SRCDIR)/font_rendering.cpp $(SRCDIR)/render_engine.cpp $(SRCDIR)/GUI/gui.cpp $(SRCDIR)/GUI/parser.cpp $(SRCDIR)/GUI/lexer.cpp

all:
	mkdir -p ./bin/gcc
	$(CC) $(CFLAGS) $(SRCFILES) $(LIBDIR)/libGLEW.a $(LIBDIR)/libfreetype.a -o $(BINDIR)/squiggly $(LINKEDLIBS)
