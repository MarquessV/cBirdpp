CC := g++
CFLAGS := -std=c++17 -Wall -Wextra -pedantic-errors # -g
LFLAGS := -lcurl -lcurlpp -lgtest

BINDIR := bin
BUILDDIR := build
LIBDIR := lib

CBIRDPPHEADER := include/cbirdpp/cbirdpp.h
OBJECTS := $(BUILDDIR)/data.o $(BUILDDIR)/DataOptionalParameters.o
TESTFILE := test/tester.cpp


test: $(TESTFILE) $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o bin/test.out

$(BUILDDIR)/data.o: $(LIBDIR)/data.cpp $(CBIRDPPHEADER)
	$(CC) $(CFLAGS) -c $(LIBDIR)/data.cpp $(LFLAGS) -o $(BUILDDIR)/data.o

$(BUILDDIR)/DataOptionalParameters.o: $(LIBDIR)/DataOptionalParameters.cpp $(CBIRDPPHEADER)
	$(CC) $(CFLAGS) -c $(LIBDIR)/DataOptionalParameters.cpp $(LFLAGS) -o $(BUILDDIR)/DataOptionalParameters.o

run:
	$(BINDIR)/test.out

clean:
	rm -f $(BUILDDIR)/*
	rm -f $(BINDIR)/*

.PHONY: clean test
