CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

LIB_NAME = s21_string.a

SRC = $(wildcard s21_*.c)
OBJ = $(SRC:.c=.o)

TEST_SRC = tests1/*.c
TEST_BIN = test

OS := $(shell uname)
ifeq ($(OS), Linux)
    CHECK_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
else
    CHECK_LIBS = -lcheck
endif

.PHONY: all clean test s21_string.a gcov_report

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	ar rcs $@ $^
	ranlib $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: clean $(LIB_NAME)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB_NAME) $(CHECK_LIBS) -o $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC) $(TEST_SRC) $(CHECK_LIBS) -o $(TEST_BIN)
	./$(TEST_BIN)
	lcov -t "s21_string_report" -o coverage.info -c -d .
	genhtml coverage.info -o report

clean:
	rm -rf *.o *.a *.gcno *.gcda *.gcov $(TEST_BIN) coverage.info report