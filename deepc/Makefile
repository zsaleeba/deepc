BINDIR  ?= ../bin
OBJDIR  ?= ../obj
DCOBJDIR=  $(OBJDIR)/deepc/deepc
DCCOBJDIR= $(OBJDIR)/deepc/libdeepcc

CFLAGS += -g -W -Wall -Wextra

TARGET	= $(BINDIR)/deepc
SRCS 	= main.c
LIBS    = $(DCCOBJDIR)/libdeepcc.a
OBJS    := $(SRCS:%.c=$(DCOBJDIR)/%.o)

all:    $(DCOBJDIR) $(TARGET)

$(TARGET): $(OBJS) $(DCCOBJDIR)/libdeepcc.a
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS) $(LIBS)

$(DCOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(DCOBJDIR):
	mkdir -p $(DCOBJDIR)

.PHONY:	all clean
