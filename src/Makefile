# Makefile for deepc
# - by Zik Saleeba 2010-05-22

# compile options
CXXFLAGS = -Wall -g
LIBS = -lm

# sources list
EXE = deepc
CXXSRCS = deepc.cpp compiler.cpp preproc.cpp
# 	datatype.cpp lexer.cpp parsetree.cpp \
#	deeplib.cpp codegen.cpp
CXXINCS = deepc.h
YSRC = dcparser.y
LSRC = dclexer.l

GENSRCS = $(YSRC:%.y=%.cpp) $(LSRC:%.l=%.cpp)
OBJS = $(CXXSRCS:%.cpp=%.o) $(GENSRCS:%.cpp=%.o)


# targets
all: depend $(EXE)


$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LIBS)

dcparser.cpp dcparser.hpp location.hh position.hh stack.hh: dcparser.y
	bison -d -o dcparser.cpp dcparser.y

dclexer.cpp dclexer.h: dclexer.l dcparser.hpp
	flex -o dclexer.cpp --header-file=dclexer.h dclexer.l


depend: $(CXXSRCS) $(GENSRCS)
	$(CXX) $(CXXFLAGS) -MM $(CXXSRCS) $(GENSRCS) >.depend

test: all
	(cd tests; make test)

clean:
	rm -f $(EXE) $(OBJS) $(GENSRCS) dcparser.hpp location.hh position.hh stack.hh dclexer.h .depend

count:
	@wc $(CSRCS) $(YSRC) $(LSRC) $(CINCS)


.PHONY: all depend clean count test

-include .depend
