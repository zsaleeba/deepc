BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
PMOBJDIR=  $(BUILD_DIR)/deepc/persmem

CFLAGS += -g -W -Wall -Wextra

TARGET	= $(PMOBJDIR)/persmem.a
SRCS 	= alloc.c allocmap.c freelist.c memsize.c persmem.c pool.c
OBJS    := $(SRCS:%.c=$(PMOBJDIR)/%.o)

all:    $(PMOBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(PMOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

test:
	(cd tests; $(MAKE) test)

$(PMOBJDIR):
	mkdir -p $(PMOBJDIR)

.PHONY:	all clean test
