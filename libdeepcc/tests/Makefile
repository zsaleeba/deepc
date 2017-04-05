BUILD_DIR  	?= ../../build
OBJDIR		= $(BUILD_DIR)/deepc/libdeepcc/tests
BINDIR  	= $(BUILD_DIR)/deepc/bin
INCLUDES	= -I..

CXXFLAGS += -g -W -Wall -Wextra -std=c++11

all:    $(OBJDIR) cbtree_test

cbtree_test: $(BINDIR)/cbtree_test

$(BINDIR)/cbtree_test: $(OBJDIR)/cbtree_test.o
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(BINDIR)/cbtree_test $(OBJDIR)/cbtree_test.o

$(OBJDIR):
	mkdir -p $(OBJDIR) $(BINDIR)

.PHONY:	all clean cbtree_test
