BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
CPOBJDIR=  $(BUILD_DIR)/deepc/cparser

CXXFLAGS += -g -std=c++11 -W -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable -I../libdeepcc

TARGET	= $(CPOBJDIR)/cparser.a
SRCS	= ctree.cpp cparserlistener.cpp
FILES	= $(SRCS) ctree.h
SRCS    := $(SRCS)
OBJS    := $(SRCS:%.cpp=$(CPOBJDIR)/%.o)

all:    $(CPOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(CPOBJDIR)/%.o: $(CPOBJDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(CPOBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(CPOBJDIR)/ctree.o: ctree.cpp ctree.h

clean:
	rm -f $(TARGET) $(OBJS)

$(CPOBJDIR):
	mkdir -p $(CPOBJDIR)

.PHONY:	all clean
