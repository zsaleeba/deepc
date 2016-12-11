BINDIR  ?= ../bin
BUILD_DIR  ?= ../build
PGOBJDIR= $(BUILD_DIR)/deepc/dcparsergen

CFLAGS += -g -W -Wall -Wextra

TARGET	= $(BINDIR)/dcparsergen
SRCS 	= grammar.c lexergen.c main.c parsergen.c util.c
OBJS    := $(SRCS:%.c=$(PGOBJDIR)/%.o)

all:    $(PGOBJDIR) $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS) $(LIBS)

$(PGOBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(PGOBJDIR):
	mkdir -p $(PGOBJDIR)

.PHONY:	all clean
