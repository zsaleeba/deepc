BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
DDCOBJDIR=  $(BUILD_DIR)/deepc/libdeepcc

CXXFLAGS += -g -W -Wall -Wextra -std=c++11

TARGET	= $(DDCOBJDIR)/libdeepcc.a
SRCS 	= sourcepos.cpp sourcespan.cpp allsources.cpp sourcefile.cpp programdb.cpp sourcemonitor.cpp diff.cpp
OBJS    := $(SRCS:%.cpp=$(DDCOBJDIR)/%.o)

all:    $(DDCOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(DDCOBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(DDCOBJDIR):
	mkdir -p $(DDCOBJDIR)

protocol:
	flatc --cpp --binary sourcefile.fbs

.PHONY:	all clean
