BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
DCCOBJDIR=  $(BUILD_DIR)/deepc/goparser

CXXFLAGS += -g -W -Wall -Wextra -I../antlr4_runtime/runtime/src

TARGET	= $(DCCOBJDIR)/goparser.a
GENSRCS	= GolangBaseListener.cpp GolangLexer.cpp GolangListener.cpp GolangParser.cpp
GENFILES = $(GENSRCS) GolangBaseListener.h GolangLexer.h GolangListener.h GolangParser.h \
	Golang.tokens GolangLexer.tokens
SRCS    := $(GENSRCS:%.cpp=$(DCCOBJDIR)/%.cpp)
OBJS    := $(GENSRCS:%.cpp=$(DCCOBJDIR)/%.o)

all:    $(DCCOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(DCCOBJDIR)/%.o: $(DCCOBJDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(DCCOBJDIR)/CBaseListener.cpp: Golang.g4
	java -jar $(HOME)/fetch/antlr4-4.5.4-SNAPSHOT.jar -o $(DCCOBJDIR) -Dlanguage=Cpp Golang.g4

clean:
	rm -f $(TARGET) $(OBJS) $(GENFILES)

$(DCCOBJDIR):
	mkdir -p $(DCCOBJDIR)

.PHONY:	all clean
