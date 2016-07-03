BINDIR  ?= ../bin
OBJDIR  ?= ../obj
DCCOBJDIR=  $(OBJDIR)/deepc/libdeepcc

CFLAGS += -g -W -Wall -Wextra

TARGET	= $(DCCOBJDIR)/libdeepcc.a
SRCS 	= dcdbfile.c
OBJS    := $(SRCS:%.c=$(DCCOBJDIR)/%.o)

all:    $(DCCOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(DCCOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(DCCOBJDIR):
	mkdir -p $(DCCOBJDIR)

.PHONY:	all clean
