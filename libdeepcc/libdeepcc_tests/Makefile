BUILD_DIR  	?= ../../build
OBJDIR		= $(BUILD_DIR)/deepc/libdeepcc/tests
BINDIR  	= $(BUILD_DIR)/deepc/bin
INCLUDES	= -I..
LIBS		= -lgtest -lpthread

CXXFLAGS += -g -W -Wall -Wextra -std=c++11

all:    $(OBJDIR) diff_test cbtree_test

diff_test: $(BINDIR)/diff_test

$(BINDIR)/diff_test: $(OBJDIR)/diff_test.o $(BUILD_DIR)/deepc/libdeepcc/libdeepcc.a
	$(CXX) $(CXXFLAGS) -o $@ $< $(BUILD_DIR)/deepc/libdeepcc/libdeepcc.a $(LIBS)

cbtree_test: $(BINDIR)/cbtree_test

$(BINDIR)/cbtree_test: $(OBJDIR)/cbtree_test.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(BINDIR)/cbtree_test $(OBJDIR)/cbtree_test.o

$(OBJDIR):
	mkdir -p $(OBJDIR) $(BINDIR)

.PHONY:	all clean cbtree_test diff_test
