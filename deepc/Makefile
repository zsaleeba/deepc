BINDIR  ?= ../bin
OBJDIR  ?= ../obj
DCOBJDIR=  $(OBJDIR)/deepc/deepc
DCCOBJDIR= $(OBJDIR)/deepc/libdeepcc
PMOBJDIR=  $(OBJDIR)/deepc/persmem

CFLAGS += -g -W -Wall -Wextra

TARGET	= $(BINDIR)/deepc
SRCS 	= main.c
LIBS    = $(DCCOBJDIR)/libdeepcc.a $(PMOBJDIR)/persmem.a
OBJS    := $(SRCS:%.c=$(DCOBJDIR)/%.o)

all:    $(DCOBJDIR) $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS) $(LIBS)

$(DCOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(DCOBJDIR):
	mkdir -p $(DCOBJDIR)

.PHONY:	all clean
