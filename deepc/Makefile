BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
DCOBJDIR=  $(BUILD_DIR)/deepc/deepc
AROBJDIR= $(BUILD_DIR)/deepc/antlr_runtime
CPOBJDIR= $(BUILD_DIR)/deepc/cparser
DCCOBJDIR= $(BUILD_DIR)/deepc/libdeepcc
PMOBJDIR=  $(BUILD_DIR)/deepc/persmem

INCLUDES= -I../cparser -I../libdeepcc
CXXFLAGS += -g -W -Wall -Wextra -std=c++11 $(INCLUDES)

TARGET	= $(BINDIR)/deepc
SRCS 	= main.cpp
LIBS    = $(CPOBJDIR)/cparser.a $(DCCOBJDIR)/libdeepcc.a $(PMOBJDIR)/persmem.a $(AROBJDIR)/antlr_runtime.a
OBJS    := $(SRCS:%.cpp=$(DCOBJDIR)/%.o)

all:    $(DCOBJDIR) $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)

$(DCOBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(DCOBJDIR):
	mkdir -p $(DCOBJDIR)

.PHONY:	all clean
