CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

LIB_NAME = s21_string.a
SRC = s21_string.c
OBJ = $(SRC:.c=.o)

TEST_SRC = tests/test_s21_string.c
TEST_BIN = test

OS := $(shell uname)

ifeq ($(OS), Linux)
	CHECK_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
else
	CHECK_LIBS = -lcheck
endif

.PHONY: all clean test gcov_report rebuild

# ---------- build library ----------

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ)
	ar rcs $(LIB_NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ---------- tests ----------

test: clean
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) $(CHECK_LIBS) -o $(TEST_BIN)
	./$(TEST_BIN)

# ---------- gcov report ----------

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRC) $(TEST_SRC) $(CHECK_LIBS) -o $(TEST_BIN)
	./$(TEST_BIN)
	gcov $(SRC) >/dev/null
	lcov -t "s21_string" -o coverage.info -c -d .
	genhtml coverage.info -o report

# ---------- clean ----------

clean:
	rm -rf *.o *.a *.gcno *.gcda *.gcov $(TEST_BIN) coverage.info report

rebuild: clean all
