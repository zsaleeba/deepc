BINDIR  ?= ../../bin
OBJDIR  ?= ../../obj
PMOBJDIR=  $(OBJDIR)/deepc/persmem
PMTESTOBJDIR=  $(PMOBJDIR)/test

CFLAGS += -g -W -Wall -Wextra -I..

TARGET	= $(BINDIR)/pmtest
PMLIB	= $(PMOBJDIR)/persmem.a
LIBS	= -lcunit
SRCS 	= pmtest.c
OBJS    := $(SRCS:%.c=$(PMTESTOBJDIR)/%.o)

all:

test:	$(PMTESTOBJDIR) $(BINDIR) $(TARGET)
	@$(TARGET)

$(TARGET): $(OBJS) $(PMLIB)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(PMLIB) $(LIBS)

$(PMTESTOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(PMTESTOBJDIR):
	mkdir -p $(PMTESTOBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY:	all clean test
