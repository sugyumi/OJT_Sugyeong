#!/bin/bash
CC = g++
CFLAGS = -W -Wall
SRCS = sutry.cpp
TARGET = simple
OPENCV := $(shell pkg-config opencv4 --libs --cflags)
LIBS = $(OPENCV)
$(TARGET):$(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) core
