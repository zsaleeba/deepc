BUILD_DIR  	?= ../../build
OBJDIR		= $(BUILD_DIR)/deepc/libdeepcc/tests
BINDIR  	= $(BUILD_DIR)/deepc/bin
INCLUDES	= -I..
LIBS		= -lgtest -lpthread
SRCS		= main.cpp cbtree_test.cpp diff_test.cpp
OBJS		= $(SRCS:%.cpp=$(OBJDIR)/%.o)
TARGET		= $(BINDIR)/libdeepcc_tests

CXXFLAGS 	+= -g -W -Wall -Wextra -std=c++11
DEPFLAGS 	= -MT $@ -MMD -MP -MF $(OBJDIR)/$*.Td

all:    $(OBJDIR) libdeepcc_tests

libdeepcc_tests: $(TARGET)

test:	$(TARGET)
	$(TARGET)

$(TARGET): $(OBJS) $(BUILD_DIR)/deepc/libdeepcc/libdeepcc.a
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(BUILD_DIR)/deepc/libdeepcc/libdeepcc.a $(LIBS)

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(OBJDIR)/%.d
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) -o $@ -c $<
	@mv -f $(OBJDIR)/$*.Td $(OBJDIR)/$*.d

clean:
	rm -f $(TARGET) $(OBJS) $(SRCS:%.cpp=$(OBJDIR)/%.d)

$(OBJDIR):
	mkdir -p $(OBJDIR) $(BINDIR)

$(OBJDIR)/%.d: ;
.PRECIOUS: $(OBJDIR)/%.d
.PHONY:	all clean libdeepcc_tests test

-include $(patsubst %,$(OBJDIR)/%.d,$(basename $(SRCS)))
