BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
DCCOBJDIR=  $(BUILD_DIR)/deepc/cparser

CXXFLAGS += -g -W -Wall -Wextra -I../antlr4_runtime/runtime/src

TARGET	= $(DCCOBJDIR)/cparser.a
GENSRCS	= CBaseListener.cpp CLexer.cpp CListener.cpp CParser.cpp
GENFILES = $(GENSRCS) CBaseListener.h CLexer.h CListener.h CParser.h \
	C.tokens CLexer.tokens
SRCS    := $(GENSRCS:%.cpp=$(DCCOBJDIR)/%.cpp)
OBJS    := $(GENSRCS:%.cpp=$(DCCOBJDIR)/%.o)

all:    $(DCCOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(DCCOBJDIR)/%.o: $(DCCOBJDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(DCCOBJDIR)/CBaseListener.cpp: C.g4
	java -jar $(HOME)/fetch/antlr4-4.5.4-SNAPSHOT.jar -o $(DCCOBJDIR) -Dlanguage=Cpp C.g4

clean:
	rm -f $(TARGET) $(OBJS) $(GENFILES)

$(DCCOBJDIR):
	mkdir -p $(DCCOBJDIR)

.PHONY:	all clean
