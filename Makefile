CC = gcc
FLAGS = -Wall -Werror -Wextra
FILE = s21_truncate.c s21_negate.c s21_round.c s21_floor.c s21_add.c s21_sub.c \
s21_mul.c s21_div.c s21_is_greater_or_equal.c s21_is_not_equal.c \
s21_is_less_or_equal.c s21_is_less.c s21_is_equal.c s21_is_greater.c \
s21_from_int_to_decimal.c s21_from_decimal_to_int.c s21_from_float_to_decimal.c \
s21_from_decimal_to_float.c s21_help.c
OBJ=${FILE:.c=.o}
	

all: s21_decimal.a

s21_decimal.a:
	$(CC) $(FLAGS) -c $(FILE)
	ar rcs s21_decimal.a $(OBJ)
	ranlib s21_decimal.a
	rm -rf *.o

clean:
	rm -rf s21_decimal.a *.o 
	rm -rf tests *.info *.gcno *.gcda report

test: s21_decimal.a
	$(CC) $(FLAGS) tests.c s21_decimal.a -lcheck -o tests
	./tests

rebuild: clean test

add_coverage:
	$(eval FLAGS += --coverage)

gcov_report: add_coverage rebuild
	lcov -t "tests" -o tests.info -c -d .
	genhtml -o report tests.info
	open report/index.html

