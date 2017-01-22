BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
DCOBJDIR=  $(BUILD_DIR)/deepc/deepc
DCCOBJDIR= $(BUILD_DIR)/deepc/libdeepcc
PMOBJDIR=  $(BUILD_DIR)/deepc/persmem

CFLAGS += -g -W -Wall -Wextra

TARGET	= $(BINDIR)/deepc
SRCS 	= main.cpp
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
