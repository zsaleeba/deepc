BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
DCCOBJDIR=  $(BUILD_DIR)/deepc/cparser

CXXFLAGS += -g -std=c++11 -W -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable -I../antlr4_runtime/runtime/src

TARGET	= $(DCCOBJDIR)/cparser.a
ORIGSRCS = cparser.cpp
GENSRCS	= CBaseListener.cpp CLexer.cpp CListener.cpp CParser.cpp
GENFILES = $(GENSRCS) CBaseListener.h CLexer.h CListener.h CParser.h \
	C.tokens CLexer.tokens
BGENFILES := $(GENFILES:%=$(DCCOBJDIR)/%)
BGENFILES1 = $(firstword $(BGENFILES))
BGENFILES_REST = $(wordlist 2,$(words $(BGENFILES)),$(BGENFILES))
SRCS    := $(ORIGSRCS) $(GENSRCS:%.cpp=$(DCCOBJDIR)/%.cpp)
OBJS    := $(SRCS:%.cpp=%.o)

all:    $(DCCOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(DCCOBJDIR)/%.o: $(DCCOBJDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(BGENFILES1): C.g4
	java -jar $(HOME)/fetch/antlr-4.6-complete.jar -o $(DCCOBJDIR) -Dlanguage=Cpp $<
	touch $(BGENFILES_REST)

$(BGENFILES_REST): $(BGENFILES1)

clean:
	rm -f $(TARGET) $(OBJS) $(BGENFILES)

$(DCCOBJDIR):
	mkdir -p $(DCCOBJDIR)

.PHONY:	all clean
