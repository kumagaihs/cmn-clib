PROGNAME := cmn-clib
INCDIR := inc
SRCDIR := src
LIBDIR := lib
OUTDIR := build
LIB_TARGET := $(OUTDIR)/$(PROGNAME).a
SRCS := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/CommonConf/*.c) $(wildcard $(SRCDIR)/CommonData/*.c) $(wildcard $(SRCDIR)/CommonLog/*.c) $(wildcard $(SRCDIR)/CommonString/*.c) $(wildcard $(SRCDIR)/CommonTime/*.c)
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(SRCS)))
TEST_TARGET := $(OUTDIR)/test
TEST_SRCS := $(wildcard test/src/*.c)
TEST_OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(TEST_SRCS)))
#$(warning $(OBJS))

CC = gcc
AR = ar
CFLAGS = -Wall -O2 -I $(INCDIR)
ARFLAG = crsv

.PHONY: all clean
all: $(LIB_TARGET) $(TEST_TARGET)

#$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) -o $@ $^
$(LIB_TARGET): $(OBJS)
	$(AR) $(ARFLAG) $(LIB_TARGET) $(OBJS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB_TARGET)

$(OUTDIR)/%.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OUTDIR)

