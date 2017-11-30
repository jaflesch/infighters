CC=g++
CFLAGS=-I./include -I./include/freetype -g -I./src -std=c++11 -m64 -w -fpermissive
SRCDIR=./src
BINDIR=./bin/gcc
LIBDIR=./lib
LINKEDLIBS=-lX11 -lGL -pthread
SRCFILES=$(SRCDIR)/font_render/os.cpp $(SRCDIR)/util.cpp $(SRCDIR)/Main.cpp $(SRCDIR)/font_render/font_rendering.cpp $(SRCDIR)/font_render/render_engine.cpp $(SRCDIR)/WindowApi/Window.cpp $(SRCDIR)/WindowApi/WindowDiv.cpp $(SRCDIR)/WindowApi/Label.cpp $(SRCDIR)/WindowApi/FontRenderer.cpp $(SRCDIR)/WindowApi/Button.cpp $(SRCDIR)/WindowApi/Shader.cpp $(SRCDIR)/WindowApi/WindowShader.cpp $(SRCDIR)/ResourceLoad/ImageLoader.cpp $(SRCDIR)/ResourceLoad/Texture.cpp

all:
	mkdir -p ./bin/gcc
	$(CC) $(CFLAGS) $(SRCFILES) $(LIBDIR)/libGLEW.a $(LIBDIR)/libfreetype.a -o $(BINDIR)/squiggly $(LINKEDLIBS)
