BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
CPOBJDIR=  $(BUILD_DIR)/deepc/cparser

CXXFLAGS += -g -std=c++11 -W -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable -I../antlr4_runtime/runtime/src -I$(CPOBJDIR)

TARGET	= $(CPOBJDIR)/cparser.a
ORIGSRCS = ctree.cpp cparserlistener.cpp
ORIGFILES = $(ORIGSRCS) ctree.h
GENSRCS	= CBaseListener.cpp CLexer.cpp CListener.cpp CParser.cpp
GENFILES = $(GENSRCS) CBaseListener.h CLexer.h CListener.h CParser.h \
	C.tokens CLexer.tokens
BGENFILES := $(GENFILES:%=$(CPOBJDIR)/%)
BGENFILES1 = $(firstword $(BGENFILES))
BGENFILES_REST = $(wordlist 2,$(words $(BGENFILES)),$(BGENFILES))
SRCS    := $(ORIGSRCS) $(GENSRCS:%.cpp=$(CPOBJDIR)/%.cpp)
OBJS    := $(ORIGSRCS:%.cpp=$(CPOBJDIR)/%.o) $(GENSRCS:%.cpp=$(CPOBJDIR)/%.o)

all:    $(CPOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(CPOBJDIR)/%.o: $(CPOBJDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(CPOBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(BGENFILES1): C.g4
	java -jar $(HOME)/fetch/antlr-4.6-complete.jar -o $(CPOBJDIR) -Dlanguage=Cpp $<
	touch $(BGENFILES_REST)

$(BGENFILES_REST): $(BGENFILES1)

$(CPOBJDIR)/ctree.o: ctree.cpp ctree.h $(CPOBJDIR)/CBaseListener.h $(CPOBJDIR)/CLexer.h $(CPOBJDIR)/CListener.h $(CPOBJDIR)/CParser.h

clean:
	rm -f $(TARGET) $(OBJS) $(BGENFILES)

$(CPOBJDIR):
	mkdir -p $(CPOBJDIR)

.PHONY:	all clean
