CC = gcc
CXX = g++
CFLAGS = -Wall
CXXFLAGS = $(CFLAGS) -std=c++11
LDFLAGS = 

TARGET = shadertool
OBJS = main.o

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
