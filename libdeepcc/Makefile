BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
OBJDIR=  $(BUILD_DIR)/deepc/libdeepcc

CXXFLAGS += -g -W -Wall -Wextra -std=c++11
DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJDIR)/$*.Td

TARGET	= $(OBJDIR)/libdeepcc.a
SRCS 	= sourcepos.cpp sourcespan.cpp allsources.cpp sourcefile.cpp programdb.cpp sourcemonitor.cpp diff.cpp
OBJS    := $(SRCS:%.cpp=$(OBJDIR)/%.o)

all:    $(OBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/%.d
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) -o $@ -c $<
	@mv -f $(OBJDIR)/$*.Td $(OBJDIR)/$*.d

clean:
	rm -f $(TARGET) $(OBJS) $(SRCS:%.cpp=$(OBJDIR)/%.d)

$(OBJDIR):
	mkdir -p $(OBJDIR)

protocol:
	flatc --cpp --binary sourcefile.fbs

$(OBJDIR)/%.d: ;
.PRECIOUS: $(OBJDIR)/%.d
.PHONY:	all clean

-include $(patsubst %,$(OBJDIR)/%.d,$(basename $(SRCS)))
