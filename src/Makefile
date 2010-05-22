# Makefile for deepc
# - by Zik Saleeba 2010-05-22

# compile options
CFLAGS = -Wall -g
LIBS = -lm

# we compile with g++ since we need to link with LLVM
CC = g++

# sources list
EXE = deepc
CSRCS = deepc.c
YSRC = dcparser.y
LSRC = dclexer.l

GENSRCS = $(YSRC:%.y=%.c) $(LSRC:%.l=%.c)
OBJS = $(CSRCS:%.c=%.o) $(GENSRCS:%.c=%.o)


# targets
all: depend $(EXE)


$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LIBS)

dcparser.c dcparser.h: dcparser.y
	bison -d -o dcparser.c dcparser.y

dclexer.c dclexer.h: dclexer.l dcparser.h
	flex -o dclexer.c --header-file=dclexer.h dclexer.l


depend: $(CSRCS) $(GENSRCS)
	$(CXX) $(CXXFLAGS) -MM $(CSRCS) $(GENSRCS) >.depend

test: all
	(cd tests; make test)

clean:
	rm -f $(EXE) $(OBJS) $(GENSRCS) dcparser.h dclexer.h

count:
	@wc $(CSRCS) $(YSRC) $(LSRC)


.PHONY: all depend clean count test

-include .depend
