# -- Configuration
CC = gcc
CXX = g++
AR = ar
CFLAGS = -Wall
CXXFLAGS = $(CFLAGS) -std=c++11
ARFLAGS = rcs

TARGET = libshader.a
OBJS = Shader.o ShaderProgram.o

# -- Targets
$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
