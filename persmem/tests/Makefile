BINDIR  ?= ../../bin
OBJDIR  ?= ../../obj
PMOBJDIR=  $(OBJDIR)/deepc/persmem
PMTESTOBJDIR=  $(PMOBJDIR)/test

CFLAGS += -g -W -Wall -Wextra -I..

TARGET	= $(BINDIR)/pmtest
LIBS	= $(PMOBJDIR)/persmem.a -lcunit
SRCS 	= pmtest.c
OBJS    := $(SRCS:%.c=$(PMTESTOBJDIR)/%.o)

all:

test:	$(PMTESTOBJDIR) $(TARGET)
	@$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(PMTESTOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(PMTESTOBJDIR):
	mkdir -p $(PMTESTOBJDIR)

.PHONY:	all clean test
