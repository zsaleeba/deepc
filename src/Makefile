BINDIR  ?= ../bin
OBJDIR  ?= ../obj
DCOBJDIR=  $(OBJDIR)/deepc

CFLAGS += -g -Wall

TARGET	= $(BINDIR)/deepc
SRCS 	= main.c
OBJS    := $(SRCS:%.c=$(DCOBJDIR)/%.o)

all:    $(DCOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS)

$(DCOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(DCOBJDIR):
	mkdir -p $(DCOBJDIR)

count:
	wc `find . -name *.c; find . -name *.h`

.PHONY:	all clean
