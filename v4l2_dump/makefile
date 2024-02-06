#!/bin/bash
CC = g++
CFLAGS = -W -Wall
SRCS = v4l2dupm.cpp
TARGET = v4l2dump
$(TARGET):$(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(OBJECTS) $(TARGET) core
