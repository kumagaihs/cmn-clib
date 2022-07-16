PROGNAME := cmn-clib
INCDIR := inc
SRCDIR := src
LIBDIR := lib
OUTDIR := build
LIB_TARGET := $(OUTDIR)/lib$(PROGNAME).a
SRCS := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/CommonConf/*.c) $(wildcard $(SRCDIR)/CommonData/*.c) $(wildcard $(SRCDIR)/CommonFile/*.c) $(wildcard $(SRCDIR)/CommonLog/*.c) $(wildcard $(SRCDIR)/CommonString/*.c) $(wildcard $(SRCDIR)/CommonTest/*.c) $(wildcard $(SRCDIR)/CommonTime/*.c) $(wildcard $(SRCDIR)/CommonNet/*.c) $(wildcard $(SRCDIR)/CommonThread/*.c) $(wildcard $(SRCDIR)/CommonWin32/*.c)
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(SRCS)))
TEST_TARGET := $(OUTDIR)/test_main
TEST_SRCS := $(wildcard test/src/*.c)
TEST_OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.c,%.o,$(TEST_SRCS)))
#$(warning $(OBJS))

CC = gcc
AR = ar
CFLAGS = -Wall -O2 -I $(INCDIR) -pthread
ARFLAG = crsv

.PHONY: all clean
all: $(LIB_TARGET) $(TEST_TARGET)

#$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) -o $@ $^
$(LIB_TARGET): $(OBJS)
	$(AR) $(ARFLAG) $(LIB_TARGET) $(OBJS)

$(TEST_TARGET): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB_TARGET)

$(OUTDIR)/%.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OUTDIR)

