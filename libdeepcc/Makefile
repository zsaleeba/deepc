BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
DDCOBJDIR=  $(BUILD_DIR)/deepc/libdeepcc

CFLAGS += -g -W -Wall -Wextra

TARGET	= $(DDCOBJDIR)/libdeepcc.a
SRCS 	= sourcepos.cpp sourcespan.cpp sourcebag.cpp sourcefile.cpp
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

.PHONY:	all clean
