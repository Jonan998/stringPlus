CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -O2
LDLIBS = -lm

TARGET = unit_tests

SRCS = \
  tests.c \
  tests_sscanf.c \
  s21_sscanf.c \
  s21_sscanf_float.c \
  s21_sscanf_format.c \
  s21_sscanf_input.c \
  s21_sscanf_int.c \
  s21_sscanf_store.c \
  s21_sscanf_text.c

OBJS = $(SRCS:.c=.o)

PKG_CONFIG = $(shell command -v pkg-config 2>/dev/null)

ifeq ($(PKG_CONFIG),)
  BREW_PREFIX = $(shell command -v brew >/dev/null 2>&1 && brew --prefix || echo "")
  ifneq ($(BREW_PREFIX),)
    CHECK_CFLAGS = -I$(BREW_PREFIX)/include
    CHECK_LIBS = -L$(BREW_PREFIX)/lib -lcheck
  else
    CHECK_CFLAGS =
    CHECK_LIBS = -lcheck
  endif
else
  CHECK_CFLAGS = $(shell pkg-config --cflags check)
  CHECK_LIBS = $(shell pkg-config --libs check)
endif

.PHONY: all test run clean rebuild

all: test
test: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) $^ $(CHECK_LIBS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all