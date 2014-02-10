# -- Configuration
CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS = $(CFLAGS) -std=c++11
LDFLAGS = -lglfw3

TARGET = shadertool
OBJS = main.o App.o Shader.o

# -- Platform specific stuff
ifeq ($(OS),Windows_NT)
	# Windows things go here
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		# OSX Specific stuff goes here
		LDFLAGS += -framework Cocoa -framework OpenGL -framework QuartzCore -framework IOKit
	else
		# Standard *nix stuff here
	endif
endif

# -- Targets
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
