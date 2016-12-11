BINDIR  ?= ../../bin
BUILD_DIR  ?= ../../build
PMOBJDIR=  $(BUILD_DIR)/deepc/persmem
PMTESTOBJDIR=  $(PMOBJDIR)/test

CFLAGS += -g -W -Wall -Wextra -I..

TARGET	= $(BINDIR)/pmtest
TARGET2	= $(BINDIR)/pmtest2
PMLIB	= $(PMOBJDIR)/persmem.a
LIBS	= -lcunit
SRCS 	= pmtest.c
SRCS2 	= pmtest2.c
OBJS    := $(SRCS:%.c=$(PMTESTOBJDIR)/%.o)
OBJS2   := $(SRCS2:%.c=$(PMTESTOBJDIR)/%.o)

all:	$(TARGET) $(TARGET2)

test:	$(PMTESTOBJDIR) $(BINDIR) $(TARGET)
	@$(TARGET)

test2:	$(PMTESTOBJDIR) $(BINDIR) $(TARGET2)
	@$(TARGET2)

$(TARGET): $(OBJS) $(PMLIB)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(PMLIB) $(LIBS)

$(TARGET2): $(OBJS2) $(PMLIB)
	$(CC) $(CFLAGS) -o $(TARGET2) $(OBJS2) $(PMLIB) $(LIBS)

$(PMTESTOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(TARGET2) $(OBJS) $(OBJS2)

$(PMTESTOBJDIR):
	mkdir -p $(PMTESTOBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY:	all clean test
