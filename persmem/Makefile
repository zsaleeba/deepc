BINDIR  ?= ../bin
OBJDIR  ?= ../obj
PMOBJDIR=  $(OBJDIR)/deepc/persmem

CFLAGS += -g -W -Wall -Wextra

TARGET	= $(PMOBJDIR)/persmem.a
SRCS 	= persmem.c memsize.c
OBJS    := $(SRCS:%.c=$(PMOBJDIR)/%.o)

all:    $(PMOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(PMOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(PMOBJDIR):
	mkdir -p $(PMOBJDIR)

.PHONY:	all clean
