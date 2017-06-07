CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell sdl2-config --cflags)
LDLIBS=$(shell sdl2-config --libs) -lSDL2_image

SRCS=src/*.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

TARGET=spaceships

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGET)

distclean: clean
	$(RM) *~ .depend

include .depend