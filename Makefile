CC=gcc
FLAGS=-Wall -Wextra -Werror -std=c11
TEST_PARTS=./tests/*.c 
GCOV_FLAGS= -fprofile-arcs -ftest-coverage
SOURCES=./backend/*.c
DIST_DIR=./archive

OS := $(shell uname)
ifeq ($(OS), Linux)
	OPEN_COMMAND=xdg-open
	RESULT=3d_viewer_c
	CHECK_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
endif

ifeq ($(OS), Darwin)
	RESULT=3d_viewer_c.app
	RUN_COMMAND=open
	OPEN_COMMAND=open
	CHECK_LIB = -lcheck
endif

all: install run

run:
	$(RUN_COMMAND) ../build/$(RESULT)  

install:
	mkdir -p ../build
	cd views && qmake && make &> /dev/null && make clean &> /dev/null && rm -f Makefile

uninstall: 
	@rm -rf ../build/$(RESULT)
	@echo "uninstalled"

clean: clean_exec clean_obj clean_gcov clean_lcov clean_lcov_report clean_dist
	@rm -rf views/.DS_Store \
        views/.qtc_clangd \
        views/3d_viewer_c.app/ \
        views/3d_viewer_v2.pro.user.4f59a73 \
        views/moc_main_window.cpp \
        views/moc_predefs.h 
	@echo "Clean finished"

dvi:
	$(OPEN_COMMAND) ../dvi/3d_viewer_c_about.html	

dist: clean_dist
	cd ../ && mkdir -p ${DIST_DIR}
	cd ../ && cp -rf src/* $(DIST_DIR)
	cd ../ && tar -czvf archive.tar.gz ${DIST_DIR}
	cd ../ && rm -rf ${DIST_DIR}

test:
	$(CC) ${FLAGS} $(TEST_PARTS) $(SOURCES) -o ./test.out $(CHECK_LIB)
	./test.out

gcov_report:
	@mkdir -p report
	$(CC) ${FLAGS} $(TEST_PARTS) $(SOURCES) -o ./gcov_report.out $(CHECK_LIB) $(GCOV_FLAGS) 
	./gcov_report.out
	lcov -t "test" -o test.info -c -d .
	genhtml -o report/ test.info
	${OPEN_COMMAND} ./report/index.html
	make clean_gcov

dist_unpack:
	cd ../ && tar -xzvf archive.tar.gz

style_check:
	@find .. -type f -name "*.c" -exec clang-format -n -style=Google {} \;
	@find .. -type f -name "*.h" -exec clang-format -n -style=Google {} \;
	@echo "Clang format style check is finished"

style:
	@find .. -type f -name "*.c" -exec clang-format -i -style=Google {} \;
	@find .. -type f -name "*.h" -exec clang-format -i -style=Google {} \;
	@echo "Clang format style apply is finished"

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz

clean_exec:
	@find .. -type f -name "*.out" -exec rm {} \;

clean_obj:
	@find .. -type f -name "*.o" -exec rm {} \;

clean_gcov:
	@find .. -type f -name "*.gcda" -exec rm {} \;
	@find .. -type f -name "*.gcno" -exec rm {} \;

clean_lcov:
	@find .. -type f -name "*.info" -exec rm {} \;

clean_lcov_report:
	@rm -rf report
