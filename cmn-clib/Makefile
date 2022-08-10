PROGNAME := cmn-clib
INCDIR := inc
SRCDIR := src
LIBDIR := lib
OUTDIR := build
LIB_TARGET := $(OUTDIR)/lib$(PROGNAME).a
SRCS := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/CmnConf/*.c) $(wildcard $(SRCDIR)/CmnData/*.c) $(wildcard $(SRCDIR)/CmnFile/*.c) $(wildcard $(SRCDIR)/CmnLog/*.c) $(wildcard $(SRCDIR)/CmnString/*.c) $(wildcard $(SRCDIR)/CmnTest/*.c) $(wildcard $(SRCDIR)/CmnTime/*.c) $(wildcard $(SRCDIR)/CmnNet/*.c) $(wildcard $(SRCDIR)/CmnThread/*.c) $(wildcard $(SRCDIR)/CmnWin32/*.c)
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

