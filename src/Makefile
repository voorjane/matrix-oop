CC			= g++
LIB			= s21_matrix_oop.a
CFLAGS		= -Wall -Wextra -Werror -std=c++17 #-pedantic -fsanitize=address
TESTFLAGS 	= -lgtest
COVFLAGS 	= -fprofile-arcs -ftest-coverage
SOURCENAME	= s21_matrix_oop


all: $(SOURCENAME).a test gcov_report

s21_matrix_oop.a:
	$(CC) $(CFLAGS) -c $(SOURCENAME).cc
	ar rcs $(LIB) s21_matrix_oop.o

test: $(SOURCENAME).cc s21_matrix_oop_test.cc s21_matrix_oop.h
	$(CC) s21_matrix_oop_test.cc $(SOURCENAME).cc -o test $(TESTFLAGS) $(COVFLAGS) -std=c++17
	./test

gcov_report:
	gcovr -r . --html --html-details -o report.html
	open report.html

clang:
	cp ../materials/linters/.clang-format .
	clang-format -n *.cc *.h
	rm -rf .clang-format

clean:
	rm -rf *.o *.a *gcda *gcno *info test *.html *.css

rebuild:
	$(MAKE) clean
	$(MAKE) all

.PHONY: all clean rebuild test clang gcov_report s21_matrix_oop.a